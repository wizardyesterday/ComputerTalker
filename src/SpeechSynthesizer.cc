//**********************************************************************
// file name: SpeechSynthesizer.cc
//**********************************************************************

#include <stdio.h>

#include "SpeechSynthesizer.h"

struct pcmEntry
{
   char *fileNamePtr;
   int16_t buffer[40000];
   int32_t bufferLength;
};

static pcmEntry pcmBuffers[] =
{
   {"A.raw",{0,0},0}
};

/************************************************************************

  Name: SpeechSynthesizer

  Purpose: The purpose of this function is to serve as the constructor
  of an SpeechSynthesizer object.

  Calling Sequence: SpeechSynthesizer(bool& success)

  Inputs:

    success - A reference to storage for the outcome of processing.
    A value of true insicates sucessful processing, and a value of
    false indicates unsuccessful processing.

  Outputs:

    None.

**************************************************************************/
SpeechSynthesizer::SpeechSynthesizer(bool& success)
{
  int i;

  return;
 
} // SpeechSynthesizer

/**************************************************************************

  Name: ~SpeechSynthesizer

  Purpose: The purpose of this function is to serve as the destructor
  of a SpeechSynthesizer object.

  Calling Sequence: ~SpeechSynthesizer()

  Inputs:

    None.

  Outputs:

    None.

**************************************************************************/
SpeechSynthesizer::~SpeechSynthesizer(void)
{

  return;

} // ~SpeechSynthesizer

