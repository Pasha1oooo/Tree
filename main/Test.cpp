#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../tree/Tree.h"

#define NUMBER_OF_FUNCS 3
#define NUMBER_OF_VARIABLES 3

Node_t * GetG(const char ** s);  //проверяет коректность "$ в конце"
Node_t * GetN(const char ** s);  //сканирует число
Node_t * GetE(const char ** s);  //выражение '+' '-'
Node_t * GetT(const char ** s);  //выражение '*' '/' '^'
Node_t * GetP(const char ** s);  //или число или функция или переменная или скобочное выражение
Node_t * GetV(const char ** s, int * i);  //переменная
Node_t * GetF(const char ** s);  //функции "sin" "cos" "ln"
Node_t * GetA(const char ** s);  //присваивание
Node_t * GetI(const char ** s);  //if
Node_t * GetO(const char ** s);  //оператор
size_t lex(const char ** s, Node_t * tokens);

variable variabls[10] = {};
int variable_ptr = 0;

int main(int argc, char* argv[]){
    FILE * fin = fopen(argv[2],"r");
    FILE * fout = fopen(argv[1],"w");
    const char * s = ReadFromFile(fin);
    const char * str = s;
    Node_t * tree = GetG(&s);
    if(tree == NULL){
        assert(1!=1);
    }
    printf("%s\n",variabls[0].name);
    printf("%s\n",variabls[1].name);
    printf("%s\n",variabls[2].name);
    PrintTreeToFile(tree , fout, variabls);
    FreeTree(tree);
    free((char*)str);
    return 0;
}

Node_t * GetG(const char ** s){
    Node_t * val = GetO(s);
    printf("\n=====new_val=======\n");
    //Node_t * new_val = GetO();
    //while(new_val != NULL){
    //    val = NewNode(OPERATOR, (Node_t_value){.operation=END}, val, new_val); ///хуйня переделать
    //    new_val = GetO();
    //}
    printf("%c G\n",*s);
    if(**s != '$'){
        assert(1!=1);
    }
    s++;
    return val;
}
Node_t * GetN(const char ** s){
    if(!((**s >= '0') && (**s <= '9'))){
        return NULL;
    }
    Node_t * val = NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
    while((**s >= '0') && (**s <= '9')){
        val->value.number = val->value.number*10 + (**s - '0');
        (*s)++;
    }
    return val;
}

Node_t * GetE(const char ** s){
    Node_t * val = GetT(s);
    while(**s == '+' || **s == '-'){
        int op = **s;
        (*s)++;
        Node_t * val2 = GetT(s);
        if(op== '+'){
            val = NewNode(OPERATOR, (Node_t_value){.operation=PLUS}, val, val2);
        }
        else{
            val = NewNode(OPERATOR, (Node_t_value){.operation=MINUS}, val, val2);
        }
    }
    return val;
}

Node_t * GetT(const char ** s){
    Node_t * val = GetP(s);
    while(**s == '*' || **s == '/' || **s == '^'){
        int op = **s;
        (*s)++;
        Node_t * val2 = GetP(s);
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

Node_t * GetF(const char ** s){
    const char * funcs[NUMBER_OF_FUNCS] = {"sin", "cos", "ln"};
    for(int i = 0; i < NUMBER_OF_FUNCS; i++){
        int compare = 0;
        int len = strlen(funcs[i]);
        compare = 1;
        for(int j = 0; j < len; j++){
            if(**s != funcs[i][j]){
                compare = 0;
            }
            (*s)++;
        }
        if(compare == 1){
            Node_t * val = NewNode(OPERATOR, (Node_t_value){.operation=(OPERATION)(5+i)}, NULL, NULL);
            if(**s == '('){
                (*s)++;
                val->left = GetE(s);
                if(**s != ')'){
                    assert(1!=1);
                }
                (*s)++;
            }
            return val;
        }
        if(compare == 0) *s-=len;
    }
    return NULL;
}

Node_t * GetP(const char ** s){
    Node_t * val = {};
    if(**s == '('){
        (*s)++;
        val = GetE(s);
        if(**s != ')'){
            assert(1!=1);
        }
        (*s)++;
    }
    else{
        val = GetF(s);
        if(val == NULL){
            int i=0;
            val = GetV(s,&i);
            if(val == NULL){
                val = GetN(s);
            }
        }
    }

    return val;
}

Node_t * GetV(const char ** s,int * t){
    if(!((**s >= 'a') && (**s <= 'z'))){
        return NULL;
    }
    int i = 0;
    char str[100];
    if(strncmp(*s,"if", 2) == 0){
        return NULL;
    }
    while((**s >= 'a') && (**s <= 'z')){
        str[i] = **s;
        i++;
        (*s)++;
    }
    *t = i;
    //printf("%s\n",str);
    for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
        if(strcmp(str,variabls[i].name) == 0){
            return NewNode(VARIABLE, (Node_t_value){.variable=i}, NULL, NULL);

        }
    }
    Node_t * val = NewNode(VARIABLE, (Node_t_value){.variable=variable_ptr}, NULL, NULL);
    strcat(variabls[variable_ptr].name, str);
    variable_ptr++;//////////////////
    return val;
}


Node_t * GetA(const char ** s){
    //printf("%c   a", *s);
    int i = 0;
    Node_t * val1 = GetV(s,&i);
    Node_t * val2 = {};
    if(**s == '='){
        (*s)++;
        val2 = GetE(s);
    }
    else{
        *s-=i;
        return NULL;
        assert(1!=1);
    }
    return NewNode(OPERATOR, (Node_t_value){.operation=EQUAL}, val1, val2);
}

Node_t * GetO(const char ** s){
    Node_t * val = {};
    val = GetA(s);
    if(val == NULL){
        printf("\nstart if\n");
        printf("%c O\n",**(s));
        val = GetI(s);
        if(val == NULL){
            return NULL;
        }
    }
    else{
        return val;
    }
    printf("%c O AI\n",**(s));
    if(**s != ';'){
        assert(1!=1);
        return NULL;
    }
    (*s)++;
    return val;
}

Node_t * GetI(const char ** s){
    //printf("%c I\n",*(s-1));
    //printf("%c I\n",*s);
    //printf("%c I\n",*(s+1));
    if(strncmp(*s,"if", 2) != 0){
        return NULL;
    }
    Node_t * val = {};
    *s+=2;
    if(**s == '('){
        val = NewNode(OPERATOR, (Node_t_value){.operation=IF}, NULL, NULL);
        (*s)++;
        val->left = GetA(s);//GetE
        if(**s != ')'){
            assert(1!=1);
        }
        (*s)++;
    }
    else{
        assert(1!=1);
        return NULL;
    }
    //printf("\n=====if o\n");
    val->right = GetO(s);
    return val;
}

size_t lex(const char ** s, Node_t * tokens){
    size_t n = 0;
    while(**s != EOF){
        if(**s == '(') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PAR_OPEN;
            n++;
            (*s)++;
            continue;
        }
        if(**s == ')') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PAR_CLOSE;
            n++;
            (*s)++;
            continue;
        }
        if(**s == '=' && *(*s+1) == '=') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = DOUBLE_EQUAL;
            n++;
            (*s)+=2;
            continue;
        }
        if(**s == '=') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = EQUAL;
            n++;
            (*s)++;
            continue;
        }
        if(strncmp(*s, "if", 2) == 0) {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = IF;
            n++;
            (*s)++;
            continue;
        }
        if(**s == '+') {
            tokens[n].type = OPERATOR;
            tokens[n].value.operation = PLUS;
            n++;
            (*s)++;
            continue;
        }
        if(**s > '0' && **s < '9') {
            tokens[n].type = NUMBER;
            tokens[n].value.number = **s - '0';
            n++;
            (*s)++;
            continue;
        }
    }
}
