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