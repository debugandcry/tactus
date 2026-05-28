#pragma once
#include "data.h"

extern Slot* current;
extern Slot* next;
extern Slot* previous;

void preload_slots(bool preload);

void rotate_slots();

void unrotate_slots();

void reset_slots();