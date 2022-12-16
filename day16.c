#include "common.h"

typedef struct {
    uint16_t x;
    uint16_t y;
} pos_t;

struct room_s;
typedef struct room_s{
    uint8_t rate;
    uint8_t active;
    uint8_t closed;
    uint8_t nTunnels;
    pos_t tunnels[6];
    int dist[20];
} room_t;

typedef struct {
    int16_t minutes;
    uint64_t pressure;
    uint64_t maxPressure;
} status_t;

room_t rooms[26][26] = {0};
room_t *posRates[20];
uint8_t nPosRates = 0;


struct path_s;
typedef struct path_s {
    room_t *room;
    uint8_t dist;
    struct path_s *next;
} path_t;

void resetClosed() {
    for (uint8_t i = 0; i < 26; i++) {
        for (uint8_t j = 0; j < 26; j++) {
            rooms[i][j].closed = 0;
        }
    }
}

void findPaths(room_t *room) {
    uint8_t found = 0; 
    path_t *path = malloc(sizeof(path_t)), *nextPath, *ePath;
    *path = (path_t){.room = room, .dist = 0, .next = NULL};
    path->room->closed = 1;
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
            if (!nextRoom->closed) {
                nextPath = malloc(sizeof(path_t));
                *nextPath = (path_t){.room = nextRoom, .dist = path->dist + 1, .next = NULL};
                nextPath->room->closed = 1;
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

void exploreRoom(status_t *status, room_t *room, int16_t dist);
void exploreRoom(status_t *status, room_t *room, int16_t dist) {
    // Entering the Room only if not yet active and activable
    uint8_t actCost = !!room->rate;
    if (room->active || (status->minutes <= (dist + actCost))) {
        return;
    }
    // Remove distance + activation (1)
    status->minutes -= dist + actCost;
    // activate room, and add released pressure up to last minute
    room->active = 1;
    status->pressure += status->minutes * room->rate;
    if (status->pressure > status->maxPressure) status->maxPressure = status->pressure;
    // From there, activate others
    for (uint8_t i = 0; i < nPosRates; i++) {
        exploreRoom(status, posRates[i], room->dist[i]);
    }
    // Step back to explore other path
    status->pressure -= status->minutes * room->rate;
    room->active = 0;
    status->minutes += dist + actCost;
}



void day(const char *part) {
    int isp1 = !strcmp(part, "p1");
    if(!isp1) return;
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
        .minutes = 30,
        .pressure = 0,
        .maxPressure = 0
    };

    // Compute all paths from rooms with non-Zero release potential (and from start zone)
    // This will dramatically reduce search cost
    findPaths(&rooms[0][0]);
    resetClosed();
#ifdef DEBUG2
    for (uint8_t i = 0; i < nPosRates; i++) {
        PRINTF2("%.2hhu ", rooms[0][0].dist[i]);
    }
    PRINTF2("\n");
#endif
    for (uint8_t i = 0; i < nPosRates; i++) {
        findPaths(posRates[i]);
        resetClosed();
#ifdef DEBUG2
        for (uint8_t j = 0;j < nPosRates; j++) {
            PRINTF2("%.2hhu ", posRates[i]->dist[j]);
        }
        PRINTF2("\n");
#endif
    }
    exploreRoom(&status, &rooms[0][0], 0);
    printf("%ld\n", status.maxPressure);
}