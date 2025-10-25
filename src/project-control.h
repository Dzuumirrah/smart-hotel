#ifndef PROJECT_CONTROL_H
#define PROJECT_CONTROL_H

#if defined(PROJECT_HILMAN)
#include "project-hilman.h"
#include "control.cpp"

#elif defined(PROJECT_MERLIN)
#include "project-merlin.h"

#elif defined(PROJECT_GALIH)
#include "project-galih.h"

#elif defined(PROJECT_FARREL)
#include "project-farrel.h"

#elif defined(PROJECT_DIPTA)
#include "project-dipta.h"

// --- ADD YOUR PROJECT HERE ---
#elif defined(PROJECT_FAZLA)
#include "project-fazla.h" // Include your new project file
// --- END OF YOUR ADDITION ---

#else
#error "No project defined. Please define one of PROJECT_HILMAN, PROJECT_MERLIN, PROJECT_GALIH, PROJECT_FARREL, or PROJECT_FAZLA."

#endif // This closes the #if defined(...) block

#endif // This closes the #ifndef PROJECT_CONTROL_H
