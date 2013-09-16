/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Lucas Herbert Jones
   Mateus Cardoso da Silva
*/

/* Declarações*/

%x MLCOMMENT
%option noyywrap

%{

#include "tokens.h"
#include "comp_dict.h"

#define IKS_SIMBOLO_INDEFINIDO		0
#define IKS_SIMBOLO_LITERAL_INT		1
#define IKS_SIMBOLO_LITERAL_FLOAT 	2
#define IKS_SIMBOLO_LITERAL_CHAR 	3
#define IKS_SIMBOLO_LITERAL_STRING	4
#define IKS_SIMBOLO_LITERAL_BOOL	5
#define IKS_SIMBOLO_IDENTIFICADOR	6
%}

/* Expressões Regulares */

ID          [_a-zA-Z][_a-zA-Z0-9]*
INT         (\-)?[0-9]+
FLOAT       (\-)?[0-9]+"."[0-9]+
CHAR        ("\'")[^\']?("\'")
STRING      ("\"")[^\"]*("\"")
WHITESPACE  [ \t\v\f]*


/* Regras */
%%
\n			    { yylineno++; }
{WHITESPACE}    { }
"int"           { return TK_PR_INT; }
"float"         { return TK_PR_FLOAT; }
"bool"          { return TK_PR_BOOL; }
"char"          { return TK_PR_CHAR; }
"string"        { return TK_PR_STRING; }
"if"            { return TK_PR_IF; }
"then"          { return TK_PR_THEN; }
"else"          { return TK_PR_ELSE; }
"while"         { return TK_PR_WHILE; }
"do"            { return TK_PR_DO; }
"input"         { return TK_PR_INPUT; }
"output"        { return TK_PR_OUTPUT; }
"return"        { return TK_PR_RETURN; }
"false"         { return TK_LIT_FALSE; }
"true"          { return TK_LIT_TRUE; }
","|";"|":"     { return yytext[0]; }
"("|")"	        { return yytext[0]; }
"["|"]"         { return yytext[0]; }
"{"|"}"         { return yytext[0]; }
"+"|"-"         { return yytext[0]; }
"*"|"/"         { return yytext[0]; }
"<"|">"	        { return yytext[0]; }
"="|"!"  	    { return yytext[0]; }
"&"|"$"  	    { return yytext[0]; }
"<="		    { return TK_OC_LE; }
">="		    { return TK_OC_GE; }
"=="		    { return TK_OC_EQ; }
"!="		    { return TK_OC_NE; }
"&&"		    { return TK_OC_AND; }
"||"		    { return TK_OC_OR; }
{ID}            { dict_insert(dictionary, yytext, TK_IDENTIFICADOR, yylineno); return TK_IDENTIFICADOR; }
{INT}           { dict_insert(dictionary, yytext, TK_LIT_INT, yylineno); return TK_LIT_INT; }
{FLOAT}         { dict_insert(dictionary, yytext, TK_LIT_FLOAT, yylineno); return TK_LIT_FLOAT; }
{CHAR}          { dict_insert(dictionary, yytext, TK_LIT_CHAR, yylineno); return TK_LIT_CHAR; }
{STRING}        { dict_insert(dictionary, yytext, TK_LIT_STRING, yylineno); return TK_LIT_STRING; }
"/*"	        { BEGIN( MLCOMMENT ); }
"//"[^\r\n]*    { }
<MLCOMMENT>"*/" { BEGIN( INITIAL ); }
<MLCOMMENT>.    {  }
<MLCOMMENT>\n	{ yylineno++; }
.               { return TOKEN_ERRO; }
%%

int getLineNumber(void){
  return yylineno;
}

void setYyin(FILE *input)
{
    yyin = input;
}

FILE* getYyin(void)
{
    return yyin;
}


