/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_tree.c
   Funções para o funcionamento da arvore
*/
#include <stdlib.h>
#include "comp_tree.h"

/*
	tree_Add
	Adiciona um novo no a uma arvore n-aria
*/
comp_tree* tree_Add(int type, comp_dict_item_t* symbol, comp_tree* node){
	comp_tree* new = malloc(sizeof(comp_tree*));
	new->type = type;
	new->symbol = symbol;
	new->list = nodeListAdd(new->list, node);
	return new;	
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
