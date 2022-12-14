#include "common.h"

int vsI(char c) {return (c - 'A');}
int meI(char c) {return (c - 'X');}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char vs, me;
    int score = 0;
    while(isp1 && scanf("%c %c\n", &vs, &me) == 2) {
        // add choice points (0,1,2)+1 and battle points : win if me = (vs + 1) %3
        // so then me - vs + 1 = 2 on victory (or -1), 1 on draw ... and 0 (or 3) on loss
        // add 3 before modulo to have only positive values, then *3 to get win/draw/loss points
        score += meI(me) + 1 + (((meI(me) - vsI(vs) + 1 + 3) % 3) * 3);
    }
    while(!isp1 && scanf("%c %c\n", &vs, &me) == 2) {
        // Now choice points depends on victory (vs + 1), draw (vs) or loss (vs - 1). The numeric
        // part is extrapolated from "me" and always add +3 for positive modulo result
        score += ((vsI(vs) + 3 + meI(me) - 1) % 3) + 1 + meI(me) * 3;
    }
    printf("%d\n", score);
}
