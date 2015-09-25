#ifndef STRAIGHT_CABLE_H__
#define STRAIGHT_CABLE_H__

#include <memory>

class TMultipoint;

class StraightCable
{
public:
    StraightCable();
	std::shared_ptr<TMultipoint> _multipoint;
};

#endif