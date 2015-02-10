
#include "Reports.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

using namespace std;

void Reports::generateCirtuitsReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits )
{
	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

	string header = "Circuito;Origem;Destino;RotaUsuario;Cabo;Comprimento;Rota;Erro;\n";
	out << QString::fromLatin1( header.c_str() );
	for( int i(0); i < inputCircuits.size(); ++i )
	{
		const InputCircuit &inputCircuit = inputCircuits[i];
		const CircuitResult &resultCircuit = resultCircuits[i];
		
		char strLength[50];
		sprintf( strLength, "%.2lf", resultCircuit.length );

		string line;
		line = inputCircuit.name + ";";
		line += inputCircuit.source + ";";
		line += inputCircuit.dest + ";";
		line += inputCircuit.getFormatedRoute() + ";";
		line += inputCircuit.cable + ";";
		line += string(strLength) + ";";
		line += InputCircuit::getFormatedRoute( resultCircuit.route ) + ";";
		line += resultCircuit.errorMessage + ";\n";

		out << QString::fromLatin1( line.c_str() );
	}
 
    file.close(); 
}



void Reports::generateBandeirolaReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits )
{
	map<string, vector<string> > circuitsOfBandeirola;
	for( int i(0); i < inputCircuits.size(); ++i )
	{
		const InputCircuit &inputCircuit = inputCircuits[i];
		const CircuitResult &resultCircuit = resultCircuits[i];
		
		for(int j(1); j < resultCircuit.route.size() - 1; ++j)
			circuitsOfBandeirola[ resultCircuit.route[j] ].push_back( inputCircuit.name );
	}

	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
 
	map<string, vector<string> >::iterator it, e = circuitsOfBandeirola.end();
	for( it = circuitsOfBandeirola.begin(); it != e; it++)
	{
		string line = "Bandeirola: " + it->first + ";\n";
		out << QString::fromLatin1( line.c_str() );

		vector<string> &circuits = it->second;
		for(int i(0); i < circuits.size(); ++i)
			out << QString::fromLatin1( circuits[i].c_str() );
			out << QString::fromLatin1( ";\n" );
	
		out << "\n";
	}

    file.close(); 
}