#include "data.h"
#include "helpers.h"
#include "controls.h"

/*for the function `song_path_single`, you need the dialog right? of course you do.
so the header file isnt enough, so on compilation, version 0.0.6 and onwards...
...you manually need to add in an "-lcomdlg32" flag.*/

int play_pause_key = VK_SPACE; 
int exit_key = VK_ESCAPE;
int restart_key = 'R';
int seek_forward_key = VK_RIGHT;
int seek_backward_key = VK_LEFT;
int skip_forward_key = VK_UP;
int skip_backward_key = VK_DOWN;

void song_path_query(std::vector<std::string> &queue) {
    
    wchar_t filepath[40960] = L"";
    
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = filepath;
    dialog.nMaxFile = 40960;
    dialog.lpstrFilter = L"Audio Files\0*.mp3;*.wav;*.flac\0";
    dialog.Flags =  OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER ;
    if (GetOpenFileNameW(&dialog)) {logger("user filepath(s) successfully received.");}
    else {logger("user filepath(s) was/were not receivable, user pressed 'cancel' or selected invalid file."); return;}
    
    wchar_t* crawler = &filepath[0];
    std::wstring dir = crawler;
    crawler += dir.length() + 1;
    
    if (*crawler == L'\0') {
        int projected_size = WideCharToMultiByte(CP_UTF8, 0, dir.c_str(), -1, NULL, 0, NULL, NULL);
        if (projected_size == 0) {return;}
        std::string proper_filepath(projected_size, 0);
        WideCharToMultiByte(CP_UTF8, 0, dir.c_str(), -1, &proper_filepath[0], projected_size, NULL, NULL);
        if (proper_filepath.empty()) {return;}
        proper_filepath.pop_back();
        queue.push_back(proper_filepath);
        logger("successfully collected filepath " + proper_filepath);
        return;
    }
    
    else {
        
        while (*crawler != L'\0') {
            std::wstring current_filename = crawler;
            std::wstring current_filepath = dir + L"\\" + current_filename; 
            int projected_size = WideCharToMultiByte(CP_UTF8, 0, current_filepath.c_str(), -1, NULL, 0, NULL, NULL);
            if (projected_size == 0) {return;}
            std::string proper_filepath(projected_size, 0);
            WideCharToMultiByte(CP_UTF8, 0, current_filepath.c_str(), -1, &proper_filepath[0], projected_size, NULL, NULL);
            if (proper_filepath.empty()) {return;}
            proper_filepath.pop_back();
            queue.push_back(proper_filepath);
            logger("successfully collected filepath " + proper_filepath);
            crawler += current_filename.length() + 1;
        }
    return;
    }
}

void transport_handler() {
        
    HANDLE inputs = GetStdHandle(STD_INPUT_HANDLE);
    DWORD console_mode;
    GetConsoleMode(inputs, &console_mode);
    console_mode &= ~ENABLE_LINE_INPUT;
    console_mode &= ~ENABLE_ECHO_INPUT;
    SetConsoleMode(inputs, console_mode);   
    INPUT_RECORD event;
    DWORD events_read;
        
    while (program_running) {
        ReadConsoleInputW(inputs, &event, 1, &events_read);
        if (event.EventType == KEY_EVENT) {
            KEY_EVENT_RECORD key = event.Event.KeyEvent;
                
            if (key.bKeyDown) {
                    
                if (key.wVirtualKeyCode == play_pause_key && key.wRepeatCount == 1) {
                    controls.play_pause = true;
                    logger("play/pause pressed, requesting play/pause.");
                }
                    
                else if (key.wVirtualKeyCode == exit_key && key.wRepeatCount == 1) {
                    controls.stop = true;
                    program_running = false;
                    logger("exit pressed, requesting exit.");
                }
                    
                else if  (key.wVirtualKeyCode == restart_key && key.wRepeatCount == 1) {
                    controls.restart = true;
                    logger("restart key pressed, requesting current song restart.");
                }
                    
                else if (key.wVirtualKeyCode == seek_forward_key && key.wRepeatCount == 1) {
                    controls.seek_forward = true;
                }
                    
                else if (key.wVirtualKeyCode == seek_backward_key && key.wRepeatCount == 1) {
                    controls.seek_backward = true;
                }
                    
                else if (key.wVirtualKeyCode == skip_forward_key && key.wRepeatCount == 1) {
                    controls.skip_forward = true;
                }
                    
                else if (key.wVirtualKeyCode == skip_backward_key && key.wRepeatCount == 1) {
                    controls.skip_backward = true;
                }
            }
        }
    }
}