
#include "CsvReader.h"
#include <QFile>
#include <QTextStream>

using namespace std;

void CsvReader::readCsv( string fileName, vector< vector< string > > &values )
{
	QFile inputFile( QString::fromStdString( fileName ) );
	if ( inputFile.open( QIODevice::ReadOnly ) )
	{
	   QTextStream in(&inputFile);
	   while (!in.atEnd())
	   {
		  QString line = in.readLine();
		  QStringList list = line.split( ";" );
		  
		  vector< string > strings;
		  for ( int i(0); i < list.size(); ++i)
			  strings.push_back( list[i].toStdString() );

		  values.push_back( strings );
	   }
	   inputFile.close();
	}
}