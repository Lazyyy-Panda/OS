#include <stdio.h>

void firstFit(int b[], int m, int p[], int n) {
    printf("\n--- First Fit ---\n");
    int t[m];
    for(int i=0;i<m;i++) t[i]=b[i];
    for(int i=0;i<n;i++){
        int j;
        for(j=0;j<m;j++){
            if(t[j]>=p[i]){ t[j]=0; break; }  // mark block as used
        }
        if(j<m)
            printf("P%d(%dK) -> Block (%dK)\n", i+1, p[i], b[j]);
        else
            printf("P%d(%dK) -> Not Allocated\n", i+1, p[i]);
    }
}

void bestFit(int b[], int m, int p[], int n) {
    printf("\n--- Best Fit ---\n");
    int t[m];
    for(int i=0;i<m;i++) t[i]=b[i];
    for(int i=0;i<n;i++){
        int best=-1;
        for(int j=0;j<m;j++){
            if(t[j]>=p[i])
                if(best==-1 || t[j]<t[best]) best=j;
        }
        if(best!=-1){
            printf("P%d(%dK) -> Block (%dK)\n", i+1, p[i], b[best]);
            t[best]=0;  // mark block as used
        } else
            printf("P%d(%dK) -> Not Allocated\n", i+1, p[i]);
    }
}

void worstFit(int b[], int m, int p[], int n) {
    printf("\n--- Worst Fit ---\n");
    int t[m];
    for(int i=0;i<m;i++) t[i]=b[i];
    for(int i=0;i<n;i++){
        int worst=-1;
        for(int j=0;j<m;j++){
            if(t[j]>=p[i])
                if(worst==-1 || t[j]>t[worst]) worst=j;
        }
        if(worst!=-1){
            printf("P%d(%dK) -> Block (%dK)\n", i+1, p[i], b[worst]);
            t[worst]=0;  // mark block as used
        } else
            printf("P%d(%dK) -> Not Allocated\n", i+1, p[i]);
    }
}

int main() {
    int blocks[]    = {400, 700, 200, 300, 600};
    int processes[] = {212, 512, 312, 526};
    int m = 5, n = 4;

    firstFit(blocks, m, processes, n);
    bestFit(blocks, m, processes, n);
    worstFit(blocks, m, processes, n);

    return 0;
}
