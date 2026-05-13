#define MA_ENABLE_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include "data.h"
#include "miniaudio.h"
#include "controls.h"
#include "helpers.h"

bool song_process_running = true;

int main() {
    Music song; 
    
    logs.open("session_logs.txt", std::ios::app);
    logger("log file started.");
    
    ma_engine engine; ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {logger("can't setup engine:" + std::to_string(status)); return -1;}
    logger("engine initialized.");
    logger("startup successful, running.");

    song.path = song_path_single();
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
    
    std::thread input_handler_thread(transport_handler, std::ref(song));
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