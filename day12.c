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

void day12(char *part) {
    struct pos_s;
    typedef struct pos_s {
        int x; // top-down, 1st num in array
        int y; // left-right, 2nd num in array
        struct pos_s *next;
    } pos_t;
    
    #define ARRSIZE_D12 500
    
    pos_t *allocpos () {
        return (pos_t*)malloc(sizeof(pos_t));
    }
    
    char s[ARRSIZE_D12][ARRSIZE_D12];
    int v[ARRSIZE_D12][ARRSIZE_D12] = {0};
    int nrows = 0, ncols;
    pos_t *beg = NULL, *end = NULL, *new, *del;
    int res = 0;
    
    while(scanf("%s\n", s[nrows++]) >= 1) {
        char *seek;
        if (end == NULL && (seek = strchr(s[nrows-1], 'S')) != NULL) 
            *(end = beg = allocpos()) = (pos_t){nrows-1, seek - s[nrows-1]};
    }
    nrows--;
    ncols = strlen(s[0]);
    v[beg->x][beg->y] = 1;
    s[beg->x][beg->y] = 'a';
    
    while(beg!=NULL) {
        // Check up
        PRINTF1("@%p ", beg);
        PRINTF1("check [%d,%d]  ",  beg->x, beg->y);
        for (int i=0; i<4;i++) { // 0:up 1:left 2:down 3:right
            #ifdef DEBUG1
            if(i==0) printf("U ");
            if(i==1) printf("L ");
            if(i==2) printf("D ");
            if(i==3) printf("R ");
            #endif
            int x = beg->x + ((i-1)%2);
            int y = beg->y + ((i-2)%2);
            char cc = s[beg->x][beg->y];
            int cval = v[beg->x][beg->y];
            if (x < 0 || x >= nrows || y < 0 || y >= ncols)
                continue;
            if (s[x][y] == 'E' && (cc=='y' || cc == 'z')) {
                if (res == 0 || cval < res)
                    res = cval;
            }
            PRINTF1("(%d,%d) ", x, y);
            if ((s[x][y] != 'E' && s[x][y] <= cc + 1) && (v[x][y] == 0 || v[x][y] > (v[beg->x][beg->y] + 1))) {
                PRINTF1("go ");
                *(new = allocpos()) = (pos_t){x, y};
                if(s[x][y] == 'a' && !strcmp(part, "p2")) {
                    v[x][y] = 1;
                } else {
                    v[x][y] = v[beg->x][beg->y] + 1;
                }
                new->next = NULL;
                end->next = new;
                end = end->next;
                PRINTF1("@%p(%d,%d)", new, x, y);
            }
        }
        PRINTF1("\n");
        del = beg;
        beg = beg->next;
        free(del);
    }
    
    for(int i=0;i<nrows;i++) {
        for(int j=0;j<ncols/3;j++) {
            PRINTF2("%c%03d ", s[i][j], v[i][j]);
        }
        PRINTF2("\n");
    }
    printf("%d", res);
}

int main(void) {
    day12("p1");
}