#ifndef ARC_CABLE_H__
#define ARC_CABLE_H__

#include <memory>

class TArco;

class ArcCable
{
public:
    ArcCable();
    std::shared_ptr<TArco> _arco;
};

#endif