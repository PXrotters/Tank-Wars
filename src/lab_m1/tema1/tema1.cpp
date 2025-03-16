#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

vector<glm::vec3> terrainVertices;
std::vector<glm::mat3> terrainTransforms;

// Verificam daca doua cercuri se ciocnesc sau se intersecteaza
bool Tema1::CheckCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2) {
    // Calculeaza distanta euclidiana dintre cele doua centre ale cercurilor
    float distance = glm::distance(center1, center2);
    return distance <= (radius1 + radius2);
}

// Afisarea vietii unui tanc
void Tema1::RenderHealthBar(float x, float y, float width, float height, int health, int maxHealth) {
    // Calcularea procentului de viata ramasa
    float healthPercentage = static_cast<float>(health) / maxHealth;

    // Renderizare contur (wireframe) al barei de viata
    glm::mat3 wireframeMatrix = glm::mat3(1.0f);
    wireframeMatrix *= transform2D::Translate(x, y);
    wireframeMatrix *= transform2D::Scale(width, height);
    RenderMesh2D(meshes["healthBarWireframe"], shaders["VertexColor"], wireframeMatrix);

    // Renderizare viata pierduta
    if (health < maxHealth) {
        glm::mat3 lostHealthMatrix = glm::mat3(1.0f);
        lostHealthMatrix *= transform2D::Translate(x + width * healthPercentage, y);  // Muta partea rosie la dreapta, dupa portiunea verde (viata ramasa)
        lostHealthMatrix *= transform2D::Scale(width * (1 - healthPercentage), height);  // Ajusteaza latimea partii rosii, care este proportionala cu viata pierduta
        RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], lostHealthMatrix);
    }

    // Renderizare viata ramasa
    glm::mat3 filledBarMatrix = glm::mat3(1.0f);
    filledBarMatrix *= transform2D::Translate(x, y);
    filledBarMatrix *= transform2D::Scale(width * healthPercentage, height);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], filledBarMatrix);
}

void Tema1::DeformTerrain(glm::vec2 explosionCenter, float radius) {
    float maxDeformation = 20.0f;  // Limiteaza deformarea maxima
    std::vector<float> newHeights(terrainVertices.size());  // Stocheaza inaltimile modificate ale terenului

    // Calcularea noilor inaltimi pentru fiecare punct de teren
    for (size_t i = 0; i < terrainVertices.size(); i++) {
        // Se calculeaza distanta de la punctul respectiv pana la centrul exploziei
        float distance = glm::distance(glm::vec2(terrainVertices[i].x, terrainVertices[i].y), explosionCenter);

        if (distance <= radius) {
            // Deformarea este mai mare pentru punctele mai apropiate de centrul exploziei
            float deformation = glm::min(radius - distance, maxDeformation);
            newHeights[i] = glm::max(terrainVertices[i].y - deformation, 0.0f); // Evita valori negative
        }
        else {
            newHeights[i] = terrainVertices[i].y;
        }
    }

    // Netezirea terenului
    for (size_t i = 1; i < terrainVertices.size() - 1; i++) {
        // Fiecare punct devine media aritmetica dintre inaltimea sa si a vecinilor sai
        newHeights[i] = (newHeights[i - 1] + newHeights[i] + newHeights[i + 1]) / 3.0f;
    }

    // Actualizeaza punctele terenului
    for (size_t i = 0; i < terrainVertices.size(); i++) {
        terrainVertices[i].y = newHeights[i];
    }

    GenerateTerrainTransforms(); // Recalculeaza matricile de transformare
}

// Genereaza matrici de transformare care definesc segmentele de teren ce vor fi desenate
void Tema1::GenerateTerrainTransforms() {
    // Stergerea transformarilor anterioare
    terrainTransforms.clear();
    float groundLevel = 0.0f;  // Nivelul solului

    for (size_t i = 0; i < terrainVertices.size() - 1; ++i) {
        glm::vec3 start = terrainVertices[i];  // Punctul de inceput al segmentului
        glm::vec3 end = terrainVertices[i + 1]; // Punctul de sfarsit al segmentului

        // Calcularea dimensiunilor segmentului 
        float segmentWidth = glm::distance(start, end);
        float segmentHeight = start.y - groundLevel;
        float shearFactor = (end.y - start.y) / (end.x - start.x);  // Factor de inclinare

        // Constructia matricii de transformare
        glm::mat3 modelMatrix = glm::mat3(1.0f);
        modelMatrix *= transform2D::Translate(start.x, groundLevel);
        modelMatrix *= transform2D::Shear(shearFactor);
        modelMatrix *= transform2D::Scale(segmentWidth, segmentHeight);

        // Adaugarea matricei la lista de transformari
        terrainTransforms.push_back(modelMatrix);
    }
}

// Generarea punctelor care definesc forma terenului pe axa X in functie de o functie matematica de teren
void Tema1::GenerateTerrain(int width) {
    terrainVertices.clear();
    float step = 1.2f;  // Controlul densitatii punctelor
    for (float x = 0; x <= width; x += step) {
        float y = TerrainFunction(x);
        terrainVertices.push_back(glm::vec3(x, y, 0.0f));
    }
}

// Functia matematica care va genera curbura terenului
float Tema1::TerrainFunction(float x) {
    float A1 = 75.0f, A2 = 165.0f, A3 = 15.0f;
    float Omega1 = 0.01f, Omega2 = 0.0025f, Omega3 = 0.025f;
    return A1 * sin(Omega1 * x) + A2 * sin(Omega2 * x) + A3 * sin(Omega3 * x) + 155.0f;
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;
    const float smoothFactor = 0.2;
    tankSpeed = 80.0f;
    leftTankX = 300.0f;  // Pozitia initiala pentru patratul controlat cu A si D
    rightTankX = 700.0f;
    tankwidth = 100.0f;
    leftTurretAngle = 0.0f;
    rightTurretAngle = 0.0f;
    isRightProjectileActive = false;
    rightTurretAngle = 0;
    adjustedRightTankY = 0;
    turretRotationSpeed = 1;
    isLeftProjectileActive = false;
    leftTankHealth = 5;
    rightTankHealth = 5;
    isLeftTankAlive = true;
    isRightTankAlive = true;
    tankHitboxRadius = 50.0f;
    projectileRadius = 10.0f;
    isRightProjectileActive = false;
    projectileSpeed = 380.0f;
    gravity = 300.0f;

    GenerateTerrain(1280);
    GenerateTerrainTransforms();

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 1, 0), true);
    Mesh* trapezoidBaseLarge = object2D::CreateTrapezoid("trapezoidBaseLarge", glm::vec3(0, 0, 0), 80, 130, 30, glm::vec3(0.710f, 0.690f, 0.569f), true); // baza mare
    Mesh* trapezoidBaseSmall = object2D::CreateTrapezoid("trapezoidBaseSmall", glm::vec3(0, 0, 0), 60, 80, 20, glm::vec3(0.278f, 0.263f, 0.173f), true); // baza mica, suprapusa
    Mesh* arcTurret = object2D::CreateArc("arcTurret", glm::vec3(0, 0, 0), 30, M_PI, 2 * M_PI, glm::vec3(0.710f, 0.690f, 0.569f), 50); // tureta
    Mesh* barrel = object2D::CreateRectangle("barrel", glm::vec3(0, 0, 0), 60, 20, glm::vec3(0.3f, 0.3f, 0.3f), true); // teava tunului
    Mesh* projectile = object2D::CreateCircle("projectile", glm::vec3(0, 0, 0), 10, glm::vec3(0, 0, 0), 20); // proiectil
    Mesh* lineSegment = object2D::CreateRectangle("lineSegment", glm::vec3(0, 0, 0), 1, 2, glm::vec3(1, 1, 1), true);
    Mesh* healthBarMesh = object2D::CreateRectangle("healthBar", glm::vec3(0, 0, 0), 1, 1, glm::vec3(1, 0, 0), true);
    Mesh* healthBarWireframe = object2D::CreateRectangle("healthBarWireframe", glm::vec3(0, 0, 0), 1, 1, glm::vec3(1, 1, 1), false);
    AddMeshToList(square1);
    AddMeshToList(trapezoidBaseLarge);
    AddMeshToList(trapezoidBaseSmall);
    AddMeshToList(arcTurret);
    AddMeshToList(barrel);
    AddMeshToList(projectile);
    AddMeshToList(lineSegment);
    AddMeshToList(healthBarMesh);
    AddMeshToList(healthBarWireframe);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderTank(float tankX, float tankY, float angle, float turretAngle) {
    glm::mat3 tankModelMatrix = glm::mat3(1.0f);
    tankModelMatrix *= transform2D::Translate(tankX, tankY);
    tankModelMatrix *= transform2D::Rotate(angle);

    // Render trapezoidul mic (baza de jos)
    glm::mat3 baseSmallMatrix = tankModelMatrix * transform2D::Rotate(M_PI) * transform2D::Translate(-30, -17);
    RenderMesh2D(meshes["trapezoidBaseSmall"], shaders["VertexColor"], baseSmallMatrix);

    // Render trapezoidul mare (suprapus, pozitionat deasupra bazei mici)
    glm::mat3 baseLargeMatrix = tankModelMatrix * transform2D::Translate(-75, 15);
    RenderMesh2D(meshes["trapezoidBaseLarge"], shaders["VertexColor"], baseLargeMatrix);

    // Render arc-ul de cerc (turela), deasupra celor doua trapezoide
    glm::mat3 turretMatrix = tankModelMatrix * transform2D::Translate(-10, 45) * transform2D::Rotate(M_PI);
    RenderMesh2D(meshes["arcTurret"], shaders["VertexColor"], turretMatrix);

    // Render teava tunului (pozitionata in centrul arcului) cu rotatie
    glm::mat3 barrelMatrix = turretMatrix * transform2D::Rotate(turretAngle) * transform2D::Translate(-80, -10);
    RenderMesh2D(meshes["barrel"], shaders["VertexColor"], barrelMatrix);
}

void Tema1::RenderLineSegment(glm::vec2 start, glm::vec2 end, glm::vec3 color) {
    // Calculam distanta si unghiul intre cele doua puncte
    float length = glm::distance(start, end);
    float angle = atan2(end.y - start.y, end.x - start.x);

    // Cream o matrice de transformare pentru a pozitiona si roti segmentul
    glm::mat3 modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(start.x, start.y);
    modelMatrix *= transform2D::Rotate(angle);
    modelMatrix *= transform2D::Scale(length, 2);  // Latimea liniei

    // Renderizam un dreptunghi subtire (ca linie)
    RenderMesh2D(meshes["lineSegment"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderTrajectory(float tankX, float tankY, float turretAngle, float projectileSpeed, float gravity) {
    std::vector<glm::vec2> trajectoryPoints;
    const int numPoints = 50;      // Numarul de puncte pe traiectorie
    const float timeStep = 0.1f;   // Pasul de timp intre punctele de pe traiectorie

    float initialVelocityX = cos(turretAngle) * projectileSpeed;  // cos unghiului turelei
    float initialVelocityY = sin(turretAngle) * projectileSpeed;  // sin unghiului turelei

    glm::vec2 startPoint = glm::vec2(tankX, tankY) + glm::vec2(cos(turretAngle), sin(turretAngle)) * 40.0f;  // Punctul de plecare al traiectoriei
    trajectoryPoints.push_back(startPoint);

    // Calcularea punctelor traiectoriei
    for (int i = 1; i <= numPoints; i++) {
        float t = i * timeStep;

        float x = startPoint.x + initialVelocityX * t;
        float y = startPoint.y + initialVelocityY * t - 0.5f * gravity * t * t;

        trajectoryPoints.push_back(glm::vec2(x, y));
    }

    // Redarea traiectoriei ca segmente de linie
    for (size_t i = 0; i < trajectoryPoints.size() - 1; i++) {
        glm::vec2 point1 = trajectoryPoints[i];
        glm::vec2 point2 = trajectoryPoints[i + 1];

        // Desenam segmentul de traiectorie
        RenderLineSegment(point1, point2, glm::vec3(1.0f, 1.0f, 1.0f));  // Alb
    }
}

// Calcului pozitiei si unghiului, randarea tancului si a barii de viata
void Tema1::UpdateTank(float tankX, float& adjustedTankY, float& tankAngle, bool isTankAlive,
    int tankHealth, float turretAngle, glm::vec2& projectilePosition, bool& isProjectileActive) {
    float tankCenterX = tankX + tankwidth / 2.0f; // Pozitia centrului tancului
    float tankY = 0.0f;

    // Iterare prin segmentele terenului
    for (size_t i = 0; i < terrainVertices.size() - 1; ++i) {
        glm::vec3 A = terrainVertices[i];
        glm::vec3 B = terrainVertices[i + 1];

        // Verificarea pozitiei tancului pe segment
        if (tankCenterX >= A.x && tankCenterX <= B.x) {
            float t = (tankCenterX - A.x) / (B.x - A.x);
            tankY = A.y + t * (B.y - A.y);
            tankAngle = atan2(B.y - A.y, B.x - A.x); // Unghiul tancului fata de teren
            break;
        }
    }

    // Ajusteaza pozitia tancului pentru centrul bazei
    adjustedTankY = tankY;

    if (isTankAlive) {
        // Renderizarea tancului
        RenderTank(tankX + tankwidth / 2, adjustedTankY, tankAngle, turretAngle);

        // Bara de viata
        float healthBarWidth = 100.0f;
        float healthBarHeight = 10.0f;
        float healthBarOffsetY = 100.0f;
        RenderHealthBar(tankX, adjustedTankY + healthBarOffsetY, healthBarWidth, healthBarHeight, tankHealth, 5);
    }

    // Coliziune proiectil cu terenul
    if (projectilePosition.y <= TerrainFunction(projectilePosition.x)) {
        isProjectileActive = false; // Proiectilul a lovit terenul
        DeformTerrain(projectilePosition, 50.0f); // Deformare teren cu raza de 50
    }
}

// Actualizeaza proiectilul unui tanc
void Tema1::UpdateProjectile(glm::vec2& projectilePosition, glm::vec2& projectileVelocity, bool& isProjectileActive,
    float projectileRadius, float gravity, float deltaTimeSeconds,
    glm::vec2 targetTankPosition, float targetTankRadius, int& targetTankHealth, bool& isTargetTankAlive) {
    // Actualizeaza viteza proiectilului pe axa Y
    projectileVelocity.y -= gravity * deltaTimeSeconds;

    // Actualizeaza pozitia proiectilului
    projectilePosition += projectileVelocity * deltaTimeSeconds;

    // Verifica coliziunea cu tancul tinta
    if (isTargetTankAlive && CheckCollision(projectilePosition, projectileRadius, targetTankPosition, targetTankRadius)) {
        isProjectileActive = false; // Proiectilul dispare
        targetTankHealth--;  // Reduce viata tancului tinta

        // Verifica daca tancul este distrus
        if (targetTankHealth <= 0) {
            isTargetTankAlive = false;
        }
    }

    // Verifica coliziunea cu terenul
    if (projectilePosition.y <= TerrainFunction(projectilePosition.x)) {
        isProjectileActive = false; // Proiectilul a lovit terenul
        DeformTerrain(projectilePosition, 50.0f); // Deformare teren cu raza de explozie 50
    }

    // Renderizeaza proiectilul daca este activ
    if (isProjectileActive) {
        glm::mat3 projectileMatrix = glm::mat3(1.0f);
        projectileMatrix *= transform2D::Translate(projectilePosition.x, projectilePosition.y);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectileMatrix);
    }
}


void Tema1::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    
    // Randarea terenului
    for (const auto& modelMatrix : terrainTransforms) {
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }

    // Alunecarea de teren
    const float epsilon = 0.5f;   // Cat de rapid se netezeste terenul
    const float threshold = 5.0f; // Pragul care determina cat de mare trebuie sa fie diferenta de inaltime intre 2 puncte

    for (size_t i = 0; i < terrainVertices.size() - 1; i++) {
        float heightDifference = terrainVertices[i].y - terrainVertices[i + 1].y;  // Diferenta de inaltime intre punctul curent si urmatorul

        if (fabs(heightDifference) > threshold) {  // Daca diferenta de inaltime calculata este mai mare decat pragul impus se aplica alunecarea
            if (heightDifference > 0) {
                terrainVertices[i].y -= epsilon * deltaTimeSeconds;
                terrainVertices[i + 1].y += epsilon * deltaTimeSeconds;
            } else {
                terrainVertices[i].y += epsilon * deltaTimeSeconds;
                terrainVertices[i + 1].y -= epsilon * deltaTimeSeconds;
            }
        }
    }

    GenerateTerrainTransforms(); // Actualizeaza terenul dupa deformare

    // Calculul pozitiei si unghiului pentru tancul din stanga
    UpdateTank(leftTankX, adjustedLeftTankY, leftTankAngle, isLeftTankAlive,
        leftTankHealth, leftTurretAngle, leftProjectilePosition, isLeftProjectileActive);

    // Calculul pozitiei si unghiului pentru tancul din dreapta
    UpdateTank(rightTankX, adjustedRightTankY, rightTankAngle, isRightTankAlive,
        rightTankHealth, rightTurretAngle, rightProjectilePosition, isRightProjectileActive);

    // Actualizeaza proiectilul tancului din stanga
    if (isLeftProjectileActive) {
        glm::vec2 rightTankPosition = glm::vec2(rightTankX + tankwidth / 2, adjustedRightTankY);
        UpdateProjectile(leftProjectilePosition, leftProjectileVelocity, isLeftProjectileActive,
            projectileRadius, gravity, deltaTimeSeconds,
            rightTankPosition, tankHitboxRadius, rightTankHealth, isRightTankAlive);
    }

    // Actualizeaza proiectilul tancului din dreapta
    if (isRightProjectileActive) {
        glm::vec2 leftTankPosition = glm::vec2(leftTankX + tankwidth / 2, adjustedLeftTankY);
        UpdateProjectile(rightProjectilePosition, rightProjectileVelocity, isRightProjectileActive,
            projectileRadius, gravity, deltaTimeSeconds,
            leftTankPosition, tankHitboxRadius, leftTankHealth, isLeftTankAlive);
    }

    // Randarea traiectoriei pentru tancul din stanga
    if (isLeftTankAlive) {
        RenderTrajectory(leftTankX + tankwidth - 45, adjustedLeftTankY + 35, leftTurretAngle + leftTankAngle, projectileSpeed, gravity);
    }

    // Randarea traiectoriei pentru tancul din dreapta
    if (isRightTankAlive) {
        RenderTrajectory(rightTankX + tankwidth - 45, adjustedRightTankY + 35, rightTurretAngle + rightTankAngle, projectileSpeed, gravity);
    }
}




void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods) {
        // Controleaza tancul din stanga cu A si D
        if (window->KeyHold(GLFW_KEY_A)) {
            leftTankX -= tankSpeed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            leftTankX += tankSpeed * deltaTime;
        }

        // Controleaza tancul din dreapta cu sagetile stanga si dreapta
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            rightTankX -= tankSpeed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            rightTankX += tankSpeed * deltaTime;
        }
        // Roteste turela tancului din stanga cu W si S
        if (window->KeyHold(GLFW_KEY_W)) {
            leftTurretAngle += turretRotationSpeed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            leftTurretAngle -= turretRotationSpeed * deltaTime;
        }

        // Roteste turela tancului din dreapta cu sagetile sus si jos
        if (window->KeyHold(GLFW_KEY_UP)) {
            rightTurretAngle -= turretRotationSpeed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            rightTurretAngle += turretRotationSpeed * deltaTime;
        }
    }


void Tema1::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_SPACE && !isLeftProjectileActive && isLeftTankAlive) {
        // Lansare proiectil pentru tancul din stanga
        glm::mat3 tankTransform = glm::mat3(1.0f);
        tankTransform *= transform2D::Translate(leftTankX, adjustedLeftTankY);
        tankTransform *= transform2D::Rotate(leftTankAngle);

        glm::mat3 barrelTransform = glm::mat3(1.0f);
        barrelTransform *= transform2D::Translate(30, 60);

        // Pozitia proiectilului calculata dupa toate transformarile
        glm::vec3 projectileStartPosition = tankTransform * barrelTransform * glm::vec3(0, 0, 1);
        leftProjectilePosition = glm::vec2(projectileStartPosition.x, projectileStartPosition.y);

        // Viteza proiectilului bazata pe unghiurile combinate ale turelei si tancului
        leftProjectileVelocity = glm::vec2(cos(leftTurretAngle + leftTankAngle), sin(leftTurretAngle + leftTankAngle)) * projectileSpeed;
        isLeftProjectileActive = true;
    }

    if (key == GLFW_KEY_ENTER && !isRightProjectileActive && isRightTankAlive) {
        // Lansare proiectil pentru tancul din dreapta
        glm::mat3 tankTransform = glm::mat3(1.0f);
        tankTransform *= transform2D::Translate(rightTankX, adjustedRightTankY);
        tankTransform *= transform2D::Rotate(rightTankAngle);

        glm::mat3 barrelTransform = glm::mat3(1.0f);
        barrelTransform *= transform2D::Translate(30, 60);

        glm::vec3 projectileStartPosition = tankTransform * barrelTransform * glm::vec3(0, 0, 1);
        rightProjectilePosition = glm::vec2(projectileStartPosition.x, projectileStartPosition.y);
        rightProjectileVelocity = glm::vec2(cos(rightTurretAngle + rightTankAngle), sin(rightTurretAngle + rightTankAngle)) * projectileSpeed;
        isRightProjectileActive = true;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
