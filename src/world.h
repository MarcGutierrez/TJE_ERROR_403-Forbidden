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
#include "mesh.h"
#include "texture.h"
#include "shader.h"


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
    
    Mesh* playerMesh = nullptr;
    Mesh* projectileMesh = nullptr;
    Mesh* powerUpMesh = nullptr;
    
    Texture* projectileTexture = nullptr;
    Texture* playerTexture = nullptr;
    Texture* powerUpTexture = nullptr;
    Texture* cdPowerUpTexture = nullptr;
    Texture* msPowerUpTexture = nullptr;
    Texture* gmPowerUpTexture = nullptr;
    
    Shader* shader = nullptr;
    
    bool free_cam = false;
    void render();
    void update(float elapsed_time);
    void cleanRoot();
    
    
};
