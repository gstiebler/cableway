#include "GraphBuilder.h"

#include <vector>
#include "UDadosGenerico.h"
#include "Graph.h"
#include "Electrical/GeometricEdges.h"
#include "Electrical/EdgeConnector.h"
#include "Electrical/ElectricalElements.h"

using namespace std;

void GraphBuilder::build( std::shared_ptr<Graph> graph, std::shared_ptr<ElectricalElements> electricalElements )
{
	shared_ptr<TVerticeGeral> temp = make_shared<TVerticeGeral>();
    graph->_verticesGerais->Adiciona( temp );

	vector< shared_ptr<GeometricEdges> > geometricEdges;
	vector< shared_ptr<EdgeConnector> > edgeConnector;

	geometricEdges.insert( geometricEdges.end(), electricalElements->_arcCables.begin(), electricalElements->_arcCables.end() );
	geometricEdges.insert( geometricEdges.end(), electricalElements->_straightCables.begin(), electricalElements->_straightCables.end() );
	geometricEdges.insert( geometricEdges.end(), electricalElements->_bandeirolas.begin(), electricalElements->_bandeirolas.end() );

	edgeConnector.insert( edgeConnector.end(), electricalElements->_arcCables.begin(), electricalElements->_arcCables.end() );
	edgeConnector.insert( edgeConnector.end(), electricalElements->_straightCables.begin(), electricalElements->_straightCables.end() );
	edgeConnector.insert( edgeConnector.end(), electricalElements->_instruments.begin(), electricalElements->_instruments.end() );

	for( auto geometricEdgesItem : geometricEdges )
		for( auto edgeConnector : edgeConnector )
			for( auto edge : geometricEdgesItem->_edges )
				edgeConnector->connectEdge( edge );
}