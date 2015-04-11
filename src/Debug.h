#ifndef __DEBUG_H
#define __DEBUG_H

#include <vector>
#include <memory>

struct TVerticesGerais;
struct TVerticeGeral;
class Graph;

class Debug
{
public:
	static void printVertices( std::shared_ptr<TVerticesGerais> _verticesGerais, std::string fileName );
	static void printGraph( std::shared_ptr<Graph> _graph, std::string fileName );
	static void generateDOTGraph( std::vector<std::shared_ptr<TVerticeGeral> >& vertices, std::string fileName );
};

#endif