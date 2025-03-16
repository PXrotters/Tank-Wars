#pragma once

#include "components/simple_scene.h"
#define SPEED_MOVEMENT 2
#define SQUARE_SIDE 1


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderTank(float tankX, float tankY, float angle, float turretAngle);
        void RenderTrajectory(float tankX, float tankY, float turretAngle, float projectileSpeed, float gravity);
        void RenderLineSegment(glm::vec2 start, glm::vec2 end, glm::vec3 color);
        void GenerateTerrain(int width);
        void GenerateTerrainTransforms();
        float TerrainFunction(float x);
        void DeformTerrain(glm::vec2 explosionCenter, float radius);
        bool Tema1::CheckCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2);
        void Tema1::RenderHealthBar(float x, float y, float width, float height, int health, int maxHealth);
        void UpdateTank(float tankX, float& adjustedTankY, float& tankAngle, bool isTankAlive,
            int tankHealth, float turretAngle, glm::vec2& projectilePosition, bool& isProjectileActive);
        void UpdateProjectile(glm::vec2& projectilePosition, glm::vec2& projectileVelocity, bool& isProjectileActive,
            float projectileRadius, float gravity, float deltaTimeSeconds,
            glm::vec2 targetTankPosition, float targetTankRadius, int& targetTankHealth, bool& isTargetTankAlive);

    protected:
        glm::mat3 modelMatrix;

        // TODO(student): If you need any other class variables, define them here.
        bool move;
        bool scale;
        bool isLeftTankAlive;
        bool isRightTankAlive;
        bool isRightProjectileActive;
        bool isLeftProjectileActive;
        float tankSpeed;
        float leftTankX;
        float rightTankX;
        float tankwidth;
        float tankHeight;
        float leftTurretAngle;
        float rightTurretAngle;
        float adjustedLeftTankY;
        float adjustedRightTankY;
        float leftTankAngle;
        float rightTankAngle;
        float turretRotationSpeed;
        float tankHitboxRadius;
        float projectileRadius;
        float projectileSpeed;
        float gravity;
        int leftTankHealth;
        int rightTankHealth;
        glm::vec2 leftProjectilePosition;
        glm::vec2 leftProjectileVelocity;
        glm::vec2 rightProjectilePosition;
        glm::vec2 rightProjectileVelocity;

    };
}   // namespace m1
