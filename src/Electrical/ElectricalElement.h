#ifndef ELECTRICAL_ELEMENT_H__
#define ELECTRICAL_ELEMENT_H__

#include <memory>
struct TDesenho;
class Graph;

class ElectricalElement
{
public:
    ElectricalElement( std::shared_ptr<Graph> graph, std::shared_ptr<TDesenho> drawing );
    
protected:
	std::shared_ptr<Graph> _graph;
    std::shared_ptr<TDesenho> _drawing;
};

#endif;