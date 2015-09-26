#include "ElectricalElementsBuilder.h"

#include "UDadosGenerico.h"
#include "StraightCable.h"
#include "ArcCable.h"
#include "Instrument.h"
#include "Bandeirola.h"

using namespace std;

void ElectricalElementsBuilder::build( std::shared_ptr<CDadosGenerico> drawingData, std::shared_ptr<ElectricalElements> electricalElements )
{
	buildStraightCable( drawingData->Multipoint, electricalElements->_straightCables, drawingData->_drawing );
	buildArcCable( drawingData->Arcos, electricalElements->_arcCables, drawingData->_drawing );
	buildInstrumentsArray( drawingData->InfoCelula.ListaCelulasInstrumentos, electricalElements->_instruments, drawingData->_drawing );
	buildBandeirolasArray( drawingData->InfoCelula.ListaCelulasBandeirolas, electricalElements->_bandeirolas, drawingData->_drawing );
}


void ElectricalElementsBuilder::buildStraightCable( vector< shared_ptr<TMultipoint> > &multiPoints, vector< shared_ptr<StraightCable> > &straightCables,
												   shared_ptr<TDesenho> drawing )
{
	for ( auto multiPoint : multiPoints )
    {
        if ( multiPoint->Nivel == CABO )
        {
			shared_ptr<StraightCable> straightCable = make_shared<StraightCable>( drawing, multiPoint );
			straightCables.push_back( straightCable );
        }
    }
}


void ElectricalElementsBuilder::buildArcCable( std::vector< std::shared_ptr<TArco> > &arcs, std::vector< shared_ptr<ArcCable> > &arcCables,
												   shared_ptr<TDesenho> drawing )
{
	for ( auto arc : arcs )
    {
		if ( arc->Nivel == CABO )
        {
			shared_ptr<ArcCable> arcCable = make_shared<ArcCable>( drawing, arc );
			arcCables.push_back( arcCable );
        }
    }
}


void ElectricalElementsBuilder::buildInstrumentsArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Instrument> > &instruments,
												   shared_ptr<TDesenho> drawing )
{
	for( auto groupItem : groupItems )
	{
		std::shared_ptr<Instrument> instrument = make_shared<Instrument>( drawing, groupItem );
		instruments.push_back( instrument );
	}
}


void ElectricalElementsBuilder::buildBandeirolasArray( std::vector<TListaItensCelula> &groupItems, std::vector< std::shared_ptr<Bandeirola> > &bandeirolas,
												   shared_ptr<TDesenho> drawing )
{
	for( auto groupItem : groupItems )
	{
		std::shared_ptr<Bandeirola> bandeirola = make_shared<Bandeirola>( drawing, groupItem );
		if( !bandeirola->isValid() )
			continue;

		bandeirolas.push_back( bandeirola );
	}
}