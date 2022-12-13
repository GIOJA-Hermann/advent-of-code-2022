#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG1
//#define DEBUG2

#ifdef DEBUG1
    #define PRINTF1(args...) printf(args)
#else
    #define PRINTF1(args...) {}
#endif
#ifdef DEBUG2
    #define PRINTF2(args...) printf(args)
#else
    #define PRINTF2(args...) {}
#endif

void day(char *part) {
    char s1[500], *s1p;
    char s2[500], *s2p;
    char s3[10];
    int dep1, dep2, n1, n2;
    int right_order;
    int idx = 0;
    int total = 0;

    while (fgets(s1, 500, stdin) && fgets(s2, 500, stdin)){
        idx++;
        fgets(s3, 10, stdin); // flush empty line
        right_order = -1;
        s1p = s1;
        s2p = s2;
        dep1 = 0;
        dep2 = 0;
        printf("%s%s", s1, s2);
        while (1) {
            while(*s1p == '[' && *s2p == '[') {
                s1p++;
                s2p++;
            }
            while (*s1p == '[' && *s2p != '[') {
                s1p++;
                dep1++;
            }
            while (*s2p == '[' && *s1p != '[') {
                s2p++;
                dep2++;
            }
            // If left has number and right is empty   => left longer  => bad order
            if (*s1p >= '0' && *s1p <= '9' && *s2p == ']')  {right_order = 0; goto found;}
            // If left is empty   and right has number => left shorter => right order
            if (*s2p >= '0' && *s2p <= '9' && *s1p == ']')  {right_order = 1; goto found;}
            // If both empty, if left deeper : bad order, if right deeper: right order
            if (*s1p == ']' && *s2p == ']' && dep1) {printf("A");right_order = 0; goto found;}
            if (*s1p == ']' && *s2p == ']' && dep2) {printf("B");right_order = 1; goto found;}
            while (*s1p >= '0' && *s1p <= '9' && *s2p >= '0' && *s2p <= '9') {
                sscanf(s1p, "%d", &n1);
                sscanf(s2p, "%d", &n2);
                if      (n1 > n2) {right_order = 0; goto found;}
                else if (n1 < n2) {right_order = 1; goto found;}
                // After number
                s1p = strpbrk(s1p, ",]");
                s2p = strpbrk(s2p, ",]");
                // If one was deeper, check only one number and try to fold 
                while (dep1 && *s1p == ']') {s1p++; dep1--;}
                while (dep2 && *s2p == ']') {s2p++; dep2--;}
                // If one is still deeper, means that it is longer
                if      (dep1) {right_order = 0; goto found;}
                else if (dep2) {right_order = 1; goto found;}
            }
            // Go past mutual closures
            while(*s1p == ']' && *s2p == ']') {
                s1p++;
                s2p++;
            }
            // Both end => right order
            if (*s1p == '\0' && *s2p == '\0') {right_order = 1; goto found;}
            // First has longer list => bad order
            if (*s1p == ','  && *s2p == ']' ) {right_order = 0; goto found;}
            // Second has longer list => right order
            if (*s2p == ','  && *s1p == ']' ) {right_order = 1; goto found;}
            // else both continues
            s1p++; s2p++;
        }

found:
        printf("right_order : %d\n", right_order);
        if (right_order)
            total += idx;
    }
    printf("TOTAL %d\n", total);

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
