#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include"Geometry.h"

struct VertexRGB {
    float x, y, z;
    float r, g, b;  // Color components (r, g, b)
};

struct Vertex
{
    float x, y, z;
};

struct Face {
    int v1, vt1, vn1;
    int v2, vt2, vn2;
    int v3, vt3, vn3;
};



void readBlenderFile(const std::string& filename, std::vector<VertexRGB>& vertices, std::vector<Face>& faces, std::vector<Vertex>& vertexNormal);
GeometryConfig createGeometryObjectFromBlender(std::string filename, float r, float g, float b);