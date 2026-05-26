#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n, m;
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int maxMatrix[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

void calcNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxMatrix[i][j] - allocation[i][j];
}

bool isSafe() {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSeq[MAX_PROCESSES], count = 0;

    for (int j = 0; j < m; j++) work[j] = available[j];

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canAlloc = true;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { canAlloc = false; break; }
                if (canAlloc) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        printf("SAFE STATE. Safe sequence: ");
        for (int i = 0; i < n; i++) printf("P%d%s", safeSeq[i], i < n-1 ? " -> " : "\n");
        return true;
    }
    printf("UNSAFE STATE! Deadlock may occur.\n");
    return false;
}

void requestResources() {
    int pid, request[MAX_RESOURCES];

    printf("\nEnter process number requesting resources: ");
    scanf("%d", &pid);
    printf("Enter request for %d resources: ", m);
    for (int j = 0; j < m; j++) scanf("%d", &request[j]);

    // Check 1: Request <= Need
    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: Request exceeds maximum claim!\n");
            return;
        }
    }

    // Check 2: Request <= Available
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            printf("Resources not available. Process P%d must wait.\n", pid);
            return;
        }
    }

    // Pretend to allocate (save old state)
    int oldAvail[MAX_RESOURCES], oldAlloc[MAX_RESOURCES], oldNeed[MAX_RESOURCES];
    for (int j = 0; j < m; j++) {
        oldAvail[j]  = available[j];
        oldAlloc[j]  = allocation[pid][j];
        oldNeed[j]   = need[pid][j];

        available[j]       -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j]       -= request[j];
    }

    // Run safety check
    printf("\nSimulating allocation for P%d...\n", pid);
    if (isSafe()) {
        printf("Request GRANTED for P%d.\n", pid);
    } else {
        // Rollback
        for (int j = 0; j < m; j++) {
            available[j]       = oldAvail[j];
            allocation[pid][j] = oldAlloc[j];
            need[pid][j]       = oldNeed[j];
        }
        printf("Request DENIED. Rolled back. P%d must wait.\n", pid);
    }
}

int main() {
    printf("Enter number of processes: "); scanf("%d", &n);
    printf("Enter number of resource types: "); scanf("%d", &m);

    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &allocation[i][j]);
    }

    printf("\nEnter Max Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &maxMatrix[i][j]);
    }

    printf("\nEnter Available Resources: ");
    for (int j = 0; j < m; j++) scanf("%d", &available[j]);

    calcNeed();

    printf("\nNeed Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++) printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\n--- Initial Safety Check ---\n");
    isSafe();

    int choice;
    do {
        printf("\n1. Request Resources\n2. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) requestResources();
    } while (choice != 2);

    return 0;
}
