#include "common.h"

int isNum(const char *s) { return (*s >= '0' && *s <= '9'); }
int getMax(int a, int b) { return ((a > b)? a : b); }
int getMin(int a, int b) { return ((a < b)? a : b); }

#define NBEST 3

int setMaxRetMin(int *max, int candidate) {
    int oldMax = *max;
    *max = getMax(*max, candidate);
    return getMin(oldMax, candidate);
}


void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char s[100];
    int cur = 0, best[NBEST+1] = {0};
    while(fgets(s, 100, stdin)) {
        if (isNum(s)) {
            // Not an empty line, increase counter
            cur += atoi(s);
        } else {
            // Empty line, check if max (or one of the X max) then reset counter
            if(isp1) {
                best[0] = getMax(best[0], cur);
            } else {
                for(int i = 1; i <= NBEST; i++) {
                    cur = setMaxRetMin(&best[i], cur);
                }
            }
            cur = 0;
        }
    }
    printf("%d\n", best[0] + best[1] + best[2] + best[3]);
}
