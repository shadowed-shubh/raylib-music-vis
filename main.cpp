#include "include/raylib.h"
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
using namespace std;

float pi = atan2f(1, 1) * 4;
float max_amp;
#define ARR_len(xs) sizeof(xs) / sizeof(xs[0])

#define N 256

float in[N];
complex<float> out[N];

struct Frames {
  float left;
  float right;
};

float amp(complex<float> z) { return abs(z); }

void fft(float in[], size_t stride, complex<float> out[], size_t n) {
  if (n == 1) {
    out[0] = in[0];
    return;
  }
  fft(in, stride * 2, out, n / 2);
  fft(in + stride, stride * 2, out + n / 2, n / 2);

  for (size_t k = 0; k < n / 2; ++k) {
    float t = (float)k / n;
    complex<float> w = polar(1.0f, -2.0f * pi * t);
    complex<float> v = w * out[k + n / 2];
    complex<float> e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

void callback(void *bufferdata, unsigned int frames) {
  if (frames < N)
    return;

  Frames *fs = (Frames *)bufferdata;

  for (size_t i = 0; i < N; ++i) {
    in[i] = fs[i].left;
  }

  fft(in, 1, out, N);

  max_amp = 0.0f;

  for (size_t i = 0; i < N; ++i) {
    float a = amp(out[i]);
    if (max_amp < a)
      max_amp = a;
  }
}

int main() {
  InitWindow(800, 600, "music");
  SetTargetFPS(60);

  SetTraceLogLevel(LOG_ALL);

  InitAudioDevice();
  Music music = LoadMusicStream("song.mp3");
  AttachAudioStreamProcessor(music.stream, callback);

  PlayMusicStream(music);

  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    int w = GetRenderWidth();
    int h = GetRenderHeight();

    BeginDrawing();

    DrawText("Playing music...", 10, 10, 20, GREEN);

    ClearBackground(BLACK);
    float cell_w = (float)w / N;

    for (size_t i = 0; i < N; ++i) {
      float t = amp(out[i]);
      DrawRectangle(i * cell_w, h / 2 - h / 2 * t, cell_w, h / 2 * t, RED);
    }
    // for (size_t i = 0; i < glo_frames_c; i++) {
    //   float samps = glo_frames[i].left;
    //   if (samps > 0) {
    //     DrawRectangle(i * cell_w, h / 2 - h / 2 * samps, 1, h / 2 * samps,
    //     RED);
    //   } else {
    //     DrawRectangle(i * cell_w, h / 2, 1, h / 2 * samps, RED);
    //   }
    // }
    EndDrawing();
  }
  return 0;
}
