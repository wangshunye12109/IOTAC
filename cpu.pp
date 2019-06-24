#include<iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
using namespace std; 
#define _LINE_LENGTH 300 
 
 
bool GetCpuMem(float &cpu,size_t &mem, int pid,int tid = -1) 
{ 
    bool ret = false; 
    char cmdline[100]; 
    sprintf(cmdline, "ps -o %%cpu,rss,%%mem,pid,tid -mp %d", pid); 
    FILE *file; 
    file = popen(cmdline, "r"); 
    if (file == NULL)  
    { 
        printf("file == NULL\n"); 
        return false; 
    } 
 
    char line[_LINE_LENGTH]; 
    float l_cpuPrec=0; 
    int l_mem=0; 
    float l_memPrec=0; 
    int l_pid=0; 
    int l_tid=0; 
    if (fgets(line, _LINE_LENGTH, file) != NULL)  
    { 
    //  printf("1st line:%s",line); 
        if (fgets(line, _LINE_LENGTH, file) != NULL)  
        { 
    //      printf("2nd line:%s",line); 
            sscanf( line, "%f %d %f %d -", &l_cpuPrec, &l_mem, &l_memPrec, &l_pid ); 
            cpu = l_cpuPrec; 
            mem = l_mem/1024; 
            if( tid == -1 ) 
                ret = true; 
            else 
            { 
                while( fgets(line, _LINE_LENGTH, file) != NULL ) 
                { 
                    sscanf( line, "%f - - - %d", &l_cpuPrec, &l_tid ); 
    //              printf("other line:%s",line); 
    //              cout<<l_cpuPrec<<'\t'<<l_tid<<endl; 
                    if( l_tid == tid ) 
                    { 
                        printf("cpuVal is tid:%d\n",tid); 
                        cpu = l_cpuPrec; 
                        ret = true; 
                        break; 
                    } 
                } 
                if( l_tid != tid ) 
                    printf("TID not exist\n"); 
            } 
        } 
        else 
            printf("PID not exist\n"); 
    } 
    else 
        printf("Command or Parameter wrong\n"); 
    pclose(file); 
    return ret; 
} 
 
int main(int argc, char** argv) 
{ 
    float cpu=0; 
    size_t mem=0; 
    int pid=0; 
    int tid=-1; 
    if( argc > 1 ) 
        pid = atoi(argv[1]); 
    else 
        pid = getpid(); 
    if( argc > 2 ) 
        tid = atoi(argv[2]); 
    while(1) 
    { 
        if( GetCpuMem( cpu, mem, pid, tid ) ) 
        { 
            printf("%%CPU:%.1f\tMEM:%dMB\n", cpu, mem); 
        } 
        else 
            printf("return false\n"); 
        //break; 
        //sleep(5); 
    } 
 
    return 0; 
} 
