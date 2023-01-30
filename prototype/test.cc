#include <stdio.h>
#include <map>
#include <list>
#include <string>

using namespace std;

void blah(std::list <string> theList)
{
  list <string>::iterator i;

  for (i = theList.begin(); i != theList.end(); i++)
  {
    printf("List A: %s\n",i->c_str());
  } // for   

  return;

} // blah

int main(int argc,char **argv)
{
  list <string> aList;
  list <string> bList; 
  list <string> cList; 
  map <char,list <string> > myMap;
  map <char, list <string> >::iterator i;

  aList.push_back("bird1");
  aList.push_back("bird2");
  aList.push_back("bird3");

  bList.push_back("bull1");
  bList.push_back("bull2");
  bList.push_back("bull3");

  cList.push_back("cat1");
  cList.push_back("cat2");
  cList.push_back("cat2");

  myMap['A'] = aList;
  myMap['B'] = bList;
  myMap['C'] = cList;

  for (i = myMap.begin(); i != myMap.end(); i++)
  {
    blah(i->second);
  } // for


  return 0;

} // main

