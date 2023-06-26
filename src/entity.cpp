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
#include "game.h"

// Container to store EACH collision
struct sCollisionData {
    Vector3 colPoint;
    Vector3 colNormal;
};

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

void shoot(Matrix44 model, float speed){
    Mesh* mesh;
    Shader* shader;
    Texture* texture;
    
    texture = new Texture();
    texture->load("data/texture.tga");

    // example of loading Mesh from Mesh Manager
    mesh = Mesh::Get("data/projectile.obj");

    // example of shader loading using the shaders manager
    shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    
    float dmg = 0.0f;
    Vector3 dir = model.frontVector();
    model.translate(0.f, 0.0f, 0.f);
    
    EntityProjectile* bullet = new EntityProjectile(model, mesh, shader, texture, speed, dmg, dir);
    World::world->get_instance()->root->addChild(bullet);
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

bool checkCollisions(const Vector3& target_pos,
    std::vector<sCollisionData>& collisions) {
    Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
    float sphereRadius = .75f;
    Vector3 colPoint, colNormal;

    // For each collider entity “e” in root:
    //for(auto e:World::world->get_instance()->root->children){
    for (int i = 0; i < World::world->get_instance()->root->children.size(); i++) {
        if (EntityCollider* e = dynamic_cast<EntityCollider*>(World::world->get_instance()->root->children[i])) {
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

void EntityPlayer::update(float elapsed_time){
    yaw += Input::mouse_delta.x * elapsed_time * 15.0f;
    
    move_dir = Vector3(0.f, 0.f, 0.f);
    
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        //model.translate(0.0f, 0.0f, -1.0f * move_speed);
        move_dir = Vector3(0.0f + move_dir.x, 0.0f, -1.0f + move_dir.z);
        
        //velocity = move_dir * speed;
        //position = position + velocity * elapsed_time;
        //model.setTranslation(position.x, 51.0f, position.z);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        //model.translate(0.0f, 0.0f, 1.0f * move_speed);
        move_dir = Vector3(0.0f + move_dir.x, 0.0f, 1.0f + move_dir.z);
        //velocity = move_dir * speed;
        //position = position + velocity * elapsed_time;
        //model.setTranslation(position.x, 51.0f, position.z); //el 51 es hardcodeado por la mesh del cubo (se
    }
    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        //model.translate(-1.0f * move_speed, 0.0f, 0.0f);
        move_dir = Vector3(-1.0f + move_dir.x, 0.0f, 0.0f + move_dir.z);
        //velocity = move_dir * speed;
        //position = position + velocity * elapsed_time;
        //model.setTranslation(position.x, 51.0f, position.z); //el 51 es hardcodeado por la mesh del cubo (se
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        //model.translate(1.0f * move_speed, 0.0f, 0.0f);
        move_dir = Vector3(1.0f + move_dir.x, 0.0f, 0.0f + move_dir.z);
        //velocity = move_dir * speed;
        //position = position + velocity * elapsed_time;
        //model.setTranslation(position.x, 51.0f, position.z); //el 51 es hardcodeado por la mesh del cubo (se
    }
    Vector3 position = model.getTranslation();
    
    move_dir.normalize();
    velocity = velocity + move_dir * speed;
    position.y = 51.0;
    std::vector <sCollisionData> collisions;
    if (checkCollisions(position + velocity * elapsed_time, collisions)) {
        //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        for (const sCollisionData& collisions : collisions) {
            
            //Vector3& velocity = velocity;
            Vector3 newDir = velocity.dot(collisions.colNormal) * collisions.colNormal;
            velocity.x -= newDir.x;
            velocity.z -= newDir.z;
        }
    } else {
        
    }
    

    std::cout << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;
    position = position + velocity * elapsed_time;
    velocity = velocity - velocity * elapsed_time * 50;
    
    model.setTranslation(position.x, 51.0f, position.z); //el 51 es hardcodeado por la mesh del cubo (se tiene en cuenta el centro de la mesh)
    if (Input::mouse_position.y < Game::instance->window_height/2){
        model.rotate(yaw, Vector3(0.0f, -1.0f, 0.0f));
    }
    if (Input::mouse_position.y >= Game::instance->window_height/2){
        model.rotate(yaw, Vector3(0.0f, 1.0f, 0.0f));
    }
    //model.rotate(yaw, Vector3(0.0f, 1.0f, 0.0f));

    //camera->lookAt(camera->eye, camera->center, camera->up);
    if  (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
        shoot(model, 50.0f);
    }
}


EntityCollider::EntityCollider(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture):EntityMesh(model,mesh,shader,texture){
}

void EntityCollider::render(){
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

void EntityCollider::update(float elapsed_time){
}

EntityProjectile::EntityProjectile(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, float speed, float dmg, Vector3 dir):EntityCollider(model, mesh, shader, texture){
    this->speed = speed;
    this->dmg = dmg;
    this->dir = dir;
}

void EntityProjectile::render(){
    
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

struct sImpactData {
    Vector3 impPoint;
    Vector3 impNormal;
};

bool checkImpacts(const Vector3& target_pos,
    std::vector<sImpactData>& impacts) {
    Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
    float sphereRadius = .75f;
    Vector3 impPoint, impNormal;

    // For each collider entity “e” in root:
    //for(auto e:World::world->get_instance()->root->children){
    for (int i = 0; i < World::world->get_instance()->root->children.size(); i++) {
        if (EntityCollider* e = dynamic_cast<EntityCollider*>(World::world->get_instance()->root->children[i])) {
            Mesh* mesh = e->mesh;

            if (mesh->testSphereCollision(e->model, center,
                sphereRadius, impPoint, impNormal)) {
                impacts.push_back({ impPoint,
                    impNormal.normalize() });
            }
        }
        // End loop
    }
    return !impacts.empty();
}

void EntityProjectile::update(float elapsed_time){
    
    Vector3 position = model.getTranslation();
    
    dir.normalize();
    velocity = velocity + dir * speed;
    
    

    //Vector3 new_position = model.getTranslation() + velocity * elapsed_time;
    //model.setTranslation(position.x, position.y, position.z);
    //model.setTranslation(new_position.x, new_position.y, new_position.z);
    //velocity = velocity + dir * speed;
    float height = model.getTranslation().y;
    position.y = height;
    
    std::vector<sImpactData> impacts;
    if (checkImpacts(position + velocity * elapsed_time, impacts)) {
        
        World::world->get_instance()->root->removeChild(this);
        
    } else {
        
    }
    position = position + velocity * elapsed_time;
    //position = position + velocity * elapsed_time;
    /*
     Vector3 position = model.getTranslation();
     
     move_dir.normalize();
     velocity = velocity + move_dir * speed;
     
     position = position + velocity * elapsed_time;
     velocity = velocity - velocity * elapsed_time * 50;
     
     model.setTranslation(position.x, 51.0f, position.z); //el 51 es hardcodeado por la mesh del cubo (se tiene en cuenta el centro de la mesh)
     */
    
    
    //Vector3 new_position = model.getTranslation() + velocity * elapsed_time;
    model.setTranslation(position.x, position.y, position.z);
    
}

/*void EntityProjectile::update(float elapsed_time){
    Vector3 velocity = dir * speed;
    Vector3 new_position = model.getTranslation() + velocity * elapsed_time;
    model.setTranslation(new_position.x, new_position.y, new_position.z);
    
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

