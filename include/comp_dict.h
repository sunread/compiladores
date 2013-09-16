/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_dict.h
   Estruturas de dados e definições do dicionário
*/

union dataValue
{
    int i;
    float f;
    char c;
    char *str;
}dataValue;

typedef struct comp_dict_item_t
{
    int token;
    char *text;
    union dataValue value;
    int lineNumber;
} comp_dict_item_t,*comp_dict_item_t_p;

typedef struct comp_dict_t
{
    int length;
    int capacity;
    comp_dict_item_t *item;
} comp_dict_t, *comp_dict_t_p;

comp_dict_t* dictionary;
int dict_find_index();
int dict_find();
comp_dict_item_t_p dict_insert();
comp_dict_t* dict_new();
void dict_free();
void dict_print();

