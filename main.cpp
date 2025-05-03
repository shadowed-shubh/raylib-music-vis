#include "include/raylib.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define ARR_len(xs) sizeof(xs) / sizeof(xs[0])

uint32_t glo_frames[1024];
size_t glo_frames_c = 0;

void callback(void *bufferdata, unsigned int frames) {
  if (frames > ARR_len(glo_frames)) {
    frames = ARR_len(glo_frames);
  }
  memcpy(glo_frames, bufferdata, frames * sizeof(float));
  glo_frames_c = frames;
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

    BeginDrawing();
    ClearBackground(BLACK);
    // float cell_w = (float)GetRenderWidth() / glo_frames_c;
    for (size_t i = 0; i < glo_frames_c; i++) {
      float samps = glo_frames[i];
      printf("%f", samps);
    }

    printf(" ");
    EndDrawing();
  }
  return 0;
}
