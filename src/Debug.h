#ifndef __DEBUG_H
#define __DEBUG_H

#include <vector>
#include <memory>

struct TVerticesGerais;

class Debug
{
public:
	static void printVertices( std::shared_ptr<TVerticesGerais> _verticesGerais, std::string fileName );
};

#endif