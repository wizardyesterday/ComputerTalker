//**********************************************************************
// file name: SpeechSynthesizer.cc
//**********************************************************************
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// This class attempts to emulate a Votrax SC-01 speech
// synthesizer.  To achieve this, when presented with a stream of
// phoneme codes, each phoneme code is mapped to a PCM buffer.  The
// contents of this buffer is written to stdout where the output
// can be piped to your favorite audio player.  The format of the
// audio files are 16-bit signed little endian quantities.  The
// sample rate is 16000S/s.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#include <stdio.h>

#include "SpeechSynthesizer.h"

// Phoneme code context.
struct pcmEntry
{
   char *fileNamePtr;
   int16_t data[8192];
   uint32_t numberOfSamples;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// Each entry of this array represents the context of each phoneme
// code.  The comments on the far right indicate the Votrax SC-01
// phoneme code an example word for the phoneme exists (the
// underlined portion), and the duration of the phonem in ms.
// These comments were derived from the Votrax SC-01 data sheet.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
static pcmEntry pcmBuffers[] =
{
   {"rawSamples/EH3.raw",{0},0},  // 00, jack_E_t, Duration: 59ms
   {"rawSamples/EH2.raw",{0},0},  // 01, E_nlist, Duration: 71ms
   {"rawSamples/EH1.raw",{0},0},  // 02, h_EA_vy, Duration: 121ms
   {"rawSamples/PA0.raw",{0},0},  // 03, PA0: silence, Duration: 47ms
   {"rawSamples/DT.raw",{0},0},   // 04, bu_TT_er, Duration: 47ms
   {"rawSamples/A2.raw",{0},0},   // 05, m_A_de, Duration: 71ms
   {"rawSamples/A1.raw",{0},0},   // 06, m_A_de, Duration: 103ms
   {"rawSamples/ZH.raw",{0},0},   // 07, a_Z_ure, Duration: 90ms
   {"rawSamples/AH2.raw",{0},0},  // 08, h_O_nest, Duration: 71ms
   {"rawSamples/I3.raw",{0},0},   // 09, inhib_I_t, Duration: 55ms
   {"rawSamples/I2.raw",{0},0},   // 0A, I_nhibit, Duration: 80ms
   {"rawSamples/I1.raw",{0},0},   // 0B, inh_I_bit, Duration: 121ms
   {"rawSamples/M.raw",{0},0},    // 0C, M_at, Duration: 103ms
   {"rawSamples/N.raw",{0},0},    // 0D, su_N, Duration: 80ms
   {"rawSamples/B.raw",{0},0},    // 0E, B_ag, Duration: 71ms
   {"rawSamples/V.raw",{0},0},    // 0F, V_an, Duration: 71ms
   {"rawSamples/CH.raw",{0},0},   // 10, CH_ip, Duration: 71ms
   {"rawSamples/SH.raw",{0},0},   // 11, SH_op, Duration: 121ms
   {"rawSamples/Z.raw",{0},0},    // 12, Z_oo, Duration: 71ms
   {"rawSamples/AW1.raw",{0},0},  // 13, l_AW_ful, Duration: 146ms
   {"rawSamples/NG.raw",{0},0},   // 14, thi_NG, Duration: 121ms
   {"rawSamples/AH1.raw",{0},0},  // 15, f_A_ther, Duration: 146ms
   {"rawSamples/OO1.raw",{0},0},  // 16, l_OO_king, Duration: 103ms
   {"rawSamples/OO.raw",{0},0},   // 17, b_OO_k, Duration: 185ms
   {"rawSamples/L.raw",{0},0},    // 18, L_and, Duration: 103ms
   {"rawSamples/K.raw",{0},0},    // 19, tri_CK, Duration: 80ms
   {"rawSamples/J.raw",{0},0},    // 1A, J_u_DG_e, Duration: 47ms
   {"rawSamples/H.raw",{0},0},    // 1B, H_ello, Duration: 71ms
   {"rawSamples/G.raw",{0},0},    // 1C, G_et, Duration: 71ms
   {"rawSamples/F.raw",{0},0},    // 1D, F_ast, Duration: 103ms
   {"rawSamples/D.raw",{0},0},    // 1E, pai_D, Duration: 55ms
   {"rawSamples/S.raw",{0},0},    // 1F, pa_SS,Duration:  90ms
   {"rawSamples/A.raw",{0},0},    // 20, d_AY, Duration: 185ms
   {"rawSamples/AY.raw",{0},0},   // 21, d_AY, Duration: 65ms
   {"rawSamples/Y1.raw",{0},0},   // 22, Y_ard, Duration: 80ms
   {"rawSamples/UH3.raw",{0},0},  // 23, miss_IO_n, Duration: 47ms
   {"rawSamples/AH.raw",{0},0},   // 24, m_O_p, Duration: 250ms
   {"rawSamples/P.raw",{0},0},    // 25, P_ast, Duration: 103ms
   {"rawSamples/O.raw",{0},0},    // 26, c_O_ld, Duration: 185ms
   {"rawSamples/I.raw",{0},0},    // 27, p_I_n, Duration: 185ms
   {"rawSamples/U.raw",{0},0},    // 28, m_O_ve, Duration: 185ms
   {"rawSamples/Y.raw",{0},0},    // 29, an_Y, Duration: 103ms
   {"rawSamples/T.raw",{0},0},    // 2A, T_ap, Duration: 71ms
   {"rawSamples/R.raw",{0},0},    // 2B, R_ed, Duration: 90ms
   {"rawSamples/E.raw",{0},0},    // 2C, m_EE_t, Duration: 185ms
   {"rawSamples/W.raw",{0},0},    // 2D, W_in, Duration: 80ms
   {"rawSamples/AE.raw",{0},0},   // 2E, d_A_d, Duration: 185ms
   {"rawSamples/AE1.raw",{0},0},  // 2F, A_fter, Duration: 103ms
   {"rawSamples/AW2.raw",{0},0},  // 30, s_A_lty, Duration: 90ms
   {"rawSamples/UH2.raw",{0},0},  // 31, A_bout, Duration: 71ms
   {"rawSamples/UH1.raw",{0},0},  // 32, U_ncle, Duration: 103ms
   {"rawSamples/UH.raw",{0},0},   // 33, c_U_p, Duration: 185ms
   {"rawSamples/O2.raw",{0},0},   // 34, f_O_r, Duration: 80ms
   {"rawSamples/O1.raw",{0},0},   // 35, ab_OA_rd, 121ms
   {"rawSamples/IU.raw",{0},0},   // 36, y_OU, Duration: 59ms
   {"rawSamples/U1.raw",{0},0},   // 37, y_OU, Duration: 90ms
   {"rawSamples/THV.raw",{0},0},  // 38, TH_e, Duration: 80ms
   {"rawSamples/TH.raw",{0},0},   // 39, TH_in, Duration: 71ms
   {"rawSamples/ER.raw",{0},0},   // 3A, b_IR_d, Duration: 146ms
   {"rawSamples/EH.raw",{0},0},   // 3B, g_E_t, Duration: 185ms
   {"rawSamples/E1.raw",{0},0},   // 3C, b_E, Duration: 121ms
   {"rawSamples/AW.raw",{0},0},   // 3D, c_A_LL, Duration: 250ms
   {"rawSamples/PA1.raw",{0},0},  // 3E, PA1: silence, Duration: 185ms
   {"rawSamples/STOP.raw",{0},0}  // 3F, STOP: silence Duration: 47ms
};
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

/************************************************************************

  Name: SpeechSynthesizer

  Purpose: The purpose of this function is to serve as the constructor
  of an SpeechSynthesizer object.  The function loads the contents
  of each phoneme into each entry of the phonemeBuffers[] array so that
  the phoneme PCM samples can be output at a later time. 

  Calling Sequence: SpeechSynthesizer(bool& success)

  Inputs:

    success - A reference to storage for the outcome of initialization.
    A value of true insicates sucessful initialization, and a value of
    false indicates unsuccessful initialization.

  Outputs:

    None.

**************************************************************************/
SpeechSynthesizer::SpeechSynthesizer(bool& success)
{
   int i;
   uint32_t numberOfPhonemes;
   FILE *streamPtr;

   // This makes code clearer.
   numberOfPhonemes = sizeof(pcmBuffers) / sizeof(pcmEntry);

   // We need this for bounds checking in the talk() method.
   maximumPhonemeValue = numberOfPhonemes - 1;

   // Default to success.
   success = true;

   for (i = 0; i < numberOfPhonemes; i++)
   {
      // Open the current phoneme PCM file.
      streamPtr = fopen(pcmBuffers[i].fileNamePtr,"r");

      if (streamPtr != NULL)
      {
         // Read in the PCM samples.
         pcmBuffers[i].numberOfSamples =
            fread(pcmBuffers[i].data,sizeof(int16_t),40000,streamPtr);

         // We're done with this phoneme PCM file.
         fclose(streamPtr);
      } // if
      else
      {
         // Indicate failure.
         success = false;
      } // else
   } // for

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

/************************************************************************

  Name: talk

  Purpose: The purpose of this function is to send PCM data to stdiout
  based upon phoneme codes.  Presently, a phoneme code in the
  phoneme buffer will be used to index into a structure that contains
  a buffer, and a buffer length.

  Calling Sequence: talk(uint8_t*& phonemeBuffer,uint32_t phonemeCount)

  Inputs:

    phonemeBuffer - A reference to storage of a binary representation
    of phonemes.

    phonemeCount - The number of phonemes to decode.

  Outputs:

    None.

**************************************************************************/
void SpeechSynthesizer::talk(uint8_t*& phonemeBuffer,
                             uint32_t phonemeCount)
{
   uint32_t i;
   uint8_t phonemeCode;

   for (i = 0; i < phonemeCount; i++)
   {
      // Retrieve the phoneme code from the buffer.
      phonemeCode = phonemeBuffer[i];

      // Make sure we're in bounds.
      if (phonemeCode <= maximumPhonemeValue)
      {
         // Write the PCM samples to stdout.
         fwrite(pcmBuffers[phonemeCode].data,
                sizeof(int16_t),
                pcmBuffers[phonemeCode].numberOfSamples,
                stdout);
      } // if
   } // for
  
   return;

} // talk
