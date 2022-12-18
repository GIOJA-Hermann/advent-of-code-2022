#include "common.h"

// MAX_COORD depends on inputs, too large is not a problem (except for memory)
// Could do dynamic allocation to avoid doing that but ... mehh ... exercise, not prod
#define MAX_COORD 19
// Use + 3 in order to : "Keep free slot before", "Keep free slot after", "Coord 0 exists"
#define R_MAX_COORD (MAX_COORD + 2)
#define ALLOC_SIZE (R_MAX_COORD + 1)

#define MAX(x,y) ((x>y)?x:y)

int8_t mx3d[6] = {-1,1,0,0,0,0};
int8_t my3d[6] = {0,0,-1,1,0,0};
int8_t mz3d[6] = {0,0,0,0,-1,1};

struct explore_s;
typedef struct explore_s {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    struct explore_s* next;
} explore_t;

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");

    uint8_t map3d[ALLOC_SIZE][ALLOC_SIZE][ALLOC_SIZE] = {0};
    int x,y,z, maxX = 0, maxY = 0, maxZ = 0;
    uint32_t borders = 0;
    while(scanf("%d,%d,%d\n", &x, &y, &z) == 3) {
        // Compute max before doing the + 1
        maxX = MAX(maxX,x);
        maxY = MAX(maxY,y);
        maxZ = MAX(maxZ,z);
        x++, y++, z++;
        map3d[x][y][z] = 1;

        // For each new lava add the 6 borders (max) then remove adjacent borders (-2 because remove for both lava blocks)
        // Low complexity since we process only the added elements, not all possible map slot
        // Consider that each lava block is added only once
        if(isp1) {
            borders += 6;
            for (int i = 0; i < 6; i++) {
                if (map3d[x+mx3d[i]][y+my3d[i]][z+mz3d[i]]) {
                    borders -= 2;
                }
            }
        }
    }
    
    // Add + 2 to the max values for extra map_border slots
    maxX += 2;
    maxY += 2;
    maxZ += 2;

    if (!isp1) {
        // From a known extorior slot (0,0,0) which is out of lava grid (since we did +1 to all)
        // Explore to find all air blocks and count adjacent lava borders
        // From 0,0,0 explore and count
        explore_t *bExp, *eExp, *exp, *nExp = malloc(sizeof(explore_t));
        *nExp = (explore_t){.x=0, .y=0, .z=0, .next=NULL};
        map3d[nExp->x][nExp->y][nExp->z] = 2;
        bExp = nExp;
        eExp = nExp;

        while (bExp != NULL) {
            exp = bExp;
            PRINTF1("%d %d %d (exp)\n", exp->x, exp->y, exp->z);
            // Check if neighbor : yes => add border / no => add explore 
            for (int i = 0; i < 6; i++) {
                int8_t nx = exp->x+mx3d[i];
                int8_t ny = exp->y+my3d[i];
                int8_t nz = exp->z+mz3d[i];
                // Skip if out of map
                if (nx > maxX || nx < 0 || ny > maxY || ny < 0 || nz > maxZ || nz < 0)
                    continue;
                uint8_t mapVal = map3d[nx][ny][nz];
                if (mapVal == 1) {
                    // Lava, add border
                    borders++;
                    PRINTF2("%d %d %d is lava\n", nx, ny, nz);
                } else if (mapVal == 0) {
                    // Unexplored air, set as air and explore
                    nExp = malloc(sizeof(explore_t));
                    *nExp = (explore_t){.x=nx, .y=ny, .z=nz, .next=NULL};
                    map3d[nExp->x][nExp->y][nExp->z] = 2;
                    eExp->next = nExp;
                    eExp = nExp;
                    PRINTF2("%d %d %d is unexplored\n", nx, ny, nz);
                }
            }
            exp = exp->next;
            free(bExp);
            bExp = exp;
        }
    }

    printf("%d\n", borders);
}