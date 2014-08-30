//---------------------------------------------------------------------------


#pragma hdrstop

#include "UAuxString.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

string stringTrim(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  return str;
}

bool ordenaStrings(string str, string str2)
{
  if ( strcmpi(str.c_str(), str2.c_str()) < 0 )
    return true;

  return false;
}
//---------------------------------------------------------------------------