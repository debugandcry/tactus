#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <conio.h>
std::string path = "C:/Users/grsma/mergere/audio/breathe_in_the_air.mp3";
int main() {
    bool is_running = true;
    std::cout << "> program started, running... \n";
    ma_engine engine;
    ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {std::cout << "> can't setup engine. \n" << status << "\n"; return -1;}
    std::cout << "> engine initialized. \n";
    ma_sound song;
    ma_result song_status = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &song);
    if (song_status != MA_SUCCESS) {std::cout << "> can't setup sound. \n" << song_status << "\n"; return -1;}
    std::cout << "> sound initialized. \n";
    ma_sound_start(&song);
    std::cout << "> sound started. \n";
    while (is_running) {
        char userinput = _getch();
        if (userinput == ' ') {if (ma_sound_is_playing(&song) == MA_TRUE) {ma_sound_stop(&song); std::cout << "> space pressed, audio stopped. \n";} else {ma_sound_start(&song); std::cout << "> space pressed, audio playing \n";}}
        else if (userinput == 'r') {ma_sound_stop(&song); ma_sound_seek_to_pcm_frame(&song, 0); ma_sound_start(&song); std::cout << "> 'r' pressed, audio restarted. \n";}
        else if (userinput == 27) {ma_sound_stop(&song); is_running = false; std::cout << "> 'esc' pressed, exiting... \n";}
    }
    ma_sound_uninit(&song); ma_engine_uninit(&engine);
    std::cout << "> cleanup successful, exiting... \n";
}