/**
 * @file semantics.c
 * @author Arthur Foscarini, Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 03 Oct 2013
 * @brief Arquivo contendo as funções para avaliação semântica
 */

#include "semantics.h"


int semanticEvaluation(comp_tree* ast){
	int result = IKS_SUCCESS;
	//inserir verificacao das declaracoes
	result = verifyIdentifier(ast); //uso correto de identificadores
	result = astTypeInference(ast); //inserir tipos e tamanho de dados
	//inserir coercao de tipos
	//inserir argumentos e parametros
	//inserir verificacao de tipos em comandos
	return result;
}



/**
 * Funcao de verificacao do uso correto dos identificadores,
 * eh necessario que ja tenha ocorrida a verificacao de suas declaracoes
 * e de escopo
 */
int verifyIdentifier(comp_tree* ast){
	if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	int result = IKS_SUCCESS;
	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node
		if(aux->type == IKS_AST_VETOR_INDEXADO){
			if(auxList->node->symbol->usage != ID_VETOR)
				return IKS_ERROR_VECTOR;
		}
		else if(aux->type == IKS_AST_CHAMADA_DE_FUNCAO){
			if(auxList->node->symbol->usage != ID_FUNCAO)
				return IKS_ERROR_FUNCTION;
		}
		else if(aux->father != NULL
							&& aux->type == IKS_AST_IDENTIFICADOR
							&& aux->father->type != IKS_AST_VETOR_INDEXADO
							&& aux->father->type != IKS_AST_CHAMADA_DE_FUNCAO){
			if(aux->symbol->usage != ID_VARIAVEL)
				return IKS_ERROR_VARIABLE;
		}
		//processing all sons
		while(auxList!=NULL){
			if(auxList->node!=NULL){
				result = verifyIdentifier(auxList->node);
				if(result != IKS_SUCCESS)
					return result;
			}
			auxList = auxList->next;
		}

		//go to next brother
		aux = aux->broList;
	}
	return IKS_SUCCESS;
}


//#define IKS_INT 1
//#define IKS_FLOAT 2
//#define IKS_CHAR 3
//#define IKS_STRING 4
//#define IKS_BOOL 5

int typeInference(int firstType, int secondType)
{
    if(firstType == IKS_INT && secondType == IKS_INT)
    {
        return IKS_INT;
    }
    else if(firstType == IKS_FLOAT && secondType == IKS_FLOAT)
    {
        return IKS_FLOAT;
    }
    else if(firstType == IKS_BOOL && secondType == IKS_BOOL)
    {
        return IKS_BOOL;
    }
    else if((firstType == IKS_FLOAT && secondType == IKS_INT) || (firstType == IKS_INT && secondType == IKS_FLOAT))
    {
        return IKS_FLOAT;
    }
    else if((firstType == IKS_BOOL && secondType == IKS_INT) || (firstType == IKS_INT && secondType == IKS_BOOL))
    {
        return IKS_INT;
    }
    else if((firstType == IKS_BOOL && secondType == IKS_FLOAT) || (firstType == IKS_FLOAT && secondType == IKS_BOOL))
    {
        return IKS_FLOAT;
    }
}


int astTypeInference(comp_tree* ast){
	if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	int result = IKS_SUCCESS;
	int firstType;
	int secondType;
	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node

		if(aux->type == IKS_AST_ARIM_SOMA)
		{
            aux->dataType = typeInference(aux->sonList->node->symbol->type, aux->sonList->next->node->symbol->type);
		}
		else if(aux->type == IKS_AST_ARIM_SUBTRACAO)
		{
		    aux->dataType = typeInference(aux->sonList->node->symbol->type, aux->sonList->next->node->symbol->type);
		}
		else if(aux->type == IKS_AST_ARIM_MULTIPLICACAO)
		{
		    aux->dataType = typeInference(aux->sonList->node->symbol->type, aux->sonList->next->node->symbol->type);
		}
		else if(aux->type == IKS_AST_ARIM_INVERSAO)
		{
		    aux->dataType = aux->sonList->node->symbol->type;
		}
		else if(aux->type == IKS_AST_LOGICO_E)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_OU)
		{
		    aux->dataType = IKS_BOOL;
		}
        else if(aux->type == IKS_AST_LOGICO_COMP_DIF)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_IGUAL)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_LE)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_GE)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_L)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_G)
		{
		    aux->dataType = IKS_BOOL;
		}
		else if(aux->type == IKS_AST_LOGICO_COMP_NEGACAO)
		{
		    aux->dataType = IKS_BOOL;
		}

		//processing all sons
		while(auxList!=NULL){
			if(auxList->node!=NULL){
				result = astTypeInference(auxList->node);
				if(result != IKS_SUCCESS)
					return result;
			}
			auxList = auxList->next;
		}

		//go to next brother
		aux = aux->broList;
	}
	return IKS_SUCCESS;
}

/**
 * Funcao de verificacao do uso correto dos parametros nas chamadas de funcao
 */
int verifyArguments(comp_tree* ast){
	if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	int result = IKS_SUCCESS;
	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node
		if(aux->type == IKS_AST_FUNCAO)
			currentFunction = aux;
		if(aux->type == IKS_AST_CHAMADA_DE_FUNCAO){
			
		
		}
		
		//processing all sons
		while(auxList!=NULL){
			if(auxList->node!=NULL){
				result = verifyArguments(auxList->node);
				if(result != IKS_SUCCESS)
					return result;
			}
			auxList = auxList->next;
		}
		
		//go to next brother
		aux = aux->broList;
	}
	return IKS_SUCCESS;	
}
