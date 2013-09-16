/* Projeto de Compiladores 2013-2 - Etapa 0
   Fernando Soller Mecking
   Mateus Cardoso da Silva

   comp_dict.c
   Funções para o funcionamento do dicionário
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comp_dict.h"
/*
    dict_find_index
    Procura no dicionário uma entrada de texto e retorna o índice caso encontre, se não retorna -1
*/

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

/*
    dict_find
    Procura no dicionário uma entrada de texto e retorna o token caso encontre, se não retorna -1

*/
int dict_find(comp_dict_t_p dict, char *text)
{
    int index = dict_find_index(dict, text);

    if(index != -1)
    {
        return dict->item[index].token;
    }

    return -1;
}

/*
    dict_insert
    Insere um item no dicionário
*/

comp_dict_item_t_p dict_insert(comp_dict_t_p dict, char *text, int token, int lineNumber, char *variableType)
{
   int index = dict_find_index(dict, text); // Procura o item
   comp_dict_item_t_p itemPointer;

   if (index == -1)
   {
       if (dict->length == dict->capacity) // Se o tamanho está igual a capacidade
       {
          dict->capacity *= 2; // Dobra a capacidade
          dict->item = (comp_dict_item_t*) realloc(dict->item, dict->capacity * sizeof(comp_dict_item_t));
       }

       if(strcmp(variableType, "i") == 0) // Se é do tipo inteiro
       {
           dict->item[dict->length].value.i = atoi(text); // Converte a string para inteiro e copia o valor
       }
       else if(strcmp(variableType, "f") == 0) // Se é do tipo float
       {
           dict->item[dict->length].value.f = atof(text); // Converte a string para float e copia o valor
       }
       else if(strcmp(variableType, "c") == 0) // Se é do tipo char
       {
           dict->item[dict->length].value.c = text[0]; // Converte a string para char e copia o valor
       }
       else if(strcmp(variableType, "s") == 0) // Se é do tipo string
       {
           dict->item[dict->length].value.str = strdup(text); // Copia a string
       }

       // Preenche a estrutura de um ítem do dicionário
       dict->item[dict->length].text = strdup(text);
       dict->item[dict->length].token = token;
       dict->item[dict->length].lineNumber = lineNumber;
       dict->length++;

       *itemPointer = dict->item[dict->length];

       return itemPointer; // Retorna o ponteiro para o item criado
   }
}

/*
    dict_new
    Aloca um novo dicionário
*/

comp_dict_t* dict_new(void)
{
    comp_dict_t proto = {0, 10, (comp_dict_item_t*) malloc(10 * sizeof(comp_dict_item_t))};
    comp_dict_t_p dict = (comp_dict_t*) malloc(sizeof(comp_dict_t));
    *dict = proto;
    return dict;
}

/*
    dict_free
    Desaloca a memória utilizada por um dicionário
*/
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

/*
    dict_print
    Imprime os itens do dicionário
*/

void dict_print(comp_dict_t* dict)
{
    printf("\nTabela de Símbolos\n");
    printf("\nLinha | Tipo | Texto \n");

    int i;
    for (i = 0; i < dict->length; i++)
    {
        printf("%4.d %7.d \t%s\n", dict->item[i].lineNumber, dict->item[i].token, dict->item[i].text);
    }
}
