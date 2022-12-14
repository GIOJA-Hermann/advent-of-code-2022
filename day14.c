#include "common.h"

typedef struct {
    int x;
    int y;
} pos_t;

void setWall(int (*grid)[1002], pos_t pos) {grid[pos.x][pos.y] = 1;}
void setLimit(int *min, int *max, int *bot, pos_t pos) {
    if (pos.y < *min) *min = pos.y;
    if (pos.y > *max) *max = pos.y;
    if (pos.x > *bot) *bot = pos.x;
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    int grid[200][1002] = {0};
    char buf[1000];
    char *bufp;
    pos_t pos1, pos2;
    int min = 1001, max = 0, bot = 0;
    int sand = 0;

    while(fgets(buf, 1000, stdin)) {
        bufp = buf;
        sscanf(bufp, "%d,%d", &pos2.y, &pos2.x);
        setLimit(&min, &max, &bot, pos2);
        setWall(grid, pos2);
        while(bufp = strstr(bufp, "->")) {
            bufp += 3;
            pos1 = pos2;
            sscanf(bufp, "%d,%d", &(pos2.y), &(pos2.x));
            setLimit(&min, &max, &bot, pos2);
            while (pos1.x != pos2.x || pos1.y != pos2.y) {
                pos1.x += (!!(pos2.x - pos1.x)) * (((pos2.x - pos1.x) > 0) ? 1 : -1); // 1 or -1
                pos1.y += (!!(pos2.y - pos1.y)) * (((pos2.y - pos1.y) > 0) ? 1 : -1); // 1 or -1
                setWall(grid, pos1);
            }
        }
    }

    // Set floor
    if (!isp1) {
        bot += 2;
        for (int i = 0; i < 1002; i++) {
            grid[bot][i] = 1;
        }
    }

    pos_t posSand = (pos_t){0,500};
    while((posSand.x < bot) && (grid[0][500] != 2)) {
        if      (!grid[posSand.x + 1][posSand.y    ]) {posSand.x++;}
        else if (!grid[posSand.x + 1][posSand.y - 1]) {posSand.x++; posSand.y--;}
        else if (!grid[posSand.x + 1][posSand.y + 1]) {posSand.x++; posSand.y++;}
        else {
            setLimit(&min, &max, &bot, posSand);
            grid[posSand.x][posSand.y] = 2;
            sand++;
            posSand = (pos_t){0,500};
        }
        PRINTF1("SAND %d (%d,%d)\n", sand, posSand.x, posSand.y);
    }

    printf("%d\n", sand);

    PRINTF2("%d %d %d\n", min, max, bot);
    for (int i = 0; i <= bot; i++) {
        for (int j = (min - 1); j <= (max + 1); j++) {
            PRINTF2("%c", (grid[i][j] == 1) ? '#' : ((grid[i][j] == 2) ? 'O':'.'));
        }
        PRINTF2("\n");
    }
    
}
