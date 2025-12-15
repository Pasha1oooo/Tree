#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// -I ./
//TODO jumps
//TODO PUSHM POPM
//TODO O+
//TODO variables
#include "../tree/Tree.h"

int main(int argc, char* argv[]){

    FILE * fin = fopen(argv[1],"r");
    FILE * fout = fopen(argv[2],"w");

    char * buff = ReadFromFile(fin);
    printf("%s\n", buff);
    int i = 0;
    Node_t * tree = LoadBase(&buff, &i);
    buff -= i;
    free(buff);
    printf("Loaded\n");
    Node_t * dtree = DiffTree(tree);
    OptimizeTree(dtree);
    PrintTreeToFile(dtree , fout);
    printf("Solved\n");
    FreeTree(tree);
    FreeTree(dtree);
    printf("Free\n");
    return 0;

}
