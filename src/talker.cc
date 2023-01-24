//**********************************************************************
// file name: talker.cc
//**********************************************************************

#include <stdio.h>
#include <string>
#include <stdint.h>

#include "PhonemMaker.h"
#include "SpeechSynthesizer.h"

//*************************************************************************
// This program provides the mainline code for a text-to-speech
// synthesizer.  It accepts input from stdin so that a user can type a
// line of text followed by a carriage return, or input can be taken from
// a text file by using I/O redirection.  In either case, when a newline
// occurrs, the text will be presented to the speech synthesis system.
// I ported the original program, written in Pascal/MT+, to C++.  It will
// be evident that I used an ancient compiler due to the short variable
// names and short procedure and function names.  I also used upper case
// text (we did that back then in the CP/M days).
// Note that the PCM samples are 16-bit, little endian, with a sample rate
// of 16000S/s.
//*************************************************************************

//***********************************************************************
// Mainline code.
//***********************************************************************
int main(int argc, char **argv)
{
   bool makerInitialized;
   bool synthesizerInitialized; 
   bool done;
   char *statusPtr;
   char englishBuffer[1000];
   std::string englishText;
   uint32_t phonemBufferLength;
   uint8_t *phonemCodePtr;
   PhonemMaker *makerPtr;
   SpeechSynthesizer *synthesizerPtr;

   // Instantiate the text to phonem convertor.
   makerPtr = new PhonemMaker(makerInitialized);

   if (makerInitialized)
   {
       // Instiatate the synthesizer.
      synthesizerPtr = new SpeechSynthesizer(synthesizerInitialized);

      if (synthesizerInitialized)
      {
         // Set up for loop entry.
         done = false;

         while (!done)
         {
            // Read the next line of text.
            statusPtr = fgets(englishBuffer,100,stdin);

            if (statusPtr != NULL)
            {
               // Nuke the newline!
               englishBuffer[strlen(englishBuffer)-1] = '\0';

               // Copy this in order to satify the interface.
               englishText = englishBuffer;

               // Generate the phonems.
               makerPtr->acceptEnglishText(englishText,
                                           phonemCodePtr,
                                           phonemBufferLength);

               // Send the phonems to the speech synthesizer.
               synthesizerPtr->talk(phonemCodePtr,phonemBufferLength);
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
   } // if
   else
   {
      fprintf(stderr,"Error:A Failed to load configuration files\n");
   } // else

   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
   // Release resources.
   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
   if (makerPtr != NULL)
   {
      delete makerPtr;
   } // if

   if (synthesizerPtr != NULL)
   {
     delete synthesizerPtr;
   } // if
   //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

   return (0);

} // main

