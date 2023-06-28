//
//  stage.cpp
//  TJE_XCODE
//
//  Created by manu on 26/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "entity.h"
#include "stage.h"
#include "loadScene.h"
#include "random.h"

Stage::Stage(){
    fin = false;
    camera = Game::instance->camera;
    
    //camera = new Camera();
    camera->lookAt(Vector3(0.f,1000.f, 1000.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); //set the projection, we want to be perspective
}

void Stage::render(){
    
}

void Stage::update(float elapsed_time){

}

TitleStage::TitleStage(){
    this->width = Game::instance->window_width;
    this->height = Game::instance->window_height;
    
    camera->lookAt(Vector3(0.f,2.f, 10.f),Vector3(0.f,2.f,0.f), Vector3(0.f,1.f,0.f));
    
    texture = new Texture();
    texture->load("data/textures/texture3.tga");

    // example of loading Mesh from Mesh Manager
    mesh = Mesh::Get("data/background.obj");

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    //model.setTranslation(10000.0, 0, 1000);
    model.rotate(-PI/2, Vector3(0,1,0));
    EntityMesh* background = new EntityMesh(model, mesh, shader, texture);
    World::world->get_instance()->root->addChild(background);
}

void TitleStage::render(){
    //set the clear color (the background color)
    glClearColor(0.f, 0.f, 0.f, 1);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Matrix44 m;

    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", Vector4(1,1,1,1));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", m);
        shader->setUniform("u_time", time);
        
        //render stage here
        World::get_instance()->render();
        
        //disable shader
        shader->disable();
    }
    drawText(150, height/2-150, "Error 403: Forbidden", Vector3(1,1,1),5);
    drawText(200, height/2+250, "Press SPACE to start", Vector3(1,1,1),4);
    
}

void TitleStage::update(float elapsed_time){
    camera->move(Vector3(0.0f, 0.0f, 1)*elapsed_time);
    if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
        fin = true;
        for (int i = 0; i < World::get_instance()->root->children.size(); i++) { //clean root
            World::get_instance()->root->removeChild(World::get_instance()->root->children[i]);
            
        }
    }
}

stageId TitleStage::getId()
{
    return stageId::TITLE;
}

void PlayStage::loadNewLvl(float seconds_elapsed)
{
    if (spawnCd <= 0.f)
    {
        int rand = get_random_enemy_num(currentDiff);
        enemyNum = (rand > 20) ? 20 : rand;
        // this code is for if we want to use it to change things via randomness or other factors like difficulty and position and to not destroy enemies on death
        for (int i = 0; i < enemyNum; i++)
        {
            int hp;
            float spd, cdShot, dispersion;
            Matrix44 model;
            Mesh* entityMesh;

            Entity* root = World::world->get_instance()->root;

            hp = 1;
            entityMesh = mesh;
            spd = get_random_spd();

            model.setTranslation(get_random_dist() * get_random_sign(), 51.f, get_random_dist() * get_random_sign());
            cdShot = get_random_cdShot();
            dispersion = get_random_disp();
            if (enemies.size() <= i)
            {
                EntityAI* newEnemy = new EntityAI(model, entityMesh, shader, texture, hp, spd, cdShot, dispersion);
                enemies.push_back(newEnemy);
                root->addChild(newEnemy);
            }
            else
            {
                enemies.at(i)->hp = hp;
                enemies.at(i)->maxhp = hp;
                enemies.at(i)->mesh = entityMesh;
                enemies.at(i)->speed = spd;
                enemies.at(i)->model = model;
                enemies.at(i)->cdShot = cdShot;
                enemies.at(i)->dispersion = dispersion;

                root->addChild(enemies.at(i));
            }
        }

        currentDiff++;
        spawnCd = 5.f;
    }
    else if (!enemyNum){
        spawnCd -= seconds_elapsed;
    }
}

PlayStage::PlayStage(){
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
    glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
    
    camera->move(Vector3(0.0f, 0.0f,-2500.0f));
    camera->rotate(0.20, Vector3(1.0f, 0.f, 0.f));
    
    
    texture = new Texture();
    texture->load("data/texture.tga");

    // example of loading Mesh from Mesh Manager
    mesh = Mesh::Get("data/box.ASE");

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    
    EntityPlayer* player = new EntityPlayer(model, mesh, shader, texture, camera);
    
    player->model.translate(0.0f, 51.0f, 0.0f);
    
    World::get_instance()->player = player;
    
    Matrix44 AImodel = Matrix44();

    AImodel.setTranslation(400.f, 51.f, 400.f);

    parseScene("data/scenes/test_room3.scene", model, World::get_instance()->root, NULL);
    
    mesh = Mesh::Get("data/enemy.obj");
    texture = Texture::Get("data/textures/enemy_texture.tga");
    
    currentDiff = 1;
    spawnCd = 5.f;
    enemyNum = 0;
    loadNewLvl(0.f);
}

void PlayStage::render(){
    //set the clear color (the background color)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set the camera as default
    camera->enable();

    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
   
    Matrix44 m;

    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", Vector4(1,1,1,1));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", m);
        shader->setUniform("u_time", time);
        
        //render stage here
        drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
        World::get_instance()->render();

        //disable shader
        shader->disable();
    }

}

void PlayStage::update(float seconds_elapsed){
    if (!enemyNum || spawnCd > 0.f) {
        loadNewLvl(seconds_elapsed);
    }
    World::get_instance()->update(seconds_elapsed);
    if (Input::wasKeyPressed(SDL_SCANCODE_Q)) { //toggle freecam
        this->free_cam = !this->free_cam;
    }
    if(World::get_instance()->player->isDead){
        World::get_instance()->cleanRoot();
        World::get_instance()->player = nullptr;
        fin = true;
    }
    /*
    if (Input::isKeyPressed(SDL_SCANCODE_P)) { //debug boton suicidio
        fin = true;
        World::get_instance()->player = nullptr;
        for (int i = 0; i < World::get_instance()->root->children.size(); i++) { //clean root
            World::get_instance()->root->removeChild(World::get_instance()->root->children[i]);
            
        }
    }*/
}

stageId PlayStage::getId()
{
    return stageId::GAME;
}

MenuStage::MenuStage(){
    
}

void MenuStage::render(){
    
}

void MenuStage::update(float elapsed_time){
    
}

stageId MenuStage::getId()
{
    return stageId::MENU;
}

EndStage::EndStage(){

    camera->lookAt(Vector3(0.f, 2.f, 10.f), Vector3(0.f, 2.f, 0.f), Vector3(0.f, 1.f, 0.f));

    texture = new Texture();
    texture->load("data/textures/texture3.tga");

    // example of loading Mesh from Mesh Manager
    mesh = Mesh::Get("data/background.obj");

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    model.rotate(-PI / 2, Vector3(0, 1, 0));
    EntityMesh* background = new EntityMesh(model, mesh, shader, texture);
    World::world->get_instance()->root->addChild(background);

    restart = false;
    retry = false;
}

void EndStage::render(){
    glClearColor(0.0f, 0.0f, 0.0f, 1);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Matrix44 m;
    
    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", Vector4(1,1,1,1));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", m);
        shader->setUniform("u_time", time);
        
        //render stage here
        World::get_instance()->render();
        
        //disable shader
        shader->disable();
    }
    drawText(Game::instance->window_width/2-130, Game::instance->window_height/2-100, "YOU DIED", Vector3(1,0,0),6);
    drawText(175, Game::instance->window_height/2+200, "Press R to play again", Vector3(1,1,1),4);
    drawText(135, Game::instance->window_height/2+250, "Or press F to exit to title screen", Vector3(1,1,1),3);
}

void EndStage::update(float elapsed_time){
    camera->move(Vector3(0.0f, 0.0f, 1)*elapsed_time);
    if (Input::wasKeyPressed(SDL_SCANCODE_R))
    {
        World::get_instance()->cleanRoot();
        retry = true;
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_F))
    {
        World::get_instance()->cleanRoot();
        restart = true;
    }
}

stageId EndStage::getId()
{
    return stageId::ENDING;
}


///TEST
/*std::vector<Stage*> stages;

stageId current_stage = stageId::TITLE;

void initStages() {
    stages.reserve(4);
    stages.push_back(new TitleStage());
    stages.push_back(new PlayStage());
    stages.push_back(new MenuStage());
    stages.push_back(new EndStage());

}

Stage* getStage(stageId stageID) {
    return stages[stageID];
}

Stage* getCurrentStage() {
    return getStage(current_stage);
}

void setCurrentStage(stageId stageID) {
    current_stage = stageID;
}
 */
///TEST
