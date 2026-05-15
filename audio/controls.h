#pragma once
#include "data.h"

extern KeyStates keys[256];

void song_path_query(std::vector<std::string> &queue);

bool pressed_query(int keyval);

void update_query(int key);

void transport_handler(Music &song, ma_engine &engine);