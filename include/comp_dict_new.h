/**
 * @file comp_dict.h
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Estruturas de dados e definições do dicionário
 */

#ifndef COMP_DICT_H
#define COMP_DICT_H

#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

union dataValue
{
    int i;
    float f;
    char c;
    char *str;
}dataValue;

typedef struct comp_dict_item_t
{
    int type;
    char *text;
    union dataValue value;
    int lineNumber;
} comp_dict_item_t,*comp_dict_item_t_p;

typedef struct comp_dict_t
{
    int length;
    int capacity;
    comp_dict_item_t_p item;
} comp_dict_t, *comp_dict_t_p;


#endif

