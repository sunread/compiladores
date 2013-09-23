/**
 * @file comp_tree.h
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Estruturas de dados e definições da arvore
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
	struct comp_tree_t* broList;
	struct nodeList_t* sonList;
}comp_tree;

comp_tree* ast;
comp_tree* parent;
comp_tree* tree_CreateNode(int type, comp_dict_item_t* symbol);
void tree_AddSon(comp_tree* father, int count, ...);
void tree_AddBro(comp_tree* brother, comp_tree* node);
nodeList* nodeListAdd(nodeList* list, comp_tree* node);
#endif
