#pragma once
// Mode of operation
#define ZOMBIE
// #define SURVIVOR

// Tag IDs
#define ZOMBIE_TOP_TAG_ID 0  // TODO: FIX
#define SURVIVOR_TOP_TAG_ID 0
#define SURVIVOR_SIDE_TAG_ID 4

// Tag sizing
#define MAX_SURVIVOR_TAG_WIDTH 100

// Unit conversions
#define MM_TO_CM 0.1f

// Sanity checker
#if (defined(ZOMBIE) + defined(SURVIVOR) != 1)
#error "Please select exactly one mode of operation"
#endif
