#include <stdio.h>
#include <string.h>

#define MAX 50

int main() {

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[MAX], bt[MAX], rt[MAX];
    int ct[MAX], tat[MAX], wt[MAX];
    char type[MAX][10];

    int sysQ[MAX], userQ[MAX];
    int frontS=0, rearS=-1, frontU=0, rearU=-1;

    int gIdx[MAX*10], gStart[MAX*10], gEnd[MAX*10];
    int gc = 0;

    int completed=0, time=0;

    // ── Input ─────────────────────────────────────────────────────
    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i+1);
        printf("  Arrival Time       : "); scanf("%d", &at[i]);
        printf("  Burst Time         : "); scanf("%d", &bt[i]);
        printf("  Type (system/user) : "); scanf("%s", type[i]);
        rt[i] = bt[i];
    }

    // ── Sort by Arrival Time ──────────────────────────────────────
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (at[i] > at[j]) {
                int tmp;
                tmp=at[i]; at[i]=at[j]; at[j]=tmp;
                tmp=bt[i]; bt[i]=bt[j]; bt[j]=tmp;
                tmp=rt[i]; rt[i]=rt[j]; rt[j]=tmp;
                char t[10];
                strcpy(t,type[i]); strcpy(type[i],type[j]); strcpy(type[j],t);
            }
        }
    }

    int i=0, current=-1;

    while (completed < n) {

        // Step 1: Enqueue arrived processes
        while (i < n && at[i] <= time) {
            if (strcmp(type[i], "system") == 0) sysQ[++rearS] = i;
            else                                userQ[++rearU] = i;
            i++;
        }

        // Step 2: Preempt user if system process waiting
        if (current != -1 && strcmp(type[current], "user") == 0 && frontS <= rearS) {
            userQ[++rearU] = current;
            current = -1;
        }

        // Step 3: Select process
        if (current == -1) {
            if      (frontS <= rearS) current = sysQ[frontS++];
            else if (frontU <= rearU) current = userQ[frontU++];
            else {
                if (gc == 0 || gIdx[gc-1] != -1)
                    { gIdx[gc]=-1; gStart[gc]=time; gEnd[gc]=time+1; gc++; }
                else gEnd[gc-1]++;
                time++;
                continue;
            }
        }

        // Step 4: Log to Gantt (merge consecutive same-process slots)
        if (gc == 0 || gIdx[gc-1] != current)
            { gIdx[gc]=current; gStart[gc]=time; gEnd[gc]=time+1; gc++; }
        else gEnd[gc-1]++;

        // Step 5: Execute 1 unit
        rt[current]--;
        time++;

        // Step 6: Completion check
        if (rt[current] == 0) {
            ct[current]  = time;
            tat[current] = time - at[current];
            wt[current]  = tat[current] - bt[current];
            completed++;
            current = -1;
        }
    }

    // ── Results Table ─────────────────────────────────────────────
    printf("\n+-------+----------+-----+-----+-----+-----+-----+\n");
    printf("| Proc  | Type     |  AT |  BT |  CT | TAT |  WT |\n");
    printf("+-------+----------+-----+-----+-----+-----+-----+\n");
    float ttat=0, twt=0;
    for (int k=0; k<n; k++) {
        printf("| P%-4d | %-8s | %-3d | %-3d | %-3d | %-3d | %-3d |\n",
               k+1, type[k], at[k], bt[k], ct[k], tat[k], wt[k]);
        ttat += tat[k]; twt += wt[k];
    }
    printf("+-------+----------+-----+-----+-----+-----+-----+\n");
    printf("  Avg TAT: %.2f            Avg WT: %.2f\n", ttat/n, twt/n);

    // ── Gantt Chart ───────────────────────────────────────────────
    printf("\nGantt Chart:\n");

    for (int k=0; k<gc; k++) printf("+------");
    printf("+\n");

    for (int k=0; k<gc; k++) {
        if (gIdx[k] == -1) printf("|  --  ");
        else                printf("|  P%-2d ", gIdx[k]+1);
    }
    printf("|\n");

    for (int k=0; k<gc; k++) printf("+------");
    printf("+\n");

    printf("%-7d", gStart[0]);
    for (int k=0; k<gc; k++) printf("%-7d", gEnd[k]);
    printf("\n");

    return 0;
}
