#include "common.h"

// MAX_COORD depends on inputs, too large is not a problem (except for memory)
// Could do dynamic allocation to avoid doing that but ... mehh ... exercise, not prod
#define MAX_COORD 19
// Use + 3 in order to : "Keep free slot before", "Keep free slot after", "Coord 0 exists"
#define R_MAX_COORD (MAX_COORD + 2)
#define ALLOC_SIZE (R_MAX_COORD + 1)

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
    int x,y,z;
    uint32_t borders = 0;
    while(scanf("%d,%d,%d\n", &x, &y, &z) == 3) {
        x++, y++, z++;
        map3d[x][y][z] = 1;

        if(isp1) {
            borders += 6;
            for (int i = 0; i < 6; i++) {
                if (map3d[x+mx3d[i]][y+my3d[i]][z+mz3d[i]]) {
                    borders -= 2;
                }
            }
        }
    }
    
    if (!isp1) {
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
                if (nx > R_MAX_COORD || nx < 0 || ny > R_MAX_COORD || ny < 0 || nz > R_MAX_COORD || nz < 0)
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