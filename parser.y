/* Projeto de Compiladores 2013-2 - Etapa 2
   Fernando Soller Mecking
   Mateus Cardoso da Silva
*/
 
%{
#include <stdio.h>
#include "comp_tree.h"
#include "comp_dict.h"
#include "iks_ast.h"
#define IKS_SYNTAX_ERRO 1

%}

%union {
    comp_dict_item_t_p symbol;
    comp_tree* ast;
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

%type<ast> programa
%type<ast> dec_funcao
%type<ast> bloco_comando
%type<ast> sequencia_comandos
%type<ast> comando
%type<ast> atribuicao
%type<ast> input
%type<ast> output
%type<ast> lista_expressoes_nao_vazia
%type<ast> return
%type<ast> controle_fluxo
%type<ast> expressao
%type<ast> lista_expressoes

%type<symbol> cabecalho

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc TK_PR_THEN ';'
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem IKS */

 programa : dec_global programa {$$ = tree_Add(IKS_AST_PROGRAMA, NULL, 0);}
			| dec_funcao programa {$$ = tree_Add(IKS_AST_PROGRAMA, NULL, 1, $1);}
			| {$$ = NULL;};
			
 dec_global : dec_variavel ';' | dec_vetor ';' ;
 dec_variavel : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_vetor : tipo_variavel ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']' ;
 tipo_variavel : TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL| TK_PR_CHAR | TK_PR_STRING ;
 
 dec_funcao : cabecalho dec_local bloco_comando {$$ = tree_Add(IKS_AST_FUNCAO, ((comp_dict_t_p)$1)->item, 1, $3);};
 
 cabecalho : tipo_variavel ':' TK_IDENTIFICADOR '(' lista_param ')'	{$$ = $3;};
 lista_param : lista_param_nao_vazia | ;
 lista_param_nao_vazia : parametro ',' lista_param_nao_vazia | parametro ;
 parametro : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_local : dec_variavel ';' dec_local	| ;
 
 bloco_comando: '{' sequencia_comandos '}' {$$ = $2;};
 
 sequencia_comandos : comando {$$ = $1;}| comando';' sequencia_comandos {$$ = tree_Add(IKS_AST_BLOCO, NULL, 2, $1, $3);};
 
 comando: bloco_comando 	{$$ = $1;}
			| dec_variavel  {$$ = NULL;}
			| dec_vetor 	{$$ = NULL;}
			| controle_fluxo {$$ = $1;}
			| atribuicao 	{$$ = $1;}
			| input 		{$$ = $1;}
			| output 		{$$ = $1;}
			| return 		{$$ = $1;};
			
 atribuicao : TK_IDENTIFICADOR '=' expressao {$$ = tree_Add(IKS_AST_ATRIBUICAO, NULL, 2, $1, $3);}
			| TK_IDENTIFICADOR '[' expressao ']' '=' expressao {$$ = tree_Add(IKS_AST_ATRIBUICAO, NULL, 2, $1, $6);};
			
 input : TK_PR_INPUT TK_IDENTIFICADOR {$$ = tree_Add(IKS_AST_INPUT, NULL, 1, $2);};
 
 output : TK_PR_OUTPUT lista_expressoes_nao_vazia {$$ = $2;};
 
 lista_expressoes_nao_vazia: expressao ',' lista_expressoes_nao_vazia {$$ = tree_Add(IKS_AST_OUTPUT, NULL, 2, $1, $3);}
			| expressao {$$ = tree_Add(IKS_AST_OUTPUT, NULL, 1, $1);};
			
 return : TK_PR_RETURN expressao {$$ = tree_Add(IKS_AST_RETURN, NULL, 1, $2);} ;
 
 controle_fluxo : TK_PR_IF '(' expressao ')' TK_PR_THEN comando {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 2, $3, $6);}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 1, $3);}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 3, $3, $6, $8);}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE comando {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 2, $3, $8);}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE ';' {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 2, $3, $6);}|
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE ';' {$$ = tree_Add(IKS_AST_IF_ELSE, NULL, 1, $3);}|

                  TK_PR_WHILE '(' expressao ')' TK_PR_DO comando {$$ = tree_Add(IKS_AST_WHILE_DO, NULL, 2, $3, $6);}|
                  TK_PR_WHILE '(' expressao ')' TK_PR_DO ';' {$$ = tree_Add(IKS_AST_WHILE_DO, NULL, 1, $3);}|
                  TK_PR_DO comando TK_PR_WHILE '(' expressao ')' {$$ = tree_Add(IKS_AST_DO_WHILE, NULL, 2, $2, $5);}|
                  TK_PR_DO ';' TK_PR_WHILE '(' expressao ')' {$$ = tree_Add(IKS_AST_DO_WHILE, NULL, 1, $5);};
                  
 expressao : TK_IDENTIFICADOR {$$  = tree_Add(IKS_AST_IDENTIFICADOR, ((comp_dict_t_p)$1)->item, 0);}
| TK_IDENTIFICADOR '[' expressao ']' {$$  = tree_Add(IKS_AST_VETOR_INDEXADO, NULL, 2, $1, $3);}
| TK_LIT_INT {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| TK_LIT_FLOAT {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| TK_LIT_FALSE {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| TK_LIT_TRUE {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| TK_LIT_CHAR {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| TK_LIT_STRING {$$  = tree_Add(IKS_AST_LITERAL, ((comp_dict_t_p)$1)->item, 0);}
| expressao '+' expressao {$$ = tree_Add(IKS_AST_ARIM_SOMA, NULL, 2, $1, $3);}
| expressao '-' expressao {$$ = tree_Add(IKS_AST_ARIM_SUBTRACAO, NULL, 2, $1, $3);}
| '-' expressao {$$ = tree_Add(IKS_AST_ARIM_INVERSAO, NULL, 1, $2);}
| expressao '*' expressao {$$ = tree_Add(IKS_AST_ARIM_MULTIPLICACAO, NULL, 2, $1, $3);}
| expressao '/' expressao {$$ = tree_Add(IKS_AST_ARIM_DIVISAO, NULL, 2, $1, $3);}
| expressao '<' expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_L, NULL, 2, $1, $3);}
| expressao '>' expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_G, NULL, 2, $1, $3);}
| '(' expressao ')' {$$ = $2;}
| expressao TK_OC_LE expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_LE, NULL, 2, $1, $3);}
| expressao TK_OC_GE expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_GE, NULL, 2, $1, $3);}
| expressao TK_OC_EQ expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_IGUAL, NULL, 2, $1, $3);}
| expressao TK_OC_NE expressao {$$ = tree_Add(IKS_AST_LOGICO_COMP_DIF, NULL, 2, $1, $3);}
| expressao TK_OC_AND expressao {$$ = tree_Add(IKS_AST_LOGICO_E, NULL, 2, $1, $3);}
| expressao TK_OC_OR expressao {$$ = tree_Add(IKS_AST_LOGICO_OU, NULL, 2, $1, $3);}
| TK_IDENTIFICADOR '(' lista_expressoes ')' {$$ = tree_Add(IKS_AST_CHAMADA_DE_FUNCAO, NULL, 2, $1, $3);}
;

 lista_expressoes : lista_expressoes_nao_vazia {$$ = $1;}| {$$ = NULL;};

%%

int yyerror(char *t) {
printf("Erro de sintaxe na linha %d\n", getLineNumber());
}
