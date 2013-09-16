
typedef struct comp_tree_t {
	int data;
	struct comp_tree_t* esq;
	struct comp_tree_t* dir;
}comp_tree;


comp_tree* tree_Add(int data, comp_tree* father_node ,comp_tree* root);
comp_tree* tree_Delete(comp_tree* node, comp_tree* root);
comp_tree* tree_Edit(int new_data, comp_tree* node, comp_tree* root);
comp_tree* tree_Search(comp_tree* node, comp_tree* root);
comp_tree* tree_Create();
void tree_Destroy(comp_tree* root);
