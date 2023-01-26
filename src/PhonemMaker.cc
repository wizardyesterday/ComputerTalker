//**********************************************************************
// file name: PhonemMaker.cc
//**********************************************************************

#include "PhonemMaker.h"

/************************************************************************
e
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

} // ~PhonemMaker

/**************************************************************************

  Name: getSystemParameters

  Purpose: The purpose of this function is to set the operating point
  of the AGC.

  Calling Sequence: success = getSystemParameters(ruleCount,phonemCount)

  Inputs:

    ruleCount - A reference variable for the number of rules returned by
    this function.

    phonemCount - A reference variable for the number of phonems returned
    by this function

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
   int ruleCount;
   char *statusPtr;
   char buffer[1000];
   char rule[100];
   int code;
   char alpha[100];
   int numberOfExistingFiles;

   // Default to failure.
   success = false;
   numberOfExistingFiles = 0;

   // We're using zero-based arrays.
   phonemCount = 0;
   ruleCount = 0;

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

            // Populate the rule.
            RUL_TBL[ruleCount] = rule;

            // Reference the next storage element.
            ruleCount++;
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

  Name: acceptEnglishText

  Purpose: The purpose of this function is to accept text and translate
  that text into a stream of phonems.  This function provides the
  interface to the analysis code in this system.

  Calling Sequence: acceptEnglishText(text,
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

  Globals:

    P_INDEX - The index into the next storage location in the phonem
    buffer. 

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    P_BUFFER - The buffer where the phonem stream is stored.   

*****************************************************************************/
void PhonemMaker::acceptEnglishText(std::string& text,
                                    uint8_t*& phonemBuffer,
                                    uint32_t& phonemCount)

{
   int INDEX;

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
      switch (E_BUFFER[E_INDEX])
      {
         case 'A':
         {
            // Process with the rules of A.
            RUL_SRCH(0,5);
            break;
         } // case

         case 'B':
          {
            // Process with the rules of B.
            RUL_SRCH(5,1);
            break;
         } // case

         case 'C':
         {
            // Process with the rules of C.
            RUL_SRCH(6,3);
            break;
         } // case

         case 'D':
         {
            // Process with the rules of D.
            RUL_SRCH(9,1);
            break;
         } // case

         case 'E':
         {
            // Process with the rules of E.
            RUL_SRCH(10,8);
            break;
         } // case

         case 'F':
         {
            // Process with the rules of F.
            RUL_SRCH(18,1);
            break;
         } // case

         case 'G':
         {
            // Process with the rules of G.
            RUL_SRCH(19,1);
            break;
         } // case

         case 'H':
         {
            // Process with the rules of H.
            RUL_SRCH(20,2);
            break;
         } // case

         case 'I':
         {
            // Process with the rules of I.
            RUL_SRCH(22,4);
            break;
         } // case

         case 'J':
         {
            // Process with the rules of J.
            RUL_SRCH(26,1);
            break;
         } // case

         case 'K':
         {
            // Process with the rules of K.
            RUL_SRCH(27,1);
            break;
         } // case

         case 'L':
         {
            // Process with the rules of L.
            RUL_SRCH(28,2);
            break;
         } // case

         case 'M':
         {
            // Process with the rules of M.
            RUL_SRCH(30,1);
            break;
         } // case

         case 'N':
         {
            // Process with the rules of N.
            RUL_SRCH(31,2);
            break;
         } // case

         case 'O':
         {
            // Process with the rules of O.
            RUL_SRCH(33,4);
            break;
         } // case

         case 'P':
         {
            // Process with the rules of P.
            RUL_SRCH(37,1);
            break;
         } // case

         case 'Q':
         {
            // Process with the rules of Q.
            RUL_SRCH(38,2);
            break;
         } // case

         case 'R':
         {
            // Process with the rules of R.
            RUL_SRCH(40,1);
            break;
         } // case

         case 'S':
         {
            // Process with the rules of S.
            RUL_SRCH(41,5);
            break;
         } // case

         case 'T':
         {
            // Process with the rules of T.
            RUL_SRCH(46,6);
            break;
         } // case

         case 'U':
         {
            // Process with the rules of U.
            RUL_SRCH(52,2);
            break;
         } // case

         case 'V':
         {
            // Process with the rules of V.
            RUL_SRCH(54,1);
            break;
         } // case

         case 'W':
         {
            // Process with the rules of W.
            RUL_SRCH(55,2);
            break;
         } // case

         case 'X':
         {
            // Process with the rules of X.
            RUL_SRCH(57,1);
            break;
         } // case

         case 'Y':
         {
            // Process with the rules of Y.
            RUL_SRCH(58,2);
            break;
         } // case

         case 'Z':
         {
            // Process with the rules of Z.
            RUL_SRCH(60,1);
            break;
         } // case

         default:
         {
            // Process as a digit or symbol.
            RUL_SRCH(61,15);
            break;
         } // case
      } // switch
   } // while

   // Set return values.  Note that references are being used.
   phonemCount = P_INDEX;
   phonemBuffer = P_BUFFER;

   return;

} // ENG_TO_PH

/************************************************************************

  Name: IS_ALPHA

  Purpose: The purpose of this function is to determine whether or
  not a character is alphabetic.

  Calling Sequence: result = IS_ALPHA(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is alphabetic, and a value of
    false indicates that the character is not alphabetic.

*****************************************************************************/
bool PhonemMaker::IS_ALPHA(char CH)
{
   bool result;

   // Leverage the standard library function.
   result = isalpha((int)CH);

   return (result);

} // IS_ALPHA

/************************************************************************

  Name: IS_VWL

  Purpose: The purpose of this function is to determine whether or
  not a character is a vowel.

  Calling Sequence: result = IS_VWL(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a vowel, and a value of
    false indicates that the character is not a vowel.

*****************************************************************************/
bool PhonemMaker::IS_VWL(char CH)
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

} // IS_VWL

/************************************************************************

  Name: IS_FR_VWL

  Purpose: The purpose of this function is to determine whether or
  not a character is a front vowel.

  Calling Sequence: result = IS_FR_VWL(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a front vowel, and a value of
    false indicates that the character is not a front vowel.

*****************************************************************************/
bool PhonemMaker::IS_FR_VWL(char CH)
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

} // IS_VWL

/************************************************************************

  Name: IS_CST

  Purpose: The purpose of this function is to determine whether or
  not a character is a consonant.

  Calling Sequence: result = IS_CST(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a consonant, and a value of
    false indicates that the character is not a consonant.

*****************************************************************************/
bool PhonemMaker::IS_CST(char CH)
{
   bool result;

   if (IS_ALPHA(CH) && (!IS_VWL(CH)))
   {
      result = true;
   } // if
   else
   {
      result = false;
   } // else

   return (result);

} // IS_CST

/************************************************************************

  Name: IS_VO_CST

  Purpose: The purpose of this function is to determine whether or
  not a character is a voiced consonant.

  Calling Sequence: result = IS_VO_CST(CH)

  Inputs:

    CH - The character to be evaluated.

  Outputs:

    result - A flag that indicates the result of the operation.  A value
    of true indicates that the character is a voiced consonant, and a
    value of false indicates that the character is not a voiced consonant.

*****************************************************************************/
bool PhonemMaker::IS_VO_CST(char CH)
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

} // IS_VO_CST

/************************************************************************

  Name: RT_PS_VWL

  Purpose: The purpose of this function is to advance the English
  buffer index to the right past all consecutive vowels in the English
  buffer.

  Calling Sequence: RT_PS_VWL(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.


  Globals:

    R_INDEX - The running index into the English buffer.

    E_LEN - The length of the English string.   

*****************************************************************************/
void PhonemMaker::RT_PS_VWL(int& R_INDEX, bool& OCCURED)
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
         if (IS_VWL(E_BUFFER[R_INDEX]))
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

} // RT_PS_VWL

/************************************************************************

  Name: RT_PS_CST

  Purpose: The purpose of this function is to advance the English
  buffer index to the right past all consecutive consonants in the
  English buffer.

  Calling Sequence: RT_PS_CST(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Globals:

    R_INDEX - The running index into the English buffer.

    E_LEN - The length of the English string.   

*****************************************************************************/
void PhonemMaker::RT_PS_CST(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

   if (R_INDEX < E_LEN)
   {
      // Set up for loop entry.
      DONE = false;

      while (!DONE)
      {
         if (!IS_CST(E_BUFFER[R_INDEX]))
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

} // RT_PS_CST

/************************************************************************

  Name: LF_PS_VWL

  Purpose: The purpose of this function is to advance the English
  buffer index to the left past all consecutive vowels in the English
  buffer.

  Calling Sequence: LF_PS_VWL(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a vowel was
    detected, and a value of false indicates that a vowel was not
    detected.

  Outputs:

    None.

  Globals:

    R_INDEX - The running index into the English buffer.

    E_BUFFER - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::LF_PS_VWL(int& R_INDEX, bool& OCCURED)
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
         if (IS_VWL(E_BUFFER[R_INDEX]))
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

} // LF_PS_VWL

/************************************************************************

  Name: LF_PS_CST

  Purpose: The purpose of this function is to advance the English
  buffer index to the left past all consecutive consonants in the
  English buffer.

  Calling Sequence: LF_PS_CST(R_INDEX,OCCURED)

  Inputs:

    R_INDEX - A reference to storage for the index into the English
    buffer.

    OCCURRED - A reference to storage that indicates whether or not
    a vowel was detected.  A value of true indicates that a consonant was
    detected, and a value of false indicates that a consonant was not
    detected.

  Outputs:

    None.

  Globals:

    R_INDEX - The running index into the English buffer.

    E_BUFFER - The buffer of English text that is to be evaluated.  

*****************************************************************************/
void PhonemMaker::LF_PS_CST(int& R_INDEX, bool& OCCURED)
{
   bool DONE;

   if (R_INDEX >= 0)
   {
      // Set up for loop entry.
      DONE = false;

      while (!DONE)
      {
         if (!IS_CST(E_BUFFER[R_INDEX]))
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

} // LF_PS_CST

/************************************************************************

  Name: BLD_LIT_P

  Purpose: The purpose of this function is to advance the extract the
  phonem string on the right side of the equation for a particular
  rule that is being processed.
  Calling Sequence: BLD_LIT_P(int RUL_INDX)

  Inputs:

    RUL_INDX - An index into the curent rule for which a phonem string
    is to be extracted.

  Outputs:

    None.

  Globals:

    R_BUFFER - A string that contains the current rule being evaluated.

    PH_STR = The current phonem string being constructed.

*****************************************************************************/
void PhonemMaker::BLD_LIT_P(int RUL_INDX)
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

} // BLD_LIT_P

/************************************************************************

  Name: SC_RT_CTX

  Purpose: The purpose of this function is to scan and evaluate the
  right-hand context of text to be evaluated.

  Calling Sequence: SC_RT_CTX(RT_INDX,RUL_INDX,FOUND)

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

  Globals:

    R_BUFFER - A string that contains the current rule being evaluated.

    RUL_INDX - An index into the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being evaluated.

*****************************************************************************/
void PhonemMaker::SC_RT_CTX(int RT_INDX, int& RUL_INDX, bool& FOUND)
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
                  WI_RULES = !IS_ALPHA(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '#':
               {
                  RT_PS_VWL(R_INDEX,OCCURED);
                  break;
               } // case

               case ':':
               {
                  RT_PS_CST(R_INDEX,OCCURED);
                  break;
               } // case


               case '+':
               {
                  WI_RULES = IS_FR_VWL(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '$':
               {
                  WI_RULES = IS_CST(E_BUFFER[R_INDEX]);
                  break;
               } // case

               case '.':
               {
                  WI_RULES = IS_VO_CST(E_BUFFER[R_INDEX]);
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

} // SC_RT_CTX

/************************************************************************

  Name: SC_LF_CTX

  Purpose: The purpose of this function is to scan and evaluate the
  left-hand context of text to be evaluated.

  Calling Sequence: SC_LF_CTX(LEF_INDX,FOUND)

  Inputs:

    LEF_INDX - An index to storage to the current rule.

    FOUND - A reference to storage for a flag that indicates that a
    left-context rule was satisfied.  A value of true indicates that
    it was not satisfied, and a value of false indicates that it was
    not satisfied.

  Outputs:

    None.

  Globals:

    R_BUFFER - A string that contains the current rule being evaluated.

    RUL_INDX - An index into the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

*****************************************************************************/
void PhonemMaker::SC_LF_CTX(int LEF_INDX, bool& FOUND)
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
               WI_RULES = !IS_ALPHA(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case '#':
            {
               LF_PS_VWL(R_INDEX,OCCURED);
               break;
            }  // case

            case ':':
            {
               LF_PS_CST(R_INDEX,OCCURED);
               break;
            }  // case

            case '+':
            {
               WI_RULES = IS_FR_VWL(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case '$':
            {
               WI_RULES = IS_CST(E_BUFFER[R_INDEX]);
               break;
            }  // case

            case'.':
            {
               WI_RULES = IS_VO_CST(E_BUFFER[R_INDEX]);
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

} // SC_LF_CTX

/************************************************************************

  Name: CMP_REF_S

  Purpose: The purpose of this function is to scan and evaluate the
  left-hand context of text to be evaluated.

  Calling Sequence: result = CMP_REF_S()

  Inputs:

    None.

  Outputs:

    result - A flag that indicates the result of the comparison.  A
    value of true indicates that the reference string matched to the
    current position in the English buffer, and a value of false
    indicates that a match was not found.

  Globals:

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being evaluated.

*****************************************************************************/
bool PhonemMaker::CMP_REF_S(void)
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

} // CMP_REF_S

/************************************************************************

  Name: BLD_REF_S

  Purpose: The purpose of this function is to extract the reference
  string from a rule.  The reference string is the text betweeh the
  parantheses.  For ecample: (AB) implies that the reference string
  is "AB".

  Calling Sequence: BLD_REF_S(LEF_INDX,RT_INDX)

  Inputs:

    LEF_INDX - An index that indicates the position of the left
    '('.

    RT_INDX - A reference to storage for the right-hand index into
    the current rule entry.

  Outputs:

    None.

  Globals:

    R_BUFFER - A string that contains the current rule being evaluated.

    REF_STR - The current reference string being constructed.

*****************************************************************************/
void PhonemMaker::BLD_REF_S(int LEF_INDX, int& RT_INDX)
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

} // BLD_REF_S

/************************************************************************

  Name: FI_LF_PAR

  Purpose: The purpose of this function is to scan the current rule
  until the left-hand '('. is found.

  Calling Sequence: FI_LF_PAR(LEF_INDX)

  Inputs:

    LEF_INDX - A reference to storage for the index of the left '('.

   Outputs:

    None.

  Globals:

    R_BUFFER - A string that contains the current rule being evaluated.

*****************************************************************************/
void PhonemMaker::FI_LF_PAR(int& LEF_INDX)
{

   // Point to beginning of buffer.
   LEF_INDX = 0;

   while (R_BUFFER[LEF_INDX] != '(')
   {
      // Bump left parent index.
      LEF_INDX = LEF_INDX + 1;
   } // while

   return;

} // FI_LF_PAR

/************************************************************************

  Name: SCAN

  Purpose: The purpose of this function is to evaluate the current
  position of the English text when compared to the current rule.
  Both left and right contexts will be evaluated (if they exist), If
  the current rule is satisfied, more phonems will be added to the
  phonem stream.

  Calling Sequence: result = SCAN()

  Inputs:

    None.

   Outputs:

    result - A flag that indicates whether or not the rule was satisfied.
    A value of true indicates that the rule was satisfied, and a value of
    false indicates that the rule was not satisfied.

  Globals:

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

    REF_STR - The current reference string being evaluated.

*****************************************************************************/
bool PhonemMaker::SCAN(void)
{
   bool result;
   int LEF_INDX;
   int RT_INDX;
   int RUL_INDX;
   bool WI_RULES;

   // Clear initially.
   WI_RULES = false;

   // Find left parent.
   FI_LF_PAR(LEF_INDX);

   // Build reference string, and return right parent.
   BLD_REF_S(LEF_INDX,RT_INDX);

   // Compare reference string to English.
   if (CMP_REF_S())
   {
      if (LEF_INDX > 0)
      {
         // Scan left context.
         SC_LF_CTX(LEF_INDX,WI_RULES);

         // If there is a left context.
         if (WI_RULES)
         {
            // Scan right context.
            SC_RT_CTX(RT_INDX,RUL_INDX,WI_RULES);
         } // if
      } // if
      else
      {
         // Scan right context.
         SC_RT_CTX(RT_INDX,RUL_INDX,WI_RULES);
      } // else
   } // if

   if (WI_RULES)
   {
      // Build literal phonem string.
      BLD_LIT_P(RUL_INDX);

      // Convert to phonem codes.
      PH_TO_COD();

      // Bump English index.
      E_INDEX = E_INDEX + REF_STR.length();
   } // if

   // Set return value based upon result.  Yes, it's redundant.
   result  = WI_RULES;

   return (result);

} // SCAN

/************************************************************************

  Name: RUL_SRC

  Purpose: The purpose of this function is to scan the rules of a
  letter in the English text and generate a phonem stream.  This text
  (extracted from the original code follows.

  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  RULE FORMAT - #(SED)!=Z,D;

    # - LEFT CONTEXT
    ( - LEFT CONTEX DELIMITER
    SED - STRING TO BE MATCHED
    ) - RIGHT CONTEXT DELIMITER
    ! - RIGHT CONTEX
    = - RIGHT CONTEX TERMINATOR
    Z - PHONEM CODE
    , - PHONEM SEPARATOR
    D - PHONEM CODE
     ; - RULE TERMINATOR

    MEANING OF RULE SYMBOLS

      ! - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH ANY NONALPHABETIC
          CHARACTER IN ENGLISH INPUT STRING. IF MATCH FAILS, REPORTS
          FAILURE. IF MATCH SUCCEEDS, MOVES RULE-STRIG POINTER BY
          ONE CHARACTER IN RULE AND MOVES INPUT STRING POINTER BY
          ONE CHARACTER IN THE ENGLISH STRING. THE DIRECTION OF
          MOVEMENT IS DETERMINED AS:
            1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
            2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

        # - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH ONE OR MORE
            VOWELS (A,E,I,O,U, OR Y). IF MATCH FAILS, REPORTS FAILURE.
            IF MATCH SUCCEEDS, MOVES RULE POINTER BY ONE CHARACTER IN
            RULES AND MOVES STRING POINTER BY THE NUMBER OF VOWELS
            MATCHED IN ENGLISH STRING. THE DIRECTION OF MOVEMENT IS
            DETERMINED AS:
              1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
              2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

        : - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH ZERO OR MORE
            CONSONANTS. MATCH ALWAYS SUCCEEDS. MOVES RULE POINTER BY
            ONE CHARACTER IN RULES AND MOVES STRING POINTER BY THE
            NUMBER OF CONSONANTS MATCHED IN ENGLISH INPUT STRING. THE
            DIRECTION OF MOVEMENT IS DETERMINED AS:
              1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
              2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

        + - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH A FRONT VOWEL
            (E,I, OR Y). IF MATCH FAILS, REPORTS FAILURE. IF MATCH
            SUCCEEDS, MOVES RULE POINTER BY ONE CHARACTER IN RULES
            AND MOVES STRING POINTER BY ONE CHARACTER IN INPUT STRING.
            DIRECTION OF MOVEMENT IS DETERMINED AS:
              1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
              2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

        $ - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH ONE CONSONANT. IF
            MATCH FAILS, REPORTS FAILURE. IF MATCH SUCCEEDS, MOVES RULE
            POINTER BY ONE CHARACTER IN RULES AND MOVES STRING POINTER
            BY ONE CHARACTER IN ENGLISH INPUT STRING. DIRECTION OF MOVEMENT
            IS DETERMINED AS:
              1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
              2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

        . - INVOKES PROCEDURE THAT ATTEMPTS TO MATCH A VOICED CONSONANT
            (B,D,G,J,L,M,N,R,V,W, OR Z). IF MATCH FAILS, REPORTS FAILURE.
            IF MATCH SUCCEEDS, MOVES RULE POINTER BY ONE CHARACTER IN RULES
            AND MOVES STRING POINTER BY ONE CHARACTER IN ENGLISH INPUT
            STRING. DIRECTION OF MOVEMENT IS DETERMINED AS:
              1. FORWARD IF RIGHT CONTEXT IS BEING SCANNED
              2. BACKWARD IF LEFT CONTEXT IS BEING SCANNED

  AFTER A MATCH IS FOUND THE INDEX INTO THE ENGLISH BUFFER IS INCREMENTED
  BY THE NUMBER OF CHARACTERS INSIDE OF THE PARENTHESIS OF THE RULE.
  /_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  Calling Sequence: RUL_SRCH(int BLK_OFF, int BLK_SIZ)

  Inputs:

    BLK_OFF - The starting index into the rules to be evaluated.

    BLK_SIZ - The number of rules to evaluate.

   Outputs:

    None.

  Globals:

    RUL_TBL - A table of rule strings for which English text os to be
    evaluated.

    R_BUFFER - A string that contains the current rule being evaluated.

    E_INDEX - The current index into the English buffer for which text
    is to be evaluated.

*****************************************************************************/
void PhonemMaker::RUL_SRCH(int BLK_OFF, int BLK_SIZ)
{
   int U_BOUNDS;
   int BLK_INDX;
   bool DONE;
   bool FOUND;

   // Set upper bounds.
   U_BOUNDS = BLK_OFF + BLK_SIZ-1;

   // Set lower bounds.
   BLK_INDX = BLK_OFF;

   // Set up for loop entry.
   DONE = false;

   while (!DONE)
   {
      // Get current rule.
      R_BUFFER = RUL_TBL[BLK_INDX];

      // Scan using current rule.
      FOUND = SCAN();

      // bump to next rule.
      BLK_INDX = BLK_INDX + 1;

      if ((BLK_INDX > U_BOUNDS) || FOUND)
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

} // RUL_SRCH

/************************************************************************

  Name: STR_T_COD

  Purpose: The purpose of this function is to convert a string
  representation of a phonem into it's binary code.  The code will
  be added to the phonem buffer.

  Calling Sequence: STR_T_COD(PH_STR)

  Inputs:

    PH_STR - A string representation of a phonem for which its binary
    equivalent is to be created.

   Outputs:

    None.

  Globals:P_INDEX

    PHO_TBL - A table that maps the textual representation of phonems to
    the binary representation.

    P_INDEX - The index into the next storage location in the phonem
    buffer. 

    P_BUFFER - The buffer where the phonem stream is stored.   

*****************************************************************************/
void PhonemMaker::STR_T_COD(std::string PH_STR)
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

} // STR_T_COD

/************************************************************************

  Name: STR_T_COD

  Purpose: The purpose of this function is to convert an array of
  string
  representations of a phonems into it's binary codes.  The code stream
  will be placed in the phonem buffer.

  Calling Sequence: PH_TO_COD()

  Inputs:

    None.

   Outputs:

    None.

    PHO_TBL - A table that maps the textual representation of phonems to
    the binary representation.

    PH_STR - A string representation of the phonem being evaluated.

*****************************************************************************/
void PhonemMaker::PH_TO_COD(void)
{
   int INDEX;

   // Point to beginning of array.
   INDEX = 0;

   while (PH_STR[INDEX] != ";")
   {
      // Convert phonems to codes.
      STR_T_COD(PH_STR[INDEX]);

      // Reference next entry.
      INDEX = INDEX + 1;
   } // while

  return;

} // PH_TO_COD
