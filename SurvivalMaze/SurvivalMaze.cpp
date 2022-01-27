#include "SurvivalMaze/SurvivalMaze.h"
#include "SurvivalMaze/Maze.h"
#include "SurvivalMaze/createObjects2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace m1;


Maze generatedMaze = Maze::Maze();

SurvivalMaze::SurvivalMaze()
{
}


SurvivalMaze::~SurvivalMaze()
{
}

void SurvivalMaze::Init()
{

    life = MAX_LIFE;

    right = 10.f;
    left = 0.01f;
    bottom = 0.0f;
    top = 10.f;
    fov = 40.f;

    posX = 0.f;
    posZ = 0.f;
    posY = 1.f;
   
    rotAngle = 0.f;

    translateGameTime = 0.0001f;

    num_bullets = 0;

    firstCameraType = false;
    gameWin = false;

    srand(time(NULL));

    generatedMaze.generateMaze();

    // pozitionare player + camera

    placeKanyeWest();


    placeEnemies();

    placeHealthPickUps();
    

    CreateMesh("skin", 0, 1, glm::vec3(0.47f, 0.278f, 0.109f));
    CreateMesh("body", 0, 1, glm::vec3(0.239f, 0.152f, 0.133f));
    CreateMesh("leg", 0, 1, glm::vec3(0.305f, 0.227f, 0.239f));

    Mesh* bar = createObjects2D::CreateBar("bar", glm::vec3(0.f, 0.f, -0.8), 100.f, glm::vec3(1, 0, 0), true);
    AddMeshToList(bar);

    Mesh* frameBar = createObjects2D::CreateBar("frameBar", glm::vec3(0.f, 0.f, -0.8), 100.f, glm::vec3(1, 0, 0), false);
    AddMeshToList(frameBar);

    Mesh* quad = createObjects2D::createQuad();
    AddMeshToList(quad);    

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    Shader* shader = new Shader("colorShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders["colorShader"] = shader;

    Shader* shader1 = new Shader("noiseShader");
    shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "NoiseVertexShader.glsl"), GL_VERTEX_SHADER);
    shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "NoiseFragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader1->CreateAndLink();
    shaders["noiseShader"] = shader1;

    Shader* shader2 = new Shader("texturedShader");
    shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextureVertexShader.glsl"), GL_VERTEX_SHADER);
    shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TextureFragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader2->CreateAndLink();
    shaders["texturedShader"] = shader2;

    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "textures");

    Texture2D* texture1 = new Texture2D();
    texture1->Load2D(PATH_JOIN(sourceTextureDir, "grass.jpg").c_str(), GL_REPEAT);
    mapTextures["grass"] = texture1;

    Texture2D* texture2 = new Texture2D();
    texture2->Load2D(PATH_JOIN(sourceTextureDir, "sky.jpg").c_str(), GL_REPEAT);
    mapTextures["sky"] = texture2;

    Texture2D* texture3 = new Texture2D();
    texture3->Load2D(PATH_JOIN(sourceTextureDir, "wall.jpg").c_str(), GL_REPEAT);
    mapTextures["wall"] = texture3;

    Texture2D* texture4 = new Texture2D();
    texture4->Load2D(PATH_JOIN(sourceTextureDir, "floor.jpg").c_str(), GL_REPEAT);
    mapTextures["floor"] = texture4;


    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

}

float SurvivalMaze::computeCoordinate(float index) {
    return index * 4 - 20;
}

void SurvivalMaze::placeKanyeWest() {

    int initPosX, initPosZ;

    initPosX = rand() % 11 + 4;
    initPosZ = rand() % 11 + 4;

    while (generatedMaze.maze[initPosX][initPosZ] != 0) {
        initPosX = rand() % 11 + 4;
        initPosZ = rand() % 11 + 4;
    }


    posX = computeCoordinate(initPosX);
    posZ = computeCoordinate(initPosZ);

    camera = new implemented::GameCamera();
    camera->Set(glm::vec3(posX, 3.5f, posZ), glm::vec3(posX + 2.f, 1.f, posZ + 2.f), glm::vec3(0, 1, 0));

    posX += 2.f;
    posZ += 2.f;
}

void SurvivalMaze::placeEnemies() 
{
    for (int i = 0; i < NUM_ENEMIES; i++) {

        srand(time(NULL));
        int startPosX = rand() % 12 + 3;
        int startPosZ = rand() % 12 + 3;

        while (generatedMaze.maze[startPosX][startPosZ] != 0) {
          
  
           startPosX = rand() % 12 + 3;
           startPosZ = rand() % 12 + 3;
           
        }

        generatedMaze.maze[startPosX][startPosZ] = 2;

        Enemy enemy;
        enemy.posX = computeCoordinate(startPosX);
        enemy.posZ = computeCoordinate(startPosZ) + 1.5f;
        enemy.posY = 1.f;
        enemy.translateX = 0.f;
        enemy.isShot = false;
        enemy.goLeft = true;
        enemy.ttl = 30;

        enemies.push_back(enemy);
    }
}

void SurvivalMaze::placeHealthPickUps()
{
    for (int i = 0; i < NUM_HEALTH_PICKUPS; i++) {

        srand(time(NULL));
        int startPosX = rand() % 12 + 3;
        int startPosZ = rand() % 12 + 3;

        while (generatedMaze.maze[startPosX][startPosZ] != 0) {


            startPosX = rand() % 12 + 3;
            startPosZ = rand() % 12 + 3;

        }

        generatedMaze.maze[startPosX][startPosZ] = 3; // 3 -> element health pick-up in  matrice

        HealthPickUp healthPickup;

        healthPickup.posX = computeCoordinate(startPosX) + 2.f;
        healthPickup.posY = 1.5f;
        healthPickup.posZ = computeCoordinate(startPosZ) + 2.f;
        healthPickup.isPicked = false;

        healthPickUps.push_back(healthPickup);

    }
}


void SurvivalMaze::FrameStart()
{
 
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    
    glViewport(0, 0, resolution.x, resolution.y);

}

void SurvivalMaze::CreateMesh(const char* name, int x, int y, glm::vec3 &color)
{
    
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(x, x,  y), color),
        VertexFormat(glm::vec3(y, x,  y), color),
        VertexFormat(glm::vec3(x,  y,  y), color),
        VertexFormat(glm::vec3(y,  y,  y), color),
        VertexFormat(glm::vec3(x, x, x), color),
        VertexFormat(glm::vec3(y, x, x), color),
        VertexFormat(glm::vec3(x,  y, x), color),
        VertexFormat(glm::vec3(y,  y, x), color),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };
    
    unsigned int VAO = 0;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

   
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));


    glBindVertexArray(0);


    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));

}

void SurvivalMaze::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color, 
    glm::mat4 projMat)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);


    glm::vec3 eyePosition = camera->position;
    int eyePosLoc= glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eyePosLoc, eyePosition.x, eyePosition.y, eyePosition.z);

    int objectColor = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(objectColor, color.r, color.g, color.b);
    
    int noiseTime = glGetUniformLocation(shader->program, "time");
    glUniform1f(noiseTime, (float) Engine::GetElapsedTime());

    srand(time(NULL));
    int randVal = glGetUniformLocation(shader->program, "rd");
    glUniform1f(randVal, rand() % 100 + 10);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projMat));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



bool SurvivalMaze::bulletEnemyCollision(Enemy enemy) {
    for (int i = 0; i < num_bullets; i++) {
        Bullet bullet = bullets[i];

        if (bullet.canRender && !enemy.isShot) {
            float distance = sqrt((enemy.translateX - bullet.posX) * (enemy.translateX - bullet.posX) +
                                  (enemy.posY - bullet.posY) * (enemy.posY - bullet.posY) +
                                  (enemy.posZ - bullet.posZ) * (enemy.posZ - bullet.posZ));

            // 0.85 -> raza cerc inamic; 0.1 -> raza cerc proiectil
            if (distance < (0.85f + 0.5f)) {
                
                bullets[i].canRender = false;
                
                return true;

            }
        }
    }

    return false;
}

void SurvivalMaze::renderHealthPickUps() 
{
    for (int i = 0; i < NUM_HEALTH_PICKUPS; i++)
    {
        if (!healthPickUps[i].isPicked)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix,
                glm::vec3(healthPickUps[i].posX, healthPickUps[i].posY, healthPickUps[i].posZ));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.96f, 0.278f, 0.278f),
                GetSceneCamera()->GetProjectionMatrix());
        }
    }
}

void SurvivalMaze::renderEnemies(float deltaTime) 
{
    for (int i = 0; i < NUM_ENEMIES; i++) {


        if (!enemies[i].isShot) {

            glm::mat4 modelMatrix = glm::mat4(1);

            if (enemies[i].goLeft) {

                enemies[i].translateX += 1.25f * deltaTime;
                modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].translateX, 1.f, enemies[i].posZ));

                // verific daca inamicul a ajuns in partea superioara a celulei pe care o patruleaza
                if (enemies[i].translateX + 1.f > enemies[i].posX + 4.f) {

                    enemies[i].goLeft = false;
                }
            }
            else {
                enemies[i].translateX -= 1.25f * deltaTime;
                modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].translateX, 1.f, enemies[i].posZ));

                if (enemies[i].translateX - 1.5f <= enemies[i].posX) {
                    enemies[i].goLeft = true;
                }
            }

            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75f));

            // coliziune
            if (bulletEnemyCollision(enemies[i])) {
                enemies[i].isShot = true;
                
            }
            else {
                RenderSimpleMesh(meshes["sphere"], shaders["colorShader"], modelMatrix, glm::vec3(0.58f, 0.815f, 0.8f), 
                    GetSceneCamera()->GetProjectionMatrix());
            }

        }
        else {

            if (enemies[i].ttl != 0) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].translateX, 1.f, enemies[i].posZ));
                RenderSimpleMesh(meshes["sphere"], shaders["noiseShader"], modelMatrix, glm::vec3(0.58f, 0.815f, 0.8f),
                    GetSceneCamera()->GetProjectionMatrix());

                enemies[i].ttl--;
            }
        }

        
    }
}

bool SurvivalMaze::bulletMazeCollision(Bullet bullet) {
    
    for (int i = 0; i < MAZE_DIM; i++) {
        for (int j = 0; j < MAZE_DIM; j++) {

            if (generatedMaze.maze[i][j] == 1) {
                float wallX = computeCoordinate(i);
                float wallZ = computeCoordinate(j);
                float wallY = 0.f;

                float x = max(wallX, min(bullet.posX, wallX + 4.f));
                float y = max(wallY, min(bullet.posY, wallY + 4.f));
                float z = max(wallZ, min(bullet.posZ, wallZ + 4.f));

                float distance = sqrt((x - bullet.posX) * (x - bullet.posX) +
                                      (y - bullet.posY) * (y - bullet.posY) +
                                      (z - bullet.posZ) * (z - bullet.posZ));

                if (distance < 0.75f) {
                    return true;
                }

            }

        }
    }

    return false;
}

void SurvivalMaze::renderBullet(float deltaTime) {
    for (int i = 0; i < num_bullets; i++) {

        // verific ttl
        time_t end = time(0);

        if ((end - bullets[i].ttl >= 3 || bulletMazeCollision(bullets[i])) && bullets[i].canRender) {
            bullets[i].canRender = false;
        }

        if (bullets[i].canRender) {
            glm::mat4 modelMatrix = glm::mat4(1);
            float d = 2.5f * deltaTime;
            bullets[i].posX += bullets[i].fwdX * d;
            bullets[i].posZ += bullets[i].fwdZ * d;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(bullets[i].posX, bullets[i].posY, bullets[i].posZ));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
            RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
        }
    }
}

bool SurvivalMaze::playerWallCollision(float nextPosX, float nextPosZ) {
    for (int i = 0; i < MAZE_DIM; i++) {
        for (int j = 0; j < MAZE_DIM; j++) {

            if (generatedMaze.maze[i][j] == 1) {
                float wallX = computeCoordinate(i);
                float wallZ = computeCoordinate(j);
                float wallY = 0.f;

                int offX = -1, offZ = -1;

                if (camera->right[0] < 0.f)
                    offX = 1.f;

                if (camera->right[2] < 0.f)
                    offZ = 1.f;

                float midX = (nextPosX + nextPosX + offX) / 2.f;
                float midZ = (nextPosZ + nextPosZ + offZ) / 2.f;

                float x = max(wallX, min(midX, wallX + MAZE_WIDTH));
                float y = max(wallY, min(posY, wallY + 2.f));
                float z = max(wallZ, min(midZ, wallZ + MAZE_WIDTH));


                float distance = sqrt((x - midX) * (x - midX) +
                                      (y - posY) * (y - posY) +
                                      (z - midZ) * (z - midZ));

                if (distance < 0.5f) 
                {
                    return true;
                }

            }

        }
    }
    
    return false;
}

bool SurvivalMaze::playerHealthPickUpCollision() 
{

    for (int i = 0; i < NUM_HEALTH_PICKUPS; i++) 
    {
        HealthPickUp pickUp = healthPickUps[i];

        if (!pickUp.isPicked) 
        {
            float distance = sqrt((pickUp.posX - posX) * (pickUp.posX - posX) + 
                (pickUp.posY - posY) * (pickUp.posY - posY) + 
                (pickUp.posZ - posZ) * (pickUp.posZ - posZ));


            if (distance < 1.f + 0.1f) 
            {
                healthPickUps[i].isPicked = true;
                return true;
            }
        }
    }

    return false;
}

bool SurvivalMaze::playerEnemyCollision() {
    
    for (int i = 0; i < NUM_ENEMIES; i++) {

        
        Enemy enemy = enemies[i];
        float x, y, z, distance;

        if (!enemy.isShot) {

            distance = sqrt((enemy.translateX - posX) * (enemy.translateX - posX) +
                (enemy.posY - posY) * (enemy.posY - posY) +
                (enemy.posZ - posZ) * (enemy.posZ - posZ));

            // 0.85f -> raza inamic randat
            if (distance < 1.f + 0.85f) {
                enemies[i].isShot = true;

                return true;
            }

        }
    }

    return false;
}

void SurvivalMaze::renderBar() 
{

    glm::mat4 barProj = glm::ortho(left, right, bottom, top, 0.01f, 200.f);

    glm::mat4 modelMatrix;
    
    // life bar
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, (camera->GetTargetPosition())[1], posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.5f, 7.5f, 3.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.025f, 0.25f, 0.25f));

    RenderSimpleMesh(meshes["frameBar"], shaders["colorShader"], modelMatrix, glm::vec3(1.f, 0.f, 0.f), barProj);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, (camera->GetTargetPosition())[1], posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.5f, 7.5f, 3.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.025f * life, 0.25f, 0.25f));

    RenderSimpleMesh(meshes["bar"], shaders["colorShader"], modelMatrix, glm::vec3(1.f, 0.f, 0.f), barProj);

    // time bar

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, (camera->GetTargetPosition())[1], posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-8.5f, 7.5f, 3.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(GAME_TIME_LIMIT, 0.25f, 0.25f));

    RenderSimpleMesh(meshes["frameBar"], shaders["colorShader"], modelMatrix, glm::vec3(0.f, 0.f, 1.f), barProj);

    // creste bara atunci pana cand jocul nu este gastigat
    if (!gameWin)
        translateGameTime = 0.0001f * Engine::GetElapsedTime();

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, (camera->GetTargetPosition())[1], posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(-8.5f, 7.5f, 3.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(translateGameTime, 0.25f, 0.25f));

    RenderSimpleMesh(meshes["bar"], shaders["colorShader"], modelMatrix, glm::vec3(0.f, 0.f, 1.f), barProj);

  
}

void SurvivalMaze::renderKanyeWest() {
    
    // Render Kanye West   
    glm::mat4 modelMatrix;

    // corp
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 1.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    RenderMesh(meshes["body"], shaders["VertexColor"], modelMatrix);

    // cap
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 2.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.25f, 0.f, 0.25f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["skin"], shaders["VertexColor"], modelMatrix);

    // mana dreapta
    modelMatrix = glm::mat4(1);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 1.5f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f, 0.f, 0.25f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 1.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f, 0.f, 0.25f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["skin"], shaders["VertexColor"], modelMatrix);

    // mana stanga
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 1.5f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.f, 0.25f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 1.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.f, 0.25f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderMesh(meshes["skin"], shaders["VertexColor"], modelMatrix);

    // picior drept
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 0.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.65, 0.f, 0.25));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f, 1.f, 0.5f));
    RenderMesh(meshes["leg"], shaders["VertexColor"], modelMatrix);

    // picior stang
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, 0.f, posZ));
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 0.25));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f, 1.f, 0.5f));
    RenderMesh(meshes["leg"], shaders["VertexColor"], modelMatrix);
}

void SurvivalMaze::renderEnviorment() {
    glm::mat4 modelMatrix;

    // render grass
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, -1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 160.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["grass"]);

    // render sky
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -80.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 150.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["sky"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 80.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 150.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["sky"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(80.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 150.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["sky"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-50.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 150.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["sky"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 50.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(1.f, 0.f, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(160.f, 170.f, 160.f));
    RenderTexturedMesh(meshes["square"], shaders["texturedShader"], modelMatrix, mapTextures["sky"]);
}

bool SurvivalMaze::hasExitMaze() 
{
    float upperLimit = computeCoordinate(MAZE_DIM);
    float lowerLimit = -20.f;

    return (posX > upperLimit || posZ > upperLimit || posX < lowerLimit || posZ < lowerLimit);
}

void SurvivalMaze::Update(float deltaTimeSeconds)
{

    if (hasExitMaze() && translateGameTime < GAME_TIME_LIMIT && !gameWin)
    {
        gameWin = true;
        cout << "You Win! Congratz" << endl;
    }

    if ((life < 0.0f || 0.0001f * Engine::GetElapsedTime() > GAME_TIME_LIMIT) && !gameWin) {
        cout << "GAME OVER" << endl;

        window->Close();
    }

    if (playerHealthPickUpCollision() && life < MAX_LIFE) {
        life += 0.1f;
    }

    if (playerEnemyCollision()) {
        life -= 0.1f;
    }

    renderEnviorment();
  
    renderKanyeWest();

    // render health pick-ups
    renderHealthPickUps();

    // render bullet
    renderBullet(deltaTimeSeconds);

    // render enemy
    renderEnemies(deltaTimeSeconds);

    // render maze
    renderMaze();

    renderBar();

}

void SurvivalMaze::renderMaze() {
    for (int i = 0; i < MAZE_DIM; i++) {
        for (int j = 0; j < MAZE_DIM; j++) {

            glm::mat4 modelMatrix;

            if (generatedMaze.maze[i][j] == 1) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, 
                    glm::vec3(computeCoordinate(i) + 2.f, 1.f, computeCoordinate(j) + 2.f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(MAZE_WIDTH, MAZE_WIDTH / 2.f, MAZE_WIDTH));
                RenderTexturedMesh(meshes["box"], shaders["texturedShader"], modelMatrix, mapTextures["wall"]);
            }
            else {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, 
                    glm::vec3(computeCoordinate(i) + 2.f, 0.f, computeCoordinate(j) + 2.f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(MAZE_WIDTH, 0.f, MAZE_WIDTH));
                RenderTexturedMesh(meshes["box"], shaders["texturedShader"], modelMatrix, mapTextures["floor"]);
            }
        }
    }
}


void SurvivalMaze::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void SurvivalMaze::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}



void SurvivalMaze::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 1.5f;
    float nextPosX = posX;
    float nextPosZ = posZ;

    if (window->KeyHold(GLFW_KEY_W) && !firstCameraType && canMove[0]) {
        
        camera->MoveForward(cameraSpeed * deltaTime);
        nextPosX = (camera->GetTargetPosition())[0];
        nextPosZ = (camera->GetTargetPosition())[2];

        if (!playerWallCollision(nextPosX, nextPosZ)) {
            posX = nextPosX;
            posZ = nextPosZ;
            memset(canMove, true, 4);
        }
        else {
            memset(canMove, true, 4);
            canMove[0] = false;
        }
    }

    if (window->KeyHold(GLFW_KEY_A) && !firstCameraType && canMove[1]) {
        
        camera->TranslateRight(-cameraSpeed * deltaTime);
        nextPosX = (camera->GetTargetPosition())[0];
        nextPosZ = (camera->GetTargetPosition())[2];

        if (!playerWallCollision(nextPosX, nextPosZ)) {
            posX = nextPosX;
            posZ = nextPosZ;
            memset(canMove, true, 4);
        }
        else {
            memset(canMove, true, 4);
            canMove[1] = false;
 
        }
    }

    if (window->KeyHold(GLFW_KEY_S) && !firstCameraType && canMove[2]) {
       
        camera->MoveForward(-cameraSpeed * deltaTime);
        nextPosX = (camera->GetTargetPosition())[0];
        nextPosZ = (camera->GetTargetPosition())[2];

        if (!playerWallCollision(nextPosX, nextPosZ)) {
            posX = nextPosX;
            posZ = nextPosZ;
            memset(canMove, true, 4);
        }
        else {
            memset(canMove, true, 4);
            canMove[2] = false;
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && !firstCameraType && canMove[3]) {
       
        camera->TranslateRight(cameraSpeed * deltaTime);
        nextPosX = (camera->GetTargetPosition())[0];
        nextPosZ = (camera->GetTargetPosition())[2];

        if (!playerWallCollision(nextPosX, nextPosZ)) {
            posX = nextPosX;
            posZ = nextPosZ;
            memset(canMove, true, 4);
        }
        else {
            memset(canMove, true, 4);
            canMove[3] = false;
        }
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        
        camera->TranslateUpward(-cameraSpeed * deltaTime);

    }

    if (window->KeyHold(GLFW_KEY_E)) {
        
        camera->TranslateUpward(cameraSpeed * deltaTime);

    }

}


void SurvivalMaze::OnKeyPress(int key, int mods)
{

    // schimb modul camerei first - third
    if (key == GLFW_KEY_LEFT_CONTROL) {
        firstCameraType ^= true;

        if (firstCameraType) {
            backupCameraPos = camera->position;
            camera->TranslateForward(camera->distanceToTarget * 1.5f);
           
        }
        else {

            camera->position = backupCameraPos;
  
        }
    }


    if (key == GLFW_KEY_SPACE && firstCameraType) {
        Bullet bullet;
        bullet.posX = camera->position[0];
        bullet.posY = 1.35f;
        bullet.posZ = camera->position[2];
        bullet.canRender = true;
        bullet.fwdX = camera->forward[0];
        bullet.fwdZ = camera->forward[2];
        bullet.ttl = time(0);


        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(bullet.posX, bullet.posY, bullet.posZ));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

        num_bullets++;

        bullets.push_back(bullet);
    }
}


void SurvivalMaze::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void SurvivalMaze::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (!firstCameraType) {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);

            rotAngle = atan2((camera->forward)[0], (camera->forward)[2]);
        }
        else {
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);

        }
    }
}


void SurvivalMaze::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void SurvivalMaze::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void SurvivalMaze::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void SurvivalMaze::OnWindowResize(int width, int height)
{
}

void SurvivalMaze::RenderTexturedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, 
    Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

