#include "common.h"

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    int a, b, y, z, ovlp = 0;
    
    while(isp1  && scanf("%d-%d,%d-%d\n", &a, &b, &y, &z) == 4)
        ovlp += ((a <= y && b >= z) || (y <= a && z >= b));

    while(!isp1 && scanf("%d-%d,%d-%d\n", &a, &b, &y, &z) == 4)
        ovlp += (a <= z && b >= y);

    printf("%d\n", ovlp);
}