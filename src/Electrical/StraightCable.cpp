#include "StraightCable.h"

StraightCable::StraightCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TMultipoint> multipoint ) :
	ElectricalElement( drawing ), 
	_multipoint( multipoint )
{
}