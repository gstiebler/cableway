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


void Debug::generateDOTGraph( vector<shared_ptr<TVerticeGeral> >& vertices, string fileName )
{
	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

	out << "graph G\n";
	out << "{\n";
	char node_format[] = "node_%09d";

	for( int i(0); i < vertices.size(); ++i )
	{
		out << "\t";
		char temp[256];
		sprintf( temp, node_format, vertices[i]->_autogenId );
		out << QString::fromUtf8( temp );
		string label = vertices[i]->texto;
		if( label.empty() )
			label = "n";
		sprintf( temp, "[ label = \"%s\" ", label.c_str() );
		out << QString::fromUtf8( temp );
		if( vertices[i]->drawing.get() )
			out << " drawing=\"" << vertices[i]->drawing->NomeArquivo.c_str() << "\"";
		out << " ]\n";
	}
	out << "\n";

	for( int i(0); i < vertices.size(); ++i )
	{
		char nodeStr[256];
		sprintf( nodeStr, node_format, vertices[i]->_autogenId );
		shared_ptr<TListaVerticesEArestas> verticesEArestas = vertices[i]->ListaVerticesEArestas;
		for( int j(0); j < verticesEArestas->list.size(); ++j)
		{
			out << "\t";
			TVerticeEAresta& verticeEAresta = verticesEArestas->list[j];
			char nodeEdgeStr[256];
			sprintf( nodeEdgeStr, node_format, verticeEAresta.Vertice->_autogenId );
			out << nodeStr << " -- " << nodeEdgeStr;

			//out << " [ label = \"" << verticeEAresta.Aresta->Tam << "\"]";
			out << "\n";
		}
	}
	out << "}";
    file.close(); 
}