#include "ArcCable.h"

ArcCable::ArcCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TArco> arc ) :
	ElectricalElement( drawing ), 
	_arc( arc )
{
} 