#ifndef ELECTRICAL_ELEMENTS_BUILDER_H__
#define ELECTRICAL_ELEMENTS_BUILDER_H__

#include <memory>
#include "UListaItensCelula.h"

class CDadosGenerico;
class StraightCable;
class ArcCable;
class Instrument;
class Bandeirola;
class Graph;
class ElectricalElements;
struct TMultipoint;
struct TArco;
struct TDesenho;

class ElectricalElementsBuilder
{
public:
	/**
	 * Generates the electrical elements from the drawing data
	 */
	ElectricalElementsBuilder( std::shared_ptr<Graph> graph, std::shared_ptr<CDadosGenerico> drawingData, 
					std::shared_ptr<ElectricalElements> electricalElements );

	void build();

private:
	/**
	 * Builds the straight cables array
	 */
	void buildStraightCable();

	/**
	 * Builds the arc cables array
	 */
	void buildArcCable();

	/**
	 * Builds the instruments array
	 */
	void buildInstrumentsArray();

	/**
	 * Builds the bandeirolas array
	 */
	void buildBandeirolasArray();

	std::shared_ptr<Graph> _graph;
	std::shared_ptr<CDadosGenerico> _drawingData;
	std::shared_ptr<ElectricalElements> _electricalElements;
};

#endif