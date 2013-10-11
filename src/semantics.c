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
	//uso correto de identificadores
	result = astTypeInference(ast); //inserir tipos e tamanho de dados
	result = astTypeCoercion(ast);//inserir coercao de tipos
	//inserir verificacao de tipos em comandos
	return result;
}

void printError(int errorCode, int line){
	switch(errorCode){
		case IKS_ERROR_DECLARED: printf("Erro semantico: identificador já declarado na linha %d\n", line);break;
		case IKS_ERROR_UNDECLARED: printf("Erro semantico: identificador nao declarado neste escopo\n");break;
		case IKS_ERROR_VARIABLE: printf("Erro semantico: mal uso da variavel declarada na linha %d\n", line);break;
		case IKS_ERROR_VECTOR: printf("Erro semantico: mal uso do vetor declarado na linha %d\n", line);break;
		case IKS_ERROR_FUNCTION: printf("Erro semantico:  mal uso da funcao declarada na linha %d\n", line);break;
		case IKS_ERROR_MISSING_ARGS: printf("Erro semantico: faltam argumentos para funcao declarada na linha %d\n", line);break;
		case IKS_ERROR_EXCESS_ARGS: printf("Erro semantico: sobram argumentos para funcao declarada na linha %d\n", line);break;
		case IKS_ERROR_WRONG_TYPE_ARGS:printf("Erro semantico: argumentos incompatíveis para funcao declarada na linha %d\n", line);break;
	}
	if(errorCode != IKS_SUCCESS)
		;//exit(errorCode);
}

int verifyDeclaration(comp_dict_item_t* decl){
	if(localScope == NULL && decl->scope != NULL) //acessando declaracao local no escopo global
		return IKS_ERROR_UNDECLARED;
	if(decl->scope != localScope) //acessando variavel local de outra funcao
		return IKS_ERROR_UNDECLARED;
	if(decl->scope == localScope && decl->usage != ID_NAO_DECLARADO){
		if(localScope != NULL){
			if(dict_find(localScope->ast_node->args, decl->text) != NULL)
				return IKS_ERROR_DECLARED;
		}
		else if(dict_find(dictionary, decl->text) != NULL)
				return IKS_ERROR_DECLARED;
	}
	return IKS_SUCCESS;
}

/**
 * Funcao de verificacao do uso correto dos identificadores,
 * eh necessario que ja tenha ocorrida a verificacao de suas declaracoes
 * e de escopo
 */
int verifyIdentifier(comp_dict_item_t* id, int usingAs){
	if(id->usage != usingAs){
		if(id->usage == ID_VARIAVEL)
			return IKS_ERROR_VARIABLE;
		else if(id->usage == ID_VETOR)
			return IKS_ERROR_VECTOR;
		else if(id->usage == ID_FUNCAO)
			return IKS_ERROR_FUNCTION;
	}
	else return IKS_SUCCESS;
}

/**
    typeInference
    Infere um tipo baseado em outros dois passados
*/
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

/**
    printType
    Imprime o nome do tipo de acordo com o parâmetro passado
*/
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

/**
    aritmeticInference
    Infere o tipo das expressões aritméticas
*/
int aritmeticInference(comp_tree* aux)
{

     if(aux->sonList->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->node->sonList->node->dataType;
     }
     else if(aux->sonList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->next->node->sonList->node->dataType;
     }
     else if(aux->sonList->node->type == IKS_AST_ARIM_INVERSAO || aux->sonList->next->node->type == IKS_AST_ARIM_INVERSAO)
     {
         aux->dataType = aux->sonList->node->dataType;
     }
     else
     {
         aux->dataType = typeInference(aux->sonList->node->dataType, aux->sonList->next->node->dataType);
     }
}

/**
    astTypeInference
    Infere dos nós da árvore
*/
int astTypeInference(comp_tree* ast){

	if(ast==NULL)
	{
	    return IKS_SUCCESS;
	}
    else
    {
        comp_tree* aux = ast;
        int firstType;
        int secondType;

        // Processing current node

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
            aux->dataType = aux->sonList->node->dataType;
		}
		else if(aux->type == IKS_AST_ATRIBUICAO)
		{
            aux->dataType = aux->sonList->node->dataType;
		}
		else if(aux->type == IKS_AST_VETOR_INDEXADO)
		{
            aux->dataType = aux->sonList->node->dataType;
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
		    aux->dataType = aux->sonList->node->dataType;
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

		return IKS_SUCCESS;
    }
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

/**
 * Funcao de verificacao do uso correto dos parametros nas chamadas de funcao
 */
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
