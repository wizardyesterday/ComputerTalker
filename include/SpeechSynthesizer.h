//**********************************************************************
// file name: SpeechSynthesizer.h
//**********************************************************************

#ifndef _SPEECHSYNTHESIZER_H_
#define _SPEECHSYNTHESIZER_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

class SpeechSynthesizer
{
   public:

   SpeechSynthesizer(bool& success);

   ~SpeechSynthesizer(void);

   void talk(uint8_t*& phonemBuffer,uint32_t phonemCount);

   private:

   //*****************************************
   // Utility functions.
   //*****************************************

   //*****************************************
   // Attributes.
   //*****************************************
   // This provides bounds checking support.
   uint32_t maximumPhonemValue;
};

#endif // _SPEECHSYNTHESIZER_H_
