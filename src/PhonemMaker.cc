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
   RUL_TBL.clear();

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
   char alpha[100];
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
            sscanf(buffer,"%d %s",&code,alpha);

            // Populate the phonem structure.
            PHO_TBL[phonemCount].ALPHA = alpha;
            PHO_TBL[phonemCount].CODE = code;

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
               RUL_TBL[keyPtr[1]].push_back(rule);
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

  Attributes (formerly globals):

    P_INDEX - The index into the next storage location in the phonem
    buffer. 

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    P_BUFFER - The buffer where the phonem stream is stored.

    RUL_TBL - The table of text-to-phoneme rules.

*****************************************************************************/
void PhonemMaker::translateEnglishText(std::string& text,
                                       uint8_t*& phonemBuffer,
                                       uint32_t& phonemCount)

{
   int INDEX;
   std::map <char, std::list <std::string> >::iterator i;
   char key;

   // Set to the beginning of the buffers.
   P_INDEX = 0;
   E_INDEX = 0;

   // Compute message length.  Yes, this is redundant.
   E_LEN = text.length();

   for (INDEX = 0; INDEX < E_LEN; INDEX++)
   {
      // Convert to upper case.
      E_BUFFER[INDEX] = toupper(text[INDEX]);
   } // for

   while (E_INDEX < E_LEN)
   {
      key = E_BUFFER[E_INDEX];

      // Determine if we have a rule.
      i = RUL_TBL.find(key);

      if (i != RUL_TBL.end())
      {
         // Process the rules.
         searchRuleList(i->second);
      } // if
      else
      {
         // Don't process this character since it's not in the rules.
         E_INDEX++;
      } // else

   } // while

   // Set return values.  Note that references are being used.
   phonemCount = P_INDEX;
   phonemBuffer = P_BUFFER;

   return;

} // ENG_TO_PH

/************************************************************************

  Name: isAlpha

  Purpose: The purpose of this function is to determine whether or
  not a character is alphabetic.

  Calling Sequence: result = isAlpha(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is alphabetic, and a value of
    false indicates that the character is not alphabetic.

*****************************************************************************/
bool PhonemMaker::isAlpha(char CH)
{
   bool result;

   // Leverage the standard library function.
   result = isalpha((int)CH);

   return (result);

} // isAlpha

/************************************************************************

  Name: isVowel

  Purpose: The purpose of this function is to determine whether or
  not a character is a vowel.

  Calling Sequence: result = isVowel(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a vowel, and a value of
    false indicates that the character is not a vowel.

*****************************************************************************/
bool PhonemMaker::isVowel(char CH)
{
   bool result;

   switch(CH)
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

  Calling Sequence: result = isFrontVowel(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a front vowel, and a value of
    false indicates that the character is not a front vowel.

*****************************************************************************/
bool PhonemMaker::isFrontVowel(char CH)
{
   bool result;

   switch(CH)
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

  Calling Sequence: result = isConsonant(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a consonant, and a value of
    false indicates that the character is not a consonant.

*****************************************************************************/
bool PhonemMaker::isConsonant(char CH)
{
   bool result;

   if (isAlpha(CH) && (!isVowel(CH)))
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

  Calling Sequence: result = isVoicedConsonant(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a voiced consonant, and a
    value of false indicates that the character is not a voiced consonant.

*****************************************************************************/
bool PhonemMaker::isVoicedConsonant(char CH)
{
   bool result;

   switch(CH)
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

  Calling Sequence: rightPastVowel(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.


  Attributes (formerly globals):

    R_INDEX - The running index into the English buffer.

    E_LEN - The length of the English string.   

*****************************************************************************/
void PhonemMaker::rightPastVowel(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

  // Clear initially.
   OCCURED = false;

   if (R_INDEX < E_LEN)
   {
      // Clear initially.
      DONE = false;

      while (!DONE)
      {
         if (isVowel(E_BUFFER[R_INDEX]))
         {
            // Indicate occurance.
            OCCURED = true;

            // Reference next character.
            R_INDEX = R_INDEX + 1;
         } // if
         else
         {
            // Bail out of scan.
            DONE = true;
         } // else

         if (R_INDEX > E_LEN-1)
         {
            // Bail out if past buffer limits.
            DONE = true;
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

  Calling Sequence: rightPastConsonant(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Attributes (formerly globals):

    R_INDEX - The running index into the English buffer.

    E_LEN - The length of the English string.   

*****************************************************************************/
void PhonemMaker::rightPastConsonant(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

   if (R_INDEX < E_LEN)
   {
      // Set up for loop entry.
      DONE = false;

      while (!DONE)
      {
         if (!isConsonant(E_BUFFER[R_INDEX]))
         {
            // Bail out of scan.
            DONE = true;
         } // if
         else
         {
            // Reference next character.
            R_INDEX = R_INDEX + 1;
         } // else

         if (R_INDEX > E_LEN-1)
         {
            // Bail out if past buffer limits.
            DONE = true;
         } // if
      } // while
   } // if

   // Indicate outcome of occurrance.
   OCCURED = true;

   return;

} // rightPastConsonant

/************************************************************************

  Name: leftPastVowel

  Purpose: The purpose of this function is to advance the English
  buffer index to the left past all consecutive vowels in the English
  buffer.

  Calling Sequence: leftPastVowel(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.

  Attributes (formerly globals):

    R_INDEX - The running index into the English buffer.

    E_BUFFER - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::leftPastVowel(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

   // Clear initially.
   OCCURED = false;

   if (R_INDEX >= 0)
   {
      // Set up for loop entry.
      DONE = false;

      while (!DONE)
      {
         if (isVowel(E_BUFFER[R_INDEX]))
         {
            // Indicate occurance.
            OCCURED = true;

            // Reference character to the left.
            R_INDEX = R_INDEX - 1;
         } // if
         else
         {
            // Bail out of scan.
            DONE = true;
         } // else

         if (R_INDEX < 0)
         {
            // Bail out if past lower bounds.
            DONE = true;
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

  Calling Sequence: leftPastConsonant(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Attributes (formerly globals):

    R_INDEX - The running index into the English buffer.

    E_BUFFER - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::leftPastConsonant(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

   if (R_INDEX >= 0)
   {
      // Set up for loop entry.
      DONE = false;

      while (!DONE)
      {
         if (!isConsonant(E_BUFFER[R_INDEX]))
         {
            // Bail out of scan.
            DONE = true;
         } // if
         else
         {
            // Reference character to the left.
            R_INDEX = R_INDEX - 1;
         } // else

         if (R_INDEX < 0)
         {
            // Bail out if past lower bounds.
            DONE = true;
         } // if
      } // while
   } // if

   // Indicate occurance.
   OCCURED = true;

   return;

} // leftPastConsonant

/************************************************************************

  Name: buildLiteralPhoneme

  Purpose: The purpose of this function is to advance the extract the
  phonem string on the right side of the equation for a particular
  rule that is being processed.
  Calling Sequence: buildLiteralPhoneme(int RUL_INDX)

  Inputs:

    RUL_INDX - An index into the curent rule for which a phonem string
    is to be extracted.

  Outputs:

    None.

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

    PH_STR = The current array of phonem tokens being constructed.  For
    example, if a rule happens to be "(AR)=AH1,R;" PH_STR will be set
    to "AH1","R",";".

*****************************************************************************/
void PhonemMaker::buildLiteralPhoneme(int RUL_INDX)
{
   int INDEX;
   int RES_INDX;
   int R_INDEX;
   bool DONE;

   // Point to beginning of result buffer.
   RES_INDX = 0;

   // Point past '=' sign.
   R_INDEX = RUL_INDX + 1;

   if (R_BUFFER[R_INDEX] != ';')
   {
      for (INDEX = 0; INDEX < NUM_PH_TOK; INDEX++)
      {
         // Clear phonem string array.
         PH_STR[INDEX] = "";
      } // for

      do
      {
         // Set up for loop entry.
         DONE = false;

         while (!DONE)
         {
            if (R_BUFFER[R_INDEX] != ';')
            {
               PH_STR[RES_INDX] = PH_STR[RES_INDX] + R_BUFFER[R_INDEX];

               // Bump rule index.
               R_INDEX = R_INDEX + 1;

               if (R_BUFFER[R_INDEX] == ',')
               {
                  // bump index past comma.
                  R_INDEX = R_INDEX + 1;

                  // Exit comma scan.
                  DONE = true;
               } // if
            } // if
            else
            {
               // Exit rule scan.
               DONE = true;
            } // else
         } // while

         // Bump result index.
         RES_INDX = RES_INDX + 1;

      } while (R_BUFFER[R_INDEX] != ';');
   } // if

   // Store terminator.
   PH_STR[RES_INDX] = ';';

  return;

} // buildLiteralPhoneme

/************************************************************************

  Name: scanRightContext

  Purpose: The purpose of this function is to scan and evaluate the
  right-hand context of text to be evaluated.

  Calling Sequence: scanRightContext(RT_INDX,RUL_INDX,FOUND)

  Inputs:

    RT_INDX - A reference to storage to the current rule.

    RUL_INDX - A reference to storage to the current rule.  This is
    a running index.

    FOUND - A reference to storage for a flag that indicates that a
    right-context rule was satisfied.  A value of true indicates that
    it was not satisfied, and a value of false indicates that it was
    not satisfied.

  Outputs:

    None.

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

    RUL_INDX - An index into the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
void PhonemMaker::scanRightContext(int RT_INDX, int& RUL_INDX, bool& FOUND)
{
   int R_INDEX;
   bool WI_RULES;
   bool EITHER;
   bool OCCURED;
   bool DONE;

   // Clear initially.
   WI_RULES = false;
   OCCURED = false;

   // Set up rule index.
   RUL_INDX = RT_INDX + 1;

   if (R_BUFFER[RUL_INDX] != '=')
   {
      if ((E_INDEX + REF_STR.length()) < E_LEN)
      {
         // Set up running index.
         R_INDEX = E_INDEX + REF_STR.length();

         // Set up for loop entry.
         DONE = false;

         while ((!DONE) && (R_BUFFER[RUL_INDX] != '='))
         {          
            switch (R_BUFFER[RUL_INDX])
            {
               case '!':
               {
                  WI_RULES = !isAlpha(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '#':
               {
                  rightPastVowel(R_INDEX,OCCURED);
                  break;
               } // case

               case ':':
               {
                  rightPastConsonant(R_INDEX,OCCURED);
                  break;
               } // case


               case '+':
               {
                  WI_RULES = isFrontVowel(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '$':
               {
                  WI_RULES = isConsonant(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '.':
               {
                  WI_RULES = isVoicedConsonant(E_BUFFER[R_INDEX]);
                  break;
               } // case

               default:
               {
                  WI_RULES = (R_BUFFER[RUL_INDX] == E_BUFFER[R_INDEX]);
                  break;
               } // case
            } // switch

            // Bump rule index.
            RUL_INDX = RUL_INDX + 1;

            if (WI_RULES)
            {
               // Bump running index.
               R_INDEX = R_INDEX + 1;
            } // if

            // Accept either case.
            EITHER = ((WI_RULES) || (OCCURED));

            // Clear flags.
            WI_RULES = false;
            OCCURED = false;

            if (R_INDEX > E_LEN)
            {
               // Exit scan loop..
               DONE = true;

               if (R_BUFFER[RUL_INDX] != '=')
               {
                  // Return false.
                  EITHER = false;
               } // if
            } // if

            if (!EITHER)
            {
               // Exit loop if no match for rule.
               DONE = true;
            } // if
         } // while

         // Return result.
         FOUND = EITHER;
      } // if
      else
      {
         FOUND = false;
      } // else
   } // if
   else
   {
      // Return true if at '=' sign.
      FOUND = true;
   } // else

   return;

} // scanRightContext

/************************************************************************

  Name: scanLeftContext

  Purpose: The purpose of this function is to scan and evaluate the
  left-hand context of text to be evaluated.

  Calling Sequence: scanLeftContext(LEF_INDX,FOUND)

  Inputs:

    LEF_INDX - An index to storage to the current rule.

    FOUND - A reference to storage for a flag that indicates that a
    left-context rule was satisfied.  A value of true indicates that
    it was not satisfied, and a value of false indicates that it was
    not satisfied.

  Outputs:

    None.

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

    RUL_INDX - An index into the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

*****************************************************************************/
void PhonemMaker::scanLeftContext(int LEF_INDX, bool& FOUND)
{
   int R_INDEX;
   int RUL_INDX;
   bool EITHER;
   bool OCCURED;
   bool DONE;
   bool WI_RULES;

   // Clear initially.
   WI_RULES = false;
   OCCURED = false;

   if (E_INDEX > 0)
   {
      // Point to the left of parent.
      RUL_INDX = LEF_INDX - 1;

      // Set up running index in English buffer.
      R_INDEX = E_INDEX - 1;

      // Clear initially.
      DONE = false;

      while ((!DONE) && (RUL_INDX >= 0))
      {
         switch (R_BUFFER[RUL_INDX])
         {
            case '!':
            {
               WI_RULES = !isAlpha(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case '#':
            {
               leftPastVowel(R_INDEX,OCCURED);
               break;
            }  // case

            case ':':
            {
               leftPastConsonant(R_INDEX,OCCURED);
               break;
            }  // case

            case '+':
            {
               WI_RULES = isFrontVowel(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case '$':
            {
               WI_RULES = isConsonant(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case'.':
            {
               WI_RULES = isVoicedConsonant(E_BUFFER[R_INDEX]);
            }  // case
               break;

            default:
            {
               WI_RULES = (R_BUFFER[RUL_INDX] == E_BUFFER[R_INDEX]);
               break;
            }  // case
         } // switch

         if (WI_RULES)
         {
            // Decrement running index.
            R_INDEX = R_INDEX - 1;
         } // if

         // Save either true result.
         EITHER = WI_RULES || OCCURED;

         // Clear flags.
         WI_RULES = false;
         OCCURED = false;

         // Decrement rule index.
         RUL_INDX = RUL_INDX - 1;

         if (R_INDEX < 0)
         {
            // Exit scan loop.
            DONE = true;

            if (RUL_INDX >= 0)
            {
               EITHER = false;
            } // if
         } // if

         if (!EITHER)
         {
            // Exit if scan failed.
            DONE = true;
         } // if
      } // while

      // Return result.
      FOUND = EITHER;
   } // if
   else
   {
      FOUND = false;
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

  Attributes (formerly globals):

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
bool PhonemMaker::compareReferenceString(void)
{
   bool result;
   int INDEX;
   int LIMIT;
   bool DONE;

   // Point to the beginning of the reference string.
   INDEX = 0;

   // Set upper limit.
   LIMIT = REF_STR.length() - 1;

   // Set up for loop entry.
   DONE = false;

   while (!DONE)
   {
      if (REF_STR[INDEX] != (E_BUFFER[E_INDEX + INDEX]))
      {
         DONE = true;
      } // if
      else
      {
         // Reference next item.
         INDEX = INDEX + 1;

         if (INDEX > LIMIT)
         {
            // Exit from loop.
            DONE = true;
         } // if
      } // else
   } // while

   if (INDEX > LIMIT)
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

  Calling Sequence: buildReferenceString(LEF_INDX,RT_INDX)

  Inputs:

    LEF_INDX - An index that indicates the position of the left
    '('.

    RT_INDX - A reference to storage for the right-hand index into
    the current rule entry.

  Outputs:

    None.

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

    REF_STR - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
void PhonemMaker::buildReferenceString(int LEF_INDX, int& RT_INDX)
{

   // Clear initially.
   REF_STR = "";

   // Bump past left parent.
   RT_INDX = LEF_INDX + 1;

   while (R_BUFFER[RT_INDX] != ')')
   {
      // Build string.
      REF_STR = REF_STR + R_BUFFER[RT_INDX];

      // Bump right parent index.
      RT_INDX = RT_INDX + 1;
   } // while

   return;

} // buildReferenceString

/************************************************************************

  Name: findLeftParent

  Purpose: The purpose of this function is to scan the current rule
  until the left-hand '('. is found.

  Calling Sequence: findLeftParent(LEF_INDX)

  Inputs:

    LEF_INDX - A reference to storage for the index of the left '('.

   Outputs:

    None.

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

*****************************************************************************/
void PhonemMaker::findLeftParent(int& LEF_INDX)
{

   // Point to beginning of buffer.
   LEF_INDX = 0;

   while (R_BUFFER[LEF_INDX] != '(')
   {
      // Bump left parent index.
      LEF_INDX = LEF_INDX + 1;
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

  Attributes (formerly globals):

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being constructed.  The
    reference string is the text that lies within the parenthesis of
    a rule.

*****************************************************************************/
bool PhonemMaker::evaluateContexts(void)
{
   bool result;
   int LEF_INDX;
   int RT_INDX;
   int RUL_INDX;
   bool WI_RULES;

   // Clear initially.
   WI_RULES = false;

   // Find left parent.
   findLeftParent(LEF_INDX);

   // Build reference string, and return right parent.
   buildReferenceString(LEF_INDX,RT_INDX);

   // Compare reference string to English.
   if (compareReferenceString())
   {
      if (LEF_INDX > 0)
      {
         // Scan left context.
         scanLeftContext(LEF_INDX,WI_RULES);

         // If there is a left context.
         if (WI_RULES)
         {
            // Scan right context.
            scanRightContext(RT_INDX,RUL_INDX,WI_RULES);
         } // if
      } // if
      else
      {
         // Scan right context.
         scanRightContext(RT_INDX,RUL_INDX,WI_RULES);
      } // else
   } // if

   if (WI_RULES)
   {
      // Build literal phonem string.
      buildLiteralPhoneme(RUL_INDX);

      // Convert to phonem codes.
      convertPhonemesToCode();

      // Bump English index.
      E_INDEX = E_INDEX + REF_STR.length();
   } // if

   // Set return value based upon result.  Yes, it's redundant.
   result  = WI_RULES;

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

  Attributes (formerly globals):

    R_BUFFER - A string that contains the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

*****************************************************************************/
void PhonemMaker::searchRuleList(std::list<std::string> rules)
{
   bool DONE;
   bool FOUND;
   std::list<std::string>::iterator i;

   // Set up for loop entry.
   DONE = false;

   // Reference the first rule in the list.
   i = rules.begin();

   while (!DONE)
   {
      // Get current rule.
      R_BUFFER = *i;

      // Scan using current rule.
      FOUND = evaluateContexts();

      // Reference the next rule.
      i++;

      if ((i == rules.end()) || FOUND)
      {
         // Exit scan.
         DONE = true;
      } // if
   } // while

   if (!FOUND)
   {
      // Reference the next English item.
      E_INDEX = E_INDEX + 1;
   } // if

   return;

} // searchRuleList

/************************************************************************

  Name: convertPhonemeToCode

  Purpose: The purpose of this function is to convert a string
  representation of a phonem into it's binary code.  The code will
  be added to the phonem buffer.

  Calling Sequence: convertPhonemeToCode(PH_STR)

  Inputs:

    PH_STR - A string representation of a phonem for which its binary
    equivalent is to be created.  This parameter differs from the
    attribute, PH_STR[], which is an array of phonem strings.

   Outputs:

    None.

  Attributes (formerly globals):

    PHO_TBL - A table that maps the textual representation of phonems to
    the binary representation.

    P_INDEX - The index into the next storage location in the phonem
    buffer. 

    P_BUFFER - The buffer where the phonem stream is stored.   

*****************************************************************************/
void PhonemMaker::convertPhonemeToCode(std::string PH_STR)
{
   int INDEX;
   bool MATCH;

   // Point to begining of phonem table.
   INDEX = 0;

   // Clear initially.
   MATCH = false;

   while (!MATCH)
   {
      if (PH_STR == PHO_TBL[INDEX].ALPHA)
      {
         // Store phonem code.
         P_BUFFER[P_INDEX] = PHO_TBL[INDEX].CODE;

         // Reference the next phonem buffer location.
         P_INDEX = P_INDEX + 1;

         // Exit loop.
         MATCH = true;
      } // if
      else
      {
         // Reference the next item.
         INDEX = INDEX + 1;
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

    PHO_TBL - A table that maps the textual representation of phonems to
    the binary representation.

    PH_STR = The current array of phonem tokens being evaluated.  For
    example, if a rule happens to be "(AR)=AH1,R;" PH_STR will be set
    to "AH1","R",";".

*****************************************************************************/
void PhonemMaker::convertPhonemesToCode(void)
{
   int INDEX;

   // Point to beginning of array of phonem strings.
   INDEX = 0;

   while (PH_STR[INDEX] != ";")
   {
      // Convert phonems to codes.
      convertPhonemeToCode(PH_STR[INDEX]);

      // Reference next entry.
      INDEX = INDEX + 1;
   } // while

  return;

} // convertPhonemesToCode
