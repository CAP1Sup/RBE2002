#pragma once
// Mode of operation
#define ZOMBIE
// #define SURVIVOR

#define MM_TO_CM 0.1f

// Sanity checker
#if (defined(ZOMBIE) + defined(SURVIVOR) != 1)
#error "Please select exactly one mode of operation"
#endif
