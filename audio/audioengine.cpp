#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <stdio.h>
#include <iostream>
std::string path = "C:/Users/grsma/mergere/audio/breathe_in_the_air.mp3";
int main() {
    ma_engine engine;
    ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {std::cout << "can't setup engine \n" << status << "\n"; return -1;}
    ma_sound song;
    ma_result song_status = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &song);
    if (song_status != MA_SUCCESS) {std::cout << "can't setup sound \n" << song_status << "\n"; return -1;}
    ma_sound_start(&song);
    std::cin.get();
    ma_sound_uninit(&song);
    ma_engine_uninit(&engine);
}