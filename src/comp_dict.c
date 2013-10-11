/**
 * @file comp_dict.c
 * @author Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 16 Sep 2013
 * @brief Funções para o funcionamento do dicionário
 */


#include "comp_dict.h"
#include "semantics.h"
/**
    dict_find_index
    Procura no dicionário uma entrada de texto e retorna o índice caso encontre, se não retorna -1
*/

int dict_find_index(comp_dict_t_p dict, char *text)
{
	int i = 0;
	while(dict != NULL){
		if (strcmp(dict->item->text, text) == 0)
        {
            return i;
        }
		else{
			dict = dict->next;
			i++;
		}
	}
    return -1;
}

/**
    dict_find
    Procura no dicionário uma entrada de texto e retorna o type caso encontre, se não retorna -1

*/
comp_dict_item_t_p dict_find(comp_dict_t_p dict, char *text)
{
	while(dict != NULL){
		if (strcmp(dict->item->text, text) == 0)
        {
            return dict->item;
        }
		else{
			dict = dict->next;
		}
	}
	return NULL;
}


comp_dict_t_p dict_insertEnd(comp_dict_t_p dict, comp_dict_t_p new){
	comp_dict_t_p aux = dict;
	new->next = NULL;
	if(aux!=NULL){
		while(aux->next!=NULL)
			aux = aux->next;
		aux->next = new;
		return dict;
	}
	return new;
}

/**
    dict_argInsert
    Insere um item do dicionario principal em um outro dicionario, como argumnetos de funcao
*/
comp_dict_t_p dict_argInsert(comp_dict_item_t_p arg){
	comp_dict_t_p newReg = (comp_dict_t_p)malloc(sizeof(comp_dict_t));
	newReg->item = arg;
	newReg->next = NULL;
	return newReg;
}

/**
    dict_insert
    Insere um item no dicionário
*/

comp_dict_item_t_p dict_insert(comp_dict_t_p *dict, char *text, int type, int lineNumber)
{
	comp_dict_item_t_p aux = dict_find(*dict, text);
	if(aux != NULL)
		return aux;
	else{

		comp_dict_item_t_p newItem = (comp_dict_item_t_p)malloc(sizeof(comp_dict_item_t));
		comp_dict_t_p newReg = (comp_dict_t_p)malloc(sizeof(comp_dict_t));
		newReg->item = newItem;

		if(type == IKS_SIMBOLO_LITERAL_INT) // Se é do tipo inteiro
		{
		   newReg->item->value.i = atoi(text); // Converte a string para inteiro e copia o valor
		   newReg->item->size = IKS_INT_SIZE;
		}
		else if(type == IKS_SIMBOLO_LITERAL_FLOAT) // Se é do tipo float
		{
		   newReg->item->value.f = atof(text); // Converte a string para float e copia o valor
		   newReg->item->size = IKS_FLOAT_SIZE;
		}
		else if(type == IKS_SIMBOLO_LITERAL_CHAR) // Se é do tipo char
		{
		   text[strlen(text)-1] = '\0';
		   memmove(text, &text[1], strlen(text));
		   newReg->item->value.c = *text; // Converte a string para char e copia o valor
		   newReg->item->size = IKS_CHAR_SIZE;
		}
		else if(type == IKS_SIMBOLO_LITERAL_BOOL) // Se é do tipo boolean
		{
		   if(strcmp(text, "true") == 0)
		   {
			   newReg->item->value.b = 1;
		   }
		   else
		   {
			   newReg->item->value.b = 0;
		   }
		   newReg->item->size = IKS_FLOAT_SIZE;
		}
		else if(type == IKS_SIMBOLO_IDENTIFICADOR) // Se é do tipo identificador
		{
		   newReg->item->value.str = strdup(text); // Copia a string
		}
		else if(type == IKS_SIMBOLO_LITERAL_STRING) // Se é do tipo string
		{
		   text[strlen(text)-1] = '\0';
		   memmove(text, &text[1], strlen(text));
		   newReg->item->value.str = strdup(text); // Copia a string
		   newReg->item->size = IKS_CHAR_SIZE * strlen(newReg->item->value.str);
		}
		// Preenche a estrutura de um item do dicionário
		newReg->item->text = strdup(text);
		newReg->item->type = type;
		newReg->item->usage = ID_NAO_DECLARADO;
		newReg->item->lineNumber = lineNumber;
		newReg->item->scope = NULL;
		*dict = dict_insertEnd(*dict, newReg);

		return newItem;
	}
}

/**
    dict_new
    Aloca um novo dicionário
*/

comp_dict_t* dict_new(void)
{
    return NULL;
}

/**
    dict_free
    Desaloca a memória utilizada por um dicionário
*/
void dict_free(comp_dict_t_p dict)
{
	comp_dict_t_p aux = dict;
	if(dict!=NULL){
		while(dict->next != NULL){
			aux = dict->next;
			free(dict->item->text);
			free(dict->item);
			free(dict);
			dict = aux;
		}
	}
}

/**
    dict_print
    Imprime os itens do dicionário
*/

void dict_print(comp_dict_t* dict)
{
	printf("\nTabela de Símbolos\n");
	printf("\nLinha | Tipo | Tamanho | Uso | Texto \n");

	if(dict != NULL){
		while(dict != NULL){
			printf("%4.d %7.d %9.d %4.d \t%s\n", dict->item->lineNumber, dict->item->type, dict->item->size, dict->item->usage, dict->item->text);
			dict = dict->next;
		}
	}
}
