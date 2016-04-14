#ifndef ARC_CABLE_H__
#define ARC_CABLE_H__

#include <memory>
#include "ElectricalElement.h"
#include "GeometricEdges.h"
#include "EdgeConnector.h"

struct TArco;

class ArcCable : public ElectricalElement, public GeometricEdges, public EdgeConnector
{
public:
    ArcCable( std::shared_ptr<Graph> graph, std::shared_ptr<TDesenho> drawing, std::shared_ptr<TArco> arc );
    std::shared_ptr<TArco> _arc;

private:
	void initializeEdges();
	void addInternalEdge();
	void connectEdge( std::shared_ptr<TVerticeGeral> edge );
};

#endif