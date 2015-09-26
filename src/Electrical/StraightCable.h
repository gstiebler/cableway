#ifndef STRAIGHT_CABLE_H__
#define STRAIGHT_CABLE_H__

#include <memory>
#include "ElectricalElement.h"
#include "GeometricEdges.h"
#include "EdgeConnector.h"

struct TMultipoint;

class StraightCable : public ElectricalElement, public GeometricEdges, public EdgeConnector
{
public:
    StraightCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TMultipoint> multipoint );
	std::shared_ptr<TMultipoint> _multipoint;

private:
	void connectEdges( std::vector< std::shared_ptr<TVerticeGeral> >& edges );
};

#endif