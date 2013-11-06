/**
 * Implementacao das funcoes de criacao de codigo ILOC
 * */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "comp_list.h"
#include "iloc.h"
#include "iks_ast.h"


const char* createRegister()
{
    char buffer[1000];

    registersCount++;
    sprintf(buffer, "%s%d", "r",registersCount);

    return strdup(buffer);
}

const char * createLabel()
{
    char buffer[1000];

    labelsCount++;
    sprintf(buffer, "%s%d", "L",labelsCount);

    return strdup(buffer);
}

comp_list* createCode(comp_list* concatOnList, int commandId, int count, ...){
	va_list ap;
	int j;
	char* param[3];
	char* commandCode;
	if(count>0){
		va_start(ap, count);
		for(j=0; j<count; j++)
			param[j] = va_arg(ap, char*);
		va_end(ap);
	}
	comp_list* newCode = list_Add(NULL, NULL);
	switch(commandId){
		case ILOC_ADD:	sprintf(commandCode, "add %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_SUB:	sprintf(commandCode, "sub %s, %s => %s", param[0], param[1], param[2]);	break;

		case ILOC_MULT: sprintf(commandCode, "mult %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_DIV:	sprintf(commandCode, "div %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_ADD_I: sprintf(commandCode, "addI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_SUB_I: sprintf(commandCode, "subI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_R_SUB_I: sprintf(commandCode, "rsubI %s, %s => %s", param[0], param[1], param[2]); 	break;

		case ILOC_MULT_I: sprintf(commandCode, "multI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_DIV_I: sprintf(commandCode, "divI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_R_DIV_I: sprintf(commandCode, "rdivI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_LSHIFT: sprintf(commandCode, "lshift %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_LSHIFT_I: sprintf(commandCode, "lshiftI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_RSHIFT: sprintf(commandCode, "rshift %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_RSHIFT_I: sprintf(commandCode, "rshiftI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_AND: sprintf(commandCode, "and %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_AND_I: sprintf(commandCode, "andI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_OR: sprintf(commandCode, "or %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_OR_I: sprintf(commandCode, "orI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_XOR: sprintf(commandCode, "xor %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_XOR_I: sprintf(commandCode, "xorI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_LOAD: sprintf(commandCode, "load %s => %s", param[0], param[1]);break;

		case ILOC_LOAD_AI: sprintf(commandCode, "loadAI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_LOAD_AO: sprintf(commandCode, "loadAO %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_CLOAD: sprintf(commandCode, "cload %s => %s", param[0], param[1]);break;

		case ILOC_CLOAD_AI:	sprintf(commandCode, "cloadAI %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_CLOAD_AO: sprintf(commandCode, "cloadAO %s, %s => %s", param[0], param[1], param[2]);break;

		case ILOC_LOAD_I: sprintf(commandCode, "loadI %s => %s", param[0], param[1]);break;

		case ILOC_STORE: sprintf(commandCode, "store %s => %s", param[0], param[1]);break;

		case ILOC_STORE_AI:	sprintf(commandCode, "storeAI %s => %s, %s", param[0], param[1], param[2]);break;

		case ILOC_STORE_AO: sprintf(commandCode, "storeAO %s => %s, %s", param[0], param[1], param[2]);break;

		case ILOC_CSTORE: sprintf(commandCode, "cstore %s => %s", param[0], param[1]);break;

		case ILOC_CSTORE_AI: sprintf(commandCode, "cstoreAI %s => %s, %s", param[0], param[1], param[2]);break;

		case ILOC_CSTORE_AO: sprintf(commandCode, "cstoreAO %s => %s, %s", param[0], param[1], param[2]);break;

		case ILOC_I2I: sprintf(commandCode, "i2i %s => %s", param[0], param[1]);break;

		case ILOC_C2C: sprintf(commandCode, "c2c %s => %s", param[0], param[1]);break;

		case ILOC_C2I: sprintf(commandCode, "c2i %s => %s", param[0], param[1]);break;

		case ILOC_I2C: sprintf(commandCode, "i2c %s => %s", param[0], param[1]);break;

		case ILOC_CMP_LT: sprintf(commandCode, "cmp_LT %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CMP_LE: sprintf(commandCode, "cmp_LE %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CMP_EQ: sprintf(commandCode, "cmp_EQ %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CMP_GE: sprintf(commandCode, "cmp_GE %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CMP_GT: sprintf(commandCode, "cmp_GT %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CMP_NE: sprintf(commandCode, "cmp_NE %s, %s -> %s", param[0], param[1], param[2]);break;

		case ILOC_CBR: sprintf(commandCode, "cbr %s -> %s, %s", param[0], param[1], param[2]);break;

		case ILOC_JUMP_I: sprintf(commandCode, "jumpI -> %s", param[0]);break;

		case ILOC_JUMP:	sprintf(commandCode, "jump -> %s", param[0]);break;

		case ILOC_NOP:	sprintf(commandCode, "nop");break;
		
		case ILOC_LABEL: sprintf(commandCode, "%s: ", param[0]);break;

	}
	newCode->code = strdup(commandCode);
	return list_Concat(concatOnList, newCode);
}

comp_list* astCode(comp_tree* ast){
	if(ast==NULL)
		return NULL;
	comp_tree* aux = ast;
	nodeList* auxList;
	comp_list* sonCode = NULL;
	comp_list* broCode = NULL;
	comp_list* fatherCode = NULL;
	const char* param = NULL;
	const char* label = NULL;
	auxList = aux->sonList;
	while(auxList!=NULL){ //processando todos os filhos
		sonCode = astCode(auxList->node);
		auxList = auxList->next;
	}
	
	//processando nodo atual
	
	switch(aux->type){//gera codigo para nodo atual
		case IKS_AST_IDENTIFICADOR: param = createRegister(); break;
		case IKS_AST_ARIM_SOMA: {
									param = createRegister();
									fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
									fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
									fatherCode = createCode(fatherCode, ILOC_ADD, 3, aux->sonList->node->code->reg, aux->sonList->next->node->code->reg, param); 
									break;
								}
		case IKS_AST_ARIM_SUBTRACAO: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_SUB, 3, aux->sonList->node->code->reg, aux->sonList->next->node->code->reg, param); 
										break;
									}
		case IKS_AST_ARIM_MULTIPLICACAO: {
											param = createRegister();
											fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
											fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
											fatherCode = createCode(fatherCode, ILOC_MULT, 3, aux->sonList->node->code->reg, aux->sonList->next->node->code->reg, param); 
											break;
										}
		case IKS_AST_ARIM_DIVISAO: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_DIV, 3, aux->sonList->node->code->reg, aux->sonList->next->node->code->reg, param); 
										break;
									}
		case IKS_AST_ARIM_INVERSAO: break;
		case IKS_AST_LOGICO_E: {
									param = createRegister();
									label = createLabel();
									fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
									fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
									fatherCode = createCode(fatherCode, ILOC_AND, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
									fatherCode = createCode(fatherCode, ILOC_CBR, 3, param, label, aux->labelF); 
									fatherCode = createCode(fatherCode, ILOC_LABEL, 1,label); 
									break;
								}
		case IKS_AST_LOGICO_OU: {
									param = createRegister();
									label = createLabel();
									fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
									fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
									fatherCode = createCode(fatherCode, ILOC_OR, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
									fatherCode = createCode(fatherCode, ILOC_CBR, 3, param, aux->labelT, label); 
									fatherCode = createCode(fatherCode, ILOC_LABEL, 1,label); 
									break;
								}
		case IKS_AST_LOGICO_COMP_DIF: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_NE, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_LOGICO_COMP_IGUAL: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_EQ, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_LOGICO_COMP_LE: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_LE, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_LOGICO_COMP_GE: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_GE, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_LOGICO_COMP_L: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_LT, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_LOGICO_COMP_G: {
										param = createRegister();
										fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
										fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
										fatherCode = createCode(fatherCode, ILOC_CMP_GT, 3, aux->sonList->node->code->reg, aux->sonList->node->code->reg, param); 
										break;
									}
		case IKS_AST_IF_ELSE: {
								label = createLabel();
								fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelT);
								fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
								fatherCode =  createCode(fatherCode, ILOC_JUMP, 1, label);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelF);
								fatherCode =  list_Concat(fatherCode, aux->sonList->next->next->node->code);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, label);
								break;
							}
		case IKS_AST_DO_WHILE: {
								label = createLabel();
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, label);
								fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
								fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelT);
								fatherCode =  createCode(fatherCode, ILOC_JUMP, 1, label);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelF);								
								break;
							}
		case IKS_AST_WHILE_DO: {
								label = createLabel();
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, label);
								fatherCode =  list_Concat(fatherCode, aux->sonList->node->code);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelT);
								fatherCode =  list_Concat(fatherCode, aux->sonList->next->node->code);
								fatherCode =  createCode(fatherCode, ILOC_JUMP, 1, label);
								fatherCode =  createCode(fatherCode, ILOC_LABEL, 1, aux->sonList->node->labelF);								
								break;
							}
		case IKS_AST_ATRIBUICAO: break;
		case IKS_AST_VETOR_INDEXADO: break;
		
	}
	
	if(aux->broList!=NULL){//percorre o irmao
		broCode = astCode(aux->broList);
		fatherCode = list_Concat(fatherCode, broCode);
	}
		
	fatherCode->reg = param;
	aux->code = fatherCode;
	return fatherCode;
}
