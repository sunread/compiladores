/**
 * @file comp_dict.h
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Estruturas de dados e definições do dicionário
 */

#ifndef COMP_DICT_H
#define COMP_DICT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

#define ID_NAO_DECLARADO 0
#define ID_VARIAVEL 1
#define ID_VETOR 2
#define ID_FUNCAO 3
#define ID_PARAMETRO 4

union dataValue
{
    int i; // Integer Value
    float f; // Float Value
    int b; // Boolean Value
    char c; // Char value
    char *str; // String Value
}dataValue;

typedef struct comp_dict_item_t
{
    int type;
    int usage;
    char *text;
    union dataValue value;
    int lineNumber;
    int size;
    int offset;
    struct comp_dict_item_t* scope; //se NULL o escopo eh GLOBAL
    struct comp_tree_t* ast_node;
} comp_dict_item_t,*comp_dict_item_t_p;

typedef struct comp_dict_t
{
    comp_dict_item_t *item;
    struct comp_dict_t* next;
} comp_dict_t, *comp_dict_t_p;

comp_dict_t* dictionary;
int dict_find_index();
comp_dict_item_t_p dict_find();
comp_dict_item_t_p dict_insert();
comp_dict_t_p dict_argInsert(comp_dict_item_t_p);
comp_dict_t_p dict_insertEnd();
comp_dict_t* dict_new();
void dict_free();
void dict_print();

#endif

