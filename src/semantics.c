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
	result = astTypeCoercion(ast);//inserir coercao de tipos
	result = verifyArguments(ast, ast);
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

const char* printType(int type)
{
    if(type == IKS_INT)
    {
        return "INT";
    }
    else if(type == IKS_FLOAT)
    {
        return "FLOAT";
    }
    else if(type == IKS_CHAR)
    {
        return "CHAR";
    }
    else if(type == IKS_STRING)
    {
        return "STRING";
    }
    else if(type == IKS_BOOL)
    {
        return "BOOL";
    }
}

int aritmeticInference(comp_tree* aux)
{
     if(aux->sonList->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->node->sonList->node->symbol->type;
     }
     else if(aux->sonList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->next->node->sonList->node->symbol->type;
     }
     else if(aux->sonList->node->type == IKS_AST_ARIM_INVERSAO || aux->sonList->next->node->type == IKS_AST_ARIM_INVERSAO)
     {
         aux->dataType == aux->sonList->node->type;
     }
     else
     {
         aux->dataType = typeInference(aux->sonList->node->symbol->type, aux->sonList->next->node->symbol->type);
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

        if(aux->type == IKS_AST_LITERAL)
		{
		    aux->dataType = aux->symbol->type;
		}
		else if(aux->type == IKS_AST_IDENTIFICADOR)
		{
		    aux->dataType = aux->symbol->type;
		}
		else if(aux->type == IKS_AST_CHAMADA_DE_FUNCAO)
		{
            aux->dataType == aux->sonList->node->symbol->type;
		}
		else if(aux->type == IKS_AST_ARIM_SOMA)
		{
            aritmeticInference(aux);
		}
		else if(aux->type == IKS_AST_ARIM_SUBTRACAO)
		{
		    aritmeticInference(aux);
		}
		else if(aux->type == IKS_AST_ARIM_MULTIPLICACAO)
		{
		    aritmeticInference(aux);
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

int astTypeCoercion(comp_tree* ast){
    if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	int result = IKS_SUCCESS;
	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node
        
		if(aux->type == IKS_AST_ARIM_SOMA)
		{
            aux->dataType = typeInference(aux->sonList->node->symbol->type, aux->sonList->next->node->symbol->type);
            if (aux->dataType == IKS_INT) {
                if (aux->sonList->node->symbol->type == IKS_BOOL || aux->sonList->node->symbol->type == IKS_SIMBOLO_LITERAL_BOOL ){
                    aux->sonList->node->coercion = COERCION_TO_INT;
                }
            }
            if (aux->dataType == IKS_FLOAT) {
                if (aux->sonList->node->symbol->type == IKS_INT || aux->sonList->node->symbol->type == IKS_SIMBOLO_LITERAL_INT ){
                    aux->sonList->node->coercion = COERCION_TO_INT;
                }
                if (aux->sonList->node->symbol->type == IKS_BOOL || aux->sonList->node->symbol->type == IKS_SIMBOLO_LITERAL_BOOL ){
                    aux->sonList->node->coercion = COERCION_TO_INT;
                }
            }
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


int isAritmeticExpression(int type)
{
    if(type == IKS_AST_ARIM_SOMA)
    {
        return 1;
    }
    else if(type == IKS_AST_ARIM_SUBTRACAO)
    {
        return 1;
    }
    else if(type == IKS_AST_ARIM_MULTIPLICACAO)
    {
        return 1;
    }
    else if(type == IKS_AST_ARIM_DIVISAO)
    {
        return 1;
    }
    else if(type == IKS_AST_ARIM_INVERSAO)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int functionType;
int verifySimpleCommand(comp_tree* ast){
	if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	comp_tree* auxBrother;
	int result = IKS_SUCCESS;

	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node
		if(aux->type == IKS_AST_FUNCAO)
		{
			functionType = aux->dataType;
		}
		else if(aux->type == IKS_AST_RETURN)
		{
		    if(aux->sonList->node->dataType != functionType)
		    {
		        printf("O tipo do retorno(%s) é diferente do tipo da função (%s)\n", printType(aux->sonList->node->dataType), printType(functionType));
		        return IKS_ERROR_WRONG_PAR_RETURN;
		    }
		}
		else if(aux->type == IKS_AST_INPUT)
		{
            if(aux->sonList->node->type != IKS_AST_IDENTIFICADOR)
		    {
		        printf("O parametro do INPUT não é um IDENTIFICADOR\n");
		        return IKS_ERROR_WRONG_PAR_INPUT;
		    }
		}
		else if(aux->type == IKS_AST_OUTPUT)
		{
            if((aux->sonList->node->dataType == IKS_STRING) || isAritmeticExpression(aux->sonList->node->type))
            {
                if(aux->sonList->node->broList != NULL)
                {
                    auxBrother = aux->sonList->node->broList;

                    while(auxBrother != NULL)
                    {
                        if((auxBrother->dataType == IKS_STRING) || isAritmeticExpression(auxBrother->type))
                        {
                            auxBrother = auxBrother->broList;
                        }
                        else
                        {
                            printf("O parametro do OUTPUT não é uma STRING ou EXPRESSÃO ARITMÉTICA\n");
                            return IKS_ERROR_WRONG_PAR_OUTPUT;
                        }
                    }
                }
            }
            else
            {
                printf("O parametro do OUTPUT não é uma STRING ou EXPRESSÃO ARITMÉTICA\n");
                return IKS_ERROR_WRONG_PAR_OUTPUT;
            }
		}
		//processing all sons
		while(auxList!=NULL){
			if(auxList->node!=NULL){
				result = verifySimpleCommand(auxList->node);
				if(result != IKS_SUCCESS)
					return result;
			}
			auxList = auxList->next;
		}

		//go to next brother
		aux = aux->broList;
	}
}

int verifyGivenParameters(comp_tree* func, comp_tree* call){
    if(func != NULL){
		nodeList* firstSon = call->sonList->next;
		comp_dict_t_p declaredArguments = func->args;
		if(declaredArguments != NULL && firstSon != NULL){
			if(declaredArguments->item->type != firstSon->node->dataType)
				return IKS_ERROR_WRONG_TYPE_ARGS;
			declaredArguments = declaredArguments->next;
			comp_tree* brothers = firstSon->node->broList;    
			while(brothers!=NULL){
				if(declaredArguments != NULL){
					if(declaredArguments->item->type != brothers->dataType)
						return IKS_ERROR_WRONG_TYPE_ARGS;
					declaredArguments = declaredArguments->next;
					brothers = brothers->broList;
				}
				else return IKS_ERROR_EXCESS_ARGS;
			}
			if(declaredArguments!= NULL)
				return IKS_ERROR_MISSING_ARGS;
			else return IKS_SUCCESS;
		}
		else if(firstSon != NULL && declaredArguments == NULL)
			return IKS_ERROR_EXCESS_ARGS;
		else if(firstSon == NULL && declaredArguments != NULL)
			return IKS_ERROR_MISSING_ARGS;
	}
}
/**
 * Funcao de verificacao do uso correto dos parametros nas chamadas de funcao
 */
int verifyArguments(comp_tree* ast, comp_tree* fullTree){
	if(ast==NULL)
		return IKS_SUCCESS;
	comp_tree* aux = ast;
	nodeList* auxList;
	comp_tree* auxBrother;
	int result = IKS_SUCCESS;

	while(aux != NULL){
		auxList = aux->sonList;
		//processing current node
		if(aux->type == IKS_AST_CHAMADA_DE_FUNCAO){
			currentFunction = tree_SearchNode(fullTree, aux->sonList->node->symbol, IKS_AST_FUNCAO);
			if(currentFunction != NULL)
				result = verifyGivenParameters(currentFunction, aux);
			if(result != IKS_SUCCESS)
					return result;
		}
		//processing all sons
		while(auxList!=NULL){
			if(auxList->node!=NULL){
				result = verifyArguments(auxList->node, fullTree);
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
