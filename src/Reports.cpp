
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
		
		char strLength[500];
		sprintf( strLength, "%.2lf", resultCircuit.length );

		string line;
		line = inputCircuit.name + ";";
		line += inputCircuit.source + ";";
		line += inputCircuit.dest + ";";
		line += inputCircuit.getFormatedRoute() + ";";
		line += inputCircuit.cable + ";";
		line += string(strLength) + ";";
		line += InputCircuit::getFormatedRoute( resultCircuit.route ) + ";";
		line += QString::fromLatin1( (resultCircuit.errorMessage + ";\n").c_str() ).toStdString();

		out << QString::fromUtf8( line.c_str() );
	}
 
    file.close(); 
}


void Reports::generateBandeirolaReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits )
{
	map<string, vector<InputCircuit> > circuitsOfBandeirola;
	for( int i(0); i < inputCircuits.size(); ++i )
	{
		const InputCircuit &inputCircuit = inputCircuits[i];
		const CircuitResult &resultCircuit = resultCircuits[i];
		
		if ( resultCircuit.route.size() < 1 )
			continue;

		for(int j(1); j < resultCircuit.route.size() - 1; ++j)
			circuitsOfBandeirola[ resultCircuit.route[j] ].push_back( inputCircuit );
	}

	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
 
	map<string, vector<InputCircuit> >::iterator it, e = circuitsOfBandeirola.end();
	for( it = circuitsOfBandeirola.begin(); it != e; it++)
	{
		string line = "Bandeirola: " + it->first + ";\n";
		out << QString::fromLatin1( line.c_str() );

		vector<InputCircuit> &circuits = it->second;
		for(int i(0); i < circuits.size(); ++i)
		{
			out << QString::fromUtf8( circuits[i].name.c_str() ) + ";";
			out << QString::fromUtf8( circuits[i].cable.c_str() ) + ";\n";
		}
	
		out << "\n";
	}

    file.close(); 
}


void Reports::generateCableReport( string fileName, const vector<InputCircuit> &inputCircuits, const vector<CircuitResult> &resultCircuits )
{
	map<string, double> cablesLength;

	for( int i(0); i < inputCircuits.size(); ++i )
	{
		const InputCircuit &inputCircuit = inputCircuits[i];
		const CircuitResult &resultCircuit = resultCircuits[i];

		cablesLength[inputCircuit.cable] += resultCircuit.length;
	}

	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
 
	map<string, double >::iterator it, e = cablesLength.end();
	for( it = cablesLength.begin(); it != e; it++)
	{
		out << QString::fromUtf8( it->first.c_str() ) + ";";
		out << QString::number( it->second, 'f', 2 ) << ";\n";
	}

    file.close(); 
}