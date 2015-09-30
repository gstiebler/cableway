#ifndef BANDEIROLA_H__
#define BANDEIROLA_H__

#include <vector>
#include <memory>
#include "UListaItensCelula.h"
#include "ElectricalElement.h"
#include "GeometricEdges.h"
#include "UGeometria.h"

struct TTexto;
struct TArco;
struct TMultipoint;

class Bandeirola : public ElectricalElement, public GeometricEdges
{
public:
	Bandeirola( std::shared_ptr<Graph> graph, std::shared_ptr<TDesenho> drawing, TListaItensCelula &groupItems ) ;
   	std::vector< std::shared_ptr<TTexto> > _texts;
	std::vector< std::shared_ptr<TArco> > _arcs;
	std::vector< std::shared_ptr<TMultipoint> > _multipoints;

	/**
	 * Returns true if the bandeirola is ok
	 */
	bool isValid();

	/**
	 * Returns the width of the bandeirola
	 */
	double getWidth();

private:
	void initializeEdges();

	/**
	 * Returns the free edges from internal elements
	 */
	void getFreeInternalEdges( std::vector<TPonto> &internalEdges );

	/** Indicates if the bandeirola has all necessary geometry components */
	bool _isValid;
};

#endif