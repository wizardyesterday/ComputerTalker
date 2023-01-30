#include <stdio.h>
#include <map>
#include <list>
#include <string>

using namespace std;

char alphabet [] =
{'A','B','C','D','E','F','G','H','I'};

void blah(char key,std::list <string> theList)
{
  list <string>::iterator i;

  for (i = theList.begin(); i != theList.end(); i++)
  {
    printf("%c  %s\n",key,i->c_str());
  } // for   

  return;

} // blah

int main(int argc,char **argv)
{
  map <char,list <string> > myMap;
  map <char, list <string> >::iterator i;

  myMap['A'].push_back("Al");
  myMap['A'].push_back("Aluch");
  myMap['A'].push_back("Abel");
  myMap['A'].push_back("Ahoman");
  myMap['A'].push_back("Asota");

  myMap['B'].push_back("Benny");
  myMap['B'].push_back("Bill");
  myMap['B'].push_back("Bonita");

  myMap['Q'].push_back("Quora");
  myMap['Q'].push_back("Quanito");

  myMap['C'].push_back("Cathead");
  myMap['C'].push_back("Charlie");
  myMap['C'].push_back("Contingo");
  myMap['C'].push_back("Coopa");
 
  printf("\n");

  for (i = myMap.begin(); i != myMap.end(); i++)
  {
    blah(i->first,i->second);
  } // for

  myMap['J'].push_back("Jammer");

  i = myMap.find('J');

  if (i != myMap.end())
  {
    blah(i->first,i->second);
  } // if

  for (i = myMap.begin(); i != myMap.end(); i++)
  {
    blah(i->first,i->second);
  } // for

  printf("Map size: %d\n",myMap.size());

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Release resources.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  for (i = myMap.begin(); i != myMap.end(); i++)
  {
    // Clear the list entry.
    i->second.clear();
  }

  // Clear the map.
  myMap.clear();
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  printf("Map size after clear: %d\n",myMap.size());

  return 0;

} // main

