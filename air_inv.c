#include <stdio.h>
#include <stdlib.h>

// Function to compare two aircraft based on descent rate
int compareAircraft(const void* a, const void* b) {
    return (*(int*)a) - (*(int*)b);
}

int maxPlanesPrevented(int startHeight[], int descentRate[], int n) {
    // Create an array to store the time each aircraft takes to land
    int timeToLand[n];

    // Calculate time to land for each aircraft
    for (int i = 0; i < n; i++) {
        timeToLand[i] = startHeight[i] / descentRate[i];
    }

    // Sort the aircraft by descending time to land
    qsort(timeToLand, n, sizeof(int),compareAircraft);

    for(int i = 0; i < n;i++){
        printf("%d ", timeToLand[i]);
    }
    printf("\n");

    int currentTime = 0;
    int maxPrevented = 0;

    for (int i = 0; i < n; i++) {
        printf("timeToLand %d, currentTime %d \n", timeToLand[i], currentTime);

        if (timeToLand[i] > currentTime) {
            maxPrevented++;
        }
        currentTime++;

    }

    return maxPrevented;
}

int main() {
    int startHeight1[] = {1, 3, 5, 4, 8};
    int descentRate1[] = {1, 2, 2, 1, 2};
    int n1 = sizeof(startHeight1) / sizeof(startHeight1[0]);

    int startHeight2[] = {4, 2, 2, 1};
    int descentRate2[] = {1, 2, 3, 1};
    int n2 = sizeof(startHeight2) / sizeof(startHeight2[0]);

    int maxPrevented1 = maxPlanesPrevented(startHeight1, descentRate1, n1);
    int maxPrevented2 = maxPlanesPrevented(startHeight2, descentRate2, n2);

    printf("Maximum number of planes prevented from landing (Example 1): %d\n", maxPrevented1);
    printf("Maximum number of planes prevented from landing (Example 2): %d\n", maxPrevented2);

    return 0;
}
