#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>

std::ofstream logs;

bool song_process_running = true;

struct Music {
    ma_sound sound;
    ma_format format;
    ma_uint32 sample_rate;
    ma_uint32 channels;
    ma_uint64 totalframes;
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
    ma_uint64 skip_frames = 5 * song.sample_rate;
    logger("sound/song/audio info scanned and set.");
    

    ma_sound_start(&song.sound);
    logger("sound/song/audio playing.");
    
    while (song_process_running) {
        
        int userinput = _getch();
        if (userinput == ' ') {if (ma_sound_is_playing(&song.sound) == MA_TRUE) {ma_sound_stop(&song.sound); logger("spacebar pressed, audio playback paused.");} else {ma_sound_start(&song.sound); logger("spacebar pressed, audio playing");}}
        
        else if (userinput == 'r' || userinput == 'p') {
            ma_sound_stop(&song.sound);
            ma_sound_seek_to_pcm_frame(&song.sound, 0); 
            ma_sound_start(&song.sound); 
            if (userinput == 'r') {
                logger("'r' key pressed, audio restarted from 00:00:00.");
            } 
            else {
                logger("'p' key pressed, audio restarted from 00:00:00, or playing previous song.");
            }
        }
        
        else if (userinput == 27) {
            ma_sound_stop(&song.sound); 
            song_process_running = false; 
            logger("'esc' key pressed, proceeding to exit.");}

        else if (userinput == 224 || userinput == 0) {
            
            int secondinput = _getch();
            ma_uint64 current_frame;
            ma_sound_get_cursor_in_pcm_frames(&song.sound, &current_frame);
            
            if (secondinput == 77) {
                if ((current_frame + skip_frames) < song.totalframes) {
                    ma_sound_seek_to_pcm_frame(&song.sound, (current_frame + skip_frames));
                    logger("right arrow key pressed, seeked 5 seconds forward into audio.");
                }
                else {
                    ma_sound_seek_to_pcm_frame(&song.sound, song.totalframes - song.sample_rate);
                    logger("right arrow key pressed, but skipping 5 seconds forward went out of audio scope, resorted to playing the last second of audio.");
                }
            }
            
            else if (secondinput == 75) {
                if (current_frame > skip_frames) {
                    ma_sound_seek_to_pcm_frame(&song.sound, (current_frame - skip_frames));
                    logger("left arrow key pressed, seeked 5 seconds backward into audio.");
                }
                else {
                    ma_sound_seek_to_pcm_frame(&song.sound, 0);
                    logger("left arrow key pressed, but skipping 5 seconds backward went out of audio scope, resorted to playing from the start.");
                }
            }

            else if (secondinput == 80 || secondinput == 72) {
                
                if (secondinput == 72) {
                    //add later - next song
                    logger("up arrow key pressed, skipped to next audio file in queue.");
                }
                
                else {
                    //add later - previous song or skip_frames
                    logger("down arrow key pressed, audio restarted from 00:00:00, or playing previous song.");
                }
            }
        }
    }
    
    ma_sound_uninit(&song.sound); ma_engine_uninit(&engine);
    logger("cleanup successful, exit.");
    
    logs.close();
    return 0;
}