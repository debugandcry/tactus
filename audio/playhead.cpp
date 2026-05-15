#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include "data.h"
#include "miniaudio.h"
#include "controls.h"
#include "helpers.h"
#include "audioengine.h"

Music song;

ma_engine engine;

std::vector<std::string> queue;

int song_number;

bool initialized_song = false;
bool program_running = true;

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
    std::thread input_thread(transport_handler, std::ref(song), std::ref(engine));
    while (program_running) {
        if (seek_song_forward) {
            seek_song_forward_single(song, engine);
        }

        else if (seek_song_backward) {
            seek_song_backward_single(song, engine);
        }

        else if (ma_sound_at_end(&song.sound) == MA_TRUE) {            
            if (initialized_song) { 
            deinitialize_song_single(song);
            song_number++; 
            initialized_song = false;
            }
            if (song_number >= queue.size()) {
                program_running = false;
                break;
            }
            if (!initialized_song) {
            initialize_song_single(song, queue[song_number], engine);
            initialized_song = true;
            }
        }
        rest(5);
    }
    input_thread.join();
    if (initialized_song) {deinitialize_song_single(song);}
    deinitialize_audio_engine(engine);
    deinitialize_logger();
    return 0;
}