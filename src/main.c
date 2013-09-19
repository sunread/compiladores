/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Mateus Cardoso da Silva
*/

#include <stdio.h>

#include "main.h"

void makeTree(comp_tree *ast);

int main (int argc, char **argv)
{
  int running = 1;
  setYyin(stdin);

  if (argc > 0) {
    if (fopen(argv[1], "r"))
    {
        setYyin(fopen(argv[1], "r"));
    }
  }

  dictionary = dict_new();

  int resultado = yyparse();	
  gv_init("arvore.dot");

  makeTree(ast);

  gv_close();

  dict_print(dictionary);

  return resultado;
}

void makeTree(comp_tree *ast){
	if(ast==NULL)
		return;
	comp_tree* aux = ast;
	nodeList* list = ast->list;
	nodeList* auxList;
	while(list != NULL){
		if(aux->type == IKS_AST_FUNCAO || aux->type == IKS_AST_IDENTIFICADOR || aux->type == IKS_AST_LITERAL)
			gv_declare(aux->type, aux, aux->symbol->text);
		else
			gv_declare(aux->type, aux, NULL);
		gv_connect(aux, list->node);
		makeTree(list->node);
		list = list->next;
	}
}
