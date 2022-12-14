#include "common.h"

void day(char *part);

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


