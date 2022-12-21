#include "common.h"

#define SIZE 4002
#define MUL 1
#define DIV 2
#define ADD 3
#define SUB 4

typedef struct {
    union {
        char str[5];
        uint32_t val;
    };
} target_t;

typedef struct {
    target_t me;
    target_t n1;
    target_t n2;
    int op;
    int64_t res;
} elem_t;

void addElem(elem_t *elems, char *s) {
    target_t t1 = {0}, t2 = {0}, t3 = {0};
    char op = 0;
    int res = 0, idx = 0;
    if (s[6] >= '0' && s[6] <= '9') {
        sscanf(&s[0], "%[^:]: %d", t1.str, &res);
    } else {
        sscanf(&s[0], "%[^:]: %s %c %s", t1.str, t2.str, &op, t3.str);
        switch (op) {
            case '*' : op = MUL; break;
            case '/' : op = DIV; break;
            case '+' : op = ADD; break;
            case '-' : op = SUB; break;
        }
        PRINTF1("%s => %s %d %s\n", t1.str, t2.str, op, t3.str);
    }
    idx = t1.val % SIZE;
    while(elems[idx].me.val != 0) idx++;
    elems[idx] = (elem_t) {t1, t2, t3, op, res};
}

int findElem(elem_t *elems, uint32_t val) {
    int idx = val % SIZE;
    while(elems[idx].me.val != val) idx++;
    return idx;
}

int64_t computeVal(elem_t *elems, uint32_t idx) {
    if (elems[idx].op != 0) {
        int64_t v1, v2;
        v1 = computeVal(elems, findElem(elems, elems[idx].n1.val));
        v2 = computeVal(elems, findElem(elems, elems[idx].n2.val));
        switch (elems[idx].op) {
            case MUL: elems[idx].res = v1 * v2; break;
            case DIV: elems[idx].res = v1 / v2; break;
            case ADD: elems[idx].res = v1 + v2; break;
            case SUB: elems[idx].res = v1 - v2; break;
        }
    }
    return elems[idx].res;
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    target_t root = {0};
    target_t humn = {0};
    strcpy(root.str, "root");
    strcpy(humn.str, "humn");

    uint32_t rootIdx, n1Idx, n2Idx, found = 0;
    int64_t n1val, n2val, humnVal, humnValKeep;

    elem_t elems[SIZE] = {0};
    elem_t elemsWork[SIZE];
    char str[30];

    while(fgets(str, 30, stdin)) {
        addElem(elems, str);
    }

    if(isp1) {
        // Recursively compute root value
        memcpy(elemsWork, elems, SIZE * sizeof(elem_t));
        elemsWork[findElem(elemsWork, humn.val)].res += 0;
        computeVal(elemsWork, findElem(elemsWork, root.val));
        rootIdx = findElem(elemsWork, root.val);
        printf("%ld\n", elemsWork[rootIdx].res);
    } else {
        int64_t incr = 1;
        int32_t shift = -1;
        // First find the minimum binary shifted number that allows to reach target (consider n1 is greater than n2, otherwise modify a bit)
        do {
            shift++;
            humnVal = (1L << shift);
            memcpy(elemsWork, elems, SIZE * sizeof(elem_t));
            elemsWork[findElem(elemsWork, humn.val)].res = humnVal;
            computeVal(elemsWork, findElem(elemsWork, root.val));
            rootIdx = findElem(elemsWork, root.val);
            n1Idx = findElem(elemsWork, elemsWork[rootIdx].n1.val);
            n2Idx = findElem(elemsWork, elemsWork[rootIdx].n2.val);
            n1val = elemsWork[n1Idx].res;
            n2val = elemsWork[n2Idx].res;
            PRINTF2("shift : %d ( %ld , %ld ), %ld\n", shift, n1val, n2val, humnVal);
        } while (n1val > n2val && shift < 64);

        // Then fill the bits below until we find the right number
        while(shift--) {
            humnVal = humnValKeep | (1L << shift);
            memcpy(elemsWork, elems, SIZE * sizeof(elem_t));
            elemsWork[findElem(elemsWork, humn.val)].res = humnVal;
            computeVal(elemsWork, findElem(elemsWork, root.val));
            rootIdx = findElem(elemsWork, root.val);
            n1Idx = findElem(elemsWork, elemsWork[rootIdx].n1.val);
            n2Idx = findElem(elemsWork, elemsWork[rootIdx].n2.val);
            n1val = elemsWork[n1Idx].res;
            n2val = elemsWork[n2Idx].res;
            PRINTF2("shift : %d ( %ld , %ld ), %ld\n", shift, n1val, n2val, humnVal);
            if(n1val >= n2val) {
                humnValKeep = humnVal;
            }
            if(n1val == n2val) {
                printf("%s%ld\n", found?"or ":"", humnVal);
                found = 1;
            }
        }
    }
}
