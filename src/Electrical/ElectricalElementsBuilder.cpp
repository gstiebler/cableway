#include "ElectricalElementsBuilder.h"

#include "UDadosGenerico.h"
#include "StraightCable.h"
#include "ArcCable.h"
#include "Instrument.h"
#include "Bandeirola.h"

using namespace std;

void ElectricalElementsBuilder::build( std::shared_ptr<CDadosGenerico> drawingData, std::shared_ptr<ElectricalElements> electricalElements )
{
	buildStraightCable( drawingData->Multipoint, electricalElements->_straightCables );
	buildArcCable( drawingData->Arcos, electricalElements->_arcCables );
	buildInstrumentsArray( drawingData->InfoCelula.ListaCelulasInstrumentos, electricalElements->_instruments );
	buildBandeirolasArray( drawingData->InfoCelula.ListaCelulasBandeirolas, electricalElements->_bandeirolas );
}


void ElectricalElementsBuilder::buildStraightCable( vector< shared_ptr<TMultipoint> > &multiPoints, vector< shared_ptr<StraightCable> > &straightCables )
{
	for ( auto multiPoint : multiPoints )
    {
        if ( multiPoint->Nivel == CABO )
        {
			shared_ptr<StraightCable> straightCable = make_shared<StraightCable>();
			straightCables.push_back( straightCable );
			straightCable->_multipoint = multiPoint;
        }
    }
}


void ElectricalElementsBuilder::buildArcCable( std::vector< std::shared_ptr<TArco> > &arcs, std::vector< shared_ptr<ArcCable> > &arcCables )
{
	for ( auto arc : arcs )
    {
		if ( arc->Nivel == CABO )
        {
			shared_ptr<ArcCable> arcCable = make_shared<ArcCable>();
			arcCable->_arco = arc;
			arcCables.push_back( arcCable );
        }
    }
}


void ElectricalElementsBuilder::buildInstrumentsArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Instrument> > &instruments )
{
	for( auto groupItem : groupItems )
	{
		std::shared_ptr<Instrument> instrument = make_shared<Instrument>();
		instrument->_arcs = groupItem._arcs;
		instrument->_multipoints = groupItem._multipoints;
		instrument->_texts = groupItem._texts;
		instruments.push_back( instrument );
	}
}


void ElectricalElementsBuilder::buildBandeirolasArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Bandeirola> > &bandeirolas )
{
	for( auto groupItem : groupItems )
	{
		std::shared_ptr<Bandeirola> bandeirola = make_shared<Bandeirola>();
		bandeirola->_arcs = groupItem._arcs;
		bandeirola->_multipoints = groupItem._multipoints;
		bandeirola->_texts = groupItem._texts;
		bandeirolas.push_back( bandeirola );
	}
}