/**
 * @file main.c
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Arquivo contendo a classe Main
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

  printf("avaliacao: %d ", semanticEvaluation(ast));
  

  gv_init("saida.dot");

  makeTree(ast);

  gv_close();
  
  dict_print(dictionary);

  return resultado;
}

void makeTree(comp_tree *ast){
	if(ast==NULL)
		return;
	comp_tree* aux = ast;
	nodeList* auxList;
	while(aux != NULL){
		auxList = aux->sonList;
		if(aux->type == IKS_AST_FUNCAO || aux->type == IKS_AST_IDENTIFICADOR || aux->type == IKS_AST_LITERAL)
			gv_declare(aux->type, aux, aux->symbol->text);
		else
			gv_declare(aux->type, aux, NULL);
		while(auxList!=NULL){
			if(auxList->node!=NULL)
				gv_connect(aux, auxList->node);
			makeTree(auxList->node);
			auxList = auxList->next;
		}
		if(aux->broList!=NULL)
				gv_connect(aux, aux->broList);
		aux = aux->broList;
	}
}
