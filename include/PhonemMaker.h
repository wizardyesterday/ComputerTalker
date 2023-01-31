//**********************************************************************
// file name: PhonemMaker.h
//**********************************************************************

#ifndef _PHONEMMAKER_H_
#define _PHONEMMAKER_H_

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
#define MAXLINE (80)     // Maximum length of input buffer.
#define MAX_PHO (1000)   // Size of phonem buffer.
#define NUM_PHON (64)    // Number of phonems
#define NUM_PH_TOK (30)  // Maximum number of phonem tokens.

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class PhonemMaker
{
   public:

   struct PhonemToCodeEntry
   {
      std::string ALPHA;
      uint8_t CODE;
   };

   PhonemMaker(bool& success);

   ~PhonemMaker(void);

   void translateEnglishText(std::string& INBUF, uint8_t*& phonemBuffer,
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
   void rightPastVowel(int& R_INDEX, bool& occurred);
   void leftPastVowel(int& R_INDEX, bool& occurred);
   void rightPastConsonant(int& R_INDEX, bool& occurred);
   void leftPastConsonant(int& R_INDEX, bool& occurred);

   void buildLiteralPhoneme(int RUL_INDX);
   void scanRightContext(int RT_INDX, int& RUL_INDX, bool& found);
   void scanLeftContext(int LEF_INDX, bool& found);
   bool compareReferenceString(void);
   void buildReferenceString(int LEF_INDX, int& RT_INDX);
   void findLeftParent(int& LEF_INDX);
   bool evaluateContexts(void);
   void searchRuleList(std::list<std::string> rules);

   void convertPhonemeToCode(std::string phonemeToken);
   void convertPhonemesToCode(void);

   //*****************************************
   // Attributes.
   //*****************************************
   // English uppercase English text buffer.
   char englishBuffer[MAXLINE];

   // Buffer used for phonem storage.
   uint8_t phonemeBuffer[MAX_PHO];

   // This table contains the textual phonetic rules.
   std::map <char, std::list <std::string> > ruleTable;

   // This table is used to map textual phonems to binary values.
   PhonemToCodeEntry phonemeTable[NUM_PHON];

   // Storage for the current rule.
   std::string currentRule;

   // This is used for string comparison with the English buffer.
   std::string referenceString;

   // Storage for phonem strings to be evaluated.
   std::string phonemeTokens[NUM_PH_TOK];

   // Number of entries in the English buffer.
   int englishBufferLength;

   // Current location in the English buffer.
   int englishBufferIndex;

   // Next available location in the phonem buffer.
   int phonemeBufferIndex;
};

#endif // _PHONEMMAKER_H_
