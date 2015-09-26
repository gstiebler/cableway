#ifndef GEOMETRICAL_EDGES_H__
#define GEOMETRICAL_EDGES_H__

#include <memory>
#include <vector>

struct TVerticeGeral; 

class GeometricEdges
{
public:
    std::vector< std::shared_ptr<TVerticeGeral> > _edges;
    
protected:
    virtual void initializeEdges() = 0;
};

#endif