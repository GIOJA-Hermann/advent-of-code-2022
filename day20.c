#include "common.h"

#define MAX(x,y) ((x>y)?x:y)
#define MIN(x,y) ((x<y)?x:y)

#define NENTRIES 5000// From number of lines in file

typedef struct {
    int64_t val;
    int16_t order;
} num_t;

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");

    int32_t idx = 0, shift1, shift2;
    int16_t order = 0;
    num_t nums[NENTRIES] = {0};
    num_t nums2[NENTRIES] = {0};
    
    while(scanf("%ld\n", &(nums[idx].val)) == 1){
        nums2[idx].val = nums[idx].val * ((isp1)?1:811589153);
        // A complete turn around the list is NENTRIES - 1 (number of intervals between others)
        // Better to avoid spinning without reason, do only the meaningfull stuff
        nums[idx].val = nums2[idx].val % (NENTRIES -1);
        nums[idx].order = idx;
        idx++;
    }
    idx = 0;

    for (int mix = 0; mix < ((isp1)?1:10); mix++) {
        order = 0;
        // Process all numbers in the right order
        while (order != NENTRIES) {
            // Find the number in the whole list (could go with a more sophisticated pointer approach, but that's enough here for required perf)
            if (nums[idx].order != order) { idx = (idx + 1) % NENTRIES; continue;}
            // Keep track of it since we will move other numbers then write it back
            num_t num = nums[idx];
            // Process number move (yes here too there are more sophisticated approches but this one is not too much of a mess)
            // number in SHIFT2 (next slot) goes in SHIFT1 (current slot)
            // SHIFT2 is left of SHIFT1 when using negative number and right for positive
            // Don't forget to wrap around on borders
            int16_t loopNum;
            loopNum = num.val;
            shift2 = idx;
            while (loopNum != 0) {
                PRINTF1("loopnum %d => %d (%d)\n", loopNum, num.val, nums2[order]);
                shift1 = shift2;
                shift2 = (shift1 + ((num.val>0)?1:-1) + NENTRIES) % NENTRIES;
                nums[shift1] = nums[shift2];
                loopNum -= ((num.val>0)?1:-1);
            }
            nums[shift2] = num;
            PRINTF1("%.4hd => %ld (%ld)\n", nums[shift2].order, num.val, nums2[order].val);
            order++;
        }
    }
    
    // Find where is 0 (hopefully there isn't 2 of it after modulo. Otherwise we could find it differently)
    for(idx=0; nums[idx].val != 0; idx++);

    PRINTF2("%d %ld\n", (idx) % NENTRIES, nums[(idx) % NENTRIES].val);
    PRINTF2("%d %ld\n", (idx + 1000) % NENTRIES, nums2[nums[(idx + 1000) % NENTRIES].order].val);
    PRINTF2("%d %ld\n", (idx + 2000) % NENTRIES, nums2[nums[(idx + 2000) % NENTRIES].order].val);
    PRINTF2("%d %ld\n", (idx + 3000) % NENTRIES, nums2[nums[(idx + 3000) % NENTRIES].order].val);
    printf("%ld\n", nums2[nums[(idx + 1000) % NENTRIES].order].val + nums2[nums[(idx + 2000) % NENTRIES].order].val + nums2[nums[(idx + 3000) % NENTRIES].order].val);
}
