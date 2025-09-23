#ifndef AUDIO_H
#define AUDIO_H

#include <portaudio.h>
#include <string>

class AudioPlayer
{
  public:
  AudioPlayer();
  ~AudioPlayer();

  bool init();
  bool load(const std::string& filename);
  void play();
  void stop();
  void close();

  private:
  PaStream* stream;
  float* buffer;
  size_t bufferSize;
  size_t currentIndex;

  static int paCallback(const void* input,
                        void* output,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData);
};

#endif
