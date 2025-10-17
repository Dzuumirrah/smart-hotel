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

#else
#error "No project defined. Please define one of PROJECT_HILMAN, PROJECT_MERLIN, PROJECT_GALIH, or PROJECT_FARREL."


#endif