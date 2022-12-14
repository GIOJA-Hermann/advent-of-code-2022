#include "common.h"

void calc(int *C, int *X, int *tot) {
    if (!((++(*C)-20)%40))
        *tot += (*C)*(*X);
}
void docalc() {
    char c[5];
    int p;
    int C=1, X=1, tot = 0;
    while(scanf("%s %d\n", c, &p) >= 1) {
        if(strcmp(c, "noop") == 0)
            calc(&C,&X,&tot);
        else {
            calc(&C,&X,&tot);
            X += p;
            calc(&C,&X,&tot);
        }
    }
    printf("%d\n", tot);
}

void prerender(int *C, char *S, int m, int M) {
    S[*C] = (((*C)%40) >= m && ((*C)%40) <= M)?'#':'.';
    (*C)++;
}

void dorender() {
    char c[5];
    int p;
    char S[240] = {0};
    int C = 0;
    int m = 0, M = 2;
    prerender(&C,S,m,M);
    while(scanf("%s %d\n", c, &p) >= 1) {
        if(strcmp(c, "noop") == 0)
            prerender(&C,S,m,M);
        else {
            prerender(&C,S,m,M);
            m += p;
            M += p;
            prerender(&C,S,m,M);
        }
    }
    for(char *P = S; P < (S+240); P += 40)
        printf("%.40s\n", P);
}

void day(char *part) {
    int isp1 = !strcmp(part, "p1");
    if (isp1) docalc();
    else dorender();
}
