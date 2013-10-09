/**
 * @file semantics.h
 * @author Arthur Foscarini, Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 03 Oct 2013
 * @brief Constantes utilizadas na análise semântica.
 */

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "comp_dict.h"
#include "comp_tree.h"
#include "iks_ast.h"

/** Tipos */
#define IKS_INT 1
#define IKS_FLOAT 2
#define IKS_CHAR 3
#define IKS_STRING 4
#define IKS_BOOL 5

/** Tamanhos **/
#define IKS_INT_SIZE 4
#define IKS_FLOAT_SIZE 8
#define IKS_CHAR_SIZE 1
#define IKS_BOOL_SIZE 1

/**Coerção**/
#define COERCION_TO_FLOAT 1
#define COERCION_TO_BOOL 2
#define COERCION_TO_INT 3

/** Resultados */
#define IKS_SUCCESS 0 //caso não houver nenhum tipo de erro
/* Verificação de declarações */
#define IKS_ERROR_UNDECLARED 1 //identificador não declarado
#define IKS_ERROR_DECLARED 2 //identificador já declarado
/* Uso correto de identificadores */
#define IKS_ERROR_VARIABLE 3 //identificador deve ser utilizado como variável
#define IKS_ERROR_VECTOR 4 //identificador deve ser utilizado como vetor
#define IKS_ERROR_FUNCTION 5 //identificador deve ser utilizado como função
/* Tipos e tamanho de dados */
#define IKS_ERROR_WRONG_TYPE 6 //tipos incompatíveis
#define IKS_ERROR_STRING_TO_X 7 //coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X 8 //coerção impossível do tipo char
/* Argumentos e parâmetros */
#define IKS_ERROR_MISSING_ARGS 9 //faltam argumentos
#define IKS_ERROR_EXCESS_ARGS 10 //sobram argumentos
#define IKS_ERROR_WRONG_TYPE_ARGS 11 //argumentos incompatíveis
/* Verificação de tipos em comandos */
#define IKS_ERROR_WRONG_PAR_INPUT 12 //parâmetro não é identificador
#define IKS_ERROR_WRONG_PAR_OUTPUT 13 //parâmetro não é literal string ou expressão
#define IKS_ERROR_WRONG_PAR_RETURN 14 //parâmetro não é expressão compatível com tipo do retorno

comp_tree* currentFunction;
int semanticEvaluation(comp_tree* ast);
int verifyIdentifier(comp_tree*);
int verifyArguments(comp_tree*, comp_tree*);
int verifyGivenParameters(comp_tree*, comp_tree*);
#endif
