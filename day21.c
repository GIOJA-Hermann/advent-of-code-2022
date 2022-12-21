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

int addElem(elem_t *elems, char *s) {
    target_t t1 = {0}, t2 = {0}, t3 = {0};
    char op;
    int res, idx, ret = 0;
    if (s[6] >= '0' && s[6] <= '9') {
        sscanf(&s[0], "%[^:]: %d", t1.str, &res);
        printf("%s => %d\n", t1.str, res);
    } else {
        sscanf(&s[0], "%[^:]: %s %c %s", t1.str, t2.str, &op, t3.str);
        switch (op) {
            case '*' : op = MUL; break;
            case '/' : op = DIV; break;
            case '+' : op = ADD; break;
            case '-' : op = SUB; break;
        }
        printf("%s => %s %d %s\n", t1.str, t2.str, op, t3.str);
        ret = 1;
    }
    idx = t1.val % SIZE;
    while(elems[idx].me.val != 0) idx++;
    elems[idx] = (elem_t) {t1, t2, t3, op, res};
    return ret;
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
    int resolve = 0;

    elem_t elems[SIZE] = {0};
    char str[30];

    while(fgets(str, 30, stdin)) {
        resolve += addElem(elems, str);
    }

    target_t root = {0};
    strcpy(root.str, "root");
    
    printf("%x %d %d\n", root.val, root.val % SIZE, findElem(elems, root.val));

    printf("%ld\n", computeVal(elems, findElem(elems, root.val)));
}
