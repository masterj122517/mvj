#include "audio.h"
#include <iostream>
#include <sndfile.h> // 用于读取音频文件 (libsndfile)

AudioPlayer::AudioPlayer()
    : stream(nullptr), buffer(nullptr), bufferSize(0), currentIndex(0) {}

AudioPlayer::~AudioPlayer()
{
  close();
}

bool AudioPlayer::init()
{
  if (Pa_Initialize() != paNoError) {
    std::cerr << "Failed to initialize PortAudio" << std::endl;
    return false;
  }
  return true;
}

bool AudioPlayer::load(const std::string& filename)
{
  SF_INFO sfInfo;
  SNDFILE* sndFile = sf_open(filename.c_str(), SFM_READ, &sfInfo);
  if (!sndFile) {
    std::cerr << "Failed to open audio file: " << filename << std::endl;
    return false;
  }

  bufferSize = sfInfo.frames * sfInfo.channels;
  buffer = new float[bufferSize];
  sf_readf_float(sndFile, buffer, sfInfo.frames);
  sf_close(sndFile);

  currentIndex = 0;

  if (Pa_OpenDefaultStream(&stream,
                           0, // no input
                           sfInfo.channels,
                           paFloat32,
                           sfInfo.samplerate,
                           paFramesPerBufferUnspecified,
                           paCallback,
                           this) != paNoError) {
    std::cerr << "Failed to open PortAudio stream" << std::endl;
    return false;
  }

  return true;
}

void AudioPlayer::play()
{
  if (stream)
    Pa_StartStream(stream);
}

void AudioPlayer::stop()
{
  if (stream)
    Pa_StopStream(stream);
}

void AudioPlayer::close()
{
  if (stream) {
    Pa_CloseStream(stream);
    stream = nullptr;
  }
  if (buffer) {
    delete[] buffer;
    buffer = nullptr;
  }
  Pa_Terminate();
}

int AudioPlayer::paCallback(const void* input,
                            void* output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData)
{
  AudioPlayer* player = static_cast<AudioPlayer*>(userData);
  float* out = static_cast<float*>(output);

  size_t remaining = player->bufferSize - player->currentIndex;
  size_t toCopy = (frameCount * 2 < remaining) ? frameCount * 2 : remaining;

  for (size_t i = 0; i < toCopy; i++) {
    out[i] = player->buffer[player->currentIndex++];
  }

  // 如果到结尾了，填 0
  for (size_t i = toCopy; i < frameCount * 2; i++) {
    out[i] = 0.0f;
  }

  return (player->currentIndex < player->bufferSize) ? paContinue : paComplete;
}
