#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/Tree.h"
#define NUMBER_OF_VARIABLES 10
Node_t * LoadBase(char ** pos);
void PrintASMToFile(Node_t * Node, FILE * fin);

variable variabls[NUMBER_OF_VARIABLES] = {};
int variable_ptr = 0;

int main(int argc, char* argv[]){
    printf("afffff");
    FILE * fin = fopen(argv[1],"r");
    FILE * fout = fopen(argv[2],"w");
    char * s = ReadFromFile(fin);
    char * str = s;
    Node_t * tree = LoadBase(&s);
    PrintASMToFile(tree , fout);
    free((char*)str);
    FreeTree(tree);
    fprintf(fout, "OUT \n");
    fprintf(fout, "HLT \n");
    return 0;
}

Node_t * LoadBase(char ** pos){
    if(**pos == '#'){
        return NULL;
    }
    else if(**pos == '('){
        Node_t * Node = (Node_t*)calloc(1, sizeof(Node_t));
        int i = 0;
        char * str = (char*)calloc(20, sizeof(char));
        (*pos)++;
        while((**pos != '#') && (**pos != '(')){
            (str)[i] = **pos;
            i++;
            (*pos)++;
        }
        if(DefineOperation(str) != UNKNOW){
            Node->type = OPERATOR;
            Node->value.operation = DefineOperation(str);
        }
        else{
            if(str[0] > '0' && str[0] < '9'){
                Node->type = NUMBER;
                Node->value.number = atoi(str);
            }
            else{
                Node->type = VARIABLE;
                int i = 0;
                for(int i = 0; i < NUMBER_OF_VARIABLES; i++){
                    if(strcmp(str,variabls[i].name) == 0){
                        Node->value.variable = i;
                    }
                }
                strcat(variabls[variable_ptr].name, str);
                Node->value.variable = variable_ptr;
                variable_ptr++;
            }
        }
        free((char*)str);
        Node->left = LoadBase(pos);
        (*pos)++;
        Node->right = LoadBase(pos);
        (*pos)++;
        return Node;
    }
}


void PrintASMToFile(Node_t * Node, FILE * fin){
    if(Node != NULL){
        //printf("%d\n", Node->value.operation);
        if(Node->type == OPERATOR && Node->value.operation == EQUAL){
            if(Node->left->type == VARIABLE){
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "PUSHM %d \n", Node->left->value.variable);
            }
            else if(Node->right->type == VARIABLE){
                PrintASMToFile(Node->left, fin);
                fprintf(fin, "PUSHM %d \n", Node->right->value.variable);
            }
        }
        if(Node->type == OPERATOR && Node->value.operation == IF){
            PrintASMToFile(Node->left, fin);
            fprintf(fin, "JMP \n");
            PrintASMToFile(Node->right, fin);
        }
        if(Node->type == OPERATOR){
            printf("oper\n");
            switch(Node->value.operation){
            case PLUS:
                PrintASMToFile(Node->left, fin);
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "ADD \n");
                break;
            case MINUS:
                PrintASMToFile(Node->left, fin);
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "SUB \n");
                break;
            case MULTIPLE:
                PrintASMToFile(Node->left, fin);
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "MUL \n");
                break;
            case DIVISION:
                PrintASMToFile(Node->left, fin);
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "DIV \n");
                break;
            case SINUS:
                PrintASMToFile(Node->left, fin);
                PrintASMToFile(Node->right, fin);
                fprintf(fin, "SIN \n");
                break;
            default:
                fprintf(fin, "\n");
                break;
            }
        }
        if(Node->type == NUMBER){
            fprintf(fin, "PUSH %lg \n", Node->value.number);
        }
    }
}
