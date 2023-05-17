//
//  entity.cpp
//  TJE_XCODE
//
//  Created by manu on 17/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include "entity.h"
#include "mesh.h"

Entity::Entity(std::string name, Matrix44 model){
    this->name = name;
    this->model = model;
}

void Entity::addChild(Entity* child){
    children.push_back(child);
}

void Entity::removeChild(Entity* child){
    children.pop_back();
}

void Entity::render(){
    for(int i = 0; i < children.size(); i++)
    children[i]->render();
}

Matrix44 Entity::getGlobalMatrix(){
    if(parent)
        return model * parent->getGlobalMatrix();
    return model;
}

EntityMesh::EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color):Entity(name, model){
    this->mesh =  mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
}

EntityMesh::render(){
    // Get the last camera that was activated
    Camera* camera = Camera::current;

    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", camera->vp);
    shader->setTexture("u_texture", texture, 1);


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();

}
EntityMesh::update(float elapsed_time){
    
}
