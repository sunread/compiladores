/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Lucas Herbert Jones
   Mateus Cardoso da Silva
*/

typedef struct comp_dict_item_t
{
    int token;
    char *text;
    int lineNumber;
} comp_dict_item_t;

typedef struct comp_dict_t
{
    int length;
    int capacity;
    comp_dict_item_t *item;
} comp_dict_t, *comp_dict_t_p;

comp_dict_t* dictionary;
int dict_find_index();
int dict_find();
void dict_insert();
comp_dict_t* dict_new();
void dict_free();
void dict_print();
