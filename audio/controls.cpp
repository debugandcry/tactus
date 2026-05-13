#include "data.h"
#include "helpers.h"
#include "controls.h"

/*for the function song_path_single, you need the dialog right?
so the header file isnt enough, so on compilation, version 0.0.6 (this one) and onwards...
you manually need to add in a "-lcomdlg32" flag.*/

int pause_play_key = VK_SPACE;
int exit_key = VK_ESCAPE;
int restart_key = 'R';
int seek_forward_key = VK_RIGHT;
int seek_backward_key = VK_LEFT;

KeyStates keys[256];

std::string song_path_single() {
    wchar_t filepath[40960] = L"";
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = filepath;
    dialog.nMaxFile = 40960;
    dialog.lpstrFilter = L"Audio Files\0*.mp3;*.wav;*.flac\0";
    dialog.Flags =  OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameW(&dialog)) {
        logger("custom filepath successfully received.");
    }
    else {
        logger("custom filepath was not receivable, or the user pressed 'cancel' or dialog.");
        return "";
    }
    int projected_size = WideCharToMultiByte(CP_UTF8, 0, filepath, -1, NULL, 0, NULL, NULL);
    if (projected_size == 0) {return "";}
    std::string proper_filepath(projected_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, filepath, -1, &proper_filepath[0], projected_size, NULL, NULL);
    if (proper_filepath.empty()) {return ""; }
    proper_filepath.pop_back();
    return proper_filepath;
}

bool pressed_query(int keyval) {
    return keys[keyval].current && !keys[keyval].previous;
}

void update_query(int key) {
    keys[key].previous = keys[key].current;
    keys[key].current = GetAsyncKeyState(key) & 0x8000;
} 

void transport_handler(Music &song) {
    while (song_process_running) {
        
        update_query(pause_play_key);
        update_query(exit_key);
        update_query(restart_key);
        update_query(seek_forward_key);
        update_query(seek_backward_key);
        
        if (pressed_query(exit_key)) {
            ma_sound_stop(&song.sound);
            song_process_running = false;
            logger("'esc' key pressed, proceeding to exit.");
        }
        
        else if (pressed_query(pause_play_key)) {
            if (ma_sound_is_playing(&song.sound) == MA_TRUE){
                ma_sound_stop(&song.sound);
                logger("spacebar pressed, audio playback paused.");    
            }
            else {
                ma_sound_start(&song.sound);
                logger("spacebar pressed, audio playback resumed.");
            }
        }

        else if (pressed_query(restart_key)) {
            ma_sound_stop(&song.sound);
            ma_sound_seek_to_pcm_frame(&song.sound, 0); 
            ma_sound_start(&song.sound);
            logger("'r' key pressed, audio restarted from 00:00:00, or pcm frame 0.");
        }
    
        else if (keys[seek_forward_key].current) {
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

        else if (keys[seek_backward_key].current) {
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