#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include "miniaudio.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <windows.h>
#include <thread>

std::ofstream logs;

bool song_process_running = true;

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

void logger(const std::string& msg) {
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_proper = std::chrono::system_clock::to_time_t(current_time);
    std::tm* local_time = std::localtime(&current_time_proper);
    logs 
        << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":" 
        << std::setw(2) << std::setfill('0') << local_time->tm_min << ":" 
        << std::setw(2) << std::setfill('0') << local_time->tm_sec << ":" 
        << " > " << msg << "\n";
    std::cout << "> " << msg << "\n";
}

struct KeyStates {
    bool current;
    bool previous;
};

KeyStates keys[256];

bool pressed_query(int keyval) {
    return keys[keyval].current && !keys[keyval].previous;
}

void update_query(int key) {
    keys[key].previous = keys[key].current;
    keys[key].current = GetAsyncKeyState(key) & 0x8000;
} 

void rest(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


void song_input_handler(Music &song) {
    while (song_process_running) {
        
        update_query(VK_SPACE);
        update_query(VK_ESCAPE);
        update_query('R');
        update_query(VK_RIGHT);
        update_query(VK_LEFT);
        
        if (pressed_query(VK_ESCAPE)) {
            ma_sound_stop(&song.sound);
            song_process_running = false;
            logger("'esc' key pressed, proceeding to exit.");
        }
        
        else if (pressed_query(VK_SPACE)) {
            if (ma_sound_is_playing(&song.sound) == MA_TRUE){
                ma_sound_stop(&song.sound);
                logger("spacebar pressed, audio playback paused.");    
            }
            else {
                ma_sound_start(&song.sound);
                logger("spacebar pressed, audio playback resumed.");
            }
        }

        else if (pressed_query('R')) {
            ma_sound_stop(&song.sound);
            ma_sound_seek_to_pcm_frame(&song.sound, 0); 
            ma_sound_start(&song.sound);
            logger("'r' key pressed, audio restarted from 00:00:00, or pcm frame 0.");
        }
    
        else if (keys[VK_RIGHT].current) {
            ma_uint64 current_frame;
            ma_sound_get_cursor_in_pcm_frames(&song.sound, &current_frame);
            if ((current_frame + song.skipframes) < song.totalframes) {
                    ma_sound_seek_to_pcm_frame(&song.sound, (current_frame + song.skipframes));
                    logger("right arrow key pressed, seeked 5 seconds forward into audio.");
                }
                else {
                    ma_sound_seek_to_pcm_frame(&song.sound, song.totalframes - song.sample_rate);
                    logger("right arrow key pressed, but skipping 5 seconds forward went out of audio scope, resorted to playing the last second of audio.");
                }
            rest(230);
        }

        else if (keys[VK_LEFT].current) {
            ma_uint64 current_frame;
            ma_sound_get_cursor_in_pcm_frames(&song.sound, &current_frame);
            if (current_frame > song.skipframes) {
                    ma_sound_seek_to_pcm_frame(&song.sound, (current_frame - song.skipframes));
                    logger("left arrow key pressed, seeked 5 seconds backward into audio.");
                }
                else {
                    ma_sound_seek_to_pcm_frame(&song.sound, 0);
                    logger("left arrow key pressed, but skipping 5 seconds backward went out of audio scope, resorted to playing from audio start.");
                }
            rest(230);
            }

        rest(20);
    }
}

int main() {
    Music song; 
    
    logs.open("session_logs.txt", std::ios::app);
    logger("log file started.");
    
    ma_engine engine; ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {logger("can't setup engine:" + std::to_string(status)); return -1;}
    logger("engine initialized.");
    logger("startup successful, running.");

    song.path = "C:/Users/grsma/mergere/audio/breathe_in_the_air.mp3";
    logger("sound/song/audio filepath set.");
    
    ma_result song_status = ma_sound_init_from_file(&engine, song.path.c_str(), 0, NULL, NULL, &song.sound);
    if (song_status != MA_SUCCESS) {logger("can't setup sound:" + std::to_string(song_status)); return -1;}
    logger("sound/song/audio successfully initialized.");
    
    ma_sound_get_data_format(&song.sound, &song.format, &song.channels, &song.sample_rate, NULL, 0);
    ma_sound_get_length_in_pcm_frames(&song.sound, &song.totalframes);
    song.skipframes = 5 * song.sample_rate;
    logger("sound/song/audio info scanned and set.");
    
    ma_sound_start(&song.sound);
    logger("sound/song/audio playing.");
    
    std::thread input_handler_thread(song_input_handler, std::ref(song));
    logger("input collection thread started.");
    while (song_process_running) {
        rest(1000);
    }
    
    input_handler_thread.join();
    logger("input collection thread successfully closed.");
    ma_sound_uninit(&song.sound); ma_engine_uninit(&engine);
    logger("cleanup successful, exit.");
    
    logs.close();
    return 0;
}