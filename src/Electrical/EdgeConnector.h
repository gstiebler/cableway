#ifndef EDGE_CONNECTOR_H__
#define EDGE_CONNECTOR_H__

#include <memory>
#include <vector>

struct TVerticeGeral; 

class EdgeConnector
{
public:
    virtual void connectEdges( std::vector< std::shared_ptr<TVerticeGeral> >& edges ) = 0;
};

#endif