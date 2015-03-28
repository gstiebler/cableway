#ifndef __REPORTS_H
#define __REPORTS_H

#include <string>
#include <vector>

class CsvReader
{
public:
	static void readCsv( std::string fileName, std::vector< std::vector< std::string > > &values );
};

#endif