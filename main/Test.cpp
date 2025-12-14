#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../tree/Tree.h"

#define NUMBER_OF_FUNCS 3
#define NUMBER_OF_VARIABLES 3

Node_t * GetG(Node_t ** tokens);  //проверяет коректность "$ в конце"
Node_t * GetN(Node_t ** tokens);  //сканирует число
Node_t * GetE(Node_t ** tokens);  //выражение '+' '-'
Node_t * GetT(Node_t ** tokens);  //выражение '*' '/' '^'
Node_t * GetP(Node_t ** tokens);  //или число или функция или переменная или скобочное выражение
Node_t * GetV(Node_t ** tokens);  //переменная
Node_t * GetF(Node_t ** tokens);  //функции "sin" "cos" "ln"
Node_t * GetA(Node_t ** tokens);  //присваивание
Node_t * GetI(Node_t ** tokens);  //if
Node_t * GetO(Node_t ** tokens);  //оператор
size_t lex(const char ** s, Node_t * tokens);

variable variabls[10] = {};
int variable_ptr = 0;

int main(int argc, char* argv[]){
    FILE * fin = fopen(argv[2],"r");
    FILE * fout = fopen(argv[1],"w");
    const char * s = ReadFromFile(fin);
    const char * str = s;
    Node_t * tokenss = (Node_t*)calloc(100, sizeof(Node_t));
    Node_t * tokens = tokenss;
    lex(&s, tokenss);
    tokenss = tokens;
    Node_t * tree = GetG(&tokenss);
    if(tree == NULL){
        assert(1!=1);
    }
    PrintTreeToFile(tree , fout, variabls);
    FreeTree(tree);
    free((char*)str);
    free((Node_t*)tokens);
    return 0;
}

Node_t * GetG(Node_t ** tokens){
    printf("var %d\n", (*tokens)->type);
    Node_t * val = GetO(tokens);

    printf("\n=====new_val=======\n");

    printf("A %d\n", (*tokens)->type);
    printf("A %d\n", (*tokens)->value.operation);
    if((*tokens)->value.operation != DOLLAR){
        assert(1!=1);
    }
    (*tokens)++;
    return val;
}
Node_t * GetN(Node_t ** tokens){
    if((*tokens)->type != NUMBER){
        return NULL;
    }
    Node_t * val = NewNode(NUMBER, (Node_t_value){.number = (*tokens)->value.number}, NULL, NULL);
    (*tokens)++;
    return val;
}

Node_t * GetE(Node_t ** tokens){
    Node_t * val = GetT(tokens);
    if(val == NULL) return NULL;
    while((*tokens)->value.operation == PLUS || (*tokens)->value.operation == MINUS){
        int op = (*tokens)->value.operation;///////////////////////////////////////////////////////////////
        (*tokens)++;
        Node_t * val2 = GetT(tokens);
        if(val2 == NULL) return NULL;
        if(op == PLUS){
        //printf("EE %d \n", op);
            val = NewNode(OPERATOR, (Node_t_value){.operation=PLUS}, val, val2);
        }
        else{
            val = NewNode(OPERATOR, (Node_t_value){.operation=MINUS}, val, val2);
        }
    }
    return val;
}

Node_t * GetT(Node_t ** tokens){
    Node_t * val = GetP(tokens);
    if(val == NULL) return NULL;
    while((*tokens)->value.operation == MULTIPLE || (*tokens)->value.operation == DIVISION || (*tokens)->value.operation == POWER){
        int op = (*tokens)->value.operation;
        (*tokens)++;
        Node_t * val2 = GetP(tokens);
        if(val2 == NULL) return NULL;
        if(op == MULTIPLE){
            val = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, val, val2);
        }
        else if(op == DIVISION){
            val = NewNode(OPERATOR, (Node_t_value){.operation=DIVISION}, val, val2);
        }
        else{
            val = NewNode(OPERATOR, (Node_t_value){.operation=POWER}, val, val2);
        }
    }
    return val;
}

Node_t * GetF(Node_t ** tokens){
    Node_t * val = NewNode(OPERATOR, (Node_t_value){.operation=(*tokens)->value.operation}, NULL, NULL);
    (*tokens)++;
    if((*tokens)->value.operation == PAR_OPEN){
        printf("===================");
        (*tokens)++;
        val->left = GetE(tokens);
        if((*tokens)->value.operation != PAR_CLOSE){
            assert(1!=1);
        }
        (*tokens)++;
    }
    return val;
}

Node_t * GetP(Node_t ** tokens){
    Node_t * val = {};
    if((*tokens)->value.operation == PAR_OPEN){
        (*tokens)++;
        val = GetE(tokens);
        if(val == NULL) return NULL;
        if((*tokens)->value.operation != PAR_CLOSE){
            assert(1!=1);
        }
        (*tokens)++;
    }
    else{
        val = GetN(tokens);
        if(val == NULL){
            //int i=0;
            //val = GetV(tokens,&i);
            //if(val == NULL){
                val = GetF(tokens);
                if(val == NULL) return NULL;
            //}
        }
    }

    return val;
}
Node_t * GetV(Node_t ** tokens){
    printf("var %d\n", (*tokens)->type);
    if((*tokens)->type != VARIABLE){
        return NULL;
    }
    Node_t * val = NewNode(VARIABLE, (Node_t_value){.variable = (*tokens)->value.variable}, NULL, NULL);
    (*tokens)++;
    return val;
}
Node_t * GetA(Node_t ** tokens){
    printf("A %d\n", (*tokens)->type);
    Node_t * val1 = GetV(tokens);
    if(val1 == NULL) return NULL;
    Node_t * val2 = {};
    if((*tokens)->value.operation == EQUAL){
        (*tokens)++;
        val2 = GetE(tokens);
        if(val2 == NULL) return NULL;
    }
    else{
        return NULL;
        assert(1!=1);
    }
    return NewNode(OPERATOR, (Node_t_value){.operation=EQUAL}, val1, val2);
}


Node_t * GetO(Node_t ** tokens){
    Node_t * val = {};
    val = GetA(tokens);
    if(val == NULL){
        printf("=============================ia\n");
        val = GetI(tokens);
        if(val == NULL){
            printf("=============================i\n");
            //val = GetI(tokens);
            assert(1!=1);
            return NULL;
        }
        (*tokens)-=1;
        printf("A %d\n", (*tokens)->type);
        printf("A %d\n", (*tokens)->value.operation);
        return val;
    }
    if((*tokens)->value.operation != END){
        printf("=============================e\n");
        return NULL;
    }
    printf("=============================ok\n");
    (*tokens)++;
    return val;
}

Node_t * GetI(Node_t ** tokens){
    if((*tokens)->value.operation != IF){
        return NULL;
    }
    Node_t * val = {};
    (*tokens)+=1;
    if((*tokens)->value.operation == PAR_OPEN){
        val = NewNode(OPERATOR, (Node_t_value){.operation=IF}, NULL, NULL);
        (*tokens)++;
        val->left = GetE(tokens);//GetE
        if(val->left == NULL) assert(1!=1);
        if((*tokens)->value.operation != PAR_CLOSE){
            return NULL;
        }
        (*tokens)++;
    }
    else{
        return NULL;
    }
    printf("\n=====if o\n");
    val->right = GetO(tokens);
    if(val->right == NULL) assert(1!=1);
    printf("\n=====if o end\n");
    (*tokens)++;
    return val;
}

size_t lex(const char ** s, Node_t * tokens){
    size_t n = 0;
    while(**s != '\0'){
        if(**s == '$') {
            printf("(lex) $\n");
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = DOLLAR;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '(') {
            printf("(lex) (\n");
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PAR_OPEN;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == ')') {
            printf("(lex) )\n");
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PAR_CLOSE;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '=' && *(*s+1) == '=') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = DOUBLE_EQUAL;
            n++;
            (*s)+=2;
            continue;
        }
        else if(**s == ';') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = END;
            n++;
            (*s)+=2;
            continue;
        }
        else if(**s == '=') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = EQUAL;
            n++;
            (*s)++;
            continue;
        }
        else if(strncmp(*s, "if", 2) == 0) {
            printf("(lex) if\n");
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = IF;
            n++;
            (*s)+=2;
            continue;
        }
        else if(strncmp(*s, "sin", 3) == 0) {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = SINUS;
            n++;
            (*s)+=3;
            continue;
        }
        else if(strncmp(*s, "cos", 3) == 0) {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = COSINUS;
            n++;
            (*s)+=3;
            continue;
        }
        else if(strncmp(*s, "ln", 2) == 0) {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = LOGARIFM;
            n++;
            (*s)+=2;
            continue;
        }
        else if(**s == '+') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PLUS;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '-') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = MINUS;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '*') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = MULTIPLE;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '/') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = DIVISION;
            n++;
            (*s)++;
            continue;
        }
        else if(**s == '^') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = POWER;
            n++;
            (*s)++;
            continue;
        }
        else if(**s >= '0' && **s <= '9') {
            int sum = 0;
            while(**s >= '0' && **s <= '9'){
                sum = sum*10 + (**s - '0');
                (*s)++;
            }
            tokens[n].type = NUMBER;
            tokens[n].value.number = sum;
            n++;
            continue;
        }
        else if(**s >= 'a' && **s <= 'z') {
            printf("var lex\n");
            tokens[n].type = VARIABLE;
            int i = 0;
            char str[100];
            while((**s >= 'a') && (**s <= 'z')){
                str[i] = **s;
                i++;
                (*s)++;
            }
            printf("%s\n", str);
            for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
                if(strcmp(str,variabls[i].name) == 0){
                    tokens[n].value.variable = i;
                    continue;
                }
            }
            strcat(variabls[variable_ptr].name, str);
            tokens[n].value.variable = variable_ptr;
            variable_ptr++;
            n++;
            continue;
        }
        else{
            (*s)++;
        }
    }
    return NULL;
}
