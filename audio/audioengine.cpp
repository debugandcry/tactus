#include "audioengine.h"
#include "helpers.h"

int initialize_audio_engine(ma_engine &engine) {
    ma_result status = ma_engine_init(NULL, &engine);
    if (status != MA_SUCCESS) {logger("can't setup engine:" + std::to_string(status)); return -1;}
    logger("audio engine successfully initialized.");    
    return 0;
}

int initialize_song_single(Music &song, std::string &path, ma_engine &engine, bool autoplay) {   
    song.path = path;
    logger("sound/song/audio filepath set.");
    
    ma_result song_status = ma_sound_init_from_file(&engine, song.path.c_str(), 0, NULL, NULL, &song.sound);
    if (song_status != MA_SUCCESS) {logger("can't setup sound:" + std::to_string(song_status)); return -1;}
    logger("sound/song/audio successfully initialized.");
    
    ma_sound_get_data_format(&song.sound, &song.format, &song.channels, &song.sample_rate, NULL, 0);
    ma_sound_get_length_in_pcm_frames(&song.sound, &song.totalframes);
    song.skipframes = 10 * song.sample_rate;
    logger("sound/song/audio info scanned and set.");
    
    if (autoplay) {
        ma_sound_start(&song.sound);
        logger("sound/song/audio playing.");
    }
    
    return 0;
}

void deinitialize_song_single(Music &song) {
    stop_song(song);
    ma_sound_uninit(&song.sound);
    logger("sound/song/audio object successfully deinitialized.");
}

void deinitialize_audio_engine(ma_engine &engine) {
    ma_engine_uninit(&engine);
    logger("audio engine successfully deinitialized");
}

void stop_song(Music &song) {
    ma_sound_stop(&song.sound);
    logger("sound/song/audio stopped");
}

void play_pause_song(Music &song) {
    if (ma_sound_is_playing(&song.sound) == MA_TRUE) {
        ma_sound_stop(&song.sound);
        logger("successfully paused.");
    }
    else {
        ma_sound_start(&song.sound);
        logger("successfully playing.");
    }

    controls.play_pause = false;
}

void restart_song(Music &song) {
    ma_sound_seek_to_pcm_frame(&song.sound, 0);
    
    logger("successfully restarted.");
    controls.restart = false;
}

void seek_forward(Music &song) {
    ma_uint64 current_frame;
    ma_sound_get_cursor_in_pcm_frames(&song.sound, &current_frame);
    current_frame += song.skipframes; 
    
    if (current_frame <= song.totalframes) {
        ma_sound_seek_to_pcm_frame(&song.sound, current_frame);
        logger("seek forward key pressed, successfully seeked 10 seconds forward.");
    }
    
    else {
        if (song.totalframes > (song.skipframes / 2)) {
            ma_sound_seek_to_pcm_frame(&song.sound, song.totalframes - (song.skipframes / 2));
            logger("went out of scope, playing the last 5 seconds.");
        }
        
        else {
            ma_sound_seek_to_pcm_frame(&song.sound, song.totalframes - 1);
            logger("the idiot is playing a file less than 5 seconds, he deserves it, playing from last frame.");
        }
    }

    controls.seek_forward = false;
}

void seek_backward(Music &song) {
    ma_uint64 current_frame;
    ma_sound_get_cursor_in_pcm_frames(&song.sound, &current_frame);

    if (song.skipframes < current_frame) {
        ma_sound_seek_to_pcm_frame(&song.sound, current_frame - song.skipframes);
        logger("seek backward key pressed, successfully seeked 10 seconds backward.");
    }

    else {
        ma_sound_seek_to_pcm_frame(&song.sound, 0);
        logger("went out of scope, playing from frame 0.");
    }

    controls.seek_backward = false;
}

void skip_forward_single(Music &song) {
    if ((song_number + 1) < queue.size()) {
        ma_sound_stop(&song.sound);
        deinitialize_song_single(song);
        song_number++;
        initialize_song_single(song, queue[song_number], engine, true);
        logger("successfully stopped number " + std::to_string(song_number - 1) + " and started number " + std::to_string(song_number));
    }
    
    else if ((song_number + 1) >= queue.size()) {
        ma_sound_seek_to_pcm_frame(&song.sound, song.totalframes - 1);
        logger("went out of scope, playing song from last frame.");
    }

    controls.skip_forward = false;
}

void skip_backward_single(Music &song) {
    if (song_number > 0) {
        ma_sound_stop(&song.sound);
        deinitialize_song_single(song);
        song_number--;
        initialize_song_single(song, queue[song_number], engine, true);
        logger("successfully stopped number " + std::to_string(song_number + 1) + " and started number " + std::to_string(song_number));
    }
    
    else if (song_number <= 0) {
        ma_sound_seek_to_pcm_frame(&song.sound, 0);
        logger("went out of scope, playing song from frame 0.");
    }

    controls.skip_backward = false;
}