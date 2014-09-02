//---------------------------------------------------------------------------

#ifndef UAuxStringH
#define UAuxStringH
//---------------------------------------------------------------------------

#include <string.h>
#include <sstream>
#include <algorithm> 
#include <stdio.h>
using namespace std;

template <class T>
inline string to_string (const T& t)
{
  stringstream ss;
  ss << t;
  return ss.str();
}
inline int to_int (string s)
{
  stringstream ss;
  ss << s;
  int temp;
  ss >> temp;
  return temp;
}
inline void string_recebe_wchar(string& dest, const wchar_t* source, int tam)
{
  char* dummy = new char[tam+1];
  memset(dummy,0,tam+1);
  wcstombs(dummy, source, tam);
  dest = dummy;
  delete [] dummy;
}
inline void string_replace(string& dest, string search, string replace)
{
  string dest_down;//, search_down;
  dest_down = "";
//  search_down = "";
  for ( int i = 0 ; i < (int)dest.size() ; i++ )
    dest_down += tolower(dest[i]);

  for ( int i = 0 ; i < (int)search.size() ; i++)
    search[i] = tolower(search[i]);
//  transform(dest.begin(), dest.end(), dest_down.begin(), tolower);
//  transform(search.begin(), search.end(), search_down.begin(), tolower);

  size_t found, rplsize;

  found = dest_down.find(search);
  if ( found == string::npos )
    return;

  rplsize = replace.size();
  dest.replace(found, rplsize, replace);
}


string stringTrim(string& str);
bool ordenaStrings(string str, string str2);



#endif
