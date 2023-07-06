//
//  world.cpp
//  TJE_XCODE
//
//  Created by manu on 17/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include <stdio.h>
#include "world.h"
#include "entity.h"
#include "input.h"

World* World::world;

World::World(){
    
    root = new Entity("root", this->model);
    
    playerMesh = Mesh::Get("data/export.MESH");
    projectileMesh = Mesh::Get("data/projectile.obj");
    projectileTexture = Texture::Get("data/textures/powerup_texture.tga");
    playerTexture = Texture::Get("data/textures/vanguard_diffuse1.tga");
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    
    powerUpMesh = Mesh::Get("data/powerUp.obj");
    powerUpTexture = Texture::Get("data/textures/powerup_texture.tga");
    
    cdPowerUpTexture = Texture::Get("data/textures/cdPowerUpIcon.png");
    msPowerUpTexture = Texture::Get("data/textures/msPowerUpIcon.png");
    gmPowerUpTexture = Texture::Get("data/textures/gmPowerUpIcon.png");
    
    HPBBTexture = Texture::Get("data/textures/Border_Style_1.png");
    HPTexture = Texture::Get("data/textures/Style_1.png");
    
}

void World::render(){
    root->render();
    if(player){
        player->render();
        
    }
}

void World::update(float elapsed_time){
    
    player->update(elapsed_time);
    root->update(elapsed_time);
}
void World::cleanRoot(){
    for (int i = root->children.size() - 1; i >= 0; i--) { //clean root
        root->removeChild(root->children[i]);
    }
}


/*if (checkPlayerCollisions(position + player->velocity * seconds_elapsed, &collisions)

   for (const sCollisionsDara& collisions: collisions)
       //if normal is pointing upwards it means we are on the floor
       float up_factor = collision.colNormal.dot(Vector(0,1,0))
       if (up_factor > 0.8)
           isOnFloor = true
       else
           //move along wall when colliding
           Vector3 newDir = player->velocity.dot(collision.colNormal) * collision.colNormal;
           player->velocity.x -= newDir.x;
           player->velocity.z -= newdir.z*/
/*
 bool World::checkLineOfSight(const Matrix44& obs, const Matrix44& target)
    Vecror3 front = normalize (obs.frontVector())
    Vector3 toTarget = normalize(target.getTranslation() - obs.getTranslation())
 
    Vector3 ray_origin =  obs.getTranslation()
    Vector3 ray_direction = toTarget;
    float distance = toTarget.length();
    
    
 
 if(toTarget.dot(front) > 0.5 //90 grados
    for(auto e:root.children)
        EntityColider* ec = dynamic_cast<EntityColider*>(e);
        if(!ec) continue
        
        ec->testRayColision(
            ec->model
            ray_origin
            ray_direction
            Vector3(),
            Vector3(),
            distance
        )){
    return true
 
 return false
 */
