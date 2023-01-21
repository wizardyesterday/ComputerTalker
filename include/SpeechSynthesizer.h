//**********************************************************************
// file name: SpeechSynthesizer.h
//**********************************************************************

#ifndef _SPEECHSYNTHESIZER_H_
#define _SPEECHSYNTHESIZER_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "DbfsCalculator.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// New AGC algorithms will be added as time progresses.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#define AGC_TYPE_LOWPASS (0)
#define AGC_TYPE_HARRIS (1)
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class SpeechSynthesizer
{
  public:

  struct pcmEntry
  {
    int16_t buffer[40000];
    int8_t bufferLength;
  };


  SpeechSynthesizer(bool &success);

  ~SpeechSynthesizer(void);

  void talk(uint8_t*& phonemBuffer,uint32_t phonemCount);

  private:

  //*****************************************
  // Utility functions.
  //*****************************************

  //*****************************************
  // Attributes.
  //*****************************************
  // PCM buffers.
  pcmEntry pcmBuffers[64];

};

#endif // _SPEECHSYNTHESIZER_H_
