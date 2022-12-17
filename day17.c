#include "common.h"

// Expected complexity => manage depth

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

void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    uint8_t *cave = (uint8_t*)malloc(4000);
    int32_t allocSize = 0;
    int64_t realMaxLevel = 0; // First empty row in cave
    int64_t maxLevel = 0; // First empty row in cave window
    uint8_t curPiece = 0;
    uint32_t curMove = 0;
    char dirs[11000] = {0};
    uint32_t maxDir;

    // Get all dirs at once
    scanf("%s", dirs);
    maxDir = strlen(dirs);

    for(uint16_t i = 0; i < 2022; i++) {
        // Get the right piece
        piece_t piece = pieces[curPiece];
        int pieceLevel = maxLevel + 3;
        curPiece = (curPiece + 1) % 5;

        while(1) {
            // Now make it move
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
        // And check for a new maxLevel
        while(cave[maxLevel] != 0) {
            if (maxLevel > 2 && (cave[maxLevel] | cave[maxLevel-1] | cave[maxLevel-2]) == 0x7F)
                printf("Cave drift @%ld\n", maxLevel);
            maxLevel++;
            //printf("LEV %ld\n", maxLevel);
        }
    }

    for (int32_t i = maxLevel; i >= 0; i--) {
        for (int8_t j = 6; j >= 0; j--) {
            PRINTF1("%c", ((cave[i] >> j) & 0x1)?'#':'.');
        }
        PRINTF1("\n");
    }
    printf("LEV %ld\n", maxLevel);
}