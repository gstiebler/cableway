
#include "CsvReader.h"
#include <QString>
#include <QStringList>
#include <fstream>

using namespace std;

void CsvReader::readCsv( string fileName, vector< vector< string > > &values )
{
	std::ifstream file( fileName );
    std::string str; 
    while (std::getline(file, str))
    {		  
		QString line = QString::fromLatin1( str.c_str() );
		  QStringList list = line.split( ";" );
		  
		  vector< string > strings;
		  for ( int i(0); i < list.size(); ++i)
			  strings.push_back( list[i].toStdString() );

		  values.push_back( strings );
    }
}