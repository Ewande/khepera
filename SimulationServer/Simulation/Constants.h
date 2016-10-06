#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <stdint.h>

// SIMULATION CONSTANTS
#define NUMBER_OF_CHECKS            3
#define DIVIDING_LEVEL              3
#define RESERVED_ID_LEVEL           1000
#define MAX_ID_LEVEL                RESERVED_ID_LEVEL + 20
#define NO_COLLISION	            -100
#define INF_COLLISION               1000000
#define EPS                         0.0001

#define DEFAULT_SIMULATION_STEP     0.04
#define DEFAULT_SIMULATION_DELAY    40

class SimEnt;
typedef std::map<uint16_t, int>     DistanceMap;
typedef std::map<uint16_t, SimEnt*> SimEntMap;

#endif