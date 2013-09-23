/**
 * @file comp_tree.c
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Funções para o funcionamento da arvore
 */

#include <stdlib.h>
#include "comp_tree.h"
#include "comp_dict.h"
#include <stdarg.h>

/**
	tree_CreateNode
	Adiciona um novo no a uma arvore n-aria
*/

comp_tree* tree_CreateNode(int type, comp_dict_item_t* symbol){
	comp_tree* new = (comp_tree*)malloc(sizeof(comp_tree));
	new->type = type;
	new->symbol = symbol;
	new->broList = NULL;
	new->sonList = NULL;
	return new;
}

/**
	tree_AddSon
	Adiciona uma lista de nos como filhos
*/

void tree_AddSon(comp_tree* father, int count, ...){
	va_list ap;
	int j;
	if(count>0){
		va_start(ap, count);
		for(j=0; j<count; j++)
		   father->sonList = nodeListAdd(father->sonList, va_arg(ap, comp_tree*));
		va_end(ap);
	}
}

/**
	tree_AddBro
	Adiciona uma lista de irmaos a um no
*/

void tree_AddBro(comp_tree* brother, comp_tree* node){
	if(brother != NULL)
		brother->broList = node;
}

/**
	nodeListAdd
	Adiciona um novo item ao final de uma lista encadeada
*/

nodeList* nodeListAdd(nodeList* list, comp_tree* node){
	if(node != NULL){
		nodeList* new = (nodeList*)malloc(sizeof(nodeList));
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
	else return list;
}
