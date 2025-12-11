#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Tree.h"
#include "../lib/MyAssert.h"
//designeted initializers
#define NUMBER_OF_OPERATIONS 10

OPERATION DefineOperation(char * str){
    const char * operations[NUMBER_OF_OPERATIONS] = {"+","-","*","/","^","sin","cos","ln","e","pi"};
    for(int i = 0; i < NUMBER_OF_OPERATIONS; i++){
        if(strcmp(str,operations[i]) == 0){
            return (OPERATION)i;
        }
    }
}

char * ReadFromFile(FILE * fin){
    fseek(fin, 0, SEEK_END);
    size_t num = (long unsigned int)ftell(fin);
    fseek(fin, 0, SEEK_SET);
    char * ptr = (char*)calloc(num + 1,sizeof(char));
    fread(ptr, sizeof(char), num,fin);
    ptr[num] = '\0';
    return ptr;
}

double Solve(Node_t * Node){
    if(Node->left->type == OPERATOR){
        Node->left->value.number = (OPERATION)Solve(Node->left);
        Node->left->type = NUMBER;
    }
    if(Node->right->type == OPERATOR){
        Node->right->value.number = (OPERATION)Solve(Node->right);
        Node->right->type = NUMBER;
    }
    if((Node->left->type == NUMBER) && (Node->right->type == NUMBER)){
        return DoOperation(Node);
    }
}

void FreeTree(Node_t * Node){
    if (Node->left != NULL){
        FreeTree(Node->left);
    }
    if (Node->right != NULL){
        FreeTree(Node->right);
    }
    free(Node);
    return;
}
double DoOperation(Node_t * Node){
    switch(Node->value.operation){
    case PLUS:
        return (double)Node->left->value.number + (double)Node->right->value.number;
    case MINUS:
        return (double)Node->left->value.number - (double)Node->right->value.number;
    case MULTIPLE:
        return (double)Node->left->value.number * (double)Node->right->value.number;
    case DIVISION:
        return (double)Node->left->value.number / (double)Node->right->value.number;
    case POWER:
        return pow((double)Node->left->value.number,(double)Node->right->value.number);
    case SINUS:
        return sin((double)Node->left->value.number);
    case COSINUS:
        return cos((double)Node->left->value.number);
    case LOGARIFM:
        return log((double)Node->left->value.number);
    default:
        return 0;
    }
}

void PrintTreeToFile(Node_t * Node, FILE * fin, variable variabls[]){
    if(Node->type == NUMBER){
        if(Node->value.operation == EXP){
            fprintf(fin, "(e");
        }
        else if(Node->value.operation == PI){
            fprintf(fin, "(pi");
        }
        else{
            fprintf(fin, "(%lg", Node->value.number);
        }
    }
    else if(Node->type == VARIABLE){
        fprintf(fin, "(%s", variabls[Node->value.variable].name);
    }
    else if(Node->type == OPERATOR){
        switch(Node->value.operation){
        case PLUS:
            fprintf(fin, "(+");
            break;
        case MINUS:
            fprintf(fin, "(-");
            break;
        case MULTIPLE:
            fprintf(fin, "(*");
            break;
        case DIVISION:
            fprintf(fin, "(/");
            break;
        case POWER:
            fprintf(fin, "(^");
            break;
        case SINUS:
            fprintf(fin, "(sin");
            break;
        case COSINUS:
            fprintf(fin, "(cos");
            break;
        case LOGARIFM:
            fprintf(fin, "(ln");
            break;
        case EQUAL:
            fprintf(fin, "(=");
            break;
        case IF:
            fprintf(fin, "(if");
            break;
        case END:
            fprintf(fin, "(;");
            break;
        case WHILE:
            fprintf(fin, "(while");
            break;
        default:
            fprintf(fin, "(");
            break;
        }
    }
    if (Node->left){
        PrintTreeToFile(Node->left, fin, variabls);
    }
    else{
        fprintf(fin, "#");
    }
    if (Node->right){
        PrintTreeToFile(Node->right, fin, variabls);
    }
    else{
        fprintf(fin, "#");
    }
    fprintf(fin, ")");
    return;
}



// struct a val = {.typ=3};
// int a[100] = {1,2,3, [56]=4};

Node_t * DiffTree(Node_t * Node){
    switch(Node->type){
    case OPERATOR:
        switch(Node->value.operation){
        case PLUS:
        case MINUS:
            Node = NewNode(OPERATOR, Node->value, DiffTree(Node->left), DiffTree(Node->right));
            //Node = Optimize(Node);
            break;
        case MULTIPLE:
            Node = NewNode(OPERATOR, (Node_t_value){.operation=PLUS}, NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, CopyNode(Node->left), DiffTree(Node->right)), NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, DiffTree(Node->left),CopyNode(Node->right)));
            //Node = Optimize(Node);
            break;
        case DIVISION:
            Node = NewNode(OPERATOR, (Node_t_value){.operation=DIVISION}, NewNode(OPERATOR, (Node_t_value){.operation=MINUS}, NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, DiffTree(Node->left), CopyNode(Node->right)), NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, CopyNode(Node->left), DiffTree(Node->right))),NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, CopyNode(Node->right), CopyNode(Node->right)));
            break;
        case SINUS:
            Node = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=COSINUS}, CopyNode(Node->left), NULL), DiffTree(Node->left));
            break;
        case COSINUS:
            Node = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=SINUS}, CopyNode(Node->left), NULL), NewNode(NUMBER, (Node_t_value){.number=-1}, NULL, NULL)), DiffTree(Node->left));
            break;
        case POWER:
            if (Node->left->type == NUMBER){
                Node = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE},NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=LOGARIFM}, CopyNode(Node->left), CopyNode(Node->left)), NewNode(OPERATOR, (Node_t_value){.operation=POWER}, CopyNode(Node->left), CopyNode(Node->right))), DiffTree(Node->right));
                break;
            }
            else{
                printf("1\n");
                Node_t * Node1 = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=LOGARIFM}, CopyNode(Node->left), CopyNode(Node->left)), CopyNode(Node->right));
                Node = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE},NewNode(OPERATOR, (Node_t_value){.operation=POWER}, NewNode(NUMBER, (Node_t_value){.operation=EXP}, NULL, NULL), Node1) , DiffTree(Node1));
                printf("2\n");
                break;
            }

        case LOGARIFM:
            Node = NewNode(OPERATOR, (Node_t_value){.operation=MULTIPLE}, NewNode(OPERATOR, (Node_t_value){.operation=DIVISION}, NewNode(NUMBER, (Node_t_value){.number=1}, NULL, NULL), CopyNode(Node->left)), DiffTree(Node->left));
            break;
        default:
            break;
        }
        //printf("a\n");
        return Node;
    case VARIABLE:
        //free(Node);
        return NewNode(NUMBER, (Node_t_value){.number=1}, NULL, NULL);
    case NUMBER:
        //free(Node);
        return NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
    default:
        break;
    }
}

Node_t * NewNode(TYPE type, Node_t_value value, Node_t * left, Node_t * right){
    Node_t * Node = (Node_t*)calloc(1, sizeof(Node_t));
    Node->type = type;
    Node->value = value;
    Node->left = left;
    Node->right = right;
    return Node;
}

Node_t * CopyNode(Node_t * Node){
    if(Node == NULL) return NULL;
    return NewNode(Node->type, Node->value, CopyNode(Node->left), CopyNode(Node->right));
}

Node_t * OptimizeTree(Node_t * Node){
    if(Node->left != NULL){
        Node->left = OptimizeTree(Node->left);
    }
    if(Node->right != NULL){
        Node->right = OptimizeTree(Node->right);
    }
    return Optimize(Node);
}

Node_t * Optimize(Node_t * Node){
    if(Node->type == OPERATOR){
        if((Node->left->type == NUMBER) && (Node->right->type == NUMBER)){
            Node_t * tmp = NewNode(NUMBER, (Node_t_value){.number=DoOperation(Node)}, NULL, NULL);
            FreeTree(Node);
            return tmp;
        }
        if ((Node->left->value.number == 0) && (Node->left->type == NUMBER)){
            if(Node->value.operation == MULTIPLE){
                Node_t * tmp = NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
                FreeTree(Node);
                return tmp;
            }
            else if(Node->value.operation == PLUS){
                Node_t * tmp = CopyNode(Node->right);
                FreeTree(Node);
                return tmp;
            }
            else if((Node->value.operation == POWER) || (Node->value.operation == DIVISION)){
                Node_t * tmp = NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
                FreeTree(Node);
                return tmp;
            }
            else{
                return Node;
            }
        }
        else if ((Node->left->value.number == 1) && (Node->left->type == NUMBER)){
            if(Node->value.operation == MULTIPLE){
                Node_t * tmp = Node->right;
                FreeTree(Node);
                return tmp;
            }
            if(Node->value.operation == POWER){
                Node_t * tmp = Node->right;
                FreeTree(Node);
                return tmp;
            }
            else{
                return Node;
            }
        }
        if ((Node->right->value.number == 0) && (Node->right->type == NUMBER)){
            //return Node;
            if(Node->value.operation == MULTIPLE){
                Node_t * tmp = NewNode(NUMBER, (Node_t_value){.number=0}, NULL, NULL);
                FreeTree(Node);
                return tmp;
            }
            else if(Node->value.operation == PLUS){
                Node_t * tmp = Node->left;
                FreeTree(Node);
                return tmp;
            }
            else if(Node->value.operation == POWER){
                Node_t * tmp = NewNode(NUMBER, (Node_t_value){.number=1}, NULL, NULL);
                FreeTree(Node);
                return tmp;
            }
            else{
                return Node;
            }
        }
        else if ((Node->right->value.number == 1) && (Node->right->type == NUMBER)){
            if(Node->value.operation == MULTIPLE){
                Node_t * tmp = CopyNode(Node->left);
                FreeTree(Node);
                return tmp;
            }
            if(Node->value.operation == POWER){
                Node_t * tmp = CopyNode(Node->left);
                FreeTree(Node);
                return tmp;
            }
            else{
                return Node;
            }
        }
        else{
            return Node;
        }
    }
    else{
        return Node;
    }
    return Node;
}


