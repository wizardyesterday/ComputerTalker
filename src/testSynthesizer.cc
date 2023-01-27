//**********************************************************************
// file name: talker.cc
//**********************************************************************

#include <stdio.h>
#include <string>
#include <stdint.h>

#include "SpeechSynthesizer.h"

//*************************************************************************
// This program provides the mainline code for testing of the speech
// synthesizer.
// Here's how this program works.  The user enters one phonem number
// followed by a return.  That number will be stored in the phonem buffer,
// and the storage location will be incremented.  The user can keep doing
// this repeatedly, and the numbers will be buffered up.  When the user
// presses the return key without entering any number, the buffered data
// will be sent to the speech synthesizer with the result that the PCM
// sample representation of each phonem (associated with each number)
// be output.  The user exits the program by typing control-D or
// control-C.
// Since the program accepts input from stdin, input can be provided by
// a text file with a number per line, and functionality will be similar.
// Note that the PCM samples are 16-bit, little endian, with a sample rate
// of 16000S/s.
//*************************************************************************

//***********************************************************************
// Mainline code.
//***********************************************************************
int main(int argc, char **argv)
{
   bool synthesizerInitialized; 
   bool done;
   uint32_t number;
   char *statusPtr;
   char buffer[128];
   uint8_t *phonemCodePtr;
   uint8_t phonemBuffer[1000];
   uint32_t phonemCount;
   SpeechSynthesizer *synthesizerPtr;

   // We need a pointer to satisfy the interface.
   phonemCodePtr = phonemBuffer;

   // We don't like dangling pointers.
   synthesizerPtr = (SpeechSynthesizer *)NULL;

   // Start out with an empty phonem buffer.
   phonemCount = 0;

   // Instiatate the synthesizer.
   synthesizerPtr = new SpeechSynthesizer(synthesizerInitialized);

   if (synthesizerInitialized)
   {
      // Set up for loop entry.
      done = false;

      while (!done)
      {
         // Read the next line of text.
         statusPtr = fgets(buffer,100,stdin);

         if (statusPtr != NULL)
         {
            // Grab the number.
            sscanf(buffer,"%u",&number);

            if ((number <= 63) && (strlen(buffer) > 1))
            {
               // Store the phonem code.
               phonemBuffer[phonemCount] = (uint8_t)number;

               // We have one more entry.
               phonemCount++;
            } // if

            // Pressing the return key results in a dump of PCM samples.
            if (strlen(buffer) == 1)
            {
               if (phonemCount > 0)
               {
                  // Send the phonems to the speech synthesizer.
                  synthesizerPtr->talk(phonemCodePtr,phonemCount);

                  // Reference the beginning of the buffer.
                  phonemCount= 0;
               } // if
            } // if

         } // if
         else
         {
            // Bail out.
            done = true;
         } // else
      } // while

   } // if
   else
   {
      fprintf(stderr,"Error: Failed to initialize the speech synthesizer\n");
   } // else

   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
   // Release resources.
   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
   if (synthesizerPtr != NULL)
   {
     delete synthesizerPtr;
   } // if
   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

   return (0);

} // main

