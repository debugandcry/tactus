#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include "data.h"
#include "miniaudio.h"
#include "controls.h"
#include "helpers.h"
#include "audioengine.h"

Music song;
Song_Controls controls;

ma_engine engine;

std::vector<std::string> queue;

int song_number;

bool initialized_song = false;
bool program_running = true;
bool logging_enabled = true;

int main() {
    song_number = 0;
    clearscreen();
    initialize_logger();
    initialize_audio_engine(engine);
    logger("startup successful, running.");
    song_path_query(queue);
    if (queue.empty()) {logger("path is empty, user pressed cancel, or invalid file selected. exitting..."); return 0;}
    if (queue[song_number] == "") {logger("path is empty, user pressed cancel, or invalid file selected. exitting..."); return 0;}
    initialize_song_single(song, queue[song_number], engine);
    initialized_song = true;
    std::thread input_thread(transport_handler);
    while (program_running) {
        if (controls.play_pause) {
            play_pause_song(song);
        }

        else if (controls.restart) {
            restart_song(song);
        }

        else if (controls.stop) {
            stop_song(song);
        }

        else if (controls.seek_forward) {
            seek_forward(song);
            rest(197);
        }

        else if (controls.seek_backward) {
            seek_backward(song);
            rest(197);
        }

        else if (controls.skip_forward) {
            skip_forward_single(song);
        }

        else if (controls.skip_backward) {
            skip_backward_single(song);
        }

        else if (ma_sound_at_end(&song.sound) == MA_TRUE) {            
            skip_forward_single(song);
        }
        
        rest(3);
    }
    input_thread.join();
    if (initialized_song) {deinitialize_song_single(song);}
    deinitialize_audio_engine(engine);
    deinitialize_logger();
    return 0;
}