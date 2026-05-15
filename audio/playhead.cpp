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
bool program_running = true;

int main() {
    clearscreen();
    initialize_logger();
    initialize_audio_engine(engine);
    logger("startup successful, running.");
    std::string path = song_path_single();
    if (path == "") {logger("path is empty, user pressed cancel, or invalid file selected. exitting..."); return 0;}
    initialize_song_single(song, path, engine);
    std::thread input_thread(transport_handler, std::ref(song));
    input_thread.join();
    deinitialize_song_single(song);
    deinitialize_audio_engine(engine);
    deinitialize_logger();
    return 0;
}