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
%type<ast> dec_global
%type<ast> dec_variavel
%type<ast> dec_vetor
%type<ast> tipo_variavel
%type<ast> dec_funcao
%type<ast> cabecalho
%type<ast> lista_param
%type<ast> lista_param_nao_vazia
%type<ast> parametro
%type<ast> dec_local
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

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc TK_PR_THEN ';'
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem IKS */

 programa : dec_global programa
			| dec_funcao programa {$$ = tree_Add(IKS_AST_PROGRAMA, NULL, 1, $1);}
			| {$$ = NULL};
			
 dec_global : dec_variavel ';' | dec_vetor ';' ;
 dec_variavel : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_vetor : tipo_variavel ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']' ;
 tipo_variavel : TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING ;
 
 dec_funcao : cabecalho dec_local bloco_comando {$$ = tree_Add(IKS_AST_FUNCAO, NULL, 1, $3);};
 
 cabecalho : tipo_variavel ':' TK_IDENTIFICADOR '(' lista_param ')'	;
 lista_param : lista_param_nao_vazia | ;
 lista_param_nao_vazia : parametro ',' lista_param_nao_vazia | parametro ;
 parametro : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_local : dec_variavel ';' dec_local	| ;
 
 bloco_comando: '{' sequencia_comandos '}' {$$ = $2;};
 
 sequencia_comandos : comando {$$ = $1;}| comando';' sequencia_comandos {$$ = tree_Add(IKS_AST_BLOCO, NULL, 2, $1, $3);};
 
 comando: bloco_comando 	{$$ = $1;}
			| dec_variavel 
			| dec_vetor 
			| controle_fluxo {$$ = $1;}
			| atribuicao 	{$$ = $1;}
			| input 		{$$ = $1;}
			| output 		{$$ = $1;}
			| return 		{$$ = $1;};
			
 atribuicao : TK_IDENTIFICADOR '=' expressao {$$ = tree_Add(IKS_AST_ATRIBUICAO, NULL, 2, $1, $3);}
			| TK_IDENTIFICADOR '[' expressao ']' '=' expressao {$$ = tree_Add(IKS_AST_ATRIBUICAO, NULL, 2, $1, $6);};
			
 input : TK_PR_INPUT TK_IDENTIFICADOR {$$ = tree_Add(IKS_AST_INPUT, NULL, 1, $2);};
 
 output : TK_PR_OUTPUT lista_expressoes_nao_vazia ;
 
 lista_expressoes_nao_vazia: expressao ',' lista_expressoes_nao_vazia | expressao ;
 return : TK_PR_RETURN expressao ;
 controle_fluxo : TK_PR_IF '(' expressao ')' TK_PR_THEN comando |
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' |
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando |
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE comando |
                  TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE ';' |
                  TK_PR_IF '(' expressao ')' TK_PR_THEN ';' TK_PR_ELSE ';' |

                  TK_PR_WHILE '(' expressao ')' TK_PR_DO comando |
                  TK_PR_WHILE '(' expressao ')' TK_PR_DO ';' |
                  TK_PR_DO comando TK_PR_WHILE '(' expressao ')' |
                  TK_PR_DO ';' TK_PR_WHILE '(' expressao ')' ;
 expressao : TK_IDENTIFICADOR
| TK_IDENTIFICADOR '[' expressao ']'
| TK_LIT_INT
| TK_LIT_FLOAT
| TK_LIT_FALSE
| TK_LIT_TRUE
| TK_LIT_CHAR
| TK_LIT_STRING
| expressao '+' expressao
| expressao '-' expressao
| '-' expressao
| expressao '*' expressao
| expressao '/' expressao
| expressao '<' expressao
| expressao '>' expressao
| '(' expressao ')'
| expressao TK_OC_LE expressao
| expressao TK_OC_GE expressao
| expressao TK_OC_EQ expressao
| expressao TK_OC_NE expressao
| expressao TK_OC_AND expressao
| expressao TK_OC_OR expressao
| TK_IDENTIFICADOR '(' lista_expressoes ')'
;

 lista_expressoes : lista_expressoes_nao_vazia | ;

%%

int yyerror(char *t) {
printf("Erro de sintaxe na linha %d\n", getLineNumber());
exit(IKS_SYNTAX_ERRO);
}
