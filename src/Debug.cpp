#include "Debug.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>

#include "UVerticesArestas.h"
#include "TDesenho.h"
#include "Graph.h"

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

		out << QString::fromUtf8( sortedVertices[i]->texto.c_str() ) << ";";
		out << sortedVertices[i]->pos.x << ";";
		out << sortedVertices[i]->pos.y << ";";
		out << QString::fromUtf8( sortedVertices[i]->drawing->NomeArquivo.c_str() ) << "\n";
	}
 
    file.close(); 
}

void Debug::printGraph( shared_ptr<Graph> _graph, string fileName )
{
	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
	for( int i(0); i < _graph->_verticesGerais->vertices.size(); ++i )
	{
		shared_ptr<TListaVerticesEArestas> verticesEArestas = _graph->_verticesGerais->vertices[i]->ListaVerticesEArestas;
		out << "Vertex id: " << _graph->_verticesGerais->vertices[i]->_autogenId;
		out << ";" << QString::fromUtf8( _graph->_verticesGerais->vertices[i]->texto.c_str() );
		if( _graph->_verticesGerais->vertices[i]->drawing )
			out << ";" << QString::fromUtf8( _graph->_verticesGerais->vertices[i]->drawing->NomeArquivo.c_str() ) << "\n";
		else
			out << ";\n";
		for( int j(0); j < verticesEArestas->list.size(); ++j)
		{
			out << "  " << verticesEArestas->list[j].Vertice->_autogenId;
			out << ";" << QString::fromUtf8( verticesEArestas->list[j].Vertice->texto.c_str() );
			out << ";" << QString::fromUtf8( verticesEArestas->list[j].Aresta->_layer.c_str() );
			if( verticesEArestas->list[j].Vertice->drawing )
				out << ";" << QString::fromUtf8( verticesEArestas->list[j].Vertice->drawing->NomeArquivo.c_str() ) << "\n";
			else
				out << ";\n";
		}
	}
    file.close(); 
}