#ifndef STRAIGHT_CABLE_H__
#define STRAIGHT_CABLE_H__

#include <memory>
#include "ElectricalElement.h"

struct TMultipoint;

class StraightCable : public ElectricalElement
{
public:
    StraightCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TMultipoint> multipoint );
	std::shared_ptr<TMultipoint> _multipoint;
};

#endif