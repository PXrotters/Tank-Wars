#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTrapezoid(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float topLength,
    float bottomLength,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    // Calculam varfurile trapezului
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color), // V0 - colt stanga jos
        VertexFormat(corner + glm::vec3(bottomLength, 0, 0), color), // V1 - colt dreapta jos
        VertexFormat(corner + glm::vec3((bottomLength - topLength) / 2.0f, height, 0), color), // V2 - colt stanga sus
        VertexFormat(corner + glm::vec3((bottomLength + topLength) / 2.0f, height, 0), color)  // V3 - colt dreapta sus
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        // Daca nu este umplut, desenam conturul
        indices = { 0, 1, 3, 2, 0 };
        trapezoid->SetDrawMode(GL_LINE_LOOP); // LINE_LOOP - asigura desenarea conturului prin conectarea tuturor punctelor si inchiderea figurii
    }
    else {
        // Daca este umplut, impartim in doua triunghiuri
        indices = { 0, 1, 2, 1, 3, 2 };
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* object2D::CreateArc(
    const std::string& name,
    glm::vec3 center,
    float radius,
    float startAngle,
    float endAngle,
    glm::vec3 color,
    int numSegments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Adaugam centrul ca primul varf
    vertices.push_back(VertexFormat(center, color));

    // Calculam varfurile de pe circumferinta :
    for (int i = 0; i <= numSegments; ++i) {
        float angle = startAngle + i * (endAngle - startAngle) / numSegments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);
        indices.push_back(i + 1); // Indicii pentru varfurile de pe arc
    }
    indices.push_back(1); // Inchidem arcul

    Mesh* arc = new Mesh(name);
    arc->SetDrawMode(GL_TRIANGLE_FAN);
    arc->InitFromData(vertices, indices);
    return arc;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Centrul cercului este adaugat ca primul punct in lista vertices. Acesta va fi punctul comun pentru toate triunghiurile.
    vertices.emplace_back(center, color);

    // Calcularea varfurilor de pe circumferinta:
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * 2.0f * M_PI / numSegments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, y, 0), color);
        indices.push_back(i + 1);
    }
    indices.push_back(1); // Inchide cercul

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}