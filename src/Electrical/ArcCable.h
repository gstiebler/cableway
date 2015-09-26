#ifndef ARC_CABLE_H__
#define ARC_CABLE_H__

#include <memory>
#include "ElectricalElement.h"
#include "GeometricEdges.h"

struct TArco;

class ArcCable : public ElectricalElement, public GeometricEdges
{
public:
    ArcCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TArco> arc );
    std::shared_ptr<TArco> _arc;
};

#endif