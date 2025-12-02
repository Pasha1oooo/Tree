#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../tree/Tree.h"

const char * s = "(1+x)^x+1$";
#define NUMBER_OF_FUNCS 3
Node_t * GetG();
Node_t * GetN();
Node_t * GetE();
Node_t * GetT();
Node_t * GetP();
Node_t * GetV();

Node_t * GetF();




int main(int argc, char* argv[]){
    FILE * fout = fopen(argv[1],"w");
    Node_t * tree = GetG();
    PrintTreeToFile(tree , fout);
    FreeTree(tree);
    return 0;
}

Node_t * GetG(){
    Node_t * val = GetE();
    if(*s != '$'){
        assert(1!=1);
    }
    s++;
    return val;
}
Node_t * GetN(){
    if(!((*s >= '0') && (*s <= '9'))){
        return NULL;
    }
    Node_t * val = NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
    while((*s >= '0') && (*s <= '9')){
        val->value.number = val->value.number*10 + (*s - '0');
        s++;
    }
    return val;
}

Node_t * GetE(){
    Node_t * val = GetT();
    while(*s == '+' || *s == '-'){
        int op = *s;
        s++;
        Node_t * val2 = GetT();
        if(op== '+'){
            val = NewNode(OPERATOR, (Node_t_value){.operation=PLUS}, val, val2);
        }
        else{
            val = NewNode(OPERATOR, (Node_t_value){.operation=MINUS}, val, val2);
        }
    }
    return val;
}

Node_t * GetT(){
    Node_t * val = GetP();
    while(*s == '*' || *s == '/' || *s == '^'){
        int op = *s;
        s++;
        Node_t * val2 = GetP();
        if(op== '*'){
            val = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, val, val2);
        }
        else if(op== '/'){
            val = NewNode(OPERATOR, (Node_t_value){.operation=DIVISION}, val, val2);
        }
        else{
            val = NewNode(OPERATOR, (Node_t_value){.operation=POWER}, val, val2);
        }
    }
    return val;
}

Node_t * GetP(){
    Node_t * val = {};
    if(*s == '('){
        s++;
        val = GetE();
        printf("%c\n", *s);
        if(*s != ')'){
            assert(1!=1);
        }
        s++;
    }
    else{
        val = GetV();
        if(val == NULL){
            val = GetF();
            if(val == NULL){
                val = GetN();
            }
        }
    }

    return val;
}

Node_t * GetV(){
    if(!((*s >= 'a') && (*s <= 'z'))){
        return NULL;
    }
    Node_t * val = NewNode(VARIBLE, (Node_t_value){.variable=0}, NULL, NULL);
    while((*s >= 'a') && (*s <= 'z')){
        val->value.variable = val->value.variable*10 + (*s - '0');
        s++;
    }
    return val;
}

Node_t * GetF(){
    const char * funcs[NUMBER_OF_FUNCS] = {"sin", "cos", "ln"};
    for(int i = 0; i < NUMBER_OF_FUNCS; i++){
        int compare = 0;
        int len = strlen(funcs[0]);
        compare = 1;
        for(int j = 0; j < len; j++){
            if(*s != funcs[i][j]){
                compare = 0;
            }
            s++;
        }
        if(compare == 1) return NewNode(VARIBLE, (Node_t_value){.variable=5+i}, NULL, NULL);
    }
    return NULL;
}

