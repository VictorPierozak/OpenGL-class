
#include"ReadBlender.h"

void readBlenderFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<Face>& faces, 
    std::vector<Vertex>& vertexNormal, std::vector<Vertex>& texture) 
{
    std::ifstream file(filename);
    std::string line;
    bool isTexture = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (type == "vn") {
            Vertex normal;
            iss >> normal.x;
            iss >> normal.y;
            iss >> normal.z;
            vertexNormal.push_back(normal);
        }
        else if (type == "f") {
            Face face;

            iss >> face.v1; iss.ignore(); iss >> face.vt1; iss.ignore(); iss >> face.vn1;
            iss >> face.v2; iss.ignore(); iss >> face.vt2; iss.ignore(); iss >> face.vn2;
            iss >> face.v3; iss.ignore(); iss >> face.vt3; iss.ignore();  iss >> face.vn3;
            faces.push_back(face);
        }
        else if (type == "vt")
        {
            isTexture = true;
            Vertex v;
            iss >> v.x;
            iss >> v.y;
            v.z = 0;
            texture.push_back(v);
        }
    }


}

GeometryConfig createGeometryObjectFromBlender(std::string filename, float r, float g, float b)
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Vertex> norm;
    std::vector<Vertex> texture;

    // Pass the vertex data array to the function
    readBlenderFile(filename, vertices, faces, norm, texture);

    std::vector<float> vbo;
    bool isTexture = !texture.empty();

    for (Face ff : faces)
    {
        vbo.push_back(vertices[ff.v1 - 1].x);
        vbo.push_back(vertices[ff.v1 - 1].y);
        vbo.push_back(vertices[ff.v1 - 1].z);
        vbo.push_back(r);
        vbo.push_back(g);
        vbo.push_back(b);
        if (isTexture)
        {
            vbo.push_back(texture[ff.vt1 - 1].x);
            vbo.push_back(texture[ff.vt1 - 1].y);
        }
        vbo.push_back(norm[ff.vn1 - 1].x);
        vbo.push_back(norm[ff.vn1 - 1].y);
        vbo.push_back(norm[ff.vn1 - 1].z);

        vbo.push_back(vertices[ff.v2 - 1].x);
        vbo.push_back(vertices[ff.v2 - 1].y);
        vbo.push_back(vertices[ff.v2 - 1].z);
        vbo.push_back(r);
        vbo.push_back(g);
        vbo.push_back(b);
        if (isTexture)
        {
            vbo.push_back(texture[ff.vt2 - 1].x);
            vbo.push_back(texture[ff.vt2 - 1].y);
        }
        vbo.push_back(norm[ff.vn2 - 1].x);
        vbo.push_back(norm[ff.vn2 - 1].y);
        vbo.push_back(norm[ff.vn2 - 1].z);

        vbo.push_back(vertices[ff.v3 - 1].x);
        vbo.push_back(vertices[ff.v3 - 1].y);
        vbo.push_back(vertices[ff.v3 - 1].z);
        vbo.push_back(r);
        vbo.push_back(g);
        vbo.push_back(b);
        if (isTexture)
        {
            vbo.push_back(texture[ff.vt3 - 1].x);
            vbo.push_back(texture[ff.vt3 - 1].y);
        }
        vbo.push_back(norm[ff.vn3 - 1].x);
        vbo.push_back(norm[ff.vn3 - 1].y);
        vbo.push_back(norm[ff.vn3 - 1].z);
    }

    GeometryConfig config{};
    config.verticesNumber = faces.size() * 3;
    config.textureDim = isTexture*2;
    config.colorAttribNumber = 3;
    config.norm = 3;
    config.dimensions = 3;
    config.data = new float[vbo.size()];
    memcpy(config.data, vbo.data(), vbo.size()*sizeof(float));
    config.elements = nullptr;
    config.elementsNum = 0;

    return config;
    
}
