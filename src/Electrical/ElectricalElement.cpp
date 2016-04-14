#include "ElectricalElement.h"

ElectricalElement::ElectricalElement( std::shared_ptr<Graph> graph, std::shared_ptr<TDesenho> drawing ) :
	_graph( graph ), 
    _drawing( drawing )
{
}