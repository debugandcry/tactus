#pragma once
#include "data.h"

extern KeyStates keys[256];

std::string song_path_single();

bool pressed_query(int keyval);

void update_query(int key);

void transport_handler(Music &song);