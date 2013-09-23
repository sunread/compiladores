/**
 * @file comp_dict.c
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Funções para o funcionamento do dicionário
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comp_dict.h"
/**
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

/**
    dict_find
    Procura no dicionário uma entrada de texto e retorna o type caso encontre, se não retorna -1

*/
int dict_find(comp_dict_t_p dict, char *text)
{
    int index = dict_find_index(dict, text);

    if(index != -1)
    {
        return dict->item[index].type;
    }

    return -1;
}

/**
    dict_insert
    Insere um item no dicionário
*/

comp_dict_item_t_p dict_insert(comp_dict_t_p dict, char *text, int type, int lineNumber)
{

       if(type == IKS_SIMBOLO_LITERAL_INT) // Se é do tipo inteiro
       {
           dict->item[dict->length].value.i = atoi(text); // Converte a string para inteiro e copia o valor
       }
       else if(type == IKS_SIMBOLO_LITERAL_FLOAT) // Se é do tipo float
       {
           dict->item[dict->length].value.f = atof(text); // Converte a string para float e copia o valor
       }
       else if(type == IKS_SIMBOLO_LITERAL_CHAR) // Se é do tipo char
       {
           text[strlen(text)-1] = '\0';
		   memmove(text, &text[1], strlen(text));
           dict->item[dict->length].value.c = *text; // Converte a string para char e copia o valor
       }
       else if(type == IKS_SIMBOLO_LITERAL_BOOL) // Se é do tipo boolean
       {
           if(strcmp(text, "true") == 0)
           {
               dict->item[dict->length].value.b = 1;
           }
           else
           {
               dict->item[dict->length].value.b = 0;
           }
       }
       else if(type == IKS_SIMBOLO_IDENTIFICADOR) // Se é do tipo identificador
       {
           dict->item[dict->length].value.str = strdup(text); // Copia a string
       }
       else if(type == IKS_SIMBOLO_LITERAL_STRING) // Se é do tipo string
       {
		   text[strlen(text)-1] = '\0';
		   memmove(text, &text[1], strlen(text));
           dict->item[dict->length].value.str = strdup(text); // Copia a string
       }

       // Preenche a estrutura de um item do dicionário
       dict->item[dict->length].text = strdup(text);
       dict->item[dict->length].type = type;
       dict->item[dict->length].lineNumber = lineNumber;
       dict->length++;

       return &dict->item[dict->length-1];
}

/**
    dict_new
    Aloca um novo dicionário
*/

comp_dict_t* dict_new(void)
{
    comp_dict_t proto = {0, 50, (comp_dict_item_t*) malloc(100 * sizeof(comp_dict_item_t))};
    comp_dict_t_p dict = (comp_dict_t*) malloc(sizeof(comp_dict_t));
    *dict = proto;
    return dict;
}

/**
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

/**
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
        printf("%4.d %7.d \t%s\n", dict->item[i].lineNumber, dict->item[i].type, dict->item[i].text);
    }
}
