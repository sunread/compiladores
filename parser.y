/**
 * @file parser.y
 * @author Arthur Foscarini, Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Arquivo do parser
 */

%{
#include <stdio.h>
#include "comp_tree.h"
#include "comp_dict.h"
#include "comp_list.h"
#include "iks_ast.h"
#include "semantics.h"
#include "iloc.h"
#define IKS_SYNTAX_ERRO 1

comp_list *currentDimensionsList;
%}

%union {
    comp_dict_item_t_p symbol;
    comp_dict_t_p dict;
    comp_tree* ast;
    int type;
    comp_list *dimensionList;
}

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token<symbol> TK_LIT_INT
%token<symbol> TK_LIT_FLOAT
%token<symbol> TK_LIT_FALSE
%token<symbol> TK_LIT_TRUE
%token<symbol> TK_LIT_CHAR
%token<symbol> TK_LIT_STRING
%token<symbol> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type<ast> p
%type<ast> programa
%type<ast> dec_funcao
%type<ast> corpo
%type<ast> bloco_comando
%type<ast> lista_comando
%type<ast> comando
%type<ast> atribuicao
%type<ast> input
%type<ast> output
%type<ast> lista_expressoes_nao_vazia
%type<ast> return
%type<ast> controle_fluxo
%type<ast> expressao
%type<ast> lista_expressoes
%type<ast> vet_index
%type<ast> chamada


%type<ast> cabecalho

%type<dict> lista_param
%type<dict> lista_param_nao_vazia
%type<dict> parametro
%type<dimensionList> dimensions

%type<type> tipo_variavel

%left TK_OC_OR
%left TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/' ';'
%right '!'

%nonassoc TK_PR_THEN
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem IKS */
 p: programa {$$ = tree_CreateNode(IKS_AST_PROGRAMA, NULL); ast = $$; localScope = NULL; functionType = 0; tree_AddSon($$, 1, $1);};

 programa : dec_global {localScope = NULL;} programa {$$ = $3;}
			| dec_funcao {localScope = NULL;} programa {$$ = $1; tree_AddBro($$, $3);}
			| {$$ = NULL;localScope = NULL;};

 tipo_variavel : TK_PR_INT {$$=IKS_INT;} | TK_PR_FLOAT {$$=IKS_FLOAT;} | TK_PR_BOOL {$$=IKS_BOOL;} | TK_PR_CHAR {$$=IKS_CHAR;} | TK_PR_STRING {$$=IKS_STRING;};
 dec_global : dec_variavel ';' | dec_vetor ';' ;
 dec_variavel : tipo_variavel ':' TK_IDENTIFICADOR {$3->scope = localScope;
													verifyDeclaration($3,0);
													$3->usage = ID_VARIAVEL;
													if(localScope!=NULL)
														localScope->ast_node->args = dict_insertEnd(localScope->ast_node->args,dict_argInsert($3));
                                                     setType($1,$3);
                                                     setSize($3);
                                                    } ;

 dec_vetor : tipo_variavel ':' TK_IDENTIFICADOR dimensions         {$3->scope = localScope;
																	verifyDeclaration($3,0);
																	$3->usage = ID_VETOR;
																	if(localScope!=NULL)
																		localScope->ast_node->args = dict_insertEnd(localScope->ast_node->args,dict_argInsert($3));
                                                                    $3->dimensionsList = currentDimensionsList;
                                                                    currentDimensionsList = NULL;

                                                                     setTypeVector($1, $3);
                                                                     setSize($3);
                                                                    };

 dimensions: '[' TK_LIT_INT ']' { currentDimensionsList = list_AddDimension($2->value.i, currentDimensionsList); };
             | '[' TK_LIT_INT ']' dimensions { currentDimensionsList = list_AddDimension($2->value.i, currentDimensionsList); };


 dec_funcao :cabecalho dec_local corpo {$$ = $1; tree_AddSon($$, 1, $3);};

 cabecalho : tipo_variavel ':' TK_IDENTIFICADOR {$3->scope = localScope;verifyDeclaration($3,0);localScope = $3;}'(' lista_param ')'	{$$ = tree_CreateNode(IKS_AST_FUNCAO, $3);
																		$3->usage = ID_FUNCAO;
																		$3->ast_node = $$;
                                                                        setType($1,$3);
																		$$->dataType = $3->type;
																		functionType = $3->type;
																		$$->args = $6;
																		offsetLocal = 8;
																		};

 lista_param : lista_param_nao_vazia {$$ = $1;} | {$$ = NULL;};
 lista_param_nao_vazia : lista_param_nao_vazia ',' parametro {$$ = dict_insertEnd($1, $3);}  | parametro {$$ = $1;};
 parametro : tipo_variavel ':' TK_IDENTIFICADOR {$3->scope = localScope;
												verifyDeclaration($3,0);
												$3->usage = ID_PARAMETRO;
												setType($1,$3);
												$$ = dict_argInsert($3);
												setSize($3);
                                                };
 dec_local : dec_variavel ';' dec_local	| ;

 corpo: '{' lista_comando '}' {$$ = $2;};

 bloco_comando: '{' lista_comando '}'{$$ = tree_CreateNode(IKS_AST_BLOCO, NULL); tree_AddSon($$, 1, $2);}

 lista_comando : comando {$$ = $1;}
				| comando ';' lista_comando {$$ = $1; tree_AddBro($$, $3);}
				| ';' lista_comando {$$ = $2;}
				| {$$ = NULL;};

 comando: 	bloco_comando {$$ = $1;}
			| controle_fluxo {$$ = $1;}
			| atribuicao 	{$$ = $1;}
			| input 		{$$ = $1;}
			| output 		{$$ = $1;}
			| return 		{$$ = $1;}
			| chamada 		{$$ = $1;};



 atribuicao : TK_IDENTIFICADOR '=' expressao {$$ = tree_CreateNode(IKS_AST_ATRIBUICAO, NULL);
												tree_AddSon($$, 2, tree_CreateNode(IKS_AST_IDENTIFICADOR, $1), $3);
												verifyIdentifier($1, ID_VARIAVEL);
												verifyDeclaration($1,1);
												astTypeInference($$);
												astTypeCoercion($$);
												}
			| vet_index '=' expressao {$$ = tree_CreateNode(IKS_AST_ATRIBUICAO, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);astTypeCoercion($$);};

 vet_index: TK_IDENTIFICADOR '[' expressao ']' {$$ = tree_CreateNode(IKS_AST_VETOR_INDEXADO, NULL);
												tree_AddSon($$, 2, tree_CreateNode(IKS_AST_IDENTIFICADOR, $1), $3);
												verifyIdentifier($1, ID_VETOR);
												verifyDeclaration($1,1);
												astTypeInference($$);
												astTypeCoercion($$);
												};


 input : TK_PR_INPUT expressao {$$ = tree_CreateNode(IKS_AST_INPUT, NULL);
										tree_AddSon($$, 1, $2);
										verifySimpleCommand($$, functionType);
										};

 output : TK_PR_OUTPUT lista_expressoes_nao_vazia {$$ = tree_CreateNode(IKS_AST_OUTPUT, NULL); tree_AddSon($$, 1, $2); verifySimpleCommand($$, functionType);};

 lista_expressoes_nao_vazia: expressao ',' lista_expressoes_nao_vazia {$$ = $1; tree_AddBro($$, $3);}
			| expressao {$$ = $1;};

 return : TK_PR_RETURN expressao {$$ = tree_CreateNode(IKS_AST_RETURN, NULL); tree_AddSon($$, 1, $2); $$->dataType = functionType; astTypeCoercion($$); verifySimpleCommand($$); } ;

 controle_fluxo : TK_PR_IF '(' expressao ')' TK_PR_THEN comando {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 2, $3, $6); $3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 1, $3);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 3, $3, $6, $8);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE comando {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 2, $3, $8);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE ';' {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 2, $3, $6);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE ';' {$$ = tree_CreateNode(IKS_AST_IF_ELSE, NULL); tree_AddSon($$, 1, $3);$3->labelT = createLabel(); $3->labelF = createLabel();}|

                  TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_comando {$$ = tree_CreateNode(IKS_AST_WHILE_DO, NULL); tree_AddSon($$, 2, $3, $6);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_WHILE '(' expressao ')' TK_PR_DO ';' {$$ = tree_CreateNode(IKS_AST_WHILE_DO, NULL); tree_AddSon($$, 1, $3);$3->labelT = createLabel(); $3->labelF = createLabel();}|
                  TK_PR_DO bloco_comando TK_PR_WHILE '(' expressao ')' {$$ = tree_CreateNode(IKS_AST_DO_WHILE, NULL); tree_AddSon($$, 2, $2, $5);$5->labelT = createLabel(); $5->labelF = createLabel();}|
                  TK_PR_DO ';' TK_PR_WHILE '(' expressao ')' {$$ = tree_CreateNode(IKS_AST_DO_WHILE, NULL); tree_AddSon($$, 1, $5);$5->labelT = createLabel(); $5->labelF = createLabel();};

 expressao : TK_IDENTIFICADOR {$$  = tree_CreateNode(IKS_AST_IDENTIFICADOR, $1); verifyDeclaration($1, 1);verifyIdentifier($1, ID_VARIAVEL); astTypeInference($$);}
| TK_IDENTIFICADOR '[' expressao ']' {$$  = tree_CreateNode(IKS_AST_VETOR_INDEXADO, $1); tree_AddSon($$, 2, tree_CreateNode(IKS_AST_IDENTIFICADOR, $1), $3); verifyDeclaration($1, 1);verifyIdentifier($1, ID_VETOR); astTypeInference($$);astTypeCoercion($$);}
| TK_LIT_INT {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| TK_LIT_FLOAT {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| TK_LIT_FALSE {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| TK_LIT_TRUE {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| TK_LIT_CHAR {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| TK_LIT_STRING {$$  = tree_CreateNode(IKS_AST_LITERAL, $1); $1->ast_node = $$; astTypeInference($$);}
| expressao '+' expressao {$$ = tree_CreateNode(IKS_AST_ARIM_SOMA, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$); astTypeCoercion($$);}
| expressao '-' expressao {$$ = tree_CreateNode(IKS_AST_ARIM_SUBTRACAO, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$); astTypeCoercion($$);}
| '-' expressao {$$ = tree_CreateNode(IKS_AST_ARIM_INVERSAO, NULL); tree_AddSon($$, 1, $2); astTypeInference($$);}
| '!' expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_NEGACAO, NULL); tree_AddSon($$, 1, $2); astTypeInference($$);}
| expressao '*' expressao {$$ = tree_CreateNode(IKS_AST_ARIM_MULTIPLICACAO, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$); astTypeCoercion($$);}
| expressao '/' expressao {$$ = tree_CreateNode(IKS_AST_ARIM_DIVISAO, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$); astTypeCoercion($$);}
| expressao '<' expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_L, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao '>' expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_G, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| '(' expressao ')' {$$ = $2;}
| expressao TK_OC_LE expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_LE, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao TK_OC_GE expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_GE, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao TK_OC_EQ expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_IGUAL, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao TK_OC_NE expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_COMP_DIF, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao TK_OC_AND expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_E, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| expressao TK_OC_OR expressao {$$ = tree_CreateNode(IKS_AST_LOGICO_OU, NULL); tree_AddSon($$, 2, $1, $3); astTypeInference($$);}
| chamada {$$ = $1;}
;

chamada: TK_IDENTIFICADOR '(' lista_expressoes ')' {$$ = tree_CreateNode(IKS_AST_CHAMADA_DE_FUNCAO, NULL); tree_AddSon($$, 2, tree_CreateNode(IKS_AST_IDENTIFICADOR, $1), $3);
                                                    verifyIdentifier($1, ID_FUNCAO);
                                                    verifyDeclaration($1,1);
                                                    verifyGivenParameters($1->ast_node, $$);
                                                    astTypeInference($$);
                                                    };

lista_expressoes : lista_expressoes_nao_vazia {$$ = $1;}| {$$ = NULL;};

%%

int yyerror(char *t) {
    printf("Erro de sintaxe na linha %d\n", getLineNumber());
}
