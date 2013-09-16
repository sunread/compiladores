/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_tree.h
   Estruturas de dados e definições da arvore
*/

typedef struct nodeList_t{
	struct comp_tree_t* node;
	struct nodeList_t* next;
}nodeList;

typedef struct comp_tree_t {
	int data;
	struct nodeList_t* list;
}comp_tree;


void tree_Add(int data, comp_tree* father_node);
nodeList* nodeListAdd(nodeList* list, comp_tree* node);
comp_tree* tree_Delete(comp_tree* node, comp_tree* root);
comp_tree* tree_Edit(int new_data, comp_tree* node, comp_tree* root);
comp_tree* tree_Search(comp_tree* node, comp_tree* root);
comp_tree* tree_Create();
void tree_Destroy(comp_tree* root);
