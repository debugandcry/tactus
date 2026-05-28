#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include "data.h"
#include "miniaudio.h"
#include "controls.h"
#include "helpers.h"
#include "audioengine.h"
#include "preloader.h"

Song_Controls controls;

ma_engine engine;

std::vector<std::string> queue;

int song_number;

bool initialized_song = false;
bool program_running = true;
bool logging_enabled = true;
bool preloader = true;

int main() {
    song_number = 0;
    clearscreen();
    initialize_logger();
    initialize_audio_engine(engine);
    logger("startup successful, running.");
    song_path_query(queue);
    if (queue.empty()) {logger("path is empty, user pressed cancel, or invalid file selected. exitting..."); return 0;}
    if (queue[song_number] == "") {logger("path is empty, user pressed cancel, or invalid file selected. exitting..."); return 0;}
    preload_slots(preloader);
    std::thread input_thread(transport_handler);
    while (program_running) {
        if (controls.play_pause) {
            play_pause_song(current->song);
        }

        else if (controls.restart) {
            restart_song(current->song);
        }

        else if (controls.stop) {
            stop_song(current->song);
        }

        else if (controls.seek_forward) {
            seek_forward(current->song);
            rest(197);
        }

        else if (controls.seek_backward) {
            seek_backward(current->song);
            rest(197);
        }

        else if (controls.skip_forward) {
            if (!preloader) {
                skip_forward_single(current->song);
            }
            else {
                rotate_slots();
                controls.skip_forward = false;
            }
            rest(197);
        }

        else if (controls.skip_backward) {
            if (!preloader) {
                skip_backward_single(current->song);
            }
            else {
                unrotate_slots();
                controls.skip_backward = false;
            }
            rest(197);
        }

        else if (ma_sound_at_end(&current->song.sound) == MA_TRUE) {
            if (!preloader) { 
                skip_forward_single(current->song);
            }
            else {
                rotate_slots();
            }
        }
        
        rest(3);
    
    }
    
    input_thread.join();
    reset_slots();
    deinitialize_audio_engine(engine);
    deinitialize_logger();
    return 0;
}