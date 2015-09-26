#ifndef ELECTRICAL_ELEMENT_H__
#define ELECTRICAL_ELEMENT_H__

#include <memory>
struct TDesenho;

class ElectricalElement
{
public:
    ElectricalElement( std::shared_ptr<TDesenho> drawing );
    
protected:
    std::shared_ptr<TDesenho> _drawing;
};

#endif;