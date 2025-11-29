#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../tree/Tree.h"

const char * s = "2*3+1*2*3*4$";
int GetG();
int GetN();
int GetE();
int GetT();




int main(int argc, char* argv[]){
    printf("%d\n",GetG());
    return 0;
}

int GetG(){
    int val = GetE();
    if(*s != '$'){
        assert(1!=1);
    }
    s++;
    return val;
}
int GetN(){
    int val = 0;
    while((*s >= '0') && (*s <= '9')){
        val = val*10 + (*s - '0');
        s++;
    }
    return val;
}

int GetE(){
    int val = GetT();
    while(*s == '+' || *s == '-'){
        int op = *s;
        s++;
        int val2 = GetT();
        if(op== '+'){
            val+=val2;
        }
        else{
            val-=val2;
        }
    }
    return val;
}

int GetT(){
    int val = GetN();
    while(*s == '*' || *s == '/'){
        int op = *s;
        s++;
        int val2 = GetN();
        if(op== '*'){
            val*=val2;
        }
        else{
            val/=val2;
        }
    }
    return val;
}
