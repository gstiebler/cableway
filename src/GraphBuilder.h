#ifndef GRAPH_BUILDER_H__
#define GRAPH_BUILDER_H__

#include <memory>

class Graph;
class ElectricalElements;

class GraphBuilder
{
public:
	static void build( std::shared_ptr<Graph> graph, std::shared_ptr<ElectricalElements> electricalElements );
};

#endif