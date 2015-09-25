#include "ElectricalElementsBuilder.h"

#include "UDadosGenerico.h"
#include "StraightCable.h"
#include "ArcCable.h"

using namespace std;

void ElectricalElementsBuilder::build( std::shared_ptr<CDadosGenerico> drawingData, std::shared_ptr<ElectricalElements> electricalElements )
{
	buildStraightCable( drawingData->Multipoint, electricalElements->_straightCables );
	buildArcCable( drawingData->Arcos, electricalElements->_arcCables );
}


void ElectricalElementsBuilder::buildStraightCable( vector< shared_ptr<TMultipoint> > &multiPoints, vector< shared_ptr<StraightCable> > &straightCables )
{
	for (int n = 0; n < multiPoints.size(); n++)
    {
        if ( multiPoints[n]->Nivel == CABO )
        {
			shared_ptr<StraightCable> straightCable = make_shared<StraightCable>();
			straightCables.push_back( straightCable );
			straightCable->_multipoint = multiPoints[n];
        }
    }
}


void ElectricalElementsBuilder::buildArcCable( std::vector< std::shared_ptr<TArco> > &arcs, std::vector< shared_ptr<ArcCable> > &arcCables )
{
	for (int n = 0; n < arcs.size(); n++)
    {
		if ( arcs[n]->Nivel == CABO )
        {
			shared_ptr<ArcCable> arcCable = make_shared<ArcCable>();
			arcCable->_arco = arcs[n];
			arcCables.push_back( arcCable );
        }
    }
}