#include "common.h"

// MAX_COORD depends on inputs, too large is not a problem (except for memory)
#define MAX_COORD 19
// Use + 3 in order to : "Keep free slot before", "Keep free slot after", "Coord 0 exists"
#define ALLOC_SIZE (MAXCOORD + 3)

int8_t mx3d[6] = {-1,1,0,0,0,0};
int8_t my3d[6] = {0,0,-1,1,0,0};
int8_t mz3d[6] = {0,0,0,0,-1,1};

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");

    if(!isp1) return;

    uint8_t map3d[22][22][22] = {0};
    int x,y,z;
    uint32_t borders = 0;
    while(scanf("%d,%d,%d\n", &x, &y, &z) == 3) {
        x++, y++, z++;

        borders += 6;
        for (int i = 0; i < 6; i++) {
            if (map3d[x+mx3d[i]][y+my3d[i]][z+mz3d[i]]) {
                borders -= 2;
            }
            map3d[x][y][z] = 1;
        }
    }
    
    printf("%d\n", borders);
}