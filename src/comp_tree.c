/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_tree.c
   Funções para o funcionamento da arvore
*/
#include <stdlib.h>
#include "comp_tree.h"
#include <stdarg.h>

/*
	tree_Add
	Adiciona um novo no a uma arvore n-aria
*/
comp_tree* tree_Add(int type, comp_dict_item_t* symbol, int count, ...){
	va_list ap;
	int j;
	comp_tree* new = malloc(sizeof(comp_tree*));
	new->type = type;
	new->symbol = symbol;
	va_start(ap, count);
	for(j=0; j<count; j++)
	   new->list = nodeListAdd(new->list, va_arg(ap, comp_tree*));
	return new;	
	va_end(ap);
}
/*
	nodeListAdd
	Adiciona um novo item ao final de uma lista encadeada,que representa os filhos de um no
*/
nodeList* nodeListAdd(nodeList* list, comp_tree* node){
	nodeList* new = malloc(sizeof(nodeList*));
	new->node = node;
	new->next = NULL;
	if(list != NULL){
		nodeList* auxList = list;
		while(auxList->next != NULL){
			auxList = auxList->next;
		}
		auxList->next = new;
		return list;
	}
	else
		return new;
}
