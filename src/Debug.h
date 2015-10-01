#ifndef __DEBUG_H
#define __DEBUG_H

#include <vector>
#include <memory>
#include <string>>

struct TVerticesGerais;
struct TVerticeGeral;
struct TAresta;
class Graph;

class Debug
{
public:
	static void printVertices( std::shared_ptr<TVerticesGerais> _verticesGerais, std::string fileName );
	static void printGraph( std::shared_ptr<Graph> _graph, std::string fileName );
	static void generateDOTGraph( std::vector<std::shared_ptr<TVerticeGeral> >& vertices, std::string fileName );
	static void printEdges( std::string source, std::string dest, const std::vector< std::shared_ptr<TAresta> > &edgesList );
	static std::string _projPath;
};

#endif