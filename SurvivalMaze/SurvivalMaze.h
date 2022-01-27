#pragma once


#include <vector>
#include <time.h>

#include "components/simple_scene.h"
#include "SurvivalMaze/lab_camera.h"
#include "components/transform.h"


#define NUM_ENEMIES 60
#define MAZE_WIDTH 4.f
#define NUM_HEALTH_PICKUPS 7
#define GAME_TIME_LIMIT  0.025f
#define MAX_LIFE 1.f

using namespace std;

namespace m1
{
    class SurvivalMaze : public gfxc::SimpleScene
    {
     public:
        SurvivalMaze();
        ~SurvivalMaze();

        void Init() override;

     private:

         struct Enemy {
             float posX;
             float posZ;
             float posY;
             float translateX;
             bool isShot;
             bool goLeft;
             int ttl;
         };

         struct Bullet {
             float posX;
             float posY;
             float posZ;
             time_t ttl;
             bool canRender;
             float fwdX;
             float fwdZ;       
         };

         struct HealthPickUp {
             float posX;
             float posY;
             float posZ;
             bool isPicked;
         };


        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CreateMesh(const char* name, int x, int y, glm::vec3 &color);

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color, glm::mat4 projMat);
        
        void RenderTexturedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

        float computeCoordinate(float index);
        void placeKanyeWest();
        void placeEnemies();
        void placeHealthPickUps();

        void renderEnemies(float deltaTime);
        void renderHealthPickUps();
        void renderMaze();
        void renderBullet(float deltaTime);
        void renderKanyeWest();
        void renderEnviorment();

        void renderBar();

        bool playerWallCollision(float nextPosX, float nextPosZ);
        bool playerEnemyCollision();
        bool playerHealthPickUpCollision();
        bool bulletMazeCollision(Bullet bullet);
        bool bulletEnemyCollision(Enemy enemy);

        bool hasExitMaze();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;        

     protected:
        implemented::GameCamera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float posX, posZ, posY, rotAngle;
        bool firstCameraType;

        vector<Enemy> enemies;
        int num_bullets;
        vector<Bullet> bullets;
        vector<HealthPickUp> healthPickUps;

        glm::vec3 backupCameraPos;

        float right, left, bottom, top, fov;

        float life, translateGameTime;
        bool gameWin;
        bool canMove[4] = { true, true, true, true };

        std::unordered_map<std::string, Texture2D*> mapTextures;
        GLboolean mixTextures;
    };
}   // namespace m1
