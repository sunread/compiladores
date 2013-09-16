%{
#include <stdio.h>

int yylex(void);
void yyerror(char*);
int lineNumber;
%}

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
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%
 /* Regras (e ações) da gramática da Linguagem K */

s: global | func ;
global: tipo ':' nome vetor ';';
nome: TK_LIT_STRING | TK_LIT_CHAR;
vetor: '[' TK_LIT_INT ']' | /* empty */;
tipo: TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING;
func: cabec local corpo;
cabec: tipo ':' nome '(' param ')';
param: /*empty*/ | tipo ':' nome virg param;
virg: ',' | /*empty*/
local: tipo ':' nome ';';
corpo: '{' bloco '}';
bloco: comando | comando bloco;
comando: atrib | fluxo | entr | saida | ret | chamfunc | /*empty*/;
atrib: nome '=' expres ';' | nome '[' expres ']' ';';
entr: TK_PR_INPUT nome;
saida: TK_PR_OUTPUT listaSaida;
listaSaida: /*empty*/ | TK_LIT_STRING virg listaSaida | expres virg listaSaida;
ret: TK_PR_RETURN espres
espres: 
%%

void yyerror(char* error)
{
  printf("Erro %s na linha %d \n",error, lineNumber);
  exit(3);
}
