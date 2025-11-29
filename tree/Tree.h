#ifndef TREE_H
#define TREE_H

#include <stdio.h>


typedef enum TYPE{
    OPERATOR = 0,
    NUMBER = 1,
    VARIBLE = 2
}TYPE;

typedef enum OPERATION{
    PLUS = 0,
    MINUS = 1,
    MULTIPLE = 2,
    DIVISION = 3,
    POWER = 4,
    SINUS = 5,
    COSINUS = 6,
    LOGARIFM = 7,
    EXP = 8,
    PI = 9
}OPERATION;

typedef union Node_t_value{
    OPERATION operation;
    double    number;
    int       variable;
}Node_t_value;

typedef struct Node_t{
    TYPE type;
    Node_t_value value;
    Node_t * left;
    Node_t * right;
}Node_t;

double DoOperation(Node_t * Node);
OPERATION DefineOperation(char * str);
double Solve(Node_t * Node);


Node_t * NewNode(TYPE type, Node_t_value value, Node_t * left, Node_t * right);

Node_t * DiffTree(Node_t * Node);
Node_t * CopyNode(Node_t * Node);
Node_t * Optimize(Node_t * Node);
Node_t * OptimizeTree(Node_t * Node);

void FreeTree(Node_t * Node);
Node_t * LoadBase(char ** pos, int * off); // deserializer code
void PrintTreeToFile(Node_t * Node, FILE * fin); // serializer
char * ReadFromFile(FILE * fin); //freadcalloc
//функция поиска , дамп графиз
#endif
