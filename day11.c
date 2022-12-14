#include "common.h"

long long adder(long long a, long long b) {
    return a+b;
}
long long multiplier(long long a, long long b) {
    return a*b;
}

void day(char *part) {
    struct worry_s;
    
    typedef struct worry_s {
        long long val;
        struct worry_s *wor;
    } worry_t;
    
    typedef struct {
        worry_t *wors;
        worry_t *ewor;
        long long (*op)(long long, long long);
        int usefixop;
        int fixop;
        int divtest;
        int gotrue;
        int gofalse;
        int count;
    } monkey_t;
    
    monkey_t monkeys[10] = {0};
    worry_t wors[40] = {0};
    int nwors = 0;
    char s[100], op[2], fixop[5];
    char *seps = " :,";
    int idx;
    char *tok;
    int globalmod = 1;
    
    while (scanf("%*s %d:\n", &idx) == 1) {
        PRINTF1("LINE 1 --- IDX: %d\n", idx);
        fgets(s, 100, stdin);
        tok = strtok(s, seps); // Starting
        tok = strtok(NULL, seps); //items
        while((tok = strtok(NULL, seps)) != NULL) {
            if (strcmp(tok, "0\n")) {
                if (monkeys[idx].ewor == NULL) {
                    PRINTF1("LINE 2 --- Add first ");
                    monkeys[idx].wors = &wors[nwors++];
                    monkeys[idx].ewor = monkeys[idx].wors;
                } else {
                    PRINTF1("LINE 2 --- Add other ");
                    monkeys[idx].ewor->wor = &wors[nwors++];
                    monkeys[idx].ewor = monkeys[idx].ewor->wor;
                }
                PRINTF1(": %d\n", atoi(tok));
                monkeys[idx].ewor->val = atoi(tok);
                monkeys[idx].ewor->wor = NULL;
            }
        }
        scanf("  %*s %*s %*s %*s %s %s", op, fixop);
        if (op[0] == '+') {
            PRINTF1("LINE 3 --- ADDER ");
            monkeys[idx].op = &adder;
        } else {
            PRINTF1("LINE 3 --- MULTIPLIER ");
            monkeys[idx].op = &multiplier;
        }
        if (!strcmp(fixop, "old")) {
            monkeys[idx].usefixop = 0;
            PRINTF1("(old, old)\n");
        } else {
            monkeys[idx].usefixop = 1;
            monkeys[idx].fixop = atoi(fixop);
            PRINTF1("(old, %d)\n", monkeys[idx].fixop);
        }
        scanf("  %*s %*s %*s %d", &monkeys[idx].divtest);
        globalmod = globalmod * monkeys[idx].divtest;
        PRINTF1("LINE 4 --- DIV: %d\n", monkeys[idx].divtest);
        scanf("    %*s %*s %*s %*s %*s %d", &monkeys[idx].gotrue);
        PRINTF1("LINE 5 --- TRUE: %d\n", monkeys[idx].gotrue);
        scanf("    %*s %*s %*s %*s %*s %d", &monkeys[idx].gofalse);
        PRINTF1("LINE 6 --- FALSE: %d\n", monkeys[idx].gofalse);
        PRINTF1("\n");
    }
    
    PRINTF2("globalmod = %d\n", globalmod);
    
    for (int i=0; i<((strcmp(part, "p1"))?10000:20); i++) {
        PRINTF1("ROUND %d\n", i);
        for (int j=0; j <= idx; j++) {
            worry_t *nwor, *wor = monkeys[j].wors;
            while (wor != NULL) {
                PRINTF1("MONKEY %d: worry %lld => ", j, wor->val);
                monkeys[j].wors = wor->wor;
                monkeys[j].count++;
                if(monkeys[j].usefixop) {
                    wor->val = monkeys[j].op(monkeys[j].fixop, wor->val);
                } else {
                    wor->val = monkeys[j].op(wor->val, wor->val);
                }
                if (strcmp(part, "p1"))
                    wor->val = wor->val % globalmod;
                else
                    wor->val = wor->val /3;

                int nidx = (wor->val % monkeys[j].divtest)?monkeys[j].gofalse:monkeys[j].gotrue;
                PRINTF1("%lld to MONKEY %d\n", wor->val, nidx);
                if (monkeys[nidx].ewor == NULL) {
                    monkeys[nidx].wors = wor;
                } else {
                    monkeys[nidx].ewor->wor = wor;
                }
                monkeys[nidx].ewor = wor;
                nwor = wor->wor;
                wor->wor = NULL;
                wor = nwor;
            }
            monkeys[j].ewor = NULL;
        }
    }
    long long max[2] = {0};
    for (int j=0; j <= idx; j++) {
        PRINTF2("Monkey %d inspected items %d times.\n", j, monkeys[j].count);
        if(monkeys[j].count > max[0]) {
            if(monkeys[j].count > max[1]) {
                max[0] = max[1];
                max[1] = monkeys[j].count; 
            } else {
                max[0] = monkeys[j].count;
            }
        }
    }
    printf("%lld\n", max[0]*max[1]);
}
