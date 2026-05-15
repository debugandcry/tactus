#include "audioengine.h"

int initialize_audio_engine(ma_engine &engine) {
    ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {logger("can't setup engine:" + std::to_string(status)); return -1;}
    logger("audio engine successfully initialized.");    
    return 0;
}

int initialize_song_single(Music &song, std::string &path, ma_engine &engine) {   
    song.path = path;
    logger("sound/song/audio filepath set.");
    
    ma_result song_status = ma_sound_init_from_file(&engine, song.path.c_str(), 0, NULL, NULL, &song.sound);
    if (song_status != MA_SUCCESS) {logger("can't setup sound:" + std::to_string(song_status)); return -1;}
    logger("sound/song/audio successfully initialized.");
    
    ma_sound_get_data_format(&song.sound, &song.format, &song.channels, &song.sample_rate, NULL, 0);
    ma_sound_get_length_in_pcm_frames(&song.sound, &song.totalframes);
    song.skipframes = 10 * song.sample_rate;
    logger("sound/song/audio info scanned and set.");
    
    ma_sound_start(&song.sound);
    logger("sound/song/audio playing.");

    return 0;
}

void deinitialize_song_single(Music &song) {
    if (ma_sound_is_playing(&song.sound) == MA_FALSE) {ma_sound_stop(&song.sound); logger("sound/song/audio stopped");}
    ma_sound_uninit(&song.sound);
    logger("sound/song/audio object successfully deinitialized.");
}

void deinitialize_audio_engine(ma_engine &engine) {
    ma_engine_uninit(&engine);
    logger("audio engine successfully deinitialized");
}

void seek_song_forward_single(Music &song, ma_engine &engine) {
    if ((song_number + 1) < queue.size()) {
        ma_sound_stop(&song.sound);
        deinitialize_song_single(song);
        song_number++;
        initialize_song_single(song, queue[song_number], engine);
        seek_song_forward = false;
        seek_song_forward_success = true;
    }
    else if (song_number >= queue.size()) {
        ma_sound_seek_to_pcm_frame(&song.sound, 0);
        seek_song_forward = false;
        seek_song_forward_success = false;
    }
}

void seek_song_backward_single(Music &song, ma_engine &engine) {
    if (song_number > 0) {
        ma_sound_stop(&song.sound);
        deinitialize_song_single(song);
        song_number--;
        initialize_song_single(song, queue[song_number], engine);
        seek_song_backward_success = true;
        seek_song_backward = false;
    }
    else if (song_number <= 0) {
        ma_sound_seek_to_pcm_frame(&song.sound, 0);
        seek_song_backward_success = false;
        seek_song_backward = false;
    }
}