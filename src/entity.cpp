//
//  entity.cpp
//  TJE_XCODE
//
//  Created by manu on 17/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include "entity.h"
#include "input.h"
#include "world.h"


Entity::Entity(std::string name, Matrix44 model){
    this->name = name;
    this->model = model;
};
Entity::~Entity() {
    
};


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
void Entity::update(float elapsed_time){
    for(int i = 0; i < children.size(); i++)
    children[i]->update(elapsed_time);
}

Matrix44 Entity::getGlobalMatrix(){
    if(parent)
        return model * parent->getGlobalMatrix();
    return model;
}

void Entity::debugChildren(){
    for(int i = 0; i < children.size(); i++)
        std::cout << i << ' ';
}

/*EntityMesh::EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color):Entity(name, model){
    this->mesh =  mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
}*/


EntityMesh::EntityMesh(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture) : Entity("Default", model)
{
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
}

/*EntityMesh::EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture) : Entity(name, Matrix44())
{
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
}*/

void EntityMesh::render(){
    // Get the last camera that was activated
    Camera* camera = Camera::current;

    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();

}

void EntityMesh::update(float elapsed_time){
    
}


/*InstancedEntityMesh::InstancedEntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color):EntityMesh(name, model, mesh, texture, shader, color){
    this->mesh =  mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
    models = std::vector<Matrix44>();
}
*/
InstancedEntityMesh::InstancedEntityMesh(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture):EntityMesh(model, mesh, shader, texture)
{
    models = std::vector<Matrix44>();
}

void InstancedEntityMesh::render(){
    // Get the last camera that was activated
    Camera* camera = Camera::current;

    // Enable shader and pass uniforms
    shader->enable();
    for(int i = 0; i < this->models.size(); i++){
        shader->setUniform("u_model", this->models[i]);
        shader->setUniform("u_viewproj", camera->viewprojection_matrix);
        shader->setTexture("u_texture", texture, 0);
        
        // Render the mesh using the shader
        mesh->render( GL_TRIANGLES );
    }


    // Disable shader after finishing rendering
    shader->disable();

}

void InstancedEntityMesh::update(float elapsed_time){
    
}

/*EntityPlayer::EntityPlayer(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color, Camera* camera):EntityMesh(name, model, mesh, texture, shader, color){
    this->camera = camera;
    
}*/

EntityPlayer::EntityPlayer(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, Camera* camera):EntityMesh(model, mesh, shader, texture){
    this->camera = camera;
}


void EntityPlayer::render(){
    // Get the last camera that was activated
    Camera* camera = Camera::current;

    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();

}

void EntityPlayer::update(float elapsed_time){
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        model.translate(0.0f, 0.0f, -1.0f * speed);
        camera->lookAt(camera->eye, camera->center, camera->up);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        model.translate(0.0f, 0.0f, 1.0f * speed);
        camera->lookAt(camera->eye, camera->center, camera->up);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        model.translate(-1.0f * speed, 0.0f, 0.0f);
        camera->lookAt(camera->eye, camera->center, camera->up);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        model.translate(1.0f * speed, 0.0f, 0.0f);
        camera->lookAt(camera->eye, camera->center, camera->up);
    }
}


/*EntityCollider::EntityCollider(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture):EntityMesh(model, mesh, shader, texture){
}


// Check if collides with wall using sphere (radius = 2)

/*bool checkPlayerCollisions(const Vector3& target_pos,
std::vector<sCollisionData>& collisions) {
    Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
    float sphereRadius = 0.75f;
    Vector3 colPoint, colNormal;

    // For each collider entity “e” in root:
    for(auto e:World::world->root->children){
        if (EntityCollider* e = dynamic_cast<EntityCollider*>(e)){
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
}*/


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

