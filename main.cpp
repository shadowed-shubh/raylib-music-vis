#include "include/raylib.h"
#include <iostream>
int main() {

  InitWindow(800, 600, "music");
  SetTargetFPS(60);

  InitAudioDevice();
  Music music = LoadMusicStream("song.mp3");
  PlayMusicStream(music);
  SetMusicVolume(music, 0.5f);

  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music)) {
        std::cout << "music stopped";
        PauseMusicStream(music);
      }
    } else {
      ResumeMusicStream(music);
    }
    BeginDrawing();
    ClearBackground(RED);
    EndDrawing();
  }
  return 0;
}
