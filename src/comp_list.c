/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_list.c
   Funções para o funcionamento da lista
*/
#include "comp_list.h"
#include <stdlib.h>
/*
	list_Create
	Seta como NULL o uma lista vazia
*/
comp_list* list_Create(){
	return NULL;
}

/*
	list_Add
	Adiciona um novo item ao final de uma lista duplamente encadeada circular
*/
comp_list* list_Add(int data, comp_list* first){
	comp_list* newItem = malloc(sizeof(comp_list*));

	newItem->data = data;

	if(first != NULL)
	{
		first->prev->next = newItem;
		newItem->prev = first->prev;
		first->prev = newItem;
		newItem->next = first;
		return first;
	}
	else
	{
		newItem->next = newItem;
		newItem->prev = newItem;
		return newItem;
	}
}

/*
	list_Search
	Procura um elemento na lista a partir de um argumento
*/
comp_list* list_Search(int data, comp_list* first){
	comp_list* cmpNode = first;
	while(cmpNode->next != first && cmpNode->data != data){
		cmpNode = cmpNode->next;
	}
	if(cmpNode->data == data)
		return cmpNode;
	else return NULL;
}

/*
	list_Remove
	Remove um item qualquer de uma lista
*/
void list_Remove(comp_list* node){
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
}

/*
	list_Concat
	Concatena duas listas circulares
*/
comp_list* list_Concat(comp_list* list1, comp_list* list2){
	comp_list* last1 = list1->prev;
	last1->next = list2;
	list2->prev->next = list1;
	list1->prev = list2->prev;
	list2->prev = last1;
	return list1;
}
