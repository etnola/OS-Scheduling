// Consulted Stack Overflow
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

# define MAX_INT (2^31)-1


void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
	int rem_bt[n];
	int i, curr_time = 0; 
	
	for (i = 0; i < n; i++){
		rem_bt[i] = plist[i].bt;
		plist[i].wt = 0; 
	}
	
	int run = 0;
	while(!run){
		run = 1; 
		for(i = 0; i < n; i++){
			if (rem_bt[i] > 0){
				run = 0; 
				if (rem_bt[i] > quantum){
					curr_time += quantum;
					rem_bt[i] -= quantum;
				}

				else{
					curr_time += rem_bt[i];
					plist[i].wt = curr_time - plist[i].bt;
					rem_bt[i] = 0; 
				}
			}
		}
	}
} 


void findWaitingTime(ProcessType plist[], int n)
{ 
    
    plist[0].wt = 0 +  plist[0].art; 
  
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
}


void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
	
	int i, proc = 0, curr_time = 0; 
	int rem_bt[n];
	
	for(i = 0; i < n; i++){
		rem_bt[i] = plist[i].bt;
	}
	
	while(proc < n){
		int min_index = 0;
		for(i = 1; i < n; i++){
			if (rem_bt[i] < rem_bt[min_index]){
				min_index = i;
			}
		}
		
		rem_bt[min_index] -= 1;
		if (rem_bt[min_index] == 0){
			proc += 1;
			plist[min_index].wt = curr_time + 1 - plist[min_index].bt;
			rem_bt[min_index] = 9999;
		}
		curr_time += 1;
	}
}  
  

void findTurnAroundTime( ProcessType plist[], int n)
{ 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  

int my_comparer(const void *this, const void *that)
{ 
 
  
    if (((ProcessType*)this)->pri < ((ProcessType*)that)->pri){
			return 1;
		}
	return 0;
} 


void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    findWaitingTime(plist, n); 
  
    findTurnAroundTime(plist, n); 
  
    printf("\n*********\nFCFS\n");
}


void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    findWaitingTimeSJF(plist, n); 
  
    findTurnAroundTime(plist, n); 
  
    printf("\n*********\nSJF\n");
}


void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    findWaitingTimeRR(plist, n, quantum);  
    findTurnAroundTime(plist, n);  
    printf("\n*********\nRR Quantum = %d\n", quantum);
}


void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
		qsort(plist, n, sizeof(ProcessType), my_comparer);
    findTurnAroundTime(plist, n); 
    findWaitingTime(plist, n); 
  
    printf("\n*********\nPriority\n");
}


void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 


ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  

int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);

    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 