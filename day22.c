#include "common.h"

// Determined by looking at entry file (otherwise make it dynamic...)
#define NROWS 200
#define NCOLS 150

#define RIGHT 0
#define DOWN  1
#define LEFT  2
#define UP    3

#define NEXTIDX(_x) ((((uint16_t)(_x - min + incr + size)) % size) + min)

uint16_t minX[7] = {0,   1,   1,  51, 101, 101, 151};
uint16_t maxX[7] = {0,  50,  50, 100, 150, 150, 200};
uint16_t minY[7] = {0,  51, 101,  51,   1,  51, 1  };
uint16_t maxY[7] = {0, 100, 150, 100,  50, 100, 50 };

void RL(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2] + (maxX[z1] - *curX)][maxY[z2]] != '#') {
        PRINTF2("RL : %hu %hu (before)\n", *curX, *curY);
        *curX = minX[z2] + (maxX[z1] - *curX);
        *curY = maxY[z2];
        PRINTF2("RL : %hu %hu (after)\n", *curX, *curY);
        *curDir = 2;
    }
}
void RU(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[maxX[z2]][minY[z2] + (*curX - minX[z1])] != '#') {
        PRINTF2("RU : %hu %hu (before)\n", *curX, *curY);
        *curY = minY[z2] + (*curX - minX[z1]); 
        *curX = maxX[z2];
        *curDir = 3;
        PRINTF2("RU : %hu %hu (after)\n", *curX, *curY);
    }
}
void DL(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2] + (*curY - minY[z1])][maxY[z2]] != '#') {
        PRINTF2("DL : %hu %hu (before)\n", *curX, *curY);
        *curX = minX[z2] + (*curY - minY[z1]); 
        *curY = maxY[z2];
        PRINTF2("DL : %hu %hu (after)\n", *curX, *curY);
        *curDir = 2;
    }
}
void DD(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2]][minY[z2] + (*curY - minY[z1])] != '#') {
        PRINTF2("DD : %hu %hu (before)\n", *curX, *curY);
        *curX = minX[z2];
        *curY = minY[z2] + (*curY - minY[z1]); 
        *curDir = 1;
        PRINTF2("DD : %hu %hu (after)\n", *curX, *curY);
    }
}
void LR(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2] + (maxX[z1] - *curX)][minY[z2]] != '#') {
        PRINTF2("LR : %hu %hu (before)\n", *curX, *curY);
        *curX = minX[z2] + (maxX[z1] - *curX);
        *curY = minY[z2];
        *curDir = 0;
        PRINTF2("LR : %hu %hu (after)\n", *curX, *curY);
    }
}
void LD(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2]][minY[z2] + (*curX - minX[z1])] != '#') {
        PRINTF2("LD : %hu %hu (before)\n", *curX, *curY);
        *curY = minY[z2] + (*curX - minX[z1]); 
        *curX = minX[z2];
        *curDir = 1;
        PRINTF2("LD : %hu %hu (after)\n", *curX, *curY);
    }
}
void UR(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2] + (*curY - minY[z1])][minY[z2]] != '#') {
        PRINTF2("UR : %hu %hu (before)\n", *curX, *curY);
        *curX = minX[z2] + (*curY - minY[z1]);
        *curY = minY[z2];
        *curDir = 0;
        PRINTF2("UR : %hu %hu (after)\n", *curX, *curY);
    }
}

void UU(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir, uint8_t z1, uint8_t z2) {
    if (grid[minX[z2]][minY[z2] + (*curX - minX[z1])] != '#') {
        PRINTF2("UU : %hu %hu (before)\n", *curX, *curY);
        *curY = minY[z2] + (*curY - minY[z1]); 
        *curX = maxX[z2];
        *curDir = 3;
        PRINTF2("UU : %hu %hu (after)\n", *curX, *curY);
    }
}

void moveOnce(char grid[NROWS + 2][NCOLS + 2], uint16_t *curX, uint16_t *curY, uint8_t *curDir) {
    uint8_t zone;
    if (*curY <= 50) {
        if (*curX <= 150) zone = 4;
        else zone = 6; 
    } else if (*curY <= 100) {
        if (*curX <= 50) zone = 1;
        else if (*curX <= 100) zone = 3;
        else zone = 5;
    } else {
        zone = 2;
    }
    if (*curDir == 0) { // RIGHT
        if (*curY != (uint8_t)grid[*curX][NCOLS + 1]) { // not max
            if (grid[*curX][*curY + 1] != '#') {
                *curY += 1;
            }
        } else {
            //printf("ZONE %hhu\n", zone);
            if (zone == 2) {
                // 2R => 5L : x = reverse, y = max
                RL(grid, curX, curY, curDir, 2, 5);
            } else if (zone == 3) {
                // 3R => 2U : x = max, y = x
                RU(grid, curX, curY, curDir, 3, 2);
            } else if (zone == 5) {
                // 5R => 2L : x = reverse, y = max
                RL(grid,curX, curY, curDir, 5, 2);
            } else if (zone == 6) {
                // 6R => 5U : x = max, y = x
                RU(grid, curX, curY, curDir, 6, 5);
            }
        }
    } else if (*curDir == 1) { // DOWN
        if (*curX != (uint8_t)grid[NROWS + 1][*curY]) { // not max
            if (grid[*curX + 1][*curY] != '#') {
                *curX += 1;
            }
        } else {
            //printf("ZONE %hhu\n", zone);
            if (zone == 2) {
                // 2D => 5L : x = y, y = max
                DL(grid, curX, curY, curDir, 2, 5);
            } else if (zone == 5) {
                // 5D => 6L : x = y, y = max
                DL(grid, curX, curY, curDir, 5, 6);
            } else if (zone == 6) {
                // 6D => 2D : x = min, y = y
                DD(grid, curX, curY, curDir, 6, 2);
            }
        }
    } else if (*curDir == 2) { // LEFT
        if (*curY != (uint8_t)grid[*curX][0]) { // not min
            if (grid[*curX][*curY - 1] != '#') {
                *curY -= 1;
            }
        } else {
            //printf("ZONE %hhu\n", zone);
            if (zone == 1) {
                // 1L => 4R : x = reverse, y = min
                LR(grid, curX, curY, curDir, 1, 4);
            } else if (zone == 3) {
                // 3L => 4D : x = min, y = x
                LD(grid, curX, curY, curDir, 3, 4);
            } else if (zone == 4) {
                // 4L => 1R : x = reverse, y = min
                LR(grid, curX, curY, curDir, 4, 1);
            } else if (zone == 6) {
                // 6L => 1D : x = min, y = x
                LD(grid, curX, curY, curDir, 6, 1);
            }
        }
    } else if (*curDir == 3) { // UP
        if (*curX != (uint8_t)grid[0][*curY]) { // not min
            if (grid[*curX - 1][*curY] != '#') {
                *curX -= 1;
            }
        } else {
            //printf("ZONE %hhu\n", zone);
            if (zone == 1) {
                // 1U => 6R : x = y, y = min
                UR(grid, curX, curY, curDir, 1, 6);
            } else if (zone == 2) {
                // 2U => 6U : x = max, y = y
                UU(grid, curX, curY, curDir, 2, 6);
            } else if (zone == 4) {
                // 4U => 3R : x = y, y = min
                UR(grid, curX, curY, curDir, 4, 3);
            }
        }
    }
}


void day(const char *part) {
    int isp1 = !strcmp(part, "p1");

    char grid[NROWS + 2][NCOLS + 2] = {0}; // +2 to have : 1 pre-elem to store "first elem idx", 1 post-elem to store "last elem idx"
    size_t idx = 1;
    size_t bIdx, eIdx;

    uint32_t move;
    uint16_t min, max, size;
    char    dir;

    uint16_t curX, curY, incr, nidx;
    uint8_t curDir;
    
    char commands[6000];

    while(fgets(&grid[idx][1], NCOLS + 2, stdin)) { // On full line it WILL overflow but its ok, it wraps on next line without consequence
        if (strlen(&grid[idx][1]) == 1) break; // empty line
        bIdx = strpbrk(&grid[idx][1], ".#") - &grid[idx][0];
        eIdx = strchr(&grid[idx][1], '\n') - &grid[idx][0];
        // First erase eol char (may be same as last slot so do it first). Not really important to do it but easier to print debug
        grid[idx][eIdx] = '\0';
        grid[idx][0] = bIdx;
        grid[idx][NCOLS + 1] = eIdx - 1;
        PRINTF1("ROW%.3lu : %hhu .. %hhu\n", idx, grid[idx][0], grid[idx][NCOLS + 1]);
        idx++;
    }

    curX = 1;
    curY = grid[1][0];
    curDir = 0;
    // For column it is a bit harder since we can't rely on nice builtin function;
    for (idx = 1; idx <= NCOLS; idx++) {
        bIdx = 1;
        //printf("%d\n", idx);
        while (grid[bIdx][idx] != '.' && grid[bIdx][idx] != '#') bIdx++;
        grid[0][idx] = bIdx;
        eIdx = bIdx;
        while (grid[eIdx][idx] == '.' || grid[eIdx][idx] == '#') {eIdx++;}
        grid[NROWS + 1][idx] = eIdx - 1;
        PRINTF1("COL%.3lu : %hhu .. %hhu\n", idx, grid[0][idx], grid[NROWS + 1][idx]);
    }

    // ---------------------- TABLE IS FILLED ----------------------- //
    // for (idx = 1; idx <= NROWS; idx++) {
    //     PRINTF1("%.*s\n", NCOLS, &grid[idx][1]);
    // }

    while(scanf("%u\n", &move) == 1) {
        if (isp1) {
            if (curDir % 2) { // UP-DOWN
                min = (uint8_t)grid[0][curY];
                max = (uint8_t)grid[NROWS + 1][curY];
                size = max - min + 1;
                incr = (curDir == 1)?1:-1;
                while(move-- && grid[nidx = NEXTIDX(curX)][curY] != '#') {
                    curX = nidx;
                }
            } else { // LEFT-RIGHT
                min = (uint8_t)grid[curX][0];
                max = (uint8_t)grid[curX][NCOLS + 1];
                size = max - min + 1;
                incr = (curDir == 0)?1:-1;
                while(move-- && grid[curX][nidx = NEXTIDX(curY)] != '#') {
                    curY = nidx;
                }
            }
        } else {
            while(move--) {
                moveOnce(grid, &curX, &curY, &curDir);
            }
        }
        PRINTF2("x:%hd y:%hd (%d)\n", curX, curY, curDir);

        if (scanf("%c\n", &dir) == 1) {
            switch(dir) {
                case 'R': curDir = (curDir + 1) % 4; break;
                case 'D': curDir = (curDir + 2) % 4; break;
                case 'L': curDir = (curDir + 3) % 4; break;
                default: break;
            }
        }
        PRINTF2("x:%hd y:%hd (%d)\n", curX, curY, curDir);
    }
    printf("%d\n", curX * 1000 + curY * 4 + curDir);

}