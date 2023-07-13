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
#include "interface.h"


int killCount;
int wave;
    
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
    this->tutorial = false;
    
    this->intermitent = true;
    
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
    
    //Menu Stuff
    currentSlot = slot1;
    //slot1 = true;
    //slot2 = false;
    //slot3 = false;
    
    //slots.push_back(slot1);
    //slots.push_back(slot2);
    //slots.push_back(slot3);
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

    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", Vector4(1,1,1,1));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", model);
        shader->setUniform("u_time", time);
        
        //render stage here
        World::get_instance()->render();
        
        //disable shader
        shader->disable();
    }
    //drawText(150, height/2-150, "Error 403: Forbidden", Vector3(1,1,1),5);
    drawText(Game::instance->window_width/2 - 250, height/2-150, "Error 403: Forbidden", Vector3(1,1,1),5);
    
    if(currentSlot == slot1)
        drawText(Game::instance->window_width/2-115, Game::instance->window_height/2, "Start Game", Vector3(1,0,0),4);
    else
        drawText(Game::instance->window_width/2-115, Game::instance->window_height/2, "Start Game", Vector3(1,1,1),4);
    if(currentSlot == slot2)
        drawText(Game::instance->window_width/2-120, Game::instance->window_height/2 + 50, "How to Play", Vector3(1,0,0),4);
    else
        drawText(Game::instance->window_width/2-120, Game::instance->window_height/2 + 50, "How to Play", Vector3(1,1,1),4);
    
    if(Game::instance->isHard)
        if(currentSlot == slot3)
            drawText(Game::instance->window_width/2-150, Game::instance->window_height/2 + 100, "Difficulty: Hard", Vector3(1,0,0),4);
        else
            drawText(Game::instance->window_width/2-150, Game::instance->window_height/2 + 100, "Difficulty: Hard", Vector3(1,1,1),4);
    else
        if(currentSlot == slot3)
            drawText(Game::instance->window_width/2-150, Game::instance->window_height/2 + 100, "Difficulty: Easy", Vector3(1,0,0),4);
        else
            drawText(Game::instance->window_width/2-150, Game::instance->window_height/2 + 100, "Difficulty: Easy", Vector3(1,1,1),4);
    
    if (intermitent)
    {
        drawText(Game::instance->window_width/2-160, Game::instance->window_height/ 2 + 250, "Insert credit(s)", Vector3(1, 1, 1), 4);
        /*if (Input::gamepads->connected)
            drawText(200, height / 2 + 250, "Press START to start", Vector3(1, 1, 1), 4);

        else
            drawText(200, height / 2 + 250, "Press SPACE to start", Vector3(1, 1, 1), 4);*/
        
    }
        
    
}

void TitleStage::update(float elapsed_time){
    if (currentSlot > 2)
        currentSlot = slot1;
    if (currentSlot < 0)
        currentSlot = slot3;
    camera->move(Vector3(0.0f, 0.0f, 1)*elapsed_time);
    th -= elapsed_time;
    if (th <= 0.f)
    {
        th = 1.f;
        intermitent = !intermitent;
    }
    if (currentSlot == slot1 && ((Input::wasKeyPressed(SDL_SCANCODE_RETURN)) || Input::wasButtonPressed(A_BUTTON))) {
        fin = true;
        for (int i = 0; i < World::get_instance()->root->children.size(); i++) { //clean root
            World::get_instance()->root->removeChild(World::get_instance()->root->children[i]);
            
        }
    }
    if (currentSlot == slot2 && ((Input::wasKeyPressed(SDL_SCANCODE_RETURN)) || Input::wasButtonPressed(A_BUTTON))) {
        World::get_instance()->cleanRoot();
        tutorial = true;
    }
    if (currentSlot == slot3 && ((Input::wasKeyPressed(SDL_SCANCODE_RETURN)) || Input::wasButtonPressed(A_BUTTON))){
        Game::instance->isHard = !Game::instance->isHard;
    }
    if(Input::wasKeyPressed(SDL_SCANCODE_DOWN)||Input::wasKeyPressed(SDL_SCANCODE_S)||Input::wasButtonPressed(PAD_DOWN)){
        currentSlot += 1;
    }
    if(Input::wasKeyPressed(SDL_SCANCODE_UP)||Input::wasKeyPressed(SDL_SCANCODE_W)||Input::wasButtonPressed(PAD_UP)){
        currentSlot -= 1;
    }
}

stageId TitleStage::getId()
{
    return stageId::TITLE;
}

void PlayStage::loadNewLvl(float seconds_elapsed)
{
    //mesh = Mesh::Get("data/enemy.obj");
    //texture = Texture::Get("data/textures/enemy_texture.tga");
    if (spawnCd <= 0.f){
        rand = get_random_enemy_num(currentDiff);
        enemyNum = (rand > 10) ? 10 : rand;
        // this code is for if we want to use it to change things via randomness or other factors like difficulty and position and to not destroy enemies on death
        for (int i = 0; i < enemyNum; i++)
        {
            root = World::world->get_instance()->root;

            hp = 1;
            entityMesh = enemyMesh;
            spd = get_random_spd();

            model.setTranslation(get_random_dist() * get_random_sign(), 0, get_random_dist() * get_random_sign());
            cdShot = get_random_cdShot();
            dispersion = get_random_disp();
            if (enemies.size() <= i)
            {
                newEnemy = new EntityAI(model, entityMesh, shader, enemyTexture, hp, spd, cdShot, dispersion);
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
    else if (!enemyNum) {
        spawnCd -= seconds_elapsed;
    }
}

void PlayStage::loadBossLvl(float seconds_elapsed){

    if (spawnCd <= 0.f) {
        isBossLvl = true;
        enemyNum = 1;
        // this code is for if we want to use it to change things via randomness or other factors like difficulty and position and to not destroy enemies on death
        root = World::world->get_instance()->root;

        hp = get_random_hpBoss(currentDiff);
        entityMesh = bossMesh;
        spd = get_random_spd() / 10;

        model.setTranslation(get_random_dist() * get_random_sign(), 0, get_random_dist() * get_random_sign());
        cdShot = get_random_cdShotBoss(); // MIRAR FUNCION BOSS
        bulletsShoot = get_random_bulletsBoss(currentDiff);
        bulletsShoot = (bulletsShoot > 5) ? 5 : bulletsShoot;
        dispersion = 1.f / (bulletsShoot);

        boss->hp = hp;
        boss->maxhp = hp;
        boss->mesh = entityMesh;
        boss->speed = spd;
        boss->model = model;
        boss->cdShot = cdShot;
        boss->dispersion = dispersion;
        boss->numBulletsShoot = bulletsShoot;
        boss->HPbar = 500;

        root->addChild(boss);

        currentDiff++;
        spawnCd = 5.f;
        soundEffPlayed = false;
    }
    else if (!enemyNum){
        spawnCd -= seconds_elapsed;
        if (spawnCd < 1.f && !soundEffPlayed)
        {
            Audio::PlayM("data/audio/boss_wave_alarm.wav");
            soundEffPlayed = true;
        }
    }
}

void PlayStage::loadPowerUp(float seconds_elapsed){
    model.setTranslation(get_random_dist() * get_random_sign(), 0.f, get_random_dist() * get_random_sign());
    lifeTime = 10.f;

    power = get_random_powerUP();
    if (lastPowerup == power)
        power = ((power + 1) % 3) + 1;
    lastPowerup = power;
    
    powerUp = new EntityPowerUp(model, World::get_instance()->powerUpMesh, shader, World::get_instance()->powerUpTexture, lifeTime, (powerUps)power);
    World::get_instance()->root->addChild(powerUp);
    
}

PlayStage::PlayStage(){
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
    glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
    
    camera->lookAt(Vector3(0.f,4500.f, 1),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    //camera->move(Vector3(0.0f, 0.0f,-1500.0f));
    //camera->rotate(0.30, Vector3(1.0f, 0.f, 0.f));
    
    
    //texture = new Texture();
    //texture->load("data/texture.tga");
    texture = World::get_instance()->playerTexture;
    
    // example of loading Mesh from Mesh Manager
    mesh = World::get_instance()->playerMesh;

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    
    EntityPlayer* player = new EntityPlayer(model, mesh, shader, texture, camera);
    
    //player->model.translate(0.0f, 51.0f, 0.0f);
    
    World::get_instance()->player = player;

    enemyMesh = Mesh::Get("data/enemy.obj");
    enemyTexture = Texture::Get("data/textures/enemy_texture.tga");
    
    bossMesh = Mesh::Get("data/boss_test2.obj");
    bossTexture = Texture::Get("data/textures/enemy_texture.tga");

    boss = new EntityBoss(model, bossMesh, shader, bossTexture, 0, 0.f, 0.f, 0.f, 0);

    parseScene("data/scenes/test_room3.scene", model, World::get_instance()->root, NULL);

    //quad = Mesh::Get("data/cdPowerUpIcon.obj");
    
    color = Vector4(0.95f,0.21f,0.67f,1);
    //color = Vector4(1,1,1,1);
    
    //mesh = Mesh::Get("data/enemy.obj");
    //texture = Texture::Get("data/textures/enemy_texture.tga");
    
    currentDiff = 1;
    spawnCd = 5.f;
    enemyNum = 0;
    soundEffPlayed = false;
    walkDownX = true;
    walkDownY = false;
    walkDownZ = false;
    gradient = true;
    gradientFactor = 0.1;
    powerUpCd = 0.f;
    lastPowerup = 0;
    //loadNewLvl(0.f);
    //loadBossLvl(0.f);
    //loadPowerUp(0.0);
    
    //UI STUFF
    this->cdIntermitent = true;
    this->msIntermitent = true;
    this->gmIntermitent = true;
    this->lifeTimeTh = 10.0f/4;
    
    slot1InUse = false;
    slot2InUse = false;
    slot3InUse = false;

    slot1 = Vector2(25, 35);
    slot2 = Vector2(70, 35);
    slot3 = Vector2(125, 35);
    
    currentSlot = slot1;
    Vector4 grayColor = Vector4(0.2f, 0.2f, 0.2f, 1);
    
    cdSlot = new UI(quad, World::get_instance()->cdPowerUpTexture, slot1.x, slot1.y, 114, 75, grayColor);
    msSlot = new UI(quad, World::get_instance()->msPowerUpTexture, slot2.x, slot2.y, 114, 75, grayColor);
    gmSlot = new UI(quad, World::get_instance()->gmPowerUpTexture, slot3.x, slot3.y, 114, 75, grayColor);

}

PlayStage::~PlayStage()
{
    delete entityMesh;
    delete root;
    delete newEnemy;
    delete nMesh;
    delete nShader;
    delete powerUp;
    delete enemyTexture;
    delete boss;
    delete bossTexture;
    delete cdSlot;
    delete msSlot;
    delete gmSlot;
    delete HPBar;
    delete HP;
    enemies.clear();
    enemies.shrink_to_fit();
    projectiles.clear();
    projectiles.shrink_to_fit();
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

    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", color);
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", model);
        shader->setUniform("u_time", time);
        
        //render stage here
        World::get_instance()->render();

        nMesh = NULL;
        nShader = World::get_instance()->shader;
        if (nShader)
        {
            //enable shader
            nShader->enable();


            for (int i = 0; i < projectiles.size(); i++)
            {
                if (projectiles.at(i)->lifeTime > 0.f)
                {
                    //upload uniforms per bullet
                    nShader->setUniform("u_color", projectiles.at(i)->color);
                    nShader->setUniform("u_viewprojection", camera->viewprojection_matrix);
                    nShader->setUniform("u_texture", projectiles.at(i)->texture, 0);
                    nShader->setUniform("u_model", projectiles.at(i)->model);
                    nMesh = projectiles.at(i)->mesh;
                    nMesh->render(GL_TRIANGLES); // render the bullet
                }
            }

            for (int i = 0; i < enemies.size(); i++)
            {

                //upload uniforms per enemy
                nShader->setUniform("u_color", Vector4(1, 1, 1, 1));
                nShader->setUniform("u_viewprojection", camera->viewprojection_matrix);
                nShader->setUniform("u_texture", enemies.at(i)->texture, 0);
                nShader->setUniform("u_model", enemies.at(i)->model);
                if (enemies.at(i)->hp > 0)
                {
                    nMesh = enemies.at(i)->mesh;
                    nMesh->render(GL_TRIANGLES); // render enemy
                }
            }
            nShader->disable();
        }

        if (boss->hp > 0)
        {
            boss->render(); // render boss
        }

        //drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
        drawText(10, 10, "Enemies Killed: " + std::to_string(killCount), Vector3(1,1,1),3);
        drawText(Game::instance->window_width-200, 10, "Wave: " + std::to_string(wave), Vector3(1,1,1),4);
        //disable shader
        shader->disable();
    }

    cdSlot->color = Vector4(0.2f, 0.2f, 0.2f, 1);
    msSlot->color = Vector4(0.2f, 0.2f, 0.2f, 1);
    gmSlot->color = Vector4(0.2f, 0.2f, 0.2f, 1);
    
    
    HPBar = new UI(quad, World::get_instance()->HPBBTexture, Game::instance->window_width/2 + 25, 35, 500, 20, Vector4(1,1,1,1));
    HP = new UI(quad, World::get_instance()->HPTexture, Game::instance->window_width/2 + 25, 35, HPtoUI, 20, Vector4(1,1,1,1));

    cdSlot->render();
    msSlot->render();
    gmSlot->render();
    
    if(World::get_instance()->player && World::get_instance()->player->hasCdPower){
        this->cdLifeTime = World::get_instance()->player->cdCadLife;
        cdSlot->color = Vector4(1, 1, 0, 1);
        if(cdLifeTime <= lifeTimeTh)
            cdIntermitent = !cdIntermitent;
        if(cdLifeTime <= 0.0019f){
            cdIntermitent = true;
        }
        if(cdIntermitent)
            cdSlot->render();
    }

    if(World::get_instance()->player && World::get_instance()->player->hasMultishot){
        this->msLifeTime = World::get_instance()->player->multiLife;
        msSlot->color = Vector4(1, 1, 0, 1);
        if(msLifeTime <= lifeTimeTh)
            msIntermitent = !msIntermitent;
        if(msLifeTime <= 0.0019f){
            msIntermitent = true;
        }
        if(msIntermitent)
            msSlot->render();
        
    }

    if(World::get_instance()->player && World::get_instance()->player->godMode){
        this->gmLifeTime = World::get_instance()->player->immortalLife;
        gmSlot->color = Vector4(1, 1, 0, 1);
        if(gmLifeTime <= lifeTimeTh)
            gmIntermitent = !gmIntermitent;
        if(gmLifeTime <= 0.0019f){
            gmIntermitent = true;
        }
        if(gmIntermitent)
            gmSlot->render();
    }
    if(isBossLvl){
        HPBar->render();
        HP->render();
    }

    
    
    /*if(World::get_instance()->player){
        if(World::get_instance()->player->hasCdPower){
            //quad.createQuad(0+currentSlot.x, 0+currentSlot.y, 114, 75, true);
            this->lifeTime = World::get_instance()->player->cdCadLife;
            powerUpUI = new UI(quad, World::get_instance()->cdPowerUpTexture, currentSlot.x, currentSlot.y);
            //if(intermitent)
            powerUpUI->render();
        }
        if(World::get_instance()->player->hasMultishot){
            //quad.createQuad(0+currentSlot.x, 0+currentSlot.y, 114, 75, true);
            this->lifeTime = World::get_instance()->player->multiLife;
            powerUpUI = new UI(quad, World::get_instance()->msPowerUpTexture,currentSlot.x, currentSlot.y);
            //if(intermitent)
            powerUpUI->render();
        }
        if(World::get_instance()->player->godMode){
            //quad.createQuad(0+currentSlot.x, 0+currentSlot.y, 114, 75, true);
            this->lifeTime = World::get_instance()->player->immortalLife;
            powerUpUI = new UI(quad, World::get_instance()->gmPowerUpTexture,currentSlot.x, currentSlot.y);
            //if(intermitent)
            powerUpUI->render();
        }
    }
     */
}

void PlayStage::update(float seconds_elapsed){
    //color = Vector4(0.95f, 0.21f, 0.67f, 1);
    if (gradient) {
        if (walkDownX) {
            color.x -= gradientFactor * seconds_elapsed;
            if (color.x <= 0.1) {
                //walkDownZ = true;
                walkDownX = false;
                walkDownZ = true;
            }
        }
        if (!walkDownX) {
            color.x += gradientFactor * seconds_elapsed;
            if (color.x >= 0.95) {
                walkUpZ = true;
            }
        }
        if (walkDownZ) {
            color.z -= gradientFactor * seconds_elapsed;
            if (color.z <= 0.2) {
                walkDownZ = false;
            }
        }
        if (walkUpZ) {
            color.z += gradientFactor * seconds_elapsed;
            if (color.z >= 0.67) {
                walkDownX = true;
                walkUpZ = false;
            }
        }
    }
    HPtoUI = this->boss->HPbar;
    
    killCount = World::get_instance()->player->killCount;
    wave = this->currentDiff-1;
    this->bossLvl = this->currentDiff % 5;
    if (!enemyNum) {
        //loadBossLvl(seconds_elapsed);
        if (!(bossLvl == 0)){
            loadNewLvl(seconds_elapsed);
            if (isBossLvl) isBossLvl = false;
        }
        else
        {
            loadBossLvl(seconds_elapsed);
        }
        if (((bossLvl == 0) || (bossLvl == 1)) && enemyNum)
        {
            Audio::Stop(Game::instance->audioChannel);
            Game::instance->musicCd = 160.f;
        }
    }
    if (enemyNum)
    {
        if (bossLvl == 0)
        {
            if (Game::instance->musicCd > 104.f)
            {
                Game::instance->audioChannel = Audio::PlayM("data/audio/boss_wave_song.mp3");
                Game::instance->musicCd = 0.f;
            }
        }
        else
        {
            if (Game::instance->musicCd > 155.f)
            {
                Game::instance->audioChannel = Audio::PlayM("data/audio/enemy_wave_song.mp3");
                Game::instance->musicCd = 0.f;
            }
        }
    }
    
    powerUpCd += seconds_elapsed;
    if (powerUpCd > 12.f)
    {
        loadPowerUp(0.0);
        powerUpCd = 0.f;
    }
    World::get_instance()->update(seconds_elapsed);
    if(World::get_instance()->player->isDead){
        std::cout << World::get_instance()->player->killCount << std::endl;
        World::get_instance()->cleanRoot();

        World::get_instance()->player = nullptr;
        fin = true;
    }
    /*if(cdLifeTime <= lifeTimeTh){
        th -= seconds_elapsed;
        if (th <= 0.f)
        {
            th = 0.15f;
            cdIntermitent = !cdIntermitent;
        }
    }*/
    /*if(World::get_instance()->player){
        if(World::get_instance()->player->puNum == 0)
            currentSlot = slot1;
        if(World::get_instance()->player->puNum == 1)
            currentSlot = slot2;
        if(World::get_instance()->player->puNum == 2)
            currentSlot = slot3;
    }
     */
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
    fin = false;
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void MenuStage::render(){
    glClearColor(0.0f, 0.0f, 0.0f, 1);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    drawText(Game::instance->window_width/2-130, Game::instance->window_height/2-125, "hola", Vector3(1,0,0),6);
    if (Input::gamepads->connected)
    {
        drawText(Game::instance->window_width/2 - 265, Game::instance->window_height / 2 + 250, "Press B to exit to title screen", Vector3(1, 1, 1), 3);
    }
    else
    {
        drawText(Game::instance->window_width/2 - 265, Game::instance->window_height / 2 + 250, "Press RETURN to exit to title screen", Vector3(1, 1, 1), 3);
    }
    
}

void MenuStage::update(float elapsed_time){
    if (Input::wasKeyPressed(SDL_SCANCODE_RETURN)||Input::wasButtonPressed(B_BUTTON))
    {
        //World::get_instance()->cleanRoot();
        fin = true;
    }
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
    if(shader)
    {
        //enable shader
        shader->enable();

        //upload uniforms
        shader->setUniform("u_color", Vector4(1,1,1,1));
        shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
        shader->setUniform("u_texture", texture, 0);
        shader->setUniform("u_model", model);
        shader->setUniform("u_time", time);
        
        //render stage here
        World::get_instance()->render();
        
        //disable shader
        shader->disable();
    }
    drawText(Game::instance->window_width/2-130, Game::instance->window_height/2-125, "YOU DIED", Vector3(1,0,0),6);
    drawText(Game::instance->window_width/2-150, Game::instance->window_width/2-75, "Enemies Killed: " + std::to_string(killCount), Vector3(1,0,0),4);
    drawText(Game::instance->window_width/2-250, Game::instance->window_width/2-125, "You survived for: " + std::to_string(wave - 1) + " rounds", Vector3(1,0,0),4);
    if (Input::gamepads->connected)
    {
        drawText(Game::instance->window_width/2 - 225, Game::instance->window_height / 2 + 200, "Press START to play again", Vector3(1, 1, 1), 4);
        drawText(Game::instance->window_width/2 - 265, Game::instance->window_height / 2 + 250, "Or press BACK to exit to title screen", Vector3(1, 1, 1), 3);
    }
    else
    {
        drawText(Game::instance->window_width/2 - 225, Game::instance->window_height / 2 + 200, "Press R to play again", Vector3(1, 1, 1), 4);
        drawText(Game::instance->window_width/2 - 265, Game::instance->window_height / 2 + 250, "Or press F to exit to title screen", Vector3(1, 1, 1), 3);
    }
}

void EndStage::update(float elapsed_time){
    camera->move(Vector3(0.0f, 0.0f, 1)*elapsed_time);
    if (Input::wasKeyPressed(SDL_SCANCODE_R) || Input::wasButtonPressed(7))
    {
        World::get_instance()->cleanRoot();
        retry = true;
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_F) || Input::wasButtonPressed(6))
    {
        World::get_instance()->cleanRoot();
        restart = true;
    }
}

stageId EndStage::getId()
{
    return stageId::ENDING;
}
