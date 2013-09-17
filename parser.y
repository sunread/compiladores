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
    comp_tree AST;
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

%left TK_OC_OR TK_OC_AND
%left '<' '>' TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE
%left '+' '-'
%left '*' '/'

%nonassoc TK_PR_THEN ';'
%nonassoc TK_PR_ELSE

%%
 /* Regras (e ações) da gramática da Linguagem IKS */

 programa : dec_global programa | dec_funcao programa | ;
 dec_global : dec_variavel ';' | dec_vetor ';' ;
 dec_variavel : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_vetor : tipo_variavel ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']' ;
 tipo_variavel : TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING ;
 dec_funcao : cabecalho dec_local bloco_comando ;
 cabecalho : tipo_variavel ':' TK_IDENTIFICADOR '(' lista_param ')' ;
 lista_param : lista_param_nao_vazia | ;
 lista_param_nao_vazia : parametro ',' lista_param_nao_vazia | parametro ;
 parametro : tipo_variavel ':' TK_IDENTIFICADOR ;
 dec_local : dec_variavel ';' dec_local | ;
 bloco_comando: '{' sequencia_comandos '}' ;
 sequencia_comandos : comando | comando';' sequencia_comandos | ;
 comando: bloco_comando | dec_variavel | dec_vetor | controle_fluxo | atribuicao | input | output | return ;
 atribuicao : TK_IDENTIFICADOR '=' expressao | TK_IDENTIFICADOR '[' expressao ']' '=' expressao ;
 input : TK_PR_INPUT TK_IDENTIFICADOR ;
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
