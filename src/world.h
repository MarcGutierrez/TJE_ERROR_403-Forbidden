//
//  world.h
//  TJE_XCODE
//
//  Created by manu on 16/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

//#ifndef world_h
//#define world_h

#pragma once
#include "entity.h"
#include "camera.h"


//#endif /* world_h */

//Entity* root;
class Camera;

class World{
public:
    static World* world;

public:
    static World* get_instance(){
        if (world != nullptr)
            return world;
        world = new World();
        return world;
    }
    
    World();
    Matrix44 model;
    Entity* root = nullptr;
    EntityPlayer* player = nullptr;
    bool free_cam = false;
    void render();
    void update(float elapsed_time);
    void cleanRoot();
    
    
};

//render
//update

//aqui van cosillas de camara

//tema mover player
/*
 player-> update(...)
 
 imicializamos todo del player
 
 Matrix44 playderModel...
 
 Vector3 velocity
 
 //aqui tema de controles
 
 move_dir normalize()
 
 player->velocity += move_dir + player->speed;
 
 bool isOnFloor = fals;
 
 
 //colisiones
 
 struct sCollisionsData{
 
 }
 
 std::vector<sCollisionsData> collisions
 if (checkPlayerCollisions(position + player->velocity * seconds_elapsed, &collsions)
 
        for (const sCollisionsDara& collisions: collisions
            /if normal is pointing upwards it means we are on the floor
            float up_factor = collision.colNormal.dot(Vector(0,1,0))
            if (up_factor > 0.8)
                isOnFloor = true
            else
                //move along wall when colliding
                Vector3 newDir = player->velocity.dot(collisoin.colNOrmal) * collision.colNormal;
                player->velocity.x -= newDir.x;
                player->velocity.z -= newdir.z
 
 
 //si no estoy en el suelo le resto una gravedad
 
 if(!isOnFloor)
    player->velocity.y += -2.5;
 else{
 
    //jump
    if(Input::Key... SPACE){
        player->velocity.z += 2.5f
    }
    else
        player.velocity.z = 0.0f
 
 }
 //update players position
 position += player->velocity * seconds_elapsed;
 
 
 //decrease velocity when not moving (simular la friccion con el suelo)
 player->velocity.x = player->velocity.x * 0.5f
 player->velocity.z = player->velocity.z * 0.5f
 
 playerModel.setTranslation(position)
 playerModel.rotate(player-> yaw, Vector3(0, 1, 0));
 
 }
 root.update(secodns_elapsed)
 */


/*IAs
 
 conn el dot product podemos ver si los vecrores de vision  estan viendo al target,
 si es mayor de 0.5 esta dentro del punto de vista de la ia
 
 mandamos raycast y si ecnuentra un obstaculo se descarta la linea de vision
 
 
 
 */
