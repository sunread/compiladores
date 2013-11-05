/**
* @file comp_tree.h
* @author Fernando Soller Mecking; Mateus Cardoso da Silva
* @date 16 Sep 2013
* @brief Estruturas de dados e definições da arvore
*/




#ifndef COMP_TREE_H
#define COMP_TREE_H
#include "comp_dict.h"

typedef struct nodeList_t{
	struct comp_tree_t* node;
	struct nodeList_t* next;
}nodeList;

typedef struct comp_tree_t {
	int type;
	int dataType;
    int coercion;
    int identifierDeclaration;
	comp_dict_item_t* symbol;
	comp_dict_t_p args;
	struct comp_tree_t* father;
	struct comp_tree_t* broList;
	struct nodeList_t* sonList;
}comp_tree;

comp_tree* ast;
comp_tree* parent;
comp_tree* tree_CreateNode(int type, comp_dict_item_t* symbol);
comp_tree* tree_SearchNode(comp_tree* ast, comp_dict_item_t* symbol, int type);
void tree_AddSon(comp_tree* father, int count, ...);
void tree_AddBro(comp_tree* brother, comp_tree* node);
nodeList* nodeListAdd(nodeList* list, comp_tree* node);
#endif
