#include "include/raylib.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

uint32_t glo_frames[1024];
size_t glo_frames_c = 0;

void callback(void *bufferData, unsigned int frames) {
  memcpy(glo_frames, bufferData, frames * sizeof(uint32_t) * frames * 2);
  glo_frames_c = frames;
}

int main() {
  InitWindow(800, 600, "music");
  SetTargetFPS(60);

  InitAudioDevice();
  Music music = LoadMusicStream("song.mp3");
  PlayMusicStream(music);
  SetMusicVolume(music, 0.5f);
  AttachAudioStreamProcessor(music.stream, callback);

  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music)) {
        PauseMusicStream(music);
      }
    } else {
      ResumeMusicStream(music);
    }
    BeginDrawing();
    ClearBackground(BLACK);
    // float cell_w = (float)GetRenderWidth() / glo_frames_c;
    for (size_t i = 0; i < glo_frames_c; ++i) {
      int16_t sample = *(int16_t *)&glo_frames[i];
      printf("%d\n", sample);
    }
    EndDrawing();
  }
  return 0;
}
