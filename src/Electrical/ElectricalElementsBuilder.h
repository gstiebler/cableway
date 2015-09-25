#ifndef ELECTRICAL_ELEMENTS_BUILDER_H__
#define ELECTRICAL_ELEMENTS_BUILDER_H__

#include <memory>
#include "ElectricalElements.h"
#include "UListaItensCelula.h"

class CDadosGenerico;
class StraightCable;
class ArcCable;
class Instrument;
class Bandeirola;
struct TMultipoint;
struct TArco;

class ElectricalElementsBuilder
{
public:
	/**
	 * Generates the electrical elements from the drawing data
	 */
	static void build( std::shared_ptr<CDadosGenerico> drawingData, std::shared_ptr<ElectricalElements> electricalElements );

private:
	/**
	 * Builds the straight cables array
	 */
	static void buildStraightCable( std::vector< std::shared_ptr<TMultipoint> > &multiPoints, std::vector< std::shared_ptr<StraightCable> > &straightCables  );

	/**
	 * Builds the arc cables array
	 */
	static void buildArcCable( std::vector< std::shared_ptr<TArco> > &arcs, std::vector< std::shared_ptr<ArcCable> > &arcCables );

	/**
	 * Builds the instruments array
	 */
	static void buildInstrumentsArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Instrument> > &instruments );

	/**
	 * Builds the bandeirolas array
	 */
	static void buildBandeirolasArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Bandeirola> > &bandeirolas );
};

#endif