#ifndef EDGE_CONNECTOR_H__
#define EDGE_CONNECTOR_H__

#include <memory>

struct TVerticeGeral; 

class EdgeConnector
{
public:
    virtual void connectEdge( std::shared_ptr<TVerticeGeral> edge ) = 0;
};

#endif