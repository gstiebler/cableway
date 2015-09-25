#ifndef ARC_CABLE_H__
#define ARC_CABLE_H__

#include <memory>
#include "ElectricalElement.h"

struct TArco;
struct TVerticeGeral; 

class ArcCable : public ElectricalElement
{
public:
    ArcCable( std::shared_ptr<TDesenho> drawing, std::shared_ptr<TArco> arc );
    std::shared_ptr<TArco> _arc;
	shared_ptr<TVerticeGeral> _edges[2];
};

#endif