/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_list.h
   Estruturas de dados e definições da lista
*/
typedef struct comp_list_t{
	int data;
	struct comp_list_t *next;
	struct comp_list_t *prev;
} comp_list_t;


comp_list_t* list_Create();
comp_list_t* list_Add(int data, comp_list_t* first);
comp_list_t* list_Search(int data, comp_list_t* first);
comp_list_t* list_Remove(comp_list_t* node);
comp_list_t* list_Concat(comp_list_t* list1, comp_list_t* list2);
