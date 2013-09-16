#include <stdlib.h>
#include "comp_tree.h"


 void tree_Add(int data, comp_tree* father_node){
	comp_tree* new = malloc(sizeof(comp_tree*));
	new->data = data;
	new->list = NULL;
	father_node->list = nodeListAdd(father_node->list, new);
	
}

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

comp_tree* tree_Delete(comp_tree* node, comp_tree* root){
		if(tree_Search(node, root)!=NULL && (node->esq == NULL && node->dir == NULL)){
			free(node);
			return root;
		}
		else return NULL;

}

comp_tree* tree_Edit(int new_data, comp_tree* node, comp_tree* root){
	if(tree_Search(node, root)!=NULL){
		node->data = new_data;
	}
	return node;
}

comp_tree* tree_Search(comp_tree* node, comp_tree* root){
	if(root == NULL)
		return NULL;
	else{
		if(node == root)
			return node;
		else if(tree_Search(node, root->esq) != NULL || tree_Search(node, root->dir) != NULL)
				return node;
			else return NULL;
	}
}

comp_tree* tree_Create(){
	return NULL;
}
void tree_Destroy(comp_tree* root){
	if(root!=NULL){
		tree_Destroy(root->esq);
		tree_Destroy(root->dir);
		free(root);
	}
}

