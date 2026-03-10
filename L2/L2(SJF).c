#include <stdio.h>

int main() {
    int n, i, j, choice;
    int at[20], bt[20], ct[20], tat[20], wt[20];
    int rt[20], p[20];
    float total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("\nProcess P%d\n", i+1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        p[i] = i+1;
        rt[i] = bt[i];   // for preemptive
    }

    printf("\n1. SJF Non-Preemptive");
    printf("\n2. SJF Preemptive (SRTF)");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    // ================= NON PREEMPTIVE =================
    if(choice == 1) {

        int temp;

        // Sort by Burst Time
        for(i = 0; i < n-1; i++) {
            for(j = i+1; j < n; j++) {
                if(bt[i] > bt[j]) {
                    temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                    temp = at[i]; at[i] = at[j]; at[j] = temp;
                    temp = p[i];  p[i] = p[j];  p[j] = temp;
                }
            }
        }

        // Completion Time
        ct[0] = at[0] + bt[0];

        for(i = 1; i < n; i++) {
            if(ct[i-1] < at[i])
                ct[i] = at[i] + bt[i];
            else
                ct[i] = ct[i-1] + bt[i];
        }
    }

    // ================= PREEMPTIVE =================
    else if(choice == 2) {

        int time = 0, completed = 0;
        int min, shortest;

        while(completed < n) {

            min = 9999;
            shortest = -1;

            for(i = 0; i < n; i++) {
                if(at[i] <= time && rt[i] > 0 && rt[i] < min) {
                    min = rt[i];
                    shortest = i;
                }
            }

            if(shortest == -1) {
                time++;
            }
            else {
                rt[shortest]--;
                time++;

                if(rt[shortest] == 0) {
                    completed++;
                    ct[shortest] = time;
                }
            }
        }
    }

    else {
        printf("Invalid choice!");
        return 0;
    }

    // ===== Calculate TAT and WT =====
    for(i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        total_tat += tat[i];
        total_wt += wt[i];
    }

    printf("\nP\tAT\tBT\tCT\tTAT\tWT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nAverage TAT = %.2f", total_tat/n);
    printf("\nAverage WT = %.2f", total_wt/n);

    return 0;
}
