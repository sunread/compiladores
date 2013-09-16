/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_graph.c
   Funções para o funcionamento do grafo
*/
#include "comp_graph.h"
#include <stdlib.h>

/*
	graph_InsertEnd
	Adiciona um novo item a lista de adjacencia de um no do grafo
*/
adjList* graph_InsertEnd(comp_graph* node, adjList* list)
{
	adjList* aux = list;
	adjList* newNode = malloc(sizeof(adjList*));
	newNode->node = node;

	if(aux == NULL){
		return newNode;
	}
	else{
		while(aux->next!=NULL){
			aux = aux->next;
		}
		aux->next = newNode;
	}
	return list;
}

/*
	graph_RemoveFirst
	Remove o primeiro item da lista de adjacencia
*/
adjList* graph_RemoveFirst(adjList* list){
	adjList* aux = list->next;
	free(list);
	return aux;
}

/*
	graph_RemoveItem
	Remove um no da lista de adjacencia
*/
adjList* graph_RemoveItem(comp_graph* node, adjList* list){
	adjList* current = list;
	adjList* previous = NULL;
	adjList* aux = list;

	while(current!=NULL){
		if(current->node == node){
			if(previous == NULL)
				 aux = current->next;
			else
				previous->next = current->next;
			free(current);
		}
		else{
			previous = current;
			current = previous->next;
		}
	}
	return aux;
}

/*
	graph_CreateList
	Inicializa como NULL a lista de adjacencia de um no do grafo
*/
adjList* graph_CreateList()
{
	return NULL;
}

/*
	graph_DestroyList
	Desaloca todo o espaco de enderecamento da lista de adjacencia
*/
adjList* graph_DestroyList(adjList* list)
{
	adjList* aux;

	while(list != NULL)
	{
		aux = list->next;
		free(list);
		list = aux;
	}
	return NULL;
}

/*
	graph_Add
	Insere um novo no em um grafo, utilizando uma arvore binaria como estrutura auxiliar
*/
comp_graph* graph_Add(int data, comp_graph* root, adjList* adj){
		comp_graph* newNode = malloc(sizeof(comp_graph));
		newNode->data = data;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->list = adj;
		if(newNode->list!=NULL && root!=NULL){
			adjList* auxList = newNode->list;
			while(auxList != NULL){
				auxList->node->list = graph_InsertEnd(newNode, auxList->node->list);
				auxList = auxList->next;
			}
		}
		if(root!=NULL){
			comp_graph* fatherNode  = root;
			adjList* treeSearch = graph_CreateList();
			treeSearch = graph_InsertEnd(fatherNode, treeSearch);
			while(treeSearch != NULL){
				if(treeSearch->node->left != NULL && treeSearch->node->right != NULL){
					treeSearch = graph_InsertEnd(treeSearch->node->left, treeSearch);
					treeSearch = graph_InsertEnd(treeSearch->node->right, treeSearch);
					treeSearch = graph_RemoveFirst(treeSearch);
				}
				else{
					fatherNode = treeSearch->node;
					treeSearch = graph_DestroyList(treeSearch);
				}
			}
			if(fatherNode->left == NULL)
				fatherNode->left = newNode;
			else fatherNode->right = newNode;
			return root;
		}
		else{
			return newNode;
		}
}

/*
	graph_Delete
	Remove um no do grafo, assim como todas as referencias a ele em outros nos
*/
comp_graph* graph_Delete(comp_graph* node, comp_graph* root)
{
    adjList* auxList = node->list;

	if(root == NULL)
	{
	    return NULL;
	}
	else
	{
		while(auxList != NULL){
			auxList->node->list = graph_RemoveItem(node, auxList->node->list);
			auxList = auxList->next;
		}
		comp_graph* fatherNode = graph_FindFatherNode(node, root);
		if(node->left == NULL && node->right == NULL){
			if(fatherNode != NULL){
				if(fatherNode->left == node)
					fatherNode->left = NULL;
				else fatherNode->right = NULL;
				free(node);
				return root;
			}
			else{
				free(node);
				return NULL;
			}
		}
		else{
			comp_graph* auxNode = node;
			do{
				while(auxNode->left != NULL)
					auxNode = auxNode->left;
				while(auxNode->right != NULL)
					auxNode = auxNode->right;
			}while(auxNode->left != NULL || auxNode->right !=NULL);
			if(fatherNode != NULL)
				if(fatherNode->left == node)
					fatherNode->left = auxNode;
				else
					fatherNode->right = auxNode;
			else{
				auxNode->right = node->right;
				auxNode->left = node->left;
				free(node);
				return root;
			}
		}
	}
}

/*
	graph_FindFatherNode
	Funcao que identifica um melhor balanceamento da arvore auxiliar
*/
comp_graph* graph_FindFatherNode(comp_graph* node, comp_graph* root){
	comp_graph* auxGraph = root;
	comp_graph* fatherNode = NULL;
	adjList* auxList = graph_InsertEnd(auxGraph, auxList);
	while(auxList != NULL && fatherNode == NULL){
		if(auxList->node->left == node || auxList->node->right == node){
			fatherNode = auxList->node;
			auxList = graph_DestroyList(auxList);
		}
		else{
			auxList = graph_InsertEnd(auxList->node->left, auxList);
			auxList = graph_InsertEnd(auxList->node->right, auxList);
			auxList = graph_RemoveFirst(auxList);
		}
	}
	return fatherNode;
}

/*
	graph_Edit
	
*/
comp_graph* graph_Edit(int new_data, comp_graph* node, comp_graph* root, adjList* adj);

/*
	graph_Contains
	Faz uma busca na arvore auxiliar para determinar que um certo no esta contido no grafo
*/
comp_graph* graph_Contains(comp_graph* node, comp_graph* root){
	if(root == NULL)
			return NULL;
		else{
			if(node == root)
				return node;
			else if(tree_Search(node, root->left) != NULL || tree_Search(node, root->right) != NULL)
					return node;
				else return NULL;
		}
}

/*
	graph_Create
	Inicializa um grafo vazio como sendo NULL
*/
comp_graph* graph_Create(){
	return NULL;
}

/*
	graph_Destroy

*/
void graph_Destroy(comp_graph* root);
