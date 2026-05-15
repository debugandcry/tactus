#pragma once
#include "data.h"
#include "miniaudio.h"
#include "controls.h"
#include "helpers.h"

extern ma_engine engine;

int initialize_audio_engine(ma_engine &engine);

int initialize_song_single(Music &song, std::string &path, ma_engine &engine);

void deinitialize_song_single(Music &song);

void deinitialize_audio_engine(ma_engine &engine);