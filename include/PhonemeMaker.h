//**********************************************************************
// file name: PhonemeMaker.h
//**********************************************************************

#ifndef _PHONEMEMAKER_H_
#define _PHONEMEMAKER_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <string>
#include <map>
#include <list>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// Buffer size defines.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#define MAXLINE (80)                  // Maximum length of input buffer.
#define PHOENEME_BUFFER_SIZE (1000)   // 
#define NUMBER_OF_PHONEMES (64)       // Number of phonems
#define NUMBER_OF_PHONEME_TOKENS (30) // Maximum number of phoneme tokens.

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class PhonemeMaker
{
   public:

   struct PhonemToCodeEntry
   {
      std::string phonemeName;
      uint8_t phonemeCode;
   };

   PhonemeMaker(bool& success);

   ~PhonemeMaker(void);

   void translateEnglishText(std::string& text, uint8_t*& phonemBuffer,
                             uint32_t& phonemCount);


   private:

   //*****************************************
   // Utility functions.
   //*****************************************
   bool getSystemParameters(void);

   bool isAlpha(char c);
   bool isVowel(char c);
   bool isFrontVowel(char c);
   bool isConsonant(char c);
   bool isVoicedConsonant(char c);

   void rightPastVowel(int& runningIndex, bool& occurred);
   void leftPastVowel(int& runningIndex, bool& occurred);
   void rightPastConsonant(int& runningIndex, bool& occurred);
   void leftPastConsonant(int& runningIndex, bool& occurred);

   void buildLiteralPhoneme(int ruleIndex);
   void scanRightContext(int rightIndex, int& ruleIndex, bool& found);
   void scanLeftContext(int leftIndex, bool& found);
   bool compareReferenceString(void);
   void buildReferenceString(int leftIndex, int& rightIndex);
   void findLeftParent(int& leftIndex);
   bool evaluateContexts(void);
   void searchRuleList(std::list<std::string> rules);

   void convertPhonemeToCode(std::string phonemeToken);
   void convertPhonemesToCode(void);

   //*****************************************
   // Attributes.
   //*****************************************
   // Uppercase English text buffer.
   char englishBuffer[MAXLINE];

   // Buffer used for phonem storage.
   uint8_t phonemeBuffer[PHOENEME_BUFFER_SIZE];

   // This table contains the textual phonetic rules.
   std::map <char, std::list <std::string> > ruleTable;

   // This table is used to map phoneme names to binary values.
   PhonemToCodeEntry phonemeTable[NUMBER_OF_PHONEMES];

   // Storage for the current rule being evaluated.
   std::string currentRule;

   // This is used for string comparison with the English buffer.
   std::string referenceString;

   // Storage for phonem strings to be evaluated.
   std::string phonemeTokens[NUMBER_OF_PHONEME_TOKENS];

   // Number of entries in the English buffer.
   int englishBufferLength;

   // Current location in the English buffer.
   int englishBufferIndex;

   // Next available location in the phonem buffer.
   int phonemeBufferIndex;
};

#endif // _PHONEMEMAKER_H_
