#include "common.h"

#define BSIZE 400

int getPrio(char c) {return c - ((c <= 'Z') ? ('A' - 27) : ('a' - 1));}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char s[4][BSIZE], *sp;
    int sumprio = 0;

    while (isp1 && scanf("%s\n", s[0]) == 1) {
        // Find first char of 2nd half of string in full string (works only if we'r sure to have one in first half)
        sumprio += getPrio(*strpbrk(s[0], s[0] + (strlen(s[0]) / 2)));
    }

    while (!isp1 && scanf("%s\n", s[0]) == 1 && scanf("%s\n", s[1]) == 1 && scanf("%s\n", s[2]) == 1 ) {
        int i = 0;
        sp = s[0];
        // Compute string of matching chars
        while(sp = strpbrk(sp, s[1])) {
            s[3][i++] = *(sp++);
        }
        s[3][i] = '\0';
        PRINTF1("\n%d %s", i, s[3]);
        // Then correlate with third string to get the only match
        sumprio += getPrio(*strpbrk(s[2],s[3]));
    }


    printf("%d\n", sumprio);
}
