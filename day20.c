#include "common.h"

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)

#define NENTRIES 5000// From number of lines in file
//#define NENTRIES 7

typedef struct {
    int16_t val;
    int16_t order;
} num_t;

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    if (!isp1) return;

    int32_t idx = 0, shift1, shift2;
    int16_t order = 0;
    num_t nums[NENTRIES] = {0};
    num_t nums2[NENTRIES] = {0};
    
    while(scanf("%hd\n", &(nums[idx].val)) == 1){
        nums2[idx] = nums[idx];
        nums[idx].val = nums[idx].val % (NENTRIES -1);
        nums[idx].order = idx;
        idx++;
    }
    idx = 0;

    while (order != NENTRIES) {
        if (nums[idx].order != order) { idx = (idx + 1) % NENTRIES; continue;}
        int16_t loopNum;
        num_t num = nums[idx];
        loopNum = num.val;
        shift2 = idx;
        PRINTF1("idx: %d (%hd, %hd)\n", idx, num.val, loopNum);
        while (loopNum != 0) {
            PRINTF1("loopnum %d => %d (%d)\n", loopNum, num.val, nums2[order]);
            shift1 = shift2;
            shift2 = (shift1 + ((num.val>0)?1:-1) + NENTRIES) % NENTRIES;
            nums[shift1] = nums[shift2];
            loopNum -= ((num.val>0)?1:-1);
        }
        nums[shift2] = num;
        PRINTF2("%.4hd => %d (%d)\n", nums[shift2].order, num.val, nums2[order].val);
        order++;

    }
    
    for(idx=0; nums[idx].val != 0; idx++);

    PRINTF2("%d %hd\n", (idx) % NENTRIES, nums[(idx) % NENTRIES].val);
    PRINTF2("%d %hd\n", (idx + 1000) % NENTRIES, nums2[nums[(idx + 1000) % NENTRIES].order].val);
    PRINTF2("%d %hd\n", (idx + 2000) % NENTRIES, nums2[nums[(idx + 2000) % NENTRIES].order].val);
    PRINTF2("%d %hd\n", (idx + 3000) % NENTRIES, nums2[nums[(idx + 3000) % NENTRIES].order].val);
    printf("%hd\n", nums2[nums[(idx + 1000) % NENTRIES].order].val + nums2[nums[(idx + 2000) % NENTRIES].order].val + nums2[nums[(idx + 3000) % NENTRIES].order].val);
}
