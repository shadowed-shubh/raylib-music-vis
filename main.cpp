#include "include/raylib.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define ARR_len(xs) sizeof(xs) / sizeof(xs[0])

typedef struct {
  float left;
  float right;
} Frames;

Frames glo_frames[4800 * 2] = {0};
size_t glo_frames_c = 0;

size_t cap = ARR_len(glo_frames);

void callback(void *bufferdata, unsigned int frames) {
  if (frames <= cap - glo_frames_c) {
    memcpy(glo_frames + glo_frames_c, bufferdata, sizeof(Frames) * frames);
    glo_frames_c += frames;
  } else if (frames <= cap) {
    memmove(glo_frames, glo_frames + frames, sizeof(Frames) * (cap - frames));
    memcpy(glo_frames + (cap - frames), bufferdata, sizeof(Frames) * frames);
  } else {
    memcpy(glo_frames, bufferdata, sizeof(Frames) * cap);
    glo_frames_c = cap;
  }
}
int main() {
  InitWindow(800, 600, "music");
  SetTargetFPS(60);

  InitAudioDevice();
  Music music = LoadMusicStream("song.mp3");
  AttachAudioStreamProcessor(music.stream, callback);

  PlayMusicStream(music);

  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    int w = GetRenderWidth();
    int h = GetRenderHeight();

    BeginDrawing();
    ClearBackground(BLACK);
    float cell_w = (float)w / glo_frames_c;
    for (size_t i = 0; i < glo_frames_c; i++) {
      float samps = glo_frames[i].left;
      if (samps > 0) {
        DrawRectangle(i * cell_w, h / 2 - h / 2 * samps, 1, h / 2 * samps, RED);
      } else {
        DrawRectangle(i * cell_w, h / 2, 1, h / 2 * samps, RED);
      }
    }

    EndDrawing();
  }
  return 0;
}
