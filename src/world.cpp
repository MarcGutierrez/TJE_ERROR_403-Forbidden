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

// Container to store EACH collision
struct sCollisionData {
    Vector3 colPoint;
    Vector3 colNormal;
};

std::vector<sCollisionData> collisions;

World::World(){
    root = new Entity("root", this->model);
    
}


bool checkCollisions(const Vector3& target_pos,
std::vector<sCollisionData>& collisions) {
    Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
    float sphereRadius = 0.75f;
    Vector3 colPoint, colNormal;

    // For each collider entity “e” in root:
    //for(auto e:World::world->get_instance()->root->children){
    for(int i = 0; i<World::world->get_instance()->root->children.size(); i++){
        if (EntityCollider* e = dynamic_cast<EntityCollider*>(World::world->get_instance()->root->children[i])){
            Mesh* mesh = e->mesh;
            
            if (mesh->testSphereCollision(e->model, center,
                                          sphereRadius, colPoint, colNormal)) {
                collisions.push_back({ colPoint,
                    colNormal.normalize() });
            }
        }
        // End loop
    }
    return !collisions.empty();
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
    if (checkCollisions(player->model.getTranslation() + player->velocity, collisions)){
        
        for (const sCollisionData& collisions: collisions){

            Vector3 newDir = player->velocity.dot(collisions.colNormal) * collisions.colNormal;
            player->velocity.x -= newDir.x;
            player->velocity.z -= newDir.z;
        }
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
