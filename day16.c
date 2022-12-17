#include "common.h"

// Room position on grid (only needed for path computation)
typedef struct {
    uint16_t x;
    uint16_t y;
} pos_t;

// Room descriptor
struct room_s;
typedef struct room_s{
    uint8_t rate;
    uint8_t active; // Prevents from using same room twice (for both pathFinding and exploration)
    uint8_t nTunnels; // Only for pathFinding
    pos_t tunnels[6]; // Only for pathFinding
    int dist[20]; // computed during pathFinding, used during exploration
} room_t;

// Exploration status
typedef struct {
    int16_t minutes;
    uint64_t pressure;
    uint64_t maxPressure;
} status_t;

// Computed during pathFinding.
// This is essentialy a compressed version of inputs,
// keeping only activable rooms and their direct distance from each others
// ==> Allows to dramatically reduce the exploration recursive-depth (and paths)
struct path_s;
typedef struct path_s {
    room_t *room;
    uint8_t dist;
    struct path_s *next;
} path_t;

// For each possible path (even non-optimal), keep track of activated rooms
// Required to evaluate best team result with any number of stakeholder
typedef struct {
    uint16_t pathBits;
    uint64_t maxPressure;
} evalPath_t;


///////////////////////////////////////////
///// Global vars ... easier that way /////
///////////////////////////////////////////
room_t rooms[26][26] = {0};     // The grid of rooms ... simply
room_t *posRates[20];           // Pointers to rooms with non-zero rates
uint8_t nPosRates = 0;          // Number of valid elements in posRates table

evalPath_t *evalPaths = NULL;   // Auto-grow buffer for path evaluation (hard to evaluate number of paths in advance)
size_t maxEvalPaths = 0;        // Maximum number of paths that buffer can hold (until next resize)
size_t nEvalPaths = 0;          // Number of valid paths explored (and saved in buffer)
///////////////////////////////////////////

// Simply reset active bit for all rooms (all grid positions ... lazy)
// Useful for successive pathFindings
// DO NOT USE in the middle of a pathFinding or exploration
void resetActive() {
    for (uint8_t i = 0; i < 26; i++) {
        for (uint8_t j = 0; j < 26; j++) {
            rooms[i][j].active = 0;
        }
    }
}

// PathFinding algorithm. From the given room, find the shortest path to all others
// Considering only posRates rooms as valid targets
// Result : save distance to each posRates room
void findPaths(room_t *room) {
    uint8_t found = 0; 
    path_t *path = malloc(sizeof(path_t)), *nextPath, *ePath;
    *path = (path_t){.room = room, .dist = 0, .next = NULL};
    path->room->active = 1;
    ePath = path;

    while(path) {
        // check if current is a posRate
        for (uint8_t i = 0; i < nPosRates; i++) {
            if (path->room == posRates[i]) {
                room->dist[i] = path->dist;
                found++;
                break;
            }
        }
        if (found == nPosRates) {
            break;
        }
        // explore tunnels (1 step)
        for (uint8_t i = 0; i < path->room->nTunnels; i++) {
            room_t *nextRoom = &rooms[path->room->tunnels[i].x][path->room->tunnels[i].y];
            if (!nextRoom->active) {
                nextPath = malloc(sizeof(path_t));
                *nextPath = (path_t){.room = nextRoom, .dist = path->dist + 1, .next = NULL};
                nextPath->room->active = 1;
                ePath->next = nextPath;
                ePath = nextPath;
            }
        }
        nextPath = path->next;
        free(path);
        path=nextPath;
    }
    // Free remaining paths
    while(path) {
        nextPath = path->next;
        free(path);
        path=nextPath;
    }
}

// Recursively check possible paths from start zone by jumping from activable room to another
// thanks to the pathFinding done before. Exploration is limited by number of minutes.
// Immediate result : maximum 1-man pressure released
// Other results : pressure released for each path followed (even non-leaf paths), useful for team exploration
void exploreRoom(status_t *status, room_t *room, int16_t dist, uint16_t pathBits);
void exploreRoom(status_t *status, room_t *room, int16_t dist, uint16_t pathBits) {
    // Entering the Room only if not yet active and activable
    uint8_t actCost = !!room->rate;
    if (room->active || (status->minutes <= (dist + actCost))) {
        return;
    }
    // Remove distance + activation (1)
    status->minutes -= dist + actCost;
    // activate room, and add released pressure up to last minute
    room->active = !!room->rate; // do not make start zone active
    status->pressure += status->minutes * room->rate;
    if (status->pressure > status->maxPressure) status->maxPressure = status->pressure;
    // Not enough space to save path evaluation, double the current size (memoryhoolic !)
    if (nEvalPaths == maxEvalPaths) evalPaths = realloc(evalPaths, sizeof(evalPath_t) * (maxEvalPaths = 2 * (maxEvalPaths?maxEvalPaths:1)));
    evalPaths[nEvalPaths++] = (evalPath_t){.pathBits = pathBits, .maxPressure = status->pressure};
    // From there, activate others
    for (uint8_t i = 0; i < nPosRates; i++) {
        exploreRoom(status, posRates[i], room->dist[i], pathBits | (0x1 << i));
    }
    // Step back to explore other path
    status->pressure -= status->minutes * room->rate;
    room->active = 0;
    status->minutes += dist + actCost;
}

// Step 1 : pathFinding : get rid of rate=0 rooms by calculating distance (in minutes) between each rate>0 rooms
//  => produces a shorter input with only rate>0 rooms and distances >= 1 minute. Less data to recurse on for next step
// Step 2 : exploration : compute result of each possible combination
// Step 3 (p2) : mix all 1-man explored paths to deduce the best team path
void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    char s[200], *sp;
    char x,y;
    uint8_t rate, idx;

    // First compute the simplified room grid
    while (fgets(s, 200, stdin)) {
        sscanf(s, "Valve %c%c has flow rate=%hhu", &x, &y, &rate);
        room_t *room = &rooms[x - 'A'][y - 'A'];
        if (rate) {
            room->rate = rate;
            posRates[nPosRates++] = room;
        }
        idx = 0;
        sp = (sp = strstr(s, "to valves"))?(sp + 9):(strstr(s, "to valve") + 8);
        while(sscanf(sp, " %c%c", &x, &y) == 2) {
            sp +=4;
            room->tunnels[room->nTunnels++] = (pos_t){.x = x - 'A', .y = y - 'A'};
        }
    }

#ifdef DEBUG1
    // Print the grid. Since it's small, can help (ahah ... or not :D)
    for (x = 0; x < 26; x++) {
        for(y = 0; y < 26; y++) {
            if (rooms[x][y].nTunnels) PRINTF1("%.2hhu ", rooms[x][y].rate);
            else PRINTF1(".. ");
        }
        PRINTF1("\n");
    }
#endif

    status_t status = {
        .minutes = (isp1)?30:26,
        .pressure = 0,
        .maxPressure = 0
    };

    // Compute all paths from rooms with non-Zero release potential (and from start zone)
    // This will dramatically reduce search cost
    findPaths(&rooms[0][0]);
    resetActive();
#ifdef DEBUG2
    for (uint8_t i = 0; i < nPosRates; i++) {
        PRINTF2("%.2hhu ", rooms[0][0].dist[i]);
    }
    PRINTF2("\n");
#endif
    for (uint8_t i = 0; i < nPosRates; i++) {
        findPaths(posRates[i]);
        resetActive();
#ifdef DEBUG2
        for (uint8_t j = 0;j < nPosRates; j++) {
            PRINTF2("%.2hhu ", posRates[i]->dist[j]);
        }
        PRINTF2("\n");
#endif
    }
    exploreRoom(&status, &rooms[0][0], 0, 0);
    // On part 2, we don't want the best "alone" but the best "combination"
    if (!isp1) {
        // Evaluate each combination once only (who takes what doesn't matter)
        for (uint32_t i = 0; i < nEvalPaths; i++) {
            for (uint32_t j = i; j < nEvalPaths; j++) {
                // If paths are compatible (no common activation), compute maxPressure of both
                if (! (evalPaths[i].pathBits & evalPaths[j].pathBits)) {
                    uint64_t pressure = evalPaths[i].maxPressure + evalPaths[j].maxPressure;
                    if (pressure > status.maxPressure) status.maxPressure = pressure;
                }
            }
        }
    }
    printf("%ld\n", status.maxPressure);
}