#pragma once
// Mode of operation
#define ZOMBIE
// #define SURVIVOR

// Sanity checker
#if (defined(ZOMBIE) + defined(SURVIVOR) != 1)
#error "Please select exactly one mode of operation"
#endif
