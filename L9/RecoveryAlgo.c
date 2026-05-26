#include <stdio.h>
#include <stdbool.h>

#define MAX_P 10
#define MAX_R 10

int n, m;
int allocation[MAX_P][MAX_R];
int request[MAX_P][MAX_R];
int available[MAX_R];

bool detect() {
    int work[MAX_R];
    bool finish[MAX_P] = {false};
    int deadlocked[MAX_P], count = 0;

    for (int j = 0; j < m; j++) work[j] = available[j];

    // Processes with no allocation are marked done
    for (int i = 0; i < n; i++) {
        bool hasAlloc = false;
        for (int j = 0; j < m; j++)
            if (allocation[i][j] > 0) { hasAlloc = true; break; }
        if (!hasAlloc) finish[i] = true;
    }

    // Find processes whose request can be satisfied
    bool found;
    do {
        found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canGrant = true;
                for (int j = 0; j < m; j++)
                    if (request[i][j] > work[j]) { canGrant = false; break; }
                if (canGrant) {
                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);

    // Any process still unfinished is deadlocked
    for (int i = 0; i < n; i++)
        if (!finish[i]) deadlocked[count++] = i;

    if (count == 0) {
        printf("No deadlock detected.\n");
        return false;
    }

    printf("DEADLOCK DETECTED! Deadlocked processes: ");
    for (int i = 0; i < count; i++) printf("P%d ", deadlocked[i]);
    printf("\n");
    return true;
}

int main() {
    printf("Enter number of processes: "); scanf("%d", &n);
    printf("Enter number of resource types: "); scanf("%d", &m);

    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &allocation[i][j]);
    }

    printf("\nEnter Request Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &request[i][j]);
    }

    printf("\nEnter Available Resources (%d values): ", m);
    for (int j = 0; j < m; j++) scanf("%d", &available[j]);

    printf("\n--- Running Deadlock Detection ---\n");
    detect();

    return 0;
}
