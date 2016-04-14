#ifndef INSTRUMENT_H__
#define INSTRUMENT_H__

#include <vector>
#include <memory>
#include "UListaItensCelula.h"
#include "ElectricalElement.h"
#include "EdgeConnector.h"
#include "UGeometria.h"

struct TTexto;
struct TArco;
struct TMultipoint;

class Instrument : public ElectricalElement, public EdgeConnector
{
public:
	Instrument( std::shared_ptr<Graph> graph, std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems );
	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;

private:
	void connectEdge( std::shared_ptr<TVerticeGeral> edge );
	void createColarEdge();
	TPonto getCenter() const;
	void initializeEdges();
	void addEdgeToCenter( std::shared_ptr<TVerticeGeral> edge );
	std::vector< std::shared_ptr<TVerticeGeral> > _instrumentVertices;
};

#endif