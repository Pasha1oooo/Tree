#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Tree.h"
#include "../lib/MyAssert.h"

OPERATION DefineOperation(char * str){
    if(strcmp(str,"+") == 0){
        return PLUS;
    }
    else if(strcmp(str,"-") == 0){
        return MINUS;
    }
    else if(strcmp(str,"*") == 0){
        return MULTIPLE;
    }
    else if(strcmp(str,"/") == 0){
        return DIVISION;
    }
    else if(strcmp(str,"^") == 0){
        return POWER;
    }
    else if(strcmp(str,"sin") == 0){
        return SINUS;
    }
    else if(strcmp(str,"cos") == 0){
        return COSINUS;
    }
    else if(strcmp(str,"ln") == 0){
        return LOGARIFM;
    }
    else if(strcmp(str,"e") == 0){
        return EXP;
    }
    else if(strcmp(str,"pi") == 0){
        return PI;
    }
    else{
        printf("Undefined operation\n");
        MyAssert(1 == 1);
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

Node_t * LoadBase(char ** pos, int * off){
    if(**pos == '#'){
        return NULL;
    }
    else if(**pos == '('){
        Node_t * Node = (Node_t*)calloc(1, sizeof(Node_t));
        char * str = (char*)calloc(10, sizeof(char));                 //10
        int i = 0;
        (*pos)++;
        (*off)++;
        while((**pos != '#') && (**pos != '(')){
            str[i] = **pos;
            (*pos)++;
            (*off)++;
            i++;
        }
        if(**pos == '('){
            printf("=========%s========\n", str);
            Node->type = OPERATOR;
            Node->value = DefineOperation(str);
        }
        else if(**pos == '#'){
            printf("=========%s========\n", str);
            if(strcmp(str, "x") == 0){//////////////////////////////only x
                Node->type = VARIBLE;
                Node->value = (OPERATION)(-1);
            }
            else{
                Node->type = NUMBER;
                Node->value = (OPERATION)atoi(str);
            }
        }
        Node->left = LoadBase(pos, off);
        (*pos)++;
        (*off)++;
        Node->right = LoadBase(pos, off);
        (*pos)++;
        (*off)++;
        free(str);
        return Node;
    }
}

double Solve(Node_t * Node){
    if(Node->left->type == OPERATOR){
        Node->left->value = (OPERATION)Solve(Node->left);
        Node->left->type = NUMBER;
    }
    if(Node->right->type == OPERATOR){
        Node->right->value = (OPERATION)Solve(Node->right);
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
    switch(Node->value){
    case PLUS:
        return (double)Node->left->value + (double)Node->right->value;
    case MINUS:
        return (double)Node->left->value - (double)Node->right->value;
    case MULTIPLE:
        return (double)Node->left->value * (double)Node->right->value;
    case DIVISION:
        return (double)Node->left->value / (double)Node->right->value;
    case POWER:
        return pow((double)Node->left->value,(double)Node->right->value);
    case SINUS:
        return sin((double)Node->left->value);
    case COSINUS:
        return cos((double)Node->left->value);
    case LOGARIFM:
        return log((double)Node->left->value);
    default:
        return 0;
    }
}

void PrintTreeToFile(Node_t * Node, FILE * fin){
    if(Node->type == NUMBER){
        if(Node->value == EXP){
            fprintf(fin, "(e");
        }
        else if(Node->value == PI){
            fprintf(fin, "(pi");
        }
        else{
            fprintf(fin, "(%d", Node->value);
        }
    }
    else if(Node->type == VARIBLE){
        fprintf(fin, "(x");
    }
    else if(Node->type == OPERATOR){
        switch(Node->value){
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
        default:
            fprintf(fin, "(");
            break;
        }
    }
    if (Node->left){
        PrintTreeToFile(Node->left, fin);
    }
    else{
        fprintf(fin, "#");
    }
    if (Node->right){
        PrintTreeToFile(Node->right, fin);
    }
    else{
        fprintf(fin, "#");
    }
    fprintf(fin, ")");
    return;
}

Node_t * DiffTree(Node_t * Node){
    switch(Node->type){
    case OPERATOR:
        switch(Node->value){
        case PLUS:
        case MINUS:
            Node = NewNode(OPERATOR, Node->value, DiffTree(Node->left), DiffTree(Node->right));
            //Node = Optimize(Node);
            break;
        case MULTIPLE:
            Node = NewNode(OPERATOR, PLUS, NewNode(OPERATOR, MULTIPLE, CopyNode(Node->left), DiffTree(Node->right)), NewNode(OPERATOR, MULTIPLE, DiffTree(Node->left),CopyNode(Node->right)));
            //Node = Optimize(Node);
            break;
        case DIVISION:
            Node = NewNode(OPERATOR, DIVISION, NewNode(OPERATOR, MINUS, NewNode(OPERATOR, MULTIPLE, DiffTree(Node->left), CopyNode(Node->right)), NewNode(OPERATOR, MULTIPLE, CopyNode(Node->left), DiffTree(Node->right))),        NewNode(OPERATOR, MULTIPLE, CopyNode(Node->right), CopyNode(Node->right)));
            break;
        case SINUS:
            Node = NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, COSINUS, CopyNode(Node->left), NULL), DiffTree(Node->left));
            break;
        case COSINUS:
            Node = NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, SINUS, CopyNode(Node->left), NULL), NewNode(NUMBER, (OPERATION)(-1), NULL, NULL)), DiffTree(Node->left));
            break;
        case POWER:
            if (Node->left->type == NUMBER){
                Node = NewNode(OPERATOR, MULTIPLE,NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, LOGARIFM, CopyNode(Node->left), CopyNode(Node->left)), NewNode(OPERATOR, POWER, CopyNode(Node->left), CopyNode(Node->right))), DiffTree(Node->right));
                break;
            }
            else{
                printf("1\n");
                Node_t * Node1 = NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, LOGARIFM, CopyNode(Node->left), CopyNode(Node->left)), CopyNode(Node->right));
                Node = NewNode(OPERATOR, MULTIPLE,NewNode(OPERATOR, POWER, NewNode(NUMBER, (OPERATION)EXP, NULL, NULL), Node1) , DiffTree(Node1));
                break;
            }

        case LOGARIFM:
            Node = NewNode(OPERATOR, MULTIPLE, NewNode(OPERATOR, DIVISION, NewNode(NUMBER, (OPERATION)1, NULL, NULL), CopyNode(Node->left)), DiffTree(Node->left));
            break;
        default:
            break;
        }
        //printf("a\n");
        return Node;
    case VARIBLE:
        //free(Node);
        return NewNode(NUMBER, (OPERATION)1, NULL, NULL);
    case NUMBER:
        //free(Node);
        return NewNode(NUMBER, (OPERATION)0, NULL, NULL);
    default:
        break;
    }
}

Node_t * NewNode(TYPE type, OPERATION value, Node_t * left, Node_t * right){
    Node_t * Node = (Node_t*)calloc(1, sizeof(Node_t));
    Node->type = type;
    Node->value = value;
    Node->left = left;
    Node->right = right;
    Node = Optimize(Node);
    return Node;
}

Node_t * CopyNode(Node_t * Node){
    if(Node == NULL) return NULL;
    return NewNode(Node->type, (OPERATION)Node->value, CopyNode(Node->left), CopyNode(Node->right));
}

Node_t * Optimize(Node_t * Node){
    if(Node->type == OPERATOR){
        //return Node;
        if((Node->left->type == NUMBER) && (Node->right->type == NUMBER)){
            Node_t * tmp = NewNode(NUMBER, (OPERATION)DoOperation(Node), NULL, NULL);
            FreeTree(Node);
            return tmp;
        }
        //else if ((Node->left->value == 0) && (Node->left->type == NUMBER)){
        //    if(Node->value == MULTIPLE){
        //        return NewNode(NUMBER, (OPERATION)0, NULL, NULL);
        //    }
        //    else if(Node->value == PLUS){
        //        return Node->right;
        //    }
        //    else if((Node->value == POWER) || (Node->value == DIVISION)){
        //        return NewNode(NUMBER, (OPERATION)0, NULL, NULL);
        //    }
        //    else{
        //        return Node;
        //    }
        //}
        //else if ((Node->left->value == 1) && (Node->left->type == NUMBER)){
        //    if(Node->value == MULTIPLE){
        //        return Node->right;
        //    }
        //    else{
        //        return Node;
        //    }
        //}
        ////else if ((Node->value == MULTIPLE) && (Node->left->value == DIVISION) && (Node->left->left->value == )){
        ////    if(Node->value == MULTIPLE){
        ////        return Node->right;
        ////    }
        ////    else{
        ////        return Node;
        ////    }
        ////}
//
        //else if ((Node->right->value == 0) && (Node->right->type == NUMBER)){
        //    if(Node->value == MULTIPLE){
        //        return NewNode(NUMBER, (OPERATION)0, NULL, NULL);
        //    }
        //    else if(Node->value == PLUS){
        //        return Node->left;
        //    }
        //    else if(Node->value == POWER){
        //        return NewNode(NUMBER, (OPERATION)1, NULL, NULL);
        //    }
        //    else{
        //        return Node;
        //    }
        //}
        //else if ((Node->right->value == 1) && (Node->right->type == NUMBER)){
        //    if(Node->value == MULTIPLE){
        //        return Node->left;
        //    }
        //    else{
        //        return Node;
        //    }
        //}
        //else{
        //    return Node;
        //}
    }
    else{
        return Node;
    }
}


