//
//  entity.h
//  TJE_XCODE
//
//  Created by manu on 11/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#ifndef entity_h
#define entity_h

#include "mesh.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"


#endif /* entity_h */

class Entity {

public:
    // Some attributes
    std::string name;
    Matrix44 model;
    Entity(std::string name, Matrix44 model);                      // Constructor
    virtual ~Entity();     // Destructor
    
    // Methods that should be overwritten
    // by derived classes
    virtual void render();
    virtual void update(float elapsed_time);
    
    Entity* parent;
    
    std::vector<Entity*> children;
    
    void addChild(Entity* child);
    void removeChild(Entity* child);

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
    EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color);

    // Methods overwritten from base class
    void render();
    void update(float elapsed_time);
};


void EntityMesh::render() {

    // Get the last camera that was activated
    Camera* camera = Camera::current;

    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", camera->eye);
    shader->setTexture("u_texture", texture, 0 );


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();
};

/*class EntityCollider :public EntityMesh {

 p*ublic:


bool isDynamic = false;

// New methods
bool testCollision(...);
};

//CREAR ENTITY PLAYER TAMBIEN

*/
