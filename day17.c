#include "common.h"

// Search for a pattern
// A pattern is :
//   - same curMove, curPiece, also verify that last 2 elements repeats properly
//     => This won't be always true but works here... so don't overcomplicate solution
//   - pattern has : a number of rocks per increment / a depth per increment
//   - if pattern repeats, it will repeat for all next also, so track one only, it will be ok
//     => Beware that it might be a false positive ... if wrong result, add more depth check

#define NUM_ROCKS_P1 2022
#define NUM_ROCKS_P2 1000000000000
// Percentage of moves to let go before looking for a pattern
// //!!\\ Increasing search make pattern search more safe (reduce false positives), while reducing may consume less time/memory
// For example, 10% is enough for the big pattern of exercise, while for the small example, need more like 70%
#define SEARCH_PATTERN_AFTER 10
// Allocation granularity is also a param to choose between performance (big chunk) vs ressource consumption (small chunk)
#define ALLOC_CHUNK 20

typedef struct {
    union {
        uint8_t row[4];
        uint32_t full;
    };
} piece_t;

piece_t rLimit = {0x01, 0x01, 0x01, 0x01};
piece_t lLimit = {0x40, 0x40, 0x40, 0x40};

// All pieces must have on row touching 0x10 and no more
piece_t pieces[5] = {
    (piece_t){.row={0x1E, 0x00, 0x00, 0x00}}, // - 
    (piece_t){.row={0x08, 0x1C, 0x08, 0x00}}, // +
    (piece_t){.row={0x1C, 0x04, 0x04, 0x00}}, // L
    (piece_t){.row={0x10, 0x10, 0x10, 0x10}}, // |
    (piece_t){.row={0x18, 0x18, 0x00, 0x00}}  // o
};

void draw(uint8_t *cave, int32_t around) {
    for (int32_t line = around + 3; line >= around - 2; line--) {
        for (int8_t j = 6; j >= 0; j--) {
            PRINTF1("%c", ((cave[line] >> j) & 0x1)?'#':'.');
        }
        PRINTF1("\n");
    }
    PRINTF1("\n");
}

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    uint8_t *cave = NULL;     // buffer to save rock positions
    int32_t allocSize = 0;    // size of the window allowing to observe the cave
    int64_t realMaxLevel = 0; // First empty row in cave (full depth)
    int64_t maxLevel = 0;     // First empty row in cave (window)
    uint64_t drift;           // Depth of a drift (see below for drift explanation)
    uint8_t curPiece = 0;     // Current piece being processed
    uint32_t curMove = 0;     // Current move from input being processed
    char dirs[11000] = {0};   // All input symbols (manually fixed size ? Yeah, don't overengineer, that's exercise, not product)
    uint32_t maxDir;          // Real number of inputs

    uint32_t curMovePattern;  // Processed move when screenshoting pattern reference
    uint8_t curPiecePattern;  // Processed piece when screenshoting pattern reference
    uint8_t caveCopy[2];      // Processed top of cave when screenshoting pattern reference
    int64_t depthPattern;     // Depth (real) when ... you got it ?
    int64_t rockIdxPattern;   // Rock index when ...
    int patternFound = 0;     // Self explanatory right ?
    int64_t patThreshold;

    int64_t numRocks = ((isp1)?NUM_ROCKS_P1:NUM_ROCKS_P2); // Exercise target

    // Get all dirs at once
    scanf("%s", dirs);
    maxDir = strlen(dirs);
    patThreshold = (maxDir * SEARCH_PATTERN_AFTER)/100;

    for(uint64_t rockIdx = 0; rockIdx < numRocks; rockIdx++) {
        // Get the right piece
        piece_t piece = pieces[curPiece];
        int pieceLevel = maxLevel + 3;
        curPiece = (curPiece + 1) % 5;

        // Keep some margin to manage window depth (could get the exact required margin but well ... exercise ? :D)
        if((pieceLevel + 10) >= allocSize) {
            allocSize += ALLOC_CHUNK;
            cave = (uint8_t *)realloc(cave, allocSize);
            memset(cave + allocSize - ALLOC_CHUNK, 0, ALLOC_CHUNK);
        }

        // Now make it move, we don't know how many moves it will take ... run and see
        while(1) {
            // First the gas push : check vs border & cave content
            if (dirs[curMove] == '>' && (piece.full & rLimit.full) == 0 && (((piece.full >> 1) & ((piece_t *)&cave[pieceLevel])->full) == 0)) {
                piece.full = piece.full >> 1;
            } else if (dirs[curMove] == '<' && (piece.full & lLimit.full) == 0 && (((piece.full << 1) & ((piece_t *)&cave[pieceLevel])->full) == 0)) {
                piece.full = piece.full << 1;
            }
            curMove = (curMove + 1) % maxDir;
            // Then the falling
            if ((pieceLevel > 0) && (piece.full & ((piece_t *)&cave[pieceLevel - 1])->full) == 0)
                pieceLevel -= 1;
            else {
                ((piece_t *)&cave[pieceLevel])->full |= piece.full;
                break;
            }
        }
        // Now check for new maxLevel and possibly for a pattern to jump from start near to the end
        // To even reduce the size of the windows to reduce memory, as soon as there is a closed path (no possibility to add a piece), let's
        // go for a window drift my forgetting about anything below that kind of wall
        drift = 0;
        while(cave[maxLevel] != 0) {
            maxLevel++;
            // Search for cave drift (The 3 lines are ordered so that we can't pass a piece)
            // => Yes, ok, algorithms for that part is garbage ... who cares ? exercise !
            if (maxLevel > 2 && (cave[maxLevel] | cave[maxLevel-1] | cave[maxLevel-2]) == 0x7F) {
                drift = maxLevel -2;
            }
            realMaxLevel++;

            // Record a pattern entry (cross finger to have a big enough patThreshold !)
            if (realMaxLevel == patThreshold) {
                curMovePattern = curMove;
                curPiecePattern = curPiece;
                caveCopy[0] = cave[maxLevel - 2];
                caveCopy[1] = cave[maxLevel - 1];
                depthPattern = realMaxLevel;
                rockIdxPattern = rockIdx;
            }

            // Few ms later ... Pattern found !!!! Apply the pattern X times until the closest possible to the target number of rocks
            if (realMaxLevel > patThreshold && !patternFound && curMove == curMovePattern && curPiece == curPiecePattern && caveCopy[0] == cave[maxLevel - 2] && caveCopy[1] == cave[maxLevel - 1]) {
                int64_t nRocksJump = (numRocks - realMaxLevel) / (rockIdx - rockIdxPattern);
                int64_t depthIncrement = realMaxLevel - depthPattern;
                PRINTF2("Pattern detected @%ld, nRocksJump = %ld, depthIncrement = %ld\n", realMaxLevel, nRocksJump, depthIncrement);
                rockIdx += (rockIdx - rockIdxPattern) * nRocksJump;
                realMaxLevel += (realMaxLevel - depthPattern) * nRocksJump;
                patternFound = 1;
            }
        }
        if (drift) {
            // Found a possibility to drift window, first copy meaningfull elements to the beginning of memory then zero others.
            draw(cave, maxLevel);
            // Copy drift -> maxLevel to 0 -> diff
            for (int64_t j = 0; j <= (maxLevel - drift); j++) {
                cave[j] = cave[drift + j];
            }
            maxLevel = maxLevel - drift;
            memset(cave + maxLevel + 1, 0, allocSize - maxLevel - 1);
            draw(cave, maxLevel);
        }
    }
    printf("%ld\n", realMaxLevel);
}