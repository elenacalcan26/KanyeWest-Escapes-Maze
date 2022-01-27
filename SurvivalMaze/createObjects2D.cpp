#include "SurvivalMaze/createObjects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Mesh* createObjects2D::CreateBar(const std::string& name, glm::vec3 leftBottomCorner, float health,
    glm::vec3 color, bool fill)
{

    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(glm::vec3(0, 5, 0), color),
        VertexFormat(glm::vec3(health, 5, 0), color),
        VertexFormat(glm::vec3(health, 0, 0), color)
    };

    std::vector<unsigned int> indices = { 0 , 1, 2, 3 };

    Mesh* bar = new Mesh(name);

    if (!fill)
    {
        bar->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    bar->InitFromData(vertices, indices);
    return bar;

}

Mesh* createObjects2D::createQuad() {
    vector<glm::vec3> vertices
    {
        glm::vec3(0.5f,   0.5f, 0.0f),    // top right
        glm::vec3(0.5f,  -0.5f, 0.0f),    // bottom right
        glm::vec3(-0.5f, -0.5f, 0.0f),    // bottom left
        glm::vec3(-0.5f,  0.5f, 0.0f),    // top left
    };

    vector<glm::vec3> normals
    {
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0)
    };

    vector<glm::vec2> textureCoords
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 1.f),
        glm::vec2(1.f, 0.f)

    };

    vector<unsigned int> indices =
    {
        0, 1, 3,
        1, 2, 3
    };

    Mesh* mesh = new Mesh("square");
    mesh->InitFromData(vertices, normals, textureCoords, indices);
    return mesh;
}

