/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_tree.h
   Estruturas de dados e definições da arvore
*/

#include "comp_dict.h"

#ifndef COMP_TREE_H
#define COMP_TREE_H

typedef struct nodeList_t{
	struct comp_tree_t* node;
	struct nodeList_t* next;
}nodeList;

typedef struct comp_tree_t {
	int type;
	comp_dict_item_t* symbol;
	struct nodeList_t* list;
}comp_tree;

comp_tree* ast;
comp_tree* tree_Add(int type, comp_dict_item_t* symbol, int count, ...);
nodeList* nodeListAdd(nodeList* list, comp_tree* node);
#endif
