#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define DEBUG1
//#define DEBUG2
#include "debug.h"
#include "debug.h"

int isNum(const char *s)     { return (*s >= '0' && *s <= '9'); }
int isBegList(const char *s) { return (*s == '[' ); }
int isEndList(const char *s) { return (*s == ']' ); }
int isSep(const char *s)     { return (*s == ',' ); }
int isEnd(const char *s)     { return (*s == '\0'); }

int isrightorder(const char *s1, const char *s2) {
    const char *s1p = s1, *s2p = s2;
    int dep1 = 0, dep2 = 0, n1, n2;
    while (1) {
        // Removing all leading list open and register depth diff
        while (isBegList(s1p)  && isBegList(s2p) ) {s1p++; s2p++; }
        while (isBegList(s1p)  && !isBegList(s2p)) {s1p++; dep1++;}
        while (!isBegList(s1p) && isBegList(s2p) ) {s2p++; dep2++;}
        // If left has number and right is empty   => left longer  => bad order
        if (isNum(s1p) && isEndList(s2p)) return 0;
        // If left is empty   and right has number => left shorter => right order
        if (isNum(s2p) && isEndList(s1p)) return 1;
        // If both empty, if left deeper : bad order, if right deeper: right order
        if (isEndList(s1p) && isEndList(s2p) && (dep1 || dep2)) return (!!dep2);
        while (isNum(s1p) && isNum(s2p)) {
            sscanf(s1p, "%d", &n1);
            sscanf(s2p, "%d", &n2);
            if      (n1 > n2) return 0;
            else if (n1 < n2) return 1;
            // After number
            s1p = strpbrk(s1p, ",]");
            s2p = strpbrk(s2p, ",]");
            // If one was deeper, check only one number and try to fold 
            while (dep1 && isEndList(s1p)) {s1p++; dep1--;}
            while (dep2 && isEndList(s2p)) {s2p++; dep2--;}
            // If one is still deeper, means that it is longer
            if (dep1 || dep2) return (!!dep2);
        }
        // Go past mutual closures
        while(isEndList(s1p) && isEndList(s2p)) {s1p++; s2p++;}
        // Both end => right order
        if (isEnd(s1p) && isEnd(s2p)) return 1;
        // First has longer list => bad order
        if (isSep(s1p) && isEndList(s2p)) return 0;
        // Second has longer list => right order
        if (isSep(s2p) && isEndList(s1p)) return 1;
        // else both continues
        s1p++; s2p++;
    }
}

void day(char *part) {
    char s1[500], s2[500], s3[10];
    int idx = 0, total = 0;

    if (!strcmp(part, "p1")) {
        while (fgets(s1, 500, stdin) && fgets(s2, 500, stdin) && ++idx){
            fgets(s3, 10, stdin); // flush empty line
            if (isrightorder(s1, s2))
                total += idx;
        }
    } else {
        char *S1 = "[[2]]";
        char *S2 = "[[6]]";
        int pos1 = 1, pos2 = 2;
        while (fgets(s1, 500, stdin) && fgets(s2, 500, stdin) && ++idx){
            fgets(s3, 10, stdin); // flush empty line
            pos1 += isrightorder(s1, S1);
            pos1 += isrightorder(s2, S1);
            pos2 += isrightorder(s1, S2);
            pos2 += isrightorder(s2, S2);
        }
        total = pos1*pos2;
    }

    printf("%d\n", total);
}

void usage() {
    printf("./dayXX.run [p1|p2] < dayXX.in");
}

int main(int argc, char *argv[]) {
    if(argc == 1) usage();
    if(strcmp(argv[1],"p1") == 0) {
        printf("===PART1===\n");
        day("p1");
        printf("\n");
    } else if(strcmp(argv[1],"p2") == 0) {
        printf("===PART2===\n");
        day("p2");
        printf("\n");
    } else {
        usage();
    }
}
