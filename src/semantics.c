/**
 * @file semantics.c
 * @author Arthur Foscarini, Fernando Soller Mecking; Mateus Cardoso da Silva
 * @date 13 Oct 2013
 * @brief Arquivo contendo as funções para avaliação semântica
 */

#include "semantics.h"

/**
**  printError
**  Imprime a mensagem de erro de acordo com o erro passado
*/
void printError(int errorCode, int line)
{
    switch(errorCode)
    {
            case IKS_ERROR_UNDECLARED: printf("Erro semântico na linha %d: Identificador não declarado neste escopo\n", getLineNumber());break;
            case IKS_ERROR_DECLARED: printf("Erro semântico na linha %d: Identificador já declarado na linha %d\n", getLineNumber(),line);break;
            case IKS_ERROR_VARIABLE: printf("Erro semântico na linha %d: Mal uso da variável declarada na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_VECTOR: printf("Erro semântico na linha %d: Mal uso do vetor declarado na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_FUNCTION: printf("Erro semântico na linha %d: Mal uso da função declarada na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_MISSING_ARGS: printf("Erro semântico na linha %d: Faltam argumentos para função declarada na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_EXCESS_ARGS: printf("Erro semântico na linha %d: Sobram argumentos para função declarada na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_WRONG_TYPE_ARGS:printf("Erro semântico na linha %d: Argumentos incompatíveis para função declarada na linha %d\n", getLineNumber(), line);break;
            case IKS_ERROR_WRONG_PAR_RETURN: printf("Erro semântico na linha %d: O tipo de retorno é diferente do tipo da função.\n", getLineNumber()); break;
            case IKS_ERROR_WRONG_PAR_INPUT: printf("Erro semântico na linha %d: O parâmetro do INPUT não é um IDENTIFICADOR.\n", getLineNumber()); break;
            case IKS_ERROR_WRONG_PAR_OUTPUT: printf("Erro semântico na linha %d: O parâmetro do OUTPUT não é uma STRING ou EXPRESSÃO ARITMÉTICA.\n", getLineNumber()); break;
            case IKS_ERROR_CHAR_TO_X: printf("Erro semântico na linha %d: Não é possível converter CHAR.\n", getLineNumber()); break;
            case IKS_ERROR_STRING_TO_X: printf("Erro semântico na linha %d: Não é possível converter STRING.\n", getLineNumber()); break;
            case IKS_ERROR_WRONG_TYPE: printf("Erro semântico na linha %d: Tipos incompatíveis.\n", getLineNumber()); break;
    }

    if(errorCode != IKS_SUCCESS)
	{
		dict_print(dictionary);
	    exit(errorCode);
	}

}

/**
**   verifyDeclaration
**   Verificação das declarações
*/
int verifyDeclaration(comp_dict_item_t* decl, int uso){
	if(localScope == NULL && decl->scope != NULL) //acessando declaracao local no escopo global
	{
	    printError(IKS_ERROR_UNDECLARED, decl->lineNumber);
	}
	if(decl->scope != localScope  && decl->scope != NULL) //acessando variavel local de outra funcao
	{
	    printError(IKS_ERROR_UNDECLARED, decl->lineNumber);
	}
	if(decl->scope == localScope && decl->usage != ID_NAO_DECLARADO && uso==0)
	{
		if(localScope != NULL)
		{
			if(dict_find(localScope->ast_node->args, decl->text) != NULL)
			{
			    printError(IKS_ERROR_DECLARED, decl->lineNumber);
			}
		}
		else if(dict_find(dictionary, decl->text) != NULL)
		{
		   printError(IKS_ERROR_DECLARED, decl->lineNumber);
		}
	}
}

/**
**   verifyIdentifier
**   Função de verificação do uso correto dos identificadores, é necessário que já tenha ocorrida
**   a verificação de suas declarações e escopo.
*/
int verifyIdentifier(comp_dict_item_t* id, int usingAs){
	if(id->usage == ID_NAO_DECLARADO)
	{
	   printError(IKS_ERROR_UNDECLARED, id->lineNumber);
	}
	else if(id->usage == ID_PARAMETRO && usingAs != ID_VARIAVEL)
	{
	    printError(IKS_ERROR_VARIABLE, id->lineNumber);
	}
	else if(id->usage != usingAs && id->usage != ID_PARAMETRO)
	{
		if(id->usage == ID_VARIAVEL)
		{
		    printError(IKS_ERROR_VARIABLE, id->lineNumber);
		}
		else if(id->usage == ID_VETOR)
		{
		    printError(IKS_ERROR_VECTOR, id->lineNumber);
		}
		else if(id->usage == ID_FUNCAO)
		{
		    printError(IKS_ERROR_FUNCTION, id->lineNumber);
		}
	}
	else
	{
        return IKS_SUCCESS;
	}
}

/**
**  typeInference
**  Infere um tipo baseado em outros dois passados
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
**  printType
**  Imprime o nome do tipo de acordo com o parâmetro passado
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
**  aritmeticInference
**  Infere o tipo das expressões aritméticas
*/
int aritmeticInference(comp_tree* aux)
{

     if(aux->sonList->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->node->dataType;
     }
     else if(aux->sonList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO)
     {
         aux->dataType = aux->sonList->next->node->sonList->node->dataType;
     }
     else if(aux->sonList->node->type == IKS_AST_ARIM_INVERSAO || aux->sonList->next->node->type == IKS_AST_ARIM_INVERSAO)
     {
         aux->dataType = aux->sonList->node->dataType;
     }

     aux->dataType = typeInference(aux->sonList->node->dataType, aux->sonList->next->node->dataType);

}

/**
**  astTypeInference
**  Infere dos nós da árvore
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
            aux->sonList->node->dataType = aux->sonList->node->symbol->type;
            aux->dataType = aux->sonList->node->dataType;
		}
		else if(aux->type == IKS_AST_ATRIBUICAO)
		{
			if(aux->sonList->node->type != IKS_AST_VETOR_INDEXADO){
				aux->sonList->node->dataType = aux->sonList->node->symbol->type;
				aux->dataType = aux->sonList->node->dataType;
			}
			else{
				aux->sonList->node->sonList->node->dataType = aux->sonList->node->sonList->node->symbol->type;
				aux->dataType = aux->sonList->node->sonList->node->dataType;
			}
		}
		else if(aux->type == IKS_AST_VETOR_INDEXADO)
		{
            aux->sonList->node->dataType = aux->sonList->node->symbol->type;
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
        else if(aux->type == IKS_AST_ARIM_DIVISAO)
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

		return IKS_SUCCESS;
    }
}

/**
**  aritmeticCoercion
**  Realiza a coerção dos tipos de expressões aritméticas
*/
void aritmeticCoercion(comp_tree* aux)
{
    if (aux->dataType == IKS_INT)
    {
        if (aux->sonList->node->dataType == IKS_BOOL)
        {
            aux->sonList->node->coercion = COERCION_TO_INT;
        }
        if (aux->sonList->next->node->dataType == IKS_BOOL)
        {
            aux->sonList->next->node->coercion = COERCION_TO_INT;
        }
    }

    if (aux->dataType == IKS_FLOAT)
    {
        if (aux->sonList->node->dataType == IKS_INT)
        {
            aux->sonList->node->coercion = COERCION_TO_FLOAT;
        }
        if (aux->sonList->next->node->dataType == IKS_INT)
        {
            aux->sonList->next->node->coercion = COERCION_TO_FLOAT;
        }
        if (aux->sonList->node->dataType == IKS_BOOL)
        {
            aux->sonList->node->coercion = COERCION_TO_FLOAT;
        }
        if (aux->sonList->next->node->dataType == IKS_BOOL)
        {
            aux->sonList->next->node->coercion = COERCION_TO_FLOAT;
        }
    }
}


/**
**  astTypeCoercion
**  Realiza a coerção dos tipos
*/
int astTypeCoercion(comp_tree* ast){
    if(ast==NULL)
    {
        return IKS_SUCCESS;
    }
    else
    {
        comp_tree* aux = ast;
        nodeList* auxList;
		auxList = aux->sonList;
		int sonType, son2Type;
		if(isExpression(aux->type))
			if(aux->sonList->next !=NULL){ //expressoes de dois operandos
				if(auxList->node->type == IKS_AST_CHAMADA_DE_FUNCAO || auxList->node->type == IKS_AST_VETOR_INDEXADO){
					sonType = auxList->node->sonList->node->symbol->type; //tipo fica no filho deste no e nao nele proprio
				}
				else{
					sonType = auxList->node->symbol->type;
				}
				if(auxList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO || auxList->next->node->type == IKS_AST_VETOR_INDEXADO){
					son2Type = auxList->next->node->sonList->node->symbol->type; //tipo fica no filho deste no e nao nele proprio
				}
				else{
					son2Type = auxList->next->node->symbol->type;
				}
				if (sonType == IKS_CHAR || son2Type == IKS_CHAR)
						printError( IKS_ERROR_CHAR_TO_X, 0);
				if (sonType == IKS_STRING || son2Type == IKS_STRING)
						printError( IKS_ERROR_STRING_TO_X, 0);
			}
			else{ //expressoes de um operando
				if(auxList->node->type == IKS_AST_CHAMADA_DE_FUNCAO || auxList->node->type == IKS_AST_VETOR_INDEXADO){
					sonType = auxList->node->sonList->node->symbol->type; //tipo fica no filho deste no e nao nele proprio
				}
				else{
					sonType = auxList->node->symbol->type;
				}
				if (sonType == IKS_CHAR)
						printError( IKS_ERROR_CHAR_TO_X, 0);
				if (sonType == IKS_STRING)
						printError( IKS_ERROR_STRING_TO_X, 0);
			}
		else{ //nodos que nao sao expressao
			if(aux->type == IKS_AST_ATRIBUICAO){
				if(auxList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO || auxList->next->node->type == IKS_AST_VETOR_INDEXADO){
					sonType = auxList->next->node->sonList->node->symbol->type; //tipo fica no filho deste no e nao nele proprio
				}
				else{
					sonType = auxList->next->node->symbol->type;
				}
				if(auxList->next->node->coercion != aux->type){
					if(auxList->node->dataType == IKS_CHAR){
						if(sonType == IKS_STRING)
							printError( IKS_ERROR_STRING_TO_X, 0);
						if(sonType != IKS_CHAR)
							printError( IKS_ERROR_WRONG_TYPE, 0);
					}
					if(auxList->node->dataType == IKS_STRING){
						if(sonType == IKS_CHAR)
							printError( IKS_ERROR_CHAR_TO_X, 0);
						if(sonType != IKS_STRING)
							printError( IKS_ERROR_WRONG_TYPE, 0);
					}
					else{
						if(sonType == IKS_CHAR)
							printError( IKS_ERROR_CHAR_TO_X, 0);
						if(sonType == IKS_STRING)
							printError( IKS_ERROR_STRING_TO_X, 0);
					}
				}
			}
			if(aux->type == IKS_AST_VETOR_INDEXADO){
				if(auxList->next->node->type == IKS_AST_CHAMADA_DE_FUNCAO || auxList->next->node->type == IKS_AST_VETOR_INDEXADO){
					sonType = auxList->next->node->sonList->node->symbol->type; //tipo fica no filho deste no e nao nele proprio
				}
				else{
					sonType = auxList->next->node->symbol->type;
				}
				if(auxList->next->node->coercion != IKS_INT){ //se filho nao faz coercao para inteiro entao nao serve para ser indice
					if(sonType == IKS_STRING)
						printError( IKS_ERROR_STRING_TO_X, 0);
					if(sonType == IKS_CHAR)
						printError( IKS_ERROR_CHAR_TO_X, 0);
					else printError( IKS_ERROR_WRONG_TYPE, 0);
				}
			}
		}

		switch(aux->type){
			case IKS_AST_RETURN:	{
										if(aux->sonList->node->dataType != IKS_CHAR && aux->sonList->node->dataType != IKS_STRING)
											aux->sonList->node->coercion = aux->dataType;
										break;
									}
			case IKS_AST_ARIM_SOMA:	aritmeticCoercion(aux);
									break;
			case IKS_AST_ARIM_SUBTRACAO: aritmeticCoercion(aux);
									break;
			case IKS_AST_ARIM_MULTIPLICACAO: aritmeticCoercion(aux);
									break;
			case IKS_AST_ARIM_DIVISAO: aritmeticCoercion(aux);
									break;
			case IKS_AST_ARIM_INVERSAO: aux->dataType = aux->sonList->node->symbol->type;
									break;
			case IKS_AST_LOGICO_E: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_OU: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_DIF: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_IGUAL: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_LE: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_GE: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_L: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_G: aux->dataType = IKS_BOOL;
									break;
			case IKS_AST_LOGICO_COMP_NEGACAO: aux->dataType = IKS_BOOL;
									break;
			}
        return IKS_SUCCESS;
    }

}

/**
**  isAritmeticExpression
**  Retorna 1 se o tipo é expressão aritmética e 0 caso contrário
*/
int isAritmeticExpression(int type)
{
	switch(type){
		case IKS_AST_ARIM_SOMA: return 1;
								break;
		case IKS_AST_ARIM_SUBTRACAO: return 1;
								break;
		case IKS_AST_ARIM_MULTIPLICACAO: return 1;
								break;
		case IKS_AST_ARIM_DIVISAO: return 1;
								break;
		case IKS_AST_ARIM_INVERSAO: return 1;
								break;
		default: 					return 0;
	}
}

int isExpression(int type){
		switch(type){
		case IKS_AST_ARIM_SOMA: return 1;
								break;
		case IKS_AST_ARIM_SUBTRACAO: return 1;
								break;
		case IKS_AST_ARIM_MULTIPLICACAO: return 1;
								break;
		case IKS_AST_ARIM_DIVISAO: return 1;
								break;
		case IKS_AST_ARIM_INVERSAO: return 1;
								break;
		case IKS_AST_LOGICO_E: return 1;
									break;
		case IKS_AST_LOGICO_OU: return 1;
								break;
		case IKS_AST_LOGICO_COMP_DIF: return 1;
								break;
		case IKS_AST_LOGICO_COMP_IGUAL: return 1;
								break;
		case IKS_AST_LOGICO_COMP_LE: return 1;
								break;
		case IKS_AST_LOGICO_COMP_GE: return 1;
								break;
		case IKS_AST_LOGICO_COMP_L: return 1;
								break;
		case IKS_AST_LOGICO_COMP_G: return 1;
								break;
		case IKS_AST_LOGICO_COMP_NEGACAO: return 1;
								break;
		default: 					return 0;
	}

}

/**
**  verifySimpleCommand
**  Verifica a semântica dos comandos simples da linguagem
*/
int verifySimpleCommand(comp_tree* ast){

	if(ast==NULL)
	{
	    return IKS_SUCCESS;
	}
	else
	{
        comp_tree* aux = ast;
        comp_tree* auxBrother;
        nodeList* auxList;
        int result = IKS_SUCCESS;

		auxList = aux->sonList;

		// Processing current node

		if(aux->type == IKS_AST_RETURN)
		{
		    if(aux->sonList->node->dataType != aux->dataType && aux->sonList->node->coercion != aux->dataType)
		    {
		        printError(IKS_ERROR_WRONG_PAR_RETURN, 0);
		        return IKS_ERROR_WRONG_PAR_RETURN;
		    }
		}
		else if(aux->type == IKS_AST_INPUT)
		{
            if(aux->sonList->node->type != IKS_AST_IDENTIFICADOR)
		    {
		        printError(IKS_ERROR_WRONG_PAR_INPUT, 0);
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
                            printError(IKS_ERROR_WRONG_PAR_OUTPUT, 0);
                            return IKS_ERROR_WRONG_PAR_OUTPUT;
                        }
                    }
                }
            }
            else
            {
                printError(IKS_ERROR_WRONG_PAR_OUTPUT, 0);
                return IKS_ERROR_WRONG_PAR_OUTPUT;
            }
		}
	}
}


/**
**  verifyGivenParameters
**  Verifica o uso correto dos parâmetros nas chamadas de função
*/
int verifyGivenParameters(comp_tree* func, comp_tree* call)
{
    if(func != NULL)
    {
		nodeList* firstSon = call->sonList->next;
		comp_dict_t_p declaredArguments = func->args;
		if(declaredArguments != NULL && firstSon != NULL && declaredArguments->item->usage == ID_PARAMETRO){
			if(declaredArguments->item->type != firstSon->node->dataType)
				switch(declaredArguments->item->type)
						{
							case IKS_INT:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_INT;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
							case IKS_BOOL:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_BOOL;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
							case IKS_FLOAT:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_FLOAT;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
						}

			declaredArguments = declaredArguments->next;
			comp_tree* brothers = firstSon->node->broList;

			while(brothers!=NULL)
			{
				if(declaredArguments != NULL && declaredArguments->item->usage == ID_PARAMETRO)
				{
					if(declaredArguments->item->type != brothers->dataType)
					{
						switch(declaredArguments->item->type)
						{
							case IKS_INT:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_INT;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
							case IKS_BOOL:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_BOOL;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
							case IKS_FLOAT:	if(firstSon->node->dataType != IKS_CHAR && firstSon->node->dataType != IKS_STRING)
												firstSon->node->coercion = COERCION_TO_FLOAT;
											else printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);
												break;
						}
					}

                    printError(IKS_ERROR_WRONG_TYPE_ARGS, call->sonList->node->symbol->lineNumber);

					declaredArguments = declaredArguments->next;
					brothers = brothers->broList;
				}
				else
				{
				    printError(IKS_ERROR_EXCESS_ARGS, call->sonList->node->symbol->lineNumber);
				}
			}
			if(declaredArguments!= NULL && declaredArguments->item->usage == ID_PARAMETRO)
			{
			    printError(IKS_ERROR_MISSING_ARGS, call->sonList->node->symbol->lineNumber);
			}
			else
			{
			    return IKS_SUCCESS;
			}
		}
		else if(firstSon != NULL && (declaredArguments == NULL || declaredArguments->item->usage != ID_PARAMETRO))
		{
		    printError(IKS_ERROR_EXCESS_ARGS, call->sonList->node->symbol->lineNumber);
		}
		else if(firstSon == NULL && (declaredArguments != NULL && declaredArguments->item->usage == ID_PARAMETRO))
		{
		   printError(IKS_ERROR_MISSING_ARGS, call->sonList->node->symbol->lineNumber);
		}
	}
	return IKS_SUCCESS;
}

void setType(int type, comp_dict_item_t* symbol){
	switch(type){
	   case IKS_INT : symbol->type = IKS_INT; symbol->size =IKS_INT_SIZE; break;
	   case IKS_FLOAT: symbol->type = IKS_FLOAT; symbol->size = IKS_FLOAT_SIZE; break;
	   case IKS_BOOL: symbol->type = IKS_BOOL; symbol->size = IKS_BOOL_SIZE; break;
	   case IKS_CHAR: symbol->type = IKS_CHAR; symbol->size = IKS_CHAR_SIZE; break;
	   case IKS_STRING: symbol->type = IKS_STRING; symbol->size = IKS_CHAR_SIZE; break;
	}
}
