#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    // Creare trapez cu coltul din stanga jos, lungime sus si jos, inaltime si culoare specificata
    Mesh* CreateTrapezoid(const std::string& name, glm::vec3 leftBottomCorner, float topLength, float bottomLength, float height, glm::vec3 color, bool fill = false);

    // Creare arc de cerc cu centru, raza, unghi de start si de final, culoare si numar de segmente
    Mesh* CreateArc(const std::string& name, glm::vec3 center, float radius, float startAngle, float endAngle, glm::vec3 color, int numSegments = 20);

    // Creare dreptunghi cu coltul din stanga jos, latime, inaltime si culoare specificata
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);

    // Creare cerc (proiectil) cu centru, raza, culoare si numar de segmente
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, int numSegments = 20);

}
