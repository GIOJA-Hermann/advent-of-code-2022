#include "common.h"

typedef struct {
    int x;
    int y;
} pos_t;

struct range_s;
typedef struct range_s {
    int min;
    int max;
    struct range_s *next;
} range_t;

struct beaconX_s;
typedef struct beaconX_s {
    int val;
    struct beaconX_s *next;
} beaconX_t;

int manhattanDist(pos_t from, pos_t to) {
    return (abs(from.x - to.x) + abs(from.y - to.y));
}

// Also used in day04
int canMerge(range_t range1, range_t range2) {
    // Increase range2 by 1 (min and max) to also catch adjacent ranges
    range2.max++;
    range2.min--;
    return (range1.min <= range2.max && range1.max >= range2.min);
}
void merge(range_t *range, range_t merged) {
    if (merged.min < range->min) range->min = merged.min;
    if (merged.max > range->max) range->max = merged.max;
}

int isIn(range_t rangeBig, range_t rangeSmall) {
    return (rangeSmall.min >= rangeBig.min && rangeSmall.max <= rangeBig.max);
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    pos_t sensor[40];
    pos_t beacon[40];
    beaconX_t *beaconXs = NULL;
    range_t *ranges = NULL;
    int nEntMax = 0, nEnt;
    int countBeacons = 0;
    int countNoBeacon = 0;
    range_t refRange = {.min = 0, .max = 4000000};
    range_t *range, *preRange, *mergeRange;
    int fullRange = 0;
    int skipMerge = 0;
    int targetX;
    int64_t p2res;

    while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sensor[nEntMax].x, &sensor[nEntMax].y, &beacon[nEntMax].x, &beacon[nEntMax].y) == 4) {nEntMax++;}

    for (int targetY = ((isp1)?(refRange.max/2):refRange.min); targetY <= ((isp1)?(refRange.max/2):refRange.max); targetY++) {
        //flush existing ranges for p2
        fullRange = 0;
        while(ranges) {
            range = ranges;
            ranges = range->next;
            free(range);
        }
        for (nEnt = 0; nEnt < nEntMax && !fullRange; nEnt++) {
            int md = manhattanDist(sensor[nEnt], beacon[nEnt]);
            int dist = abs(sensor[nEnt].y - targetY);
            // Add range in ranges if reaches targetY
            if (dist <= md) {
                range_t newRange = {
                    .min = sensor[nEnt].x - (md - dist),
                    .max = sensor[nEnt].x + (md - dist),
                    .next = NULL
                };

                range = ranges;
                preRange = NULL;
                // Skip existing ranges which are too low (stop at first range overlap or after new one)
                // So then preRange is fully BEFORE newRange
                while (range && newRange.min > (range->max + 1)) {
                    preRange = range;
                    range = range->next;
                };
                // If can't merge for any reason
                skipMerge = 0;
                if (range == NULL || !canMerge(*range, newRange)) {
                    skipMerge = 1;
                    range_t *rangeAlloc = malloc(sizeof(range_t));
                    *rangeAlloc = newRange;
                    rangeAlloc->next = range;
                    if (preRange) {
                        preRange->next = rangeAlloc;
                    } else {
                        ranges = rangeAlloc;
                    }
                    if (!isp1 && isIn(*rangeAlloc, refRange)) {
                        fullRange = 1;
                        break;
                    }
                }
                // Merge in from first to last overlapping
                if (!skipMerge) {
                    mergeRange = range;
                    merge(mergeRange, newRange);
                    range = range->next;
                    while (range && canMerge(*mergeRange, *range)) {
                        merge(mergeRange, *range);
                        preRange = range;
                        range = range->next;
                        free(preRange);
                    }
                    mergeRange->next = range;
                    if (!isp1 && isIn(*mergeRange, refRange)) {
                        fullRange = 1;
                        break;
                    }
                }
            }

            // We don't care existing beacons in p2
            if (isp1) {
                // Add beacon in list if on target
                if (beacon[nEnt].y == targetY) {
                    beaconX_t *beaconX = beaconXs, *preBeaconX = NULL;
                    while (beaconX && beacon[nEnt].x > beaconX->val) {
                        preBeaconX = beaconX;
                        beaconX = beaconX->next;
                    }
                    // Beacon already exists
                    if (beaconX && beacon[nEnt].x == beaconX->val)
                        continue;
                    else {
                        beaconX_t *beaconAlloc = malloc(sizeof(beaconX_t));
                        beaconAlloc->val = beacon[nEnt].x;
                        beaconAlloc->next = beaconX;
                        if (preBeaconX) {
                            preBeaconX->next = beaconAlloc;
                        } else {
                            beaconXs = beaconAlloc;
                        }
                        countBeacons++;
                    }
                }
            }
        }
        if (!isp1 && !fullRange) {
            range = ranges;
            if (range->min == refRange.min + 1)
                targetX = refRange.min;
            while (range) {
                if (range->next && range->max + 2 == range->next->min)
                    targetX = range->max +1;
                if (!range->next && range->max == refRange.max - 1)
                    targetX = refRange.max;
                range = range->next;
            }
            p2res = ((int64_t)targetX * (int64_t)refRange.max) + (int64_t)targetY;
            break;
        }
        
    }

    if (isp1) {
        range = ranges;
        while (range) {
            PRINTF1("%d -> %d\n", range->min, range->max);
            countNoBeacon += range->max - range->min + 1;
            range = range->next;
        }
        countNoBeacon -= countBeacons;
        printf("%d\n", countNoBeacon);
    } else {
        printf("%ld\n", p2res);
    }
}
