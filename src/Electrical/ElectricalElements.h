#ifndef ELECTRICAL_ELEMENTS_H__
#define ELECTRICAL_ELEMENTS_H__

#include <memory>
#include <vector>

class ArcCable;
class StraightCable;
class Instrument;
class Bandeirola;

struct ElectricalElements
{
    std::vector< std::shared_ptr<ArcCable> > _arcCables;
    std::vector< std::shared_ptr<StraightCable> > _straightCables;
    std::vector< std::shared_ptr<Instrument> > _instruments;
    std::vector< std::shared_ptr<Bandeirola> > _bandeirolas;
};

#endif