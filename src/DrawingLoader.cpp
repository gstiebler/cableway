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



void DrawingLoader::addLine(const DL_LineData &lineData)
{
    printf("addLine (%f, %f) (%f,%f)\n", lineData.x1, lineData.x2, lineData.y1, lineData.y2 );
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



void DrawingLoader::addLayer(const DL_LayerData &layerData)
{
    printf("addLayer %s\n", layerData.name.c_str() );
}



void DrawingLoader::addBlock(const DL_BlockData &blockData)
{
    printf( "addBlock name: %s, (%f, %f, %f)\n", blockData.name.c_str(), blockData.bpx, blockData.bpy, blockData.bpz );
}



void DrawingLoader::endBlock()
{
    printf("endBlock\n");
}



void DrawingLoader::addPoint(const DL_PointData&)
{
    printf("addPoint\n");
}



void DrawingLoader::addText(const DL_TextData &textData)
{
    printf("addText %s %f %f\n", textData.text.c_str(), textData.apx, textData.ipx);
}



void DrawingLoader::addPolyline(const DL_PolylineData &polyData)
{
    printf("addPolyline %d %d %d\n", polyData.m, polyData.n, polyData.number);
}



void DrawingLoader::addVertex(const DL_VertexData &vertexData)
{
    printf("addVertex %f %f %f\n", vertexData.x, vertexData.y, vertexData.z );
}



