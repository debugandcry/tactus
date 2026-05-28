#include "data.h"
#include "preloader.h"
#include "audioengine.h"

Slot alpha;
Slot beta;
Slot charlie;
Slot* current = nullptr;
Slot* next = nullptr;
Slot* previous = nullptr;
Slot* old = nullptr;
int target_index = 0;

void preload_slots(bool preload) {
    current = &alpha;
    next = &beta;
    previous = &charlie;
    previous->loaded = false;
    previous->queue_index = -1;
    initialize_song_single(current->song, queue[0], engine, true);
    current->loaded = true;
    current->queue_index = 0;
    if (preload) {
    target_index = current->queue_index + 1;
    if (target_index < queue.size()) {
        initialize_song_single(next->song, queue[target_index], engine, false);
        next->loaded = true;
        next->queue_index = target_index;
        }
    }
    logger("preloaded slots");
    return;
}

void rotate_slots() {
    if (!next->loaded) {return;}
    stop_song(current->song);
    old = previous;
    previous = current;
    current = next;
    next = old;
    ma_sound_seek_to_pcm_frame(&current->song.sound, 0);
    if (ma_sound_is_playing(&current->song.sound) == MA_FALSE) {ma_sound_start(&current->song.sound);}
    if (next->loaded) {deinitialize_song_single(next->song);}
    next->loaded = false;
    next->queue_index = -1;
    target_index = current->queue_index + 1;
    if (target_index < queue.size()) {
        initialize_song_single(next->song, queue[target_index], engine, false);
        next->loaded = true;
        next->queue_index = target_index;
    }
    logger("rotated slots forward");
    return;
}

void unrotate_slots() {
    if (!previous->loaded) {return;}
    stop_song(current->song);
    old = next;
    next = current;
    current = previous;
    previous = old;
    ma_sound_seek_to_pcm_frame(&current->song.sound, 0);
    if (ma_sound_is_playing(&current->song.sound) == MA_FALSE) {ma_sound_start(&current->song.sound);}
    if (previous->loaded) {deinitialize_song_single(previous->song);}
    previous->loaded = false;
    previous->queue_index = -1;
    target_index = current->queue_index - 1;
    if (target_index >= 0) {
        initialize_song_single(previous->song, queue[target_index], engine, false);
        previous->loaded = true;
        previous->queue_index = target_index;
    }
    logger("rotated slots backward");
    return;
}

void reset_slots() {
    if (previous->loaded) {deinitialize_song_single(previous->song);}
    if (current->loaded) {deinitialize_song_single(current->song);}
    if (next->loaded) {deinitialize_song_single(next->song);}
    logger("reset all slots.");
    return;
}