#pragma once
#include "miniaudio.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <thread>
#include <functional>
#include <windows.h>
#include <commdlg.h>

extern bool song_process_running;

struct Music {
    ma_sound sound;
    ma_format format;
    ma_uint32 sample_rate;
    ma_uint32 channels;
    ma_uint64 totalframes;
    ma_uint64 skipframes;
    std::string name;
    std::string path;
    std::string artist;
    std::string cover_art_path;
    float duration;
    bool state;
};


struct KeyStates {
    bool current;
    bool previous;
};