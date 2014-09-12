/*
 * DrawingLoader.h
 *
 *  Created on: 10/09/2014
 *      Author: Guilherme
 */

#ifndef DRAWINGLOADER_H_
#define DRAWINGLOADER_H_

#include <dl_creationadapter.h>
#include "dl_dxf.h"
#include <string>

class CDadosGenerico;

class DrawingLoader : public DL_CreationAdapter
{
public:
    DrawingLoader( CDadosGenerico *dados );
    virtual ~DrawingLoader();

    static void loadDXF( std::string fileName, CDadosGenerico *dados );

private:

    void addLine(const DL_LineData &d);
    void addArc(const DL_ArcData&);
    void addCircle(const DL_CircleData&);
    void addEllipse(const DL_EllipseData&);
    void addLayer(const DL_LayerData&);
    void addBlock(const DL_BlockData&);
    void endBlock();
    void addPoint(const DL_PointData&);
    void addText(const DL_TextData&);
    void addPolyline(const DL_PolylineData&);
    void addVertex(const DL_VertexData&);

    CDadosGenerico *_dados;
};

#endif /* DRAWINGLOADER_H_ */
