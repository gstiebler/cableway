#ifndef STRAIGHT_CABLE_H__
#define STRAIGHT_CABLE_H__

#include <memory>
#include "ElectricalElement.h"
#include "GeometricEdges.h"

struct TMultipoint;

class StraightCable : public ElectricalElement, public GeometricEdges
{
public:
    StraightCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TMultipoint> multipoint );
	std::shared_ptr<TMultipoint> _multipoint;
};

#endif