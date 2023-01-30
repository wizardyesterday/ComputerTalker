#include <stdio.h>
#include <string>

char buffer[128];

int main(int argc,char **argv)
{
  char *myPtr;
  char key;

  fgets(buffer,80,stdin);

  myPtr = index(buffer,'(');

  if (myPtr != NULL)
  {
    key = myPtr[1];
    printf("key: %c\n",key);
  } // if
  else
  {
    printf("( not found\n");
  } // else

  return 0;

} // main

