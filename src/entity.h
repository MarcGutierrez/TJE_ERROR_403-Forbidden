//
//  entity.h
//  TJE_XCODE
//
//  Created by manu on 11/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

/*#ifndef entity_h
#define entity_h*/

#pragma once
#include <math.h>
#include "mesh.h"
#include "string.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"


//#endif /* entity_h */

class Entity {

public:
    Entity(std::string name, Matrix44 model);         // Constructor
    virtual ~Entity();     // Destructor
    
    // Some attributes
    std::string name;
    Matrix44 model;
    // Methods that should be overwritten
    // by derived classes
    virtual void render();
    virtual void update(float elapsed_time);
    
    Entity* parent;
    
    std::vector<Entity*> children;
    
    void addChild(Entity* child);
    void removeChild(Entity* child);
    void debugChildren();

    // Some useful methods
    Matrix44 getGlobalMatrix();
};

class EntityMesh : public Entity {

public:

    // Attributes of the derived class
    Mesh* mesh = nullptr;
    Texture* texture = nullptr;
    Shader* shader = nullptr;
    Vector4 color;
    //EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    EntityMesh(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture);
    //EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture);

    // Methods overwritten from base class
    void render();
    void update(float elapsed_time);
};

class InstancedEntityMesh : public EntityMesh {
    public:
    Mesh* mesh = nullptr;
    Texture* texture = nullptr;
    Shader* shader = nullptr;
    Vector4 color;
    std::vector<Matrix44> models;

    //InstancedEntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
    InstancedEntityMesh(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture);

    // Methods overwritten from base class
    void render();
    void update(float elapsed_time);
};

class EntityPlayer : public EntityMesh{
    public:
        // Attributes of the derived class
        float speed = 1000.0f;
        Vector3 velocity = Vector3(0,0,0);
        Vector3 move_dir;
        Camera* camera = nullptr;
        //EntityPlayer(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Camera* camera);
        EntityPlayer(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, Camera* camera);
        // Methods overwritten from base class
        void render();
        void update(float elapsed_time);
};


class EntityCollider : public EntityMesh{

    public:

        bool isDynamic = false;
     
        EntityCollider(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture);
        
        // New methods
        //bool checkPlayerCollisions(Vector3 position, Vector3 &colisions);
        void render();
        void update(float elapsed_time);
};


/*class EntityEnemy : public EntityMesh{
    public:
        // Attributes of the derived class
        float speed = 1.0f;
        EntityEnemy(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);
        // Methods overwritten from base class
        void render();
        void update(float elapsed_time);
};*/

/*

//CREAR ENTITY PLAYER TAMBIEN
 entity colider
 entity enemy

 
 
 class EntityGUIelement : Entitty Mesh{}
 
 is3d
 update3Dpos
 
 render
 update(){
 
    Vector2 mouse_position = INput:mouse_position;
    
    if(button_id !: mouse_position &&
        mouse_pos.x > (position.x - size.x * 0.5) &&
        mouse_pos.x < (position.x + size.x * 0.5) &&
         mouse_pos.y > (position.y - size.x * 0.5) &&
         mouse_pos.y < (position.y + size.x * 0.5)){
    color = Vector4(1,0,0,1);
    if input::isMousePressed...
        StageManager::getInstance()->onButtonPressed()
 
 }
 }
*/
