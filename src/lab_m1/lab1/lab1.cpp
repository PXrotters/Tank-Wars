#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

#define speed 1.5f
#define jumpSpeed 4.5f

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    x = 0;
    y = 0;
    z = 0;
    red = 0;
    green = 0;
    blue = 0;
    obiectIndex = 0;
    obiect = "box";
    jumpAngle = 0.0f;
    isJumping = false;
    jumpSpeedX = 0;
    jumpSpeedY = 0;
    gravity = 8.9f;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(red, green, blue, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(meshes["sphere"], glm::vec3(-2, 0.3f, 1));

    RenderMesh(meshes["teapot"], glm::vec3(1, 1, 1));

    RenderMesh(meshes["quad"], glm::vec3(0, 0.5f, -1), glm::vec3(0.5f));

    RenderMesh(meshes[obiect], glm::vec3(x, y, z), glm::vec3(0.5f));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    if (window->KeyHold(GLFW_KEY_D)) {
        x += speed * deltaTime;

    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        y -= speed * deltaTime;
    }


    if (window->KeyHold(GLFW_KEY_A)) {
        x -= speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        y += speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        z -= speed * deltaTime;

    }

    if (window->KeyHold(GLFW_KEY_S)) {
        z += speed * deltaTime;
    }

    if (isJumping) {
        x += jumpSpeedX * deltaTime;
        y += jumpSpeedY * deltaTime;

        // Verificare daca obiectul a ajuns la sol
        jumpSpeedY -= gravity * deltaTime;

        if (y <= 0) {
            y = 0;
            isJumping = false;
            jumpSpeedY = 0;
        }
    }


    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

}


void Lab1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F) {
        red = static_cast<float>(rand()) / RAND_MAX;
        green = static_cast<float>(rand()) / RAND_MAX;
        blue = static_cast<float>(rand()) / RAND_MAX;
    }

    // Resetam lumea initiala
    if (key == GLFW_KEY_R) {
        red = 0;
        green = 0;
        blue = 0;
    }

    if (key == GLFW_KEY_O) {
        obiectIndex = (obiectIndex + 1) % 3;

        if (obiectIndex == 0) {
            obiect = "box";
        }
        else if (obiectIndex == 1) {
            obiect = "teapot";
        }
        else if (obiectIndex == 2) {
            obiect = "sphere";
        }
    }

    if (key == GLFW_KEY_SPACE && !isJumping) {
        isJumping = true;

        jumpAngle = 3.14159f / 4;  // pi/4 radiani

        jumpSpeedX = jumpSpeed * cos(jumpAngle);
        jumpSpeedY = jumpSpeed * sin(jumpAngle);
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}