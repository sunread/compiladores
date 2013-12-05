/**
 * @file comp_list.h
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Estruturas de dados e definições da lista
 */


#ifndef COMP_LIST_H
#define COMP_LIST_H

typedef struct comp_list_t{
	char code[1000];
	char reg[1000];
	int dimension;
	char* label;
	char* command;
	char* operators[3];
	struct comp_list_t *next;
	struct comp_list_t *prev;
} comp_list;


comp_list* list_Create();
comp_list* list_Add(const char* code, comp_list* first);
comp_list* list_AddDimension(int dimension, comp_list* first);
int list_GetArraySize(int type, comp_list* dimensionsList);
comp_list* list_Search(char* code, comp_list* first);
void list_Remove(comp_list* node);
comp_list* list_Concat(comp_list* list1, comp_list* list2);
#endif
