#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    system("make all");
    system("./run/tree2 out.txt in.txt");
    system("./run/mid out.txt out2.txt");
    system("../MyStack/run/asm out2.txt ../MyStack/run/ByteCode.txt");
	system("../MyStack/run/spu2 ../MyStack/run/ByteCode.txt");
    return 0;
}
