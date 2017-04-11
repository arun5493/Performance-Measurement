#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <cpuid.h>
#include <stdlib.h>
#include <sys/time.h>

int size = 1024*1024;
int step = 5;

/* Code from linux manual */
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}


int main(int argc, char* argv[])
{
	unsigned eax, ebx, ecx, edx, level;
	eax = 1; /* processor info and feature bits */
	level = 1;
    uint64_t initial, final;

    int a[size],j,i;
    FILE* fp;
    fp = fopen("loop_overhead.txt", "a");
    // Following code is for write Bandwidth
    for(i=0;i<size;i = i+step){
        __cpuid(level, eax, ebx, ecx, edx); // for serializing the instructions
            initial = rdtsc();
	for( j=0 ; j<step ; j++){
	        srand(time(NULL));
	        a[i+j] = rand()+5;
	}
        __cpuid(level, eax, ebx, ecx, edx); // for serializing the instructions
            final = rdtsc();
          
        fprintf(fp, "%d\n", (final-initial));
    }
    fprintf(fp, "\n Write BW Ends here \n Read BW Starts here \n");
    
    // Following code is for Read Bandwidth
    int b;
    for( i=0;i<size;i = i+step){
        __cpuid(level, eax, ebx, ecx, edx); // for serializing the instructions
            initial = rdtsc();

	for(j=0;j<step;j++){
        b = a[i+j] + 5;
	}
        __cpuid(level, eax, ebx, ecx, edx); // for serializing the instructions
            final = rdtsc();
          
        fprintf(fp, "%d\n", (final-initial));
    }
    fclose(fp); 
}
