#include "common.h"

#define MAX(x,y) ((x>y)?x:y)

struct way_s;
typedef struct way_s {
    uint8_t  robots[4];
    uint16_t resources[4];
    uint8_t minute;
    uint64_t endGeode;
    struct way_s *next;
} way_t;

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");

    // x = type (ore, clay, obsidian, geode) , y0 = ore cost, y1 = other cost
    int cost[4][2];
    int maxCostOre;
    way_t *bWay = NULL, *eWay = NULL, *way = NULL, *nWay = NULL;
    uint8_t produce;
    uint8_t geodeMax;
    uint64_t endGeodeMax;

    uint8_t gday;
    uint64_t total = (isp1)?0:1;
    int bp = 0;

    const uint8_t minutesMax = (isp1)?24:32;

    while (scanf("Blueprint %*d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.\n", &cost[0][0], &cost[1][0], &cost[2][0], &cost[2][1], &cost[3][0], &cost[3][1]) == 6) {
        maxCostOre = 0;
        maxCostOre = MAX(cost[2][0],cost[1][0]);
        maxCostOre = MAX(cost[3][0],maxCostOre);

        geodeMax = 0;
        endGeodeMax = 0;
        gday = 1;

        PRINTF1("Blueprint : %d %d %d %d %d %d (%d)\n", cost[0][0], cost[1][0], cost[2][0], cost[2][1], cost[3][0], cost[3][1], maxCostOre);
        bp++;

        bWay = malloc(sizeof(way_t));
        *bWay = (way_t){0};
        bWay->robots[0] = 1;
        bWay->minute = 1;
        eWay = bWay;

        // Test every combination by building a tree of them
        while (way = bWay) {
            if (gday == way->minute) {
                if (!isp1) PRINTF2("MINUTE %d\n", way->minute);
                gday++;
            }

            // Let's do the only simple filter I could find first :
            // Since we produce at most 1 robot per minute, we don't need more robot of a kind than the maximum of its resource required for another robot.
            produce = 0;
            if (way->minute < minutesMax) {
                if (cost[3][0] <= way->resources[0] && cost[3][1] <= way->resources[2])
                    produce |= 0x8;
                if (cost[2][0] <= way->resources[0] && cost[2][1] <= way->resources[1] && way->robots[2] < cost[3][1])
                    produce |= 0x4;
                if (cost[1][0] <= way->resources[0] && way->robots[1] < cost[2][1])
                    produce |= 0x2;
                if (cost[0][0] <= way->resources[0] && way->robots[0] < maxCostOre)
                    produce |= 0x1;
            }
    
            for (int j=0; j<4;j++) {
                way->resources[j] += way->robots[j];
            }

            if (!isp1) {
                // Add a set of arbitrary threshold (still reasonable) to flush some of the worst possibilities as soon as possible
                // This part is quite dumb and will most likely work for any input but ... without it, segfaulting at minute 6.
                // This allows to quite greatly cut down the number of elements in memory (but still ugly :D).
                if (way->minute == 10 && way->robots[1] == 0 || way->minute == 15 && way->robots[2] < 1 || way->minute == 20 && way->robots[2] < 2 || way->minute == 25 && way->robots[2] < 3 || way->minute == 23 && way->robots[3] < 1 || way->minute == 26 && way->robots[3] < 2 || way->minute == 29 && way->robots[3] < 3) {
                    goto gonext;
                }
            }

            if (way->minute < minutesMax) {
                // Increment now, way will be added back to the loop too
                way->minute++;
                for (int j=0; j<4;j++) {
                    if (produce & (0x1 << j)) {
                        nWay = malloc(sizeof(way_t));
                        *nWay = *way;
                        nWay->robots[j]++;
                        nWay->resources[0] -= cost[j][0];
                        // Time during which new robot will produce
                        if (j == 3) nWay->endGeode += minutesMax - nWay->minute + 1;
                        endGeodeMax = MAX(endGeodeMax, nWay->endGeode);
                        nWay->next = NULL;
                        if (j > 1) {
                            nWay->resources[j - 1] -= cost[j][1];
                        }
                        // A bit harder optimization for "last minutes" : If way used will not be able to catch up other better ones, stop trying it
                        if ((nWay->endGeode + ((minutesMax - nWay->minute)*(minutesMax - nWay->minute + 1))/2) >= endGeodeMax) {
                            eWay->next = nWay;
                            eWay = nWay;
                        } else {
                            free(nWay);
                        }
                    }
                }
            }

            if (way->minute == minutesMax) {
gonext:
                if (way->resources[3] > geodeMax)
                    geodeMax = way->resources[3];
                bWay = way->next;
                free(way);
            } else {
                bWay = (way->next)?(way->next):(way);
                way->next = NULL;
                eWay->next = way;
                eWay = way;
            }

        }
        if (isp1) {
            total = total + (geodeMax * bp);
        } else {
            total = total * geodeMax;
            if (bp == 3) break;
        }
    }
    printf("%lu\n", total);
}