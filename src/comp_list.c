#include "comp_list.h"
#include <stdlib.h>

comp_list* list_Create(){
	return NULL;
}

//adds an new item at the end of an circular list
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

comp_list* list_Search(int data, comp_list* first){
	comp_list* cmpNode = first;
	while(cmpNode->next != first && cmpNode->data != data){
		cmpNode = cmpNode->next;
	}
	if(cmpNode->data == data)
		return cmpNode;
	else return NULL;
}

void list_Remove(comp_list* node){
	node->prev->next = node->next;
	node->next->prev = node->prev;
	free(node);
}

comp_list* list_Concat(comp_list* list1, comp_list* list2){
	comp_list* last1 = list1->prev;
	last1->next = list2;
	list2->prev->next = list1;
	list1->prev = list2->prev;
	list2->prev = last1;
	return list1;
}
