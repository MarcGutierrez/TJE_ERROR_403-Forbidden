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


#include <cmath>

Stage::Stage(){
    fin = false;
}

void Stage::render(){
    
}

void Stage::update(float elapsed_time){
    if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) {
        fin = true;
    }
}

TitleStage::TitleStage(){
    this->width = Game::instance->window_width;
    this->height = Game::instance->window_height;
}

void TitleStage::render(){
    
}

void TitleStage::update(float elapsed_time){

}

stageId TitleStage::getId()
{
    return stageId::TITLE;
}

PlayStage::PlayStage(){
        
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
    glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
    
    camera = Game::instance->camera;
    
    //camera = new Camera();
    camera->lookAt(Vector3(0.f,1000.f, 1000.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    camera->setPerspective(70.f,Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); //set the projection, we want to be perspective
    
    texture = new Texture();
    texture->load("data/texture.tga");

    // example of loading Mesh from Mesh Manager
    mesh = Mesh::Get("data/box.ASE");

    //Mesh* plane = new Mesh();
    //plane->createSubdividedPlane(1000.0f);

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

    // example of loading Mesh from Mesh Manager
    
    //Mesh* test;
    //test = Mesh::Get("data/test_scene.obj");

    // example of shader loading using the shaders manager
    
    EntityPlayer* player = new EntityPlayer(model, mesh, shader, texture, camera);
    
    //EntityMesh* test_scene = new EntityMesh(model, test, shader, texture);
    
    //EntityMesh* test_plane = new EntityMesh(model, plane, shader, texture);
    
    player->model.translate(0.0f, 51.0f, 0.0f);
    
    World::get_instance()->root->addChild(player);
    //world->root->addChild(test_scene);
    //World::get_instance()->root->addChild(test_plane);
    //world->root->addChild(testentity);
    
    
    parseScene("data/scenes/myscene.scene", model, World::get_instance()->root, NULL);
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
   
    //create model matrix for cube
    Matrix44 m;
    //m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

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

        //do the draw call
        //mesh->render( GL_TRIANGLES );
        
        //render stage here
        World::get_instance()->render();
        

        //disable shader
        shader->disable();
    }

}

void PlayStage::update(float seconds_elapsed){
    World::get_instance()->update(seconds_elapsed);
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
    
}

void EndStage::render(){
    
}

void EndStage::update(float elapsed_time){
    
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
