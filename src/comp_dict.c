/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Lucas Herbert Jones
   Mateus Cardoso da Silva
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <comp_dict.h>


int dict_find_index(comp_dict_t_p dict, char *text)
{
    int i;

    for (i = 0; i < dict->length; i++)
    {
        if (strcmp(dict->item[i].text, text) == 0)
        {
            return i;
        }
    }
    return -1;
}

int dict_find(comp_dict_t_p dict, char *text)
{
    int index = dict_find_index(dict, text);

    if(index != -1)
    {
        return dict->item[index].token;
    }

    return -1;
}

void dict_insert(comp_dict_t_p dict, char *text, int token, int lineNumber)
{
   int index = dict_find_index(dict, text);

   if (index == -1)
   {
       if (dict->length == dict->capacity)
       {
          dict->capacity *= 2;
          dict->item = (comp_dict_item_t*) realloc(dict->item, dict->capacity * sizeof(comp_dict_item_t));
       }

       dict->item[dict->length].text = strdup(text);
       dict->item[dict->length].token = token;
       dict->item[dict->length].lineNumber = lineNumber;
       dict->length++;
   }
}

comp_dict_t* dict_new(void)
{
    comp_dict_t proto = {0, 10, (comp_dict_item_t*) malloc(10 * sizeof(comp_dict_item_t))};
    comp_dict_t_p dict = (comp_dict_t*) malloc(sizeof(comp_dict_t));
    *dict = proto;
    return dict;
}

void dict_free(comp_dict_t_p dict)
{
    int i;
    for (i = 0; i < dict->length; i++)
    {
        free(dict->item[i].text);
    }
    free(dict->item);
    free(dict);
}

void dict_print(comp_dict_t* dict)
{
    printf("\nTabela de Símbolos\n");
    printf("\nLinha | Token | Texto\n");

    int i;
    for (i = 0; i < dict->length; i++)
    {
        printf("%4.d %7.d \t%s\n", dict->item[i].lineNumber, dict->item[i].token, dict->item[i].text);
    }
}
