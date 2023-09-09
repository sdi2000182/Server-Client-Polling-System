#include <stdio.h>

int main(){
FILE* fd = fopen("inputFile","w");
for(int i =0 ;i < 10 ;i++){
	fprintf(fd,"%dim nayeon momo\n",i);
}
for(int i =0 ;i < 10 ;i++){
        fprintf(fd,"%dyou jeongyeon jihyo\n",i);
}
for(int i =0 ;i < 10 ;i++){
        fprintf(fd,"%dmomo hirai nayeon\n",i);
}
for(int i =0 ;i < 10 ;i++){
        fprintf(fd,"%dson chae nayeon\n",i);
}
for(int i =0 ;i < 10 ;i++){
        fprintf(fd,"%dchou tzuyu dahyun\n",i);
}
}
