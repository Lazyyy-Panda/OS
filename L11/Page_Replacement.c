
#include <stdio.h>
#include <string.h>

#define FRAMES 3
#define PAGES  12

int pages[PAGES] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3};

int inFrames(int frames[], int page) {
    for (int i = 0; i < FRAMES; i++)
        if (frames[i] == page) return 1;
    return 0;
}

void FIFO() {
    int frames[FRAMES] = {-1, -1, -1};
    int faults = 0, ptr = 0;

    printf("\n--- FIFO ---\n");
    for (int i = 0; i < PAGES; i++) {
        if (!inFrames(frames, pages[i])) {
            frames[ptr] = pages[i];
            ptr = (ptr + 1) % FRAMES;
            faults++;
            printf("Page %d -> [%d %d %d] FAULT\n", pages[i], frames[0], frames[1], frames[2]);
        } else {
            printf("Page %d -> [%d %d %d] HIT\n", pages[i], frames[0], frames[1], frames[2]);
        }
    }
    printf("Total Faults: %d\n", faults);
}

void LRU() {
    int frames[FRAMES] = {-1, -1, -1};
    int lastUsed[FRAMES] = {0};
    int faults = 0;

    printf("\n--- LRU ---\n");
    for (int i = 0; i < PAGES; i++) {
        if (!inFrames(frames, pages[i])) {
            // Find LRU slot (prefer empty slot first)
            int replace = 0;
            for (int j = 0; j < FRAMES; j++) {
                if (frames[j] == -1) { replace = j; break; }
                if (lastUsed[j] < lastUsed[replace]) replace = j;
            }
            frames[replace] = pages[i];
            lastUsed[replace] = i;
            faults++;
            printf("Page %d -> [%d %d %d] FAULT\n", pages[i], frames[0], frames[1], frames[2]);
        } else {
            for (int j = 0; j < FRAMES; j++)
                if (frames[j] == pages[i]) lastUsed[j] = i;
            printf("Page %d -> [%d %d %d] HIT\n", pages[i], frames[0], frames[1], frames[2]);
        }
    }
    printf("Total Faults: %d\n", faults);
}

void Optimal() {
    int frames[FRAMES] = {-1, -1, -1};
    int faults = 0, filled = 0;

    printf("\n--- OPTIMAL ---\n");
    for (int i = 0; i < PAGES; i++) {
        if (!inFrames(frames, pages[i])) {
            if (filled < FRAMES) {
                frames[filled++] = pages[i];
            } else {
                // Find the frame whose page is used farthest in future
                int replace = 0, farthest = -1;
                for (int j = 0; j < FRAMES; j++) {
                    int nextUse = PAGES; // assume never used again
                    for (int k = i + 1; k < PAGES; k++) {
                        if (pages[k] == frames[j]) { nextUse = k; break; }
                    }
                    if (nextUse > farthest) { farthest = nextUse; replace = j; }
                }
                frames[replace] = pages[i];
            }
            faults++;
            printf("Page %d -> [%d %d %d] FAULT\n", pages[i], frames[0], frames[1], frames[2]);
        } else {
            printf("Page %d -> [%d %d %d] HIT\n", pages[i], frames[0], frames[1], frames[2]);
        }
    }
    printf("Total Faults: %d\n", faults);
}

int main() {
    printf("Page String: 7 0 1 2 0 3 0 4 2 3 0 3");
    printf("\nFrames: %d", FRAMES);
    FIFO();
    LRU();
    Optimal();
    return 0;
}
