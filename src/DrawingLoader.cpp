/*
 * DrawingLoader.cpp
 *
 *  Created on: 10/09/2014
 *      Author: Guilherme
 */

#include "DrawingLoader.h"

DrawingLoader::DrawingLoader( CDadosGenerico *dados ) :
    _dados( dados )
{
}



DrawingLoader::~DrawingLoader()
{
}



void DrawingLoader::loadDXF( std::string fileName, CDadosGenerico *dados )
{
    DrawingLoader f( dados );
    DL_Dxf dxf;
    if (!dxf.in(fileName.c_str(), &f)) {
        std::cerr << fileName.c_str() << " could not be opened.\n";
    }
}



void DrawingLoader::addLine(const DL_LineData &d)
{
    printf("addLine\n");
}



void DrawingLoader::addArc(const DL_ArcData&)
{
    printf("addArc\n");
}



void DrawingLoader::addCircle(const DL_CircleData&)
{
    printf("addCircle\n");
}



void DrawingLoader::addEllipse(const DL_EllipseData&)
{
    printf("addEllipse\n");
}



void DrawingLoader::addLayer(const DL_LayerData&)
{
    printf("addLayer\n");
}



void DrawingLoader::addBlock(const DL_BlockData&)
{
    printf("addBlock\n");
}



void DrawingLoader::endBlock()
{
    printf("endBlock\n");
}



void DrawingLoader::addPoint(const DL_PointData&)
{
    printf("addPoint\n");
}



void DrawingLoader::addText(const DL_TextData&)
{
    printf("addText\n");
}



