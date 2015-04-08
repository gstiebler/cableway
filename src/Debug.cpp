#include "Debug.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

#include "UVerticesArestas.h"

void Debug::printVertices( shared_ptr<TVerticesGerais> _verticesGerais, string fileName )
{
	vector< shared_ptr<TVerticeGeral> > sortedVertices;
	_verticesGerais->ListaOrd( sortedVertices );
	
	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
	for( int i(0); i < sortedVertices.size(); ++i )
	{
		if( sortedVertices[i]->texto.empty() )
			continue;

		out << QString::fromUtf8( sortedVertices[i]->texto.c_str() ) << "\n";
	}
 
    file.close(); 
}