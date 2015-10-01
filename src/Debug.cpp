#include "Debug.h"

#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <set>

#include "UVerticesArestas.h"
#include "TDesenho.h"
#include "Graph.h"

using namespace std;

string Debug::_projPath;

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
	char node_format[] = "node_%09d";

	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

	out << "graph G\n";
	out << "{\n";
	out << "\tlabel = \"root\"\n";

	map< string, vector< shared_ptr<TVerticeGeral> > > drawingsVertices;

	for( int i(0); i < vertices.size(); ++i )
	{
		if( vertices[i]->drawing.get() )
			drawingsVertices[ vertices[i]->drawing->NomeArquivo ].push_back( vertices[i] );
	}

	map< string, vector< shared_ptr<TVerticeGeral> > >::iterator it, e = drawingsVertices.end();
	for( it = drawingsVertices.begin(); it != e; ++it )
	{
		vector< shared_ptr<TVerticeGeral> > &drawingVertices = it->second;
		out << "subgraph \"" << it->first.c_str() << "\" {\n";
		out << "\tlabel = \"" << it->first.c_str() << "\";\n";
		out << "\tparent = \"G\";\n";
		for( int i(0); i < drawingVertices.size(); ++i )
		{
			shared_ptr<TVerticeGeral> vertex = drawingVertices[i];
			out << "\t";
			char temp[256];
			sprintf( temp, node_format, vertex->_autogenId );
			out << QString::fromUtf8( temp );
			string label = vertex->texto;
			if( label.empty() )
				label = "n";
			sprintf( temp, "[ label = \"%s\" ", label.c_str() );
			out << QString::fromUtf8( temp );
			out << " ]\n";
		}
		out << "}\n";
	}

	set<TAresta*> printedEdges;
	for( int i(0); i < vertices.size(); ++i )
	{
		char nodeStr[256];
		sprintf( nodeStr, node_format, vertices[i]->_autogenId );
		shared_ptr<TListaVerticesEArestas> verticesEArestas = vertices[i]->ListaVerticesEArestas;
		for( int j(0); j < verticesEArestas->list.size(); ++j)
		{
			TVerticeEAresta& verticeEAresta = verticesEArestas->list[j];
			if( printedEdges.find( verticeEAresta.Aresta.get() ) != printedEdges.end() )
				continue;
			out << "\t";
			char nodeEdgeStr[256];
			sprintf( nodeEdgeStr, node_format, verticeEAresta.Vertice->_autogenId );
			out << nodeStr << " -- " << nodeEdgeStr;

			//out << " [ label = \"" << verticeEAresta.Aresta->Tam << "\"]";
			out << "\n";

			printedEdges.insert( verticeEAresta.Aresta.get() );
		}
	}
	out << "}";
    file.close(); 
}


void Debug::printEdges( std::string source, std::string dest, const std::vector< shared_ptr<TAresta> > &edgesList )
{	
#ifdef NDEBUG
	return;
#endif

	string fileName = _projPath + "/debug/" + source + "_" + dest + "_" + "arestas.txt";
	QFile file( QString::fromLatin1( fileName.c_str() ) );
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
	out << "Circuito: " << QString::fromUtf8( source.c_str() ) << "/" << QString::fromUtf8( dest.c_str() );
	for( auto edge : edgesList )
	{
		out << "Tamanho: " << edge->Tam << "\n";
		for( auto vertex : edge->_vertices )
		{
			out << "  Texto: " << QString::fromUtf8( vertex->texto.c_str() ) << "\n";
			out << "  Desenho: " << QString::fromUtf8( vertex->drawing->NomeArquivo.c_str() ) << "\n";
			out << "  Pos: " << vertex->pos.x << ", " << vertex->pos.y << "\n";
		}
		out << "\n";
	}
 
    file.close(); 
}