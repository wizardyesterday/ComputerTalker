//**********************************************************************
// file name: PhonemMaker.cc
//**********************************************************************

#include "PhonemMaker.h"

/************************************************************************

  Name: PhonemMaker

  Purpose: The purpose of this function is to serve as the constructor
  of a PhonemMaker object.

  Calling Sequence: PhonemMaker(success)

  Inputs:

    success - A reference to storage for the outcome of initialization.
    A value of true insicates sucessful initialization, and a value of
    false indicates unsuccessful initialization.

  Outputs:

    None.

**************************************************************************/
PhonemMaker::PhonemMaker(bool& success)
{

   // Retrieve the inputs to the PhonemMaker constructure.
   success = getSystemParameters();

} // PhonemMaker

/************************************************************************

  Name: ~PhonemMaker

  Purpose: The purpose of this function is to serve as the constructor
  of a PhonemMaker object.
synthesizerInitialized
  Calling Sequence: PhonemMaker(rules,
                                numberOfRules,
                                phonemMapPtr,
                                numberOfPhonems)

  Inputs:

    rules - An array of strings that represent the phonetic rules.

    numberOfRules - The number of phonetic rules.

    phonemTable - An array of structures that maps textual phonems to
    the binary phonem code that is presented to the voice synthesizer.

    numberOfPhonems - The number of phonems in the phonem table.

  Outputs:

    None.

**************************************************************************/
PhonemMaker::~PhonemMaker(void)
{

   // Release storage
   ruleTable.clear();

} // ~PhonemMaker

/**************************************************************************

  Name: getSystemParameters

  Purpose: The purpose of this function is to retrieve the parameters
  from the phonem file and rules file.

  Calling Sequence: success = getSystemParameters()

  Inputs:

    None.

  Outputs:

    success - An indicator of the outcome of this function.  A value of
    true indicates that the system parameters were successfully retrieved,
    and a value of false indicates failure.

**************************************************************************/
bool PhonemMaker::getSystemParameters(void)
{
   bool success;
   bool done;
   FILE *phonemStream;
   FILE *ruleStream;
   int phonemCount;
   char *statusPtr;
   char buffer[1000];
   char rule[100];
   int code;
   char name[100];
   int numberOfExistingFiles;
   char key;
   char *keyPtr;

   // Default to failure.
   success = false;
   numberOfExistingFiles = 0;

   // We're using zero-based arrays.
   phonemCount = 0;

   // Open the textual phonem to binary code mapping file.
   phonemStream = fopen("configuration/phonems.txt","r");

   if (phonemStream != NULL)
   {
      // The phonem file exists.
      numberOfExistingFiles++;

      // Set up for loop entry.
      done = false;

      while (!done)
      {
         statusPtr = fgets(buffer,80,phonemStream);

         if (statusPtr != NULL)
         {
            sscanf(buffer,"%d %s",&code,name);

            // Populate the phonem structure.
            phonemeTable[phonemCount].phonemeName = name;
            phonemeTable[phonemCount].phonemeCode = code;

            // Reference the next storage element.
            phonemCount++;
         } // if
         else
         {
            // Bail out.
            done = true;
         } // else

      } // while */

      // We're done with this file.
      fclose(phonemStream);
   } // if

   // Open the phonetic rules file.
   ruleStream = fopen("configuration/rules.txt","r");

   if (ruleStream != NULL)
   {
      // The rules file exists.
      numberOfExistingFiles++;

      // Set up for loop entry.
      done = false;

      while (!done)
      {
         statusPtr = fgets(buffer,100,ruleStream);

         if (statusPtr != NULL)
         {
            // Just copy it so deal with literal whitespace.
            strncpy(rule,buffer,sizeof(rule));

            // Nuke the \n.
            rule[strlen(rule)-1] = '\0';

            keyPtr = index(rule,'(');

            if (keyPtr !=  NULL)
            {
               // Populate the rule since it is valid.
               ruleTable[keyPtr[1]].push_back(rule);
            } // if

         } // if
         else
         {
            // Bail out.
            done = true;
         } // else

      } // while */

      // We're done with this file.
      fclose(ruleStream);
   } // if

   // Does the phonem file and the phonetic rules exist?
   if (numberOfExistingFiles == 2)
   {
      // We're good to go!
      success = true;
   } // if


   return (success);

} // getSystemParameters

/************************************************************************

  Name: translateEnglishText

  Purpose: The purpose of this function is to accept text and translate
  that text into a stream of phonems.  This function provides the
  interface to the analysis code in this system.

  Calling Sequence: translateEnglishText(text,
                                         phonemBuffer,
                                         phonemCount)

  Inputs:

    text - The text which is to be converted to phonem strings.

    phonemBuffer - A reference to memory for which phonems are to be
    stored.

    phonemCount - A reference to memory for which the number of
    phonems is to be stored.

  Outputs:

    None.

  Attributes:

    phonemeBufferIndex - The index into the next storage location in the
    phonem buffer. 

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

    phonemeBuffer - The buffer where the phonem stream is stored.

    ruleTable - The table of text-to-phoneme rules.

*****************************************************************************/
void PhonemMaker::translateEnglishText(std::string& text,
                                       uint8_t*& phonemBuffer,
                                       uint32_t& phonemCount)

{
   int j;
   std::map <char, std::list <std::string> >::iterator i;
   char key;

   // Set to the beginning of the buffers.
   phonemeBufferIndex = 0;
   englishBufferIndex = 0;

   // Compute message length.  Yes, this is redundant.
   englishBufferLength = text.length();

   for (j = 0; j < englishBufferLength; j++)
   {
      // Convert to upper case.
      englishBuffer[j] = toupper(text[j]);
   } // for

   while (englishBufferIndex < englishBufferLength)
   {
      key = englishBuffer[englishBufferIndex];

      // Determine if we have a rule.
      i = ruleTable.find(key);

      if (i != ruleTable.end())
      {
         // Process the rules.
         searchRuleList(i->second);
      } // if
      else
      {
         // Don't process this character since it's not in the rules.
         englishBufferIndex++;
      } // else

   } // while

   // Set return values.  Note that references are being used.
   phonemCount = phonemeBufferIndex;
   phonemBuffer = phonemeBuffer;

   return;

} // translateEnglishText

/************************************************************************

  Name: isAlpha

  Purpose: The purpose of this function is to determine whether or
  not a character is alphabetic.

  Calling Sequence: result = isAlpha(c)

  Inputs:

    c - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is alphabetic, and a value of
    false indicates that the character is not alphabetic.

*****************************************************************************/
bool PhonemMaker::isAlpha(char c)
{
   bool result;

   // Leverage the standard library function.
   result = isalpha((int)c);

   return (result);

} // isAlpha

/************************************************************************

  Name: isVowel

  Purpose: The purpose of this function is to determine whether or
  not a character is a vowel.

  Calling Sequence: result = isVowel(c)

  Inputs:

    c - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a vowel, and a value of
    false indicates that the character is not a vowel.

*****************************************************************************/
bool PhonemMaker::isVowel(char c)
{
   bool result;

   switch(c)
   {
      case 'A':
      case 'E':
      case 'I':
      case 'O':
      case 'U':
      case 'Y':
      {
         // The character is a vowel.
         result = true;
         break;
      } // case

      default:
      {
         // The character is not a vowel.
         result = false;;
         break;
      } // case
   } // switch

   return (result);

} // isVowel

/************************************************************************

  Name: isFrontVowel

  Purpose: The purpose of this function is to determine whether or
  not a character is a front vowel.

  Calling Sequence: result = isFrontVowel(c)

  Inputs:

    c - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a front vowel, and a value of
    false indicates that the character is not a front vowel.

*****************************************************************************/
bool PhonemMaker::isFrontVowel(char c)
{
   bool result;

   switch(c)
   {
      case 'E':
      case 'I':
      case 'Y':
      {
         // The character is a front vowel.
         result = true;
         break;
      } // case

      default:
      {
         // The character is not a front vowel.
         result = false;;
         break;
      } // case
   } // switch

   return (result);

} // isVowel

/************************************************************************

  Name: isConsonant

  Purpose: The purpose of this function is to determine whether or
  not a character is a consonant.

  Calling Sequence: result = isConsonant(c)

  Inputs:

    c - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a consonant, and a value of
    false indicates that the character is not a consonant.

*****************************************************************************/
bool PhonemMaker::isConsonant(char c)
{
   bool result;

   if (isAlpha(c) && (!isVowel(c)))
   {
      result = true;
   } // if
   else
   {
      result = false;
   } // else

   return (result);

} // isConsonant

/************************************************************************

  Name: isVoicedConsonant

  Purpose: The purpose of this function is to determine whether or
  not a character is a voiced consonant.

  Calling Sequence: result = isVoicedConsonant(c)

  Inputs:

    c - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a voiced consonant, and a
    value of false indicates that the character is not a voiced consonant.

*****************************************************************************/
bool PhonemMaker::isVoicedConsonant(char c)
{
   bool result;

   switch(c)
   {
      case 'B':
      case 'D':
      case 'G':
      case 'J':
      case 'L':
      case 'M':
      case 'N':
      case 'R':
      case 'V':
      case 'W':
      case 'Z':
      {
         // The character is a voiced consonant.
         result = true;
         break;
      } // case

      default:
      {
         // The character is not a voiced consonant.
         result = false;;
         break;
      } // case
   } // switch

   return (result);

} // isVoicedConsonant

/************************************************************************

  Name: rightPastVowel

  Purpose: The purpose of this function is to advance the English
  buffer index to the right past all consecutive vowels in the English
  buffer.

  Calling Sequence: rightPastVowel(runningIndex,occurred)

  Inputs:

    runningIndex - A reference to storage for the index into the English
    buffer.

    occurred - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.


  Attributes:

    runningIndex - The running index into the English buffer.

    englishBufferLength - The length of the English string.   

*****************************************************************************/
void PhonemMaker::rightPastVowel(int& runningIndex, bool& occurred)
{
   bool done;

  // Clear initially.
   occurred = false;

   if (runningIndex < englishBufferLength)
   {
      // Clear initially.
      done = false;

      while (!done)
      {
         if (isVowel(englishBuffer[runningIndex]))
         {
            // Indicate occurance.
            occurred = true;

            // Reference next character.
            runningIndex = runningIndex + 1;
         } // if
         else
         {
            // Bail out of scan.
            done = true;
         } // else

         if (runningIndex > englishBufferLength-1)
         {
            // Bail out if past buffer limits.
            done = true;
         } // if
      } // while
   } // if

   return;

} // rightPastVowel

/************************************************************************

  Name: rightPastConsonant

  Purpose: The purpose of this function is to advance the English
  buffer index to the right past all consecutive consonants in the
  English buffer.

  Calling Sequence: rightPastConsonant(runningIndex,occurred)

  Inputs:

    runningIndex - A reference to storage for the index into the English
    buffer.

    occurred - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Attributes:

    runningIndex - The running index into the English buffer.

    englishBufferLength - The length of the English string.   

*****************************************************************************/
void PhonemMaker::rightPastConsonant(int& runningIndex, bool& occurred)
{
   bool done;

   if (runningIndex < englishBufferLength)
   {
      // Set up for loop entry.
      done = false;

      while (!done)
      {
         if (!isConsonant(englishBuffer[runningIndex]))
         {
            // Bail out of scan.
            done = true;
         } // if
         else
         {
            // Reference next character.
            runningIndex = runningIndex + 1;
         } // else

         if (runningIndex > englishBufferLength-1)
         {
            // Bail out if past buffer limits.
            done = true;
         } // if
      } // while
   } // if

   // Indicate outcome of occurrance.
   occurred = true;

   return;

} // rightPastConsonant

/************************************************************************

  Name: leftPastVowel

  Purpose: The purpose of this function is to advance the English
  buffer index to the left past all consecutive vowels in the English
  buffer.

  Calling Sequence: leftPastVowel(runningIndex,occurred)

  Inputs:

    runningIndex - A reference to storage for the index into the English
    buffer.

    occurred - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.

  Attributes:

    runningIndex - The running index into the English buffer.

    englishBuffer - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::leftPastVowel(int& runningIndex, bool& occurred)
{
   bool done;

   // Clear initially.
   occurred = false;

   if (runningIndex >= 0)
   {
      // Set up for loop entry.
      done = false;

      while (!done)
      {
         if (isVowel(englishBuffer[runningIndex]))
         {
            // Indicate occurance.
            occurred = true;

            // Reference character to the left.
            runningIndex = runningIndex - 1;
         } // if
         else
         {
            // Bail out of scan.
            done = true;
         } // else

         if (runningIndex < 0)
         {
            // Bail out if past lower bounds.
            done = true;
         } // if
      } // while
   } // if

   return;

} // leftPastVowel

/************************************************************************

  Name: leftPastConsonant

  Purpose: The purpose of this function is to advance the English
  buffer index to the left past all consecutive consonants in the
  English buffer.

  Calling Sequence: leftPastConsonant(runningIndex,occurred)

  Inputs:

    runningIndex - A reference to storage for the index into the English
    buffer.

    occurred - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Attributes:

    runningIndex - The running index into the English buffer.

    englishBuffer - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::leftPastConsonant(int& runningIndex, bool& occurred)
{
   bool done;

   if (runningIndex >= 0)
   {
      // Set up for loop entry.
      done = false;

      while (!done)
      {
         if (!isConsonant(englishBuffer[runningIndex]))
         {
            // Bail out of scan.
            done = true;
         } // if
         else
         {
            // Reference character to the left.
            runningIndex = runningIndex - 1;
         } // else

         if (runningIndex < 0)
         {
            // Bail out if past lower bounds.
            done = true;
         } // if
      } // while
   } // if

   // Indicate occurance.
   occurred = true;

   return;

} // leftPastConsonant

/************************************************************************

  Name: buildLiteralPhoneme

  Purpose: The purpose of this function is to advance the extract the
  phonem string on the right side of the equation for a particular
  rule that is being processed.
  Calling Sequence: buildLiteralPhoneme(int ruleIndex)

  Inputs:

    ruleIndex - An index into the curent rule for which a phonem string
    is to be extracted.

  Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

    phonemeTokens = The current array of phonem tokens being constructed.
    For example, if a rule happens to be "(AR)=AH1,R;" phonemeTokens will
    be set to "AH1","R",";".

*****************************************************************************/
void PhonemMaker::buildLiteralPhoneme(int ruleIndex)
{
   int i;
   int resultIndex;
   int runningIndex;
   bool done;

   // Point to beginning of result buffer.
   resultIndex = 0;

   // Point past '=' sign.
   runningIndex = ruleIndex + 1;

   if (currentRule[runningIndex] != ';')
   {
      for (i = 0; i < NUMBER_OF_PHONEME_TOKENS; i++)
      {
         // Clear phonem string array.
         phonemeTokens[i] = "";
      } // for

      do
      {
         // Set up for loop entry.
         done = false;

         while (!done)
         {
            if (currentRule[runningIndex] != ';')
            {
               phonemeTokens[resultIndex] =
                  phonemeTokens[resultIndex] + currentRule[runningIndex];

               // Bump rule index.
               runningIndex = runningIndex + 1;

               if (currentRule[runningIndex] == ',')
               {
                  // bump index past comma.
                  runningIndex = runningIndex + 1;

                  // Exit comma scan.
                  done = true;
               } // if
            } // if
            else
            {
               // Exit rule scan.
               done = true;
            } // else
         } // while

         // Bump result index.
         resultIndex = resultIndex + 1;

      } while (currentRule[runningIndex] != ';');
   } // if

   // Store terminator.
   phonemeTokens[resultIndex] = ';';

  return;

} // buildLiteralPhoneme

/************************************************************************

  Name: scanRightContext

  Purpose: The purpose of this function is to scan and evaluate the
  right-hand context of text to be evaluated.

  Calling Sequence: scanRightContext(rightIndex,ruleIndex,found)

  Inputs:

    rightIndex - A reference to storage to the current rule.

    ruleIndex - A reference to storage to the current rule.  This is
    a running index.

    found - A reference to storage for a flag that indicates that a
    right-context rule was satisfied.  A value of true indicates that
    it was not satisfied, and a value of false indicates that it was
    not satisfied.

  Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

    ruleIndex - An index into the current rule being evaluated.

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

    referenceString - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
void PhonemMaker::scanRightContext(int rightIndex,
                                   int& ruleIndex,
                                   bool& found)
{
   int runningIndex;
   bool withinRules;
   bool either;
   bool occurred;
   bool done;

   // Clear initially.
   withinRules = false;
   occurred = false;

   // Set up rule index.
   ruleIndex = rightIndex + 1;

   if (currentRule[ruleIndex] != '=')
   {
      if ((englishBufferIndex + referenceString.length()) < englishBufferLength)
      {
         // Set up running index.
         runningIndex = englishBufferIndex + referenceString.length();

         // Set up for loop entry.
         done = false;

         while ((!done) && (currentRule[ruleIndex] != '='))
         {          
            switch (currentRule[ruleIndex])
            {
               case '!':
               {
                  withinRules = !isAlpha(englishBuffer[runningIndex]);
                  break;
               } // case

               case '#':
               {
                  rightPastVowel(runningIndex,occurred);
                  break;
               } // case

               case ':':
               {
                  rightPastConsonant(runningIndex,occurred);
                  break;
               } // case


               case '+':
               {
                  withinRules = isFrontVowel(englishBuffer[runningIndex]);
                  break;
               } // case

               case '$':
               {
                  withinRules = isConsonant(englishBuffer[runningIndex]);
                  break;
               } // case

               case '.':
               {
                  withinRules = isVoicedConsonant(englishBuffer[runningIndex]);
                  break;
               } // case

               default:
               {
                  withinRules =
                     (currentRule[ruleIndex] == englishBuffer[runningIndex]);
                  break;
               } // case
            } // switch

            // Bump rule index.
            ruleIndex = ruleIndex + 1;

            if (withinRules)
            {
               // Bump running index.
               runningIndex = runningIndex + 1;
            } // if

            // Accept either case.
            either = ((withinRules) || (occurred));

            // Clear flags.
            withinRules = false;
            occurred = false;

            if (runningIndex > englishBufferLength)
            {
               // Exit scan loop..
               done = true;

               if (currentRule[ruleIndex] != '=')
               {
                  // Return false.
                  either = false;
               } // if
            } // if

            if (!either)
            {
               // Exit loop if no match for rule.
               done = true;
            } // if
         } // while

         // Return result.
         found = either;
      } // if
      else
      {
         found = false;
      } // else
   } // if
   else
   {
      // Return true if at '=' sign.
      found = true;
   } // else

   return;

} // scanRightContext

/************************************************************************

  Name: scanLeftContext

  Purpose: The purpose of this function is to scan and evaluate the
  left-hand context of text to be evaluated.

  Calling Sequence: scanLeftContext(leftIndex,found)

  Inputs:

    leftIndex - An index to storage to the current rule.

    found - A reference to storage for a flag that indicates that a
    left-context rule was satisfied.  A value of true indicates that
    it was not satisfied, and a value of false indicates that it was
    not satisfied.

  Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

    ruleIndex - An index into the current rule being evaluated.

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

*****************************************************************************/
void PhonemMaker::scanLeftContext(int leftIndex, bool& found)
{
   int runningIndex;
   int ruleIndex;
   bool either;
   bool occurred;
   bool done;
   bool withinRules;

   // Clear initially.
   withinRules = false;
   occurred = false;

   if (englishBufferIndex > 0)
   {
      // Point to the left of parent.
      ruleIndex = leftIndex - 1;

      // Set up running index in English buffer.
      runningIndex = englishBufferIndex - 1;

      // Clear initially.
      done = false;

      while ((!done) && (ruleIndex >= 0))
      {
         switch (currentRule[ruleIndex])
         {
            case '!':
            {
               withinRules = !isAlpha(englishBuffer[runningIndex]);
               break;
            }  // case

            case '#':
            {
               leftPastVowel(runningIndex,occurred);
               break;
            }  // case

            case ':':
            {
               leftPastConsonant(runningIndex,occurred);
               break;
            }  // case

            case '+':
            {
               withinRules = isFrontVowel(englishBuffer[runningIndex]);
               break;
            }  // case

            case '$':
            {
               withinRules = isConsonant(englishBuffer[runningIndex]);
               break;
            }  // case

            case'.':
            {
               withinRules = isVoicedConsonant(englishBuffer[runningIndex]);
            }  // case
               break;

            default:
            {
               withinRules = 
                  (currentRule[ruleIndex] == englishBuffer[runningIndex]);
               break;
            }  // case
         } // switch

         if (withinRules)
         {
            // Decrement running index.
            runningIndex = runningIndex - 1;
         } // if

         // Save either true result.
         either = withinRules || occurred;

         // Clear flags.
         withinRules = false;
         occurred = false;

         // Decrement rule index.
         ruleIndex = ruleIndex - 1;

         if (runningIndex < 0)
         {
            // Exit scan loop.
            done = true;

            if (ruleIndex >= 0)
            {
               either = false;
            } // if
         } // if

         if (!either)
         {
            // Exit if scan failed.
            done = true;
         } // if
      } // while

      // Return result.
      found = either;
   } // if
   else
   {
      found = false;
   } // else

   return;

} // scanLeftContext

/************************************************************************

  Name: compareReferenceString

  Purpose: The purpose of this function is to scan and evaluate the
  left-hand context of text to be evaluated.

  Calling Sequence: result = compareReferenceString()

  Inputs:

    None.

  Outputs:

    result - A flag that indicates the result of the comparison.  A
    value of true indicates that the reference string matched to the
    current position in the English buffer, and a value of false
    indicates that a match was not found.

  Attributes:

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

    referenceString - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
bool PhonemMaker::compareReferenceString(void)
{
   bool result;
   int i;
   int upperLimit;
   bool done;

   // Point to the beginning of the reference string.
   i = 0;

   // Set upper limit.
   upperLimit = referenceString.length() - 1;

   // Set up for loop entry.
   done = false;

   while (!done)
   {
      if (referenceString[i] != 
            (englishBuffer[englishBufferIndex + i]))
      {
         done = true;
      } // if
      else
      {
         // Reference next item.
         i = i + 1;

         if (i > upperLimit)
         {
            // Exit from loop.
            done = true;
         } // if
      } // else
   } // while

   if (i > upperLimit)
   {
      result = true;
   } // if
   else
   {
      result = false;
   } // if

   return (result);

} // compareReferenceString

/************************************************************************

  Name: buildReferenceString

  Purpose: The purpose of this function is to extract the reference
  string from a rule.  The reference string is the text betweeh the
  parantheses.  For ecample: (AB) implies that the reference string
  is "AB".

  Calling Sequence: buildReferenceString(leftIndex,rightIndex)

  Inputs:

    leftIndex - An index that indicates the position of the left
    '('.

    rightIndex - A reference to storage for the right-hand index into
    the current rule entry.

  Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

    referenceString - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
void PhonemMaker::buildReferenceString(int leftIndex, int& rightIndex)
{

   // Clear initially.
   referenceString = "";

   // Bump past left parent.
   rightIndex = leftIndex + 1;

   while (currentRule[rightIndex] != ')')
   {
      // Build string.
      referenceString = referenceString + currentRule[rightIndex];

      // Bump right parent index.
      rightIndex = rightIndex + 1;
   } // while

   return;

} // buildReferenceString

/************************************************************************

  Name: findLeftParent

  Purpose: The purpose of this function is to scan the current rule
  until the left-hand '('. is found.

  Calling Sequence: findLeftParent(leftIndex)

  Inputs:

    leftIndex - A reference to storage for the index of the left '('.

   Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

*****************************************************************************/
void PhonemMaker::findLeftParent(int& leftIndex)
{

   // Point to beginning of buffer.
   leftIndex = 0;

   while (currentRule[leftIndex] != '(')
   {
      // Bump left parent index.
      leftIndex = leftIndex + 1;
   } // while

   return;

} // findLeftParent

/************************************************************************

  Name: evaluateContexts

  Purpose: The purpose of this function is to evaluate the current
  position of the English text when compared to the current rule.
  Both left and right contexts will be evaluated (if they exist), If
  the current rule is satisfied, more phonems will be added to the
  phonem stream.

  Calling Sequence: result = evaluateContexts()

  Inputs:

    None.

   Outputs:

    result - A flag that indicates whether or not the rule was satisfied.
    A value of true indicates that the rule was satisfied, and a value of
    false indicates that the rule was not satisfied.

  Attributes:

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

    referenceString - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
bool PhonemMaker::evaluateContexts(void)
{
   bool result;
   int leftIndex;
   int rightIndex;
   int ruleIndex;
   bool withinRules;

   // Clear initially.
   withinRules = false;

   // Find left parent.
   findLeftParent(leftIndex);

   // Build reference string, and return right parent.
   buildReferenceString(leftIndex,rightIndex);

   // Compare reference string to English.
   if (compareReferenceString())
   {
      if (leftIndex > 0)
      {
         // Scan left context.
         scanLeftContext(leftIndex,withinRules);

         // If there is a left context.
         if (withinRules)
         {
            // Scan right context.
            scanRightContext(rightIndex,ruleIndex,withinRules);
         } // if
      } // if
      else
      {
         // Scan right context.
         scanRightContext(rightIndex,ruleIndex,withinRules);
      } // else
   } // if

   if (withinRules)
   {
      // Build literal phonem string.
      buildLiteralPhoneme(ruleIndex);

      // Convert to phonem codes.
      convertPhonemesToCode();

      // Bump English index.
      englishBufferIndex = englishBufferIndex + referenceString.length();
   } // if

   // Set return value based upon result.  Yes, it's redundant.
   result  = withinRules;

   return (result);

} // evaluateContexts

/************************************************************************

  Name: searchRuleList

  Purpose: The purpose of this function is to scan the rules of a
  letter in the English text and generate a phonem stream.  This text
  (extracted from the original code follows.

  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  Rule format - #(SED)!=Z,D;

    # - Left context
    ( - Left context delimiter
    SED - String to be matched
    ) - Right context delimiter
    ! - Right context
    = - Right context terminator
    Z - Phoneme code
    , - Phoneme separator
    D - Phoneme code
    ; - Rule terminator

    Meaning of rule symbols

      ! - Invokes a procedure that attempts to match any nonalphabetic
          character in English input string. If match failes, reports
          feilure. If match succeeds, moves rule string pointer by
          one character in rule and moves input string pointer by
          one character in the English string. The direction of
          movement is determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

        # - Invokes a procedure that attempts to match one or more
            vowels (A,E,I,O,U, OR Y). If match fails, reports failure.
            If match succeeds, moves rule pointer by one character in
            rules and moves string pointer by the number of vowels
            matched in English string. The direction of movement is
            determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

        : - Invokes a procedure that attempts to match zero or more
            consonants. Match always succeeds. Moves rule pointer by
            one character in rules and moves string pointer by the
            number of consonants matched in English input string. The
            direction of movement is determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

        + - Invokes a procedure that attempts to match a front vowel
            (E,I, OR Y). If match fails, reports failure. If match
            succeeds, moves rule pointer by one character in rules
            and moves string pointer by one character in input string.
            The direction of movement is determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

        $ - Invokes a procedure that attempts to match one consonant. If
            match fails, reports failure. If match succeeds, moves rule
            pointer by one character in rules and moves string pointer
            by one character in English input string. Direction of movement
            is determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

        . - Invokes a procedure that attempts to match a voiced consonant
            (B,D,G,J,L,M,N,R,V,W, OR Z). If match fails, reports failure.
            If match succeeds, moves rule pointer by one character in rules
            and moves string pointer by one character in English input
            string. Direction of movement is determined as:

              1. Forward if right context is being scanned
              2. Backward if left context is being scanned

  After a match is found, the index into the English buffer is incremented
  by the number of characters inside of the parenthesis of the rule.
  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  Calling Sequence: searchRuleList(rules)

  Inputs:

    rules - A list of rules that are to be evaluated.

   Outputs:

    None.

  Attributes:

    currentRule - A string that contains the current rule being evaluated.

    englishBufferIndex - The current index into the English buffer for which
    text is to be evaluated.

*****************************************************************************/
void PhonemMaker::searchRuleList(std::list<std::string> rules)
{
   bool done;
   bool found;
   std::list<std::string>::iterator i;

   // Set up for loop entry.
   done = false;

   // Reference the first rule in the list.
   i = rules.begin();

   while (!done)
   {
      // Get current rule.
      currentRule = *i;

      // Scan using current rule.
      found = evaluateContexts();

      // Reference the next rule.
      i++;

      if ((i == rules.end()) || found)
      {
         // Exit scan.
         done = true;
      } // if
   } // while

   if (!found)
   {
      // Reference the next English item.
      englishBufferIndex = englishBufferIndex + 1;
   } // if

   return;

} // searchRuleList

/************************************************************************

  Name: convertPhonemeToCode

  Purpose: The purpose of this function is to convert a string
  representation of a phonem into it's binary code.  The code will
  be added to the phonem buffer.

  Calling Sequence: convertPhonemeToCode(phonemeToken)

  Inputs:

    phonemeToken - A string representation of a phonem for which its binary
    equivalent is to be created.  This parameter differs from the
    attribute, phonemeTokens[], which is an array of phonem strings.

   Outputs:

    None.

  Attributes:

    phonemeTable - A table that maps the textual representation of phonems to
    the binary representation.

    phonemeBufferIndex - The index into the next storage location in the
    phonem buffer. 

    phonemeBuffer - The buffer where the phonem stream is stored.   

*****************************************************************************/
void PhonemMaker::convertPhonemeToCode(std::string phonemeToken)
{
   int i;
   bool match;

   // Point to begining of phonem table.
   i = 0;

   // Clear initially.
   match = false;

   while (!match)
   {
      if (phonemeToken == phonemeTable[i].phonemeName)
      {
         // Store phonem code.
         phonemeBuffer[phonemeBufferIndex] = phonemeTable[i].phonemeCode;

         // Reference the next phonem buffer location.
         phonemeBufferIndex = phonemeBufferIndex + 1;

         // Exit loop.
         match = true;
      } // if
      else
      {
         // Reference the next item.
         i = i + 1;
      } // else
   } // while

  return;

} // convertPhonemeToCode

/************************************************************************

  Name: convertPhonemeToCode

  Purpose: The purpose of this function is to convert an array of
  string
  representations of a phonems into it's binary codes.  The code stream
  will be placed in the phonem buffer.

  Calling Sequence: convertPhonemesToCode()

  Inputs:

    None.

   Outputs:

    None.

  Attributes:

     phonemeTokens = The current array of phonem tokens being evaluated.  For
    example, if a rule happens to be "(AR)=AH1,R;" phonemeTokens will be set
    to "AH1","R",";".

*****************************************************************************/
void PhonemMaker::convertPhonemesToCode(void)
{
   int i;

   // Point to beginning of array of phonem strings.
   i = 0;

   while (phonemeTokens[i] != ";")
   {
      // Convert phonems to codes.
      convertPhonemeToCode(phonemeTokens[i]);

      // Reference next entry.
      i = i + 1;
   } // while

  return;

} // convertPhonemesToCode
