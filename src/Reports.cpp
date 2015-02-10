
#include "Reports.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

using namespace std;

void Reports::generateCirtuisReport( std::string fileName, const std::vector<InputCircuit> &inputCircuits, const std::vector<CircuitResult> &resultCircuits )
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