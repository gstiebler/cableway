#include "ElectricalElementsBuilder.h"

#include "UDadosGenerico.h"
#include "StraightCable.h"
#include "ArcCable.h"
#include "Instrument.h"
#include "Bandeirola.h"
#include "ElectricalElements.h"

using namespace std;

ElectricalElementsBuilder::ElectricalElementsBuilder( shared_ptr<Graph> graph, shared_ptr<CDadosGenerico> drawingData, 
													 shared_ptr<ElectricalElements> electricalElements ) :
	_graph( graph ),
	_drawingData( drawingData ),
	_electricalElements( electricalElements )
{
}


void ElectricalElementsBuilder::build()
{
	buildStraightCable();
	buildArcCable();
	buildInstrumentsArray();
	buildBandeirolasArray();
}


void ElectricalElementsBuilder::buildStraightCable()
{
	for ( auto multiPoint : _drawingData->Multipoint )
    {
        if ( multiPoint->Nivel == CABO )
        {
			shared_ptr<StraightCable> straightCable = make_shared<StraightCable>( _graph, _drawingData->_drawing, multiPoint );
			_electricalElements->_straightCables.push_back( straightCable );
        }
    }
}


void ElectricalElementsBuilder::buildArcCable()
{
	for ( auto arc : _drawingData->Arcos)
    {
		if ( arc->Nivel == CABO )
        {
			shared_ptr<ArcCable> arcCable = make_shared<ArcCable>( _graph, _drawingData->_drawing, arc );
			_electricalElements->_arcCables.push_back( arcCable );
        }
    }
}


void ElectricalElementsBuilder::buildInstrumentsArray()
{
	for( auto groupItem : _drawingData->InfoCelula.ListaCelulasInstrumentos )
	{
		std::shared_ptr<Instrument> instrument = make_shared<Instrument>( _graph, _drawingData->_drawing, groupItem );
		_electricalElements->_instruments.push_back( instrument );
	}
}


void ElectricalElementsBuilder::buildBandeirolasArray()
{
	for( auto groupItem : _drawingData->InfoCelula.ListaCelulasBandeirolas )
	{
		std::shared_ptr<Bandeirola> bandeirola = make_shared<Bandeirola>( _graph, _drawingData->_drawing, groupItem );
		if( !bandeirola->isValid() )
			continue;

		_electricalElements->_bandeirolas.push_back( bandeirola );
	}
}