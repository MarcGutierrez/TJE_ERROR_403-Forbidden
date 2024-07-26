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

#include "random.h"

Vector3 dirShoot, newDir, center, colPoint, colNormal, position, move_dir;
float sphereRadius, normalShootSpd, multiShootSpd, normDist;
Vector2 mouse_pos;
bool isBoss;
EntityPlayer* e;
EntityBoss* b;
EntityCollider* c;
EntityProjectile* p;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
// Container to store EACH collision
struct sCollisionData {
    Vector3 colPoint;
    Vector3 colNormal;
};

std::vector <sCollisionData> collisions;

Entity::Entity(std::string name, Matrix44 model){
    this->name = name;
    this->model = model;
}

Entity::~Entity() {
    delete parent;
    children.clear();
    children.shrink_to_fit();
}


void Entity::addChild(Entity* child){
    children.push_back(child);
    child->parent = this;
}

void Entity::removeChild(Entity* child){
    auto it = std::find(children.begin(), children.end(), child);
    if (it == children.end()) {
        std::cout << "Child not in list" << std::endl;
        return;
    }
    
    children.erase(it);
    child->parent = nullptr;
}

void Entity::render(){
    for (int i = 0; i < children.size(); i++)
    {
        if (dynamic_cast<EntityProjectile*>(children[i])) continue;
        if (dynamic_cast<EntityAI*>(children[i])) continue;
        children[i]->render();
    }
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

EntityMesh::~EntityMesh()
{
    delete mesh;
    delete texture;
    delete shader;
}

/*EntityMesh::EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture) : Entity(name, Matrix44())
{
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
}*/

void EntityMesh::render(){
    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
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

InstancedEntityMesh::~InstancedEntityMesh()
{
    delete mesh;
    delete texture;
    delete shader;
    models.clear();
    models.shrink_to_fit();
}

void InstancedEntityMesh::render(){
    // Enable shader and pass uniforms
    shader->enable();
    for(int i = 0; i < this->models.size(); i++){
        shader->setUniform("u_model", this->models[i]);
        shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
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
    this->hp = 750;
    this->yaw = 0.f;
    this->isDead = false;
    this->godMode = false;
    
    
    this->anim = IDLE;
    idle = Animation::Get("data/animations/idle.skanim");
    
    killCount = 0;
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    speed = 1500.0f;
    shootCd = 0.f;
    hasMultishot = false;
    hasCdPower = false;
    cdPowerUp = 0.f;
    cdCadLife = 0.f;
    multiLife = 0.f;
    puNum = 0;
}

EntityPlayer::~EntityPlayer()
{
    delete idle;
    delete walk;
    delete walkBack;
    delete walkLeft;
    delete walkRight;
    delete shot;
    delete die;
    delete camera;
    delete mesh;
    delete texture;
    delete shader;
}

void shoot(Matrix44 model, float speed, float dispersion, bool isEnemy){
    dirShoot = model.frontVector();
    if (dispersion)
    {
        dirShoot.x += random(dispersion, -dispersion / 2);
        dirShoot.z += random(dispersion, -dispersion / 2);
    }
    model.translate(0.0f, 51.0f, 51.f);

    if (((PlayStage*)Game::instance->current_stage)->projectiles.size() < 50) // to check max of bullets in array
    {
        // generates and expands the bullet array if not fully grown
        EntityProjectile* bullet = new EntityProjectile(model, World::get_instance()->projectileMesh,
            World::get_instance()->shader, World::get_instance()->projectileTexture, speed, 0.f, dirShoot, isEnemy);
        ((PlayStage*)Game::instance->current_stage)->projectiles.push_back(bullet);
        World::world->get_instance()->root->addChild(bullet);
    }
    else
    {
        for (EntityProjectile* p : ((PlayStage*)Game::instance->current_stage)->projectiles)
        {
            if (p->lifeTime <= 0.f)
            {
                // regenerats a bullet if array at full size
                p->model = model;
                p->mesh = World::get_instance()->projectileMesh;
                p->shader =
                    World::get_instance()->shader;
                p->texture = World::get_instance()->projectileTexture;
                p->speed = speed;
                p->dir = dirShoot;
                p->isEnemy = isEnemy;
                p->lifeTime = 4.f;

                World::world->get_instance()->root->addChild(p);
                break;
            }
        }
    }

    if (random() > 0.5f) Audio::PlayS("data/audio/363698__jofae__retro-gun-shot.mp3");
    else Audio::PlayS("data/audio/mixkit-game-gun-shot-1662.mp3");

    //std::cout << stage->projectiles.size() << std::endl;
}

void multishot(Matrix44 model, float speed, int bulletsShoot, float dispersion, bool isEnemy){
    
    dirShoot = model.frontVector();
    int sign = (sgn(dirShoot.x) * sgn(dirShoot.z));
    /*if (dispersion) //multishot sin dispersion
    {
        dir.x += random(dispersion, -dispersion / 2);
        dir.z += random(dispersion, -dispersion / 2);
    }*/
    model.translate(0.0f, 51.0f, 51.f);
    int range = bulletsShoot / 2;
    for (int i = -range; i <= range; i++) // para generar el numero de balas adecuado
    {
        newDir = dirShoot - Vector3(i * dispersion, 0, sign*i * dispersion); // nueva direccion con dispersion de bala aplicado
        newDir.normalize(); // normaliza la direccion para que las balas vayan a la misma velocidad

        if (((PlayStage*)Game::instance->current_stage)->projectiles.size() < 50) // to check that we have a maximum of bullets
        {
            // to fill the array with the new entities
            EntityProjectile* newBullet;
            newBullet = new EntityProjectile(model, World::get_instance()->projectileMesh, World::get_instance()->shader, World::get_instance()->projectileTexture, speed, 0.f, newDir, isEnemy);
            ((PlayStage*)Game::instance->current_stage)->projectiles.push_back(newBullet);
            World::world->get_instance()->root->addChild(newBullet);
        }
        else
        {
            for (EntityProjectile* p : ((PlayStage*)Game::instance->current_stage)->projectiles)
            {
                if (p->lifeTime <= 0.f)
                {
                    // reloads the bullet entities
                    p->model = model;
                    p->mesh = World::get_instance()->projectileMesh;
                    p->shader = World::get_instance()->shader;
                    p->texture = World::get_instance()->projectileTexture;
                    p->speed = speed;
                    p->dir = newDir;
                    p->isEnemy = isEnemy;
                    p->lifeTime = 4.f;
                    World::world->get_instance()->root->addChild(p);
                    break;
                }
            }
        }
    }
    if (random() > 0.5f) Audio::PlayS("data/audio/363698__jofae__retro-gun-shot.mp3");
    else Audio::PlayS("data/audio/mixkit-game-gun-shot-1662.mp3");
}

void youDie(Entity* entity, EntityProjectile* p){
    if (p->isEnemy){
        if(e = dynamic_cast<EntityPlayer*>(entity)){
            if (!e->godMode){
                e->isDead = true;
                Audio::Play("data/audio/videogame-death-sound-43894.mp3");
            }
            //std::cout << "u suck" << std::endl;
        }
    }
    else{
        if (b = dynamic_cast<EntityBoss*>(entity))
        {
            b->hp--;

            b->hasBeenAttacked = true;
            b->isHurt = true;
            b->color = Vector4(1, 0, 0, 1);

            p->lifeTime = 0.f;
            World::get_instance()->root->removeChild(p);
            //std::cout << b->hp << std::endl;
            if (b->hp == 0)
            {
                b->color = Vector4(1, 1, 1, 1);
                World::get_instance()->root->removeChild(entity);
                ((PlayStage*)Game::instance->current_stage)->enemyNum--;
                World::get_instance()->player->killCount++;
                b->HPbar = 0;
                Audio::Play("data/audio/expl6.wav");
            }
            else{
                Audio::Play("data/audio/hitmarker_2.mp3");
                b->HPbar -= 500/b->maxhp;
            }
        }
        else
        {
            ((EntityAI*)entity)->hp = 0;
            World::get_instance()->root->removeChild(entity);
            p->lifeTime = 0.f;
            World::get_instance()->root->removeChild(p);
            ((PlayStage*)Game::instance->current_stage)->enemyNum--;
            World::get_instance()->player->killCount++;
            Audio::Play("data/audio/expl6.wav");
        }
    }
}

void EntityPlayer::render(){
    
    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    if (anim == IDLE) {
        idle->assignTime(Game::instance->time);
        mesh->renderAnimated(GL_TRIANGLES, &idle->skeleton);
    }
    //mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();

}

void EntityPlayer::addPowerUp(EntityPowerUp* pu)
{
    switch (pu->effect)
    {
    case MORECADENCE:
        hasCdPower = true;
        cdPowerUp += 0.15f;
        cdCadLife = 10.f;
        puNum += 1;
        break;
    case MULTISHOT:
        hasMultishot = true;
        cdPowerUp -= 0.2f;
        multiLife = 10.f;
        puNum += 1;
        break;
    case IMMORTAL:
        godMode = true;
        immortalLife = 10.f;
        puNum += 1;
        break;
    default:
        break;
    }
}

bool checkCollisions(const Vector3& target_pos, std::vector<sCollisionData>& collisions, EntityMesh* entity, float radiusSphere) {
    center = target_pos + Vector3(0.f, 51.25f, 0.f);
    sphereRadius = radiusSphere;

    // For each collider entity “e” in root:
    //for(auto e:World::world->get_instance()->root->children){
    for (int i = 0; i < World::world->get_instance()->root->children.size(); i++) {
        if (c = dynamic_cast<EntityCollider*>(World::world->get_instance()->root->children[i])) {
            if (p = dynamic_cast<EntityProjectile*>(c)) {
                if (c->mesh->testSphereCollision(c->model, center, sphereRadius, colPoint, colNormal)) {
                    youDie(entity, p);
                }
            }
            else
            {
                if (c->mesh->testSphereCollision(c->model, center, sphereRadius, colPoint, colNormal)) {
                    if (colNormal.x != 0.f || colNormal.z != 0.f)
                        collisions.push_back({ colPoint, colNormal.normalize() });
                }
            }
            // End loop
        }
    }
    return !collisions.empty();
}

Vector3 lookingAt()
{
    mouse_pos = Input::mouse_position;
    return Vector3((mouse_pos.x / Game::instance->window_width) * 8200 - 4100, 51.f, (mouse_pos.y / Game::instance->window_height) * 6000 - 3000);;
}

void EntityPlayer::update(float elapsed_time){
    position = model.getTranslation();

    if (cdCadLife > 0.f)
    {
        cdCadLife -= elapsed_time;
        if (cdCadLife <= 0.f)
        {
            cdPowerUp -= .15f;
            hasCdPower = false;
            puNum -= 1;
        }
    }
    if (multiLife > 0.f)
    {
        multiLife -= elapsed_time;
        if (multiLife <= 0.f)
        {
            cdPowerUp += 0.2f;
            hasMultishot = false;
            puNum -= 1;
        }
    }
    if (immortalLife > 0.f)
    {
        immortalLife -= elapsed_time;
        if (immortalLife <= 0.f)
        {
            godMode = false;
            puNum -= 1;
        }
    }
    
    move_dir = Vector3(0.0f, 0.0f, 0.0f);
    
    if (Input::isKeyPressed(SDL_SCANCODE_W)) {
        move_dir = Vector3(0.0f + move_dir.x, 0.0f, -1.0f + move_dir.z);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_S)) {
        move_dir = Vector3(0.0f + move_dir.x, 0.0f, 1.0f + move_dir.z);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_A)) {
        move_dir = Vector3(-1.0f + move_dir.x, 0.0f, 0.0f + move_dir.z);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_D)) {
        move_dir = Vector3(1.0f + move_dir.x, 0.0f, 0.0f + move_dir.z);
    }
    if (Input::gamepads->axis[0] != 0.f || Input::gamepads->axis[1] != 0.f)
    {
        move_dir = Vector3(Input::gamepads->axis[0], 0, Input::gamepads->axis[1]);
    }
    
        //Debug hotkeys disabled

        //if (Input::wasKeyPressed(SDL_SCANCODE_X) || Input::wasButtonPressed(8)) {
        //    this->godMode = !this->godMode;
        //    if (godMode)
        //        std::cout << "God Mode Activated" << std::endl;
        //    if (!godMode)
        //        std::cout << "God Mode Deactivated" << std::endl;
        //}
        /*if (Input::wasKeyPressed(SDL_SCANCODE_Z) || Input::wasButtonPressed(9)) {
            this->hasMultishot = !this->hasMultishot;
            if (hasMultishot)
                std::cout << "Multishot Activated" << std::endl;
            if (!hasMultishot)
                std::cout << "Multishot Deactivated" << std::endl;
        }
        if (Input::wasKeyPressed(SDL_SCANCODE_C) || Input::wasButtonPressed(10)) {
            this->hasCdPower = !this->hasCdPower;
            if (hasCdPower)
                std::cout << "Rapid Fire Activated" << std::endl;
            if (!hasCdPower)
                std::cout << "Rapid Fire Deactivated" << std::endl;
        }*/
    
    move_dir.normalize();
    velocity = move_dir * speed;
    collisions = std::vector <sCollisionData>();
    position.y = 0.0f; //el 51 es hardcodeado por la mesh del cubo (se tiene en cuenta el centro de la mesh)
    if (checkCollisions(position + velocity * elapsed_time, collisions, this, 12.5f)) {
        for (const sCollisionData& collisions : collisions) {
            newDir = Vector3(collisions.colNormal.x, 0.f, collisions.colNormal.z).normalize();
            velocity.x += newDir.x * speed;
            velocity.z += newDir.z * speed;
        }
    }
    position = position + velocity * elapsed_time;
    
    velocity -= velocity * elapsed_time;
    
    model.setTranslation(position.x, position.y, position.z); // position.y = 0 harcoceado

    if (Input::gamepads->axis[2] != 0.f || Input::gamepads->axis[3] != 0.f)
    {
        yaw = this->model.getYawRotationToAimTo(Vector3(this->model.getTranslation().x + Input::gamepads->axis[2], 51.f, this->model.getTranslation().z + Input::gamepads->axis[3]));
    }
    else
    {
        yaw = this->model.getYawRotationToAimTo(lookingAt());
    }
    model.rotate(yaw, Vector3(0.0f, 1.0f, 0.0f));

    shootCd += elapsed_time;
    //camera->lookAt(camera->eye, camera->center, camera->up);
    if (Input::isKeyPressed(SDL_SCANCODE_SPACE) || Input::isButtonPressed(5)) {
        if (shootCd + cdPowerUp > .3f)
        {
            if (hasMultishot)
                multishot(model, 2500.f, 5, 0.15f, false);
            else
                shoot(model, 4000.0f, 0, false);
            shootCd = 0.f;
        }
    }
}

EntityAI::EntityAI(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, int hp, float speed, float cdShot, float dispersion)
    :EntityCollider(model, mesh, shader, texture) {
    this->hp = hp;
    this->maxhp = hp;
    this->speed = speed;
    this->currentBehaviour = NOTHING;
    this->cdShot = cdShot;
    this->dispersion = dispersion;
    this->yaw = 0.f;
    this->shotCdTime = 0.f;
    this->wanderChange = 30.f;
    this->hasBeenAttacked = false;
}

EntityAI::~EntityAI() {}

void EntityAI::render()
{
    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render(GL_TRIANGLES);

    // Disable shader after finishing rendering
    shader->disable();
}

bool EntityAI::canSeePlayer()
{
    position = World::get_instance()->player->model.getTranslation() - this->model.getTranslation();
    position = position.normalize();
    dirShoot = this->model.frontVector();
    dirShoot = dirShoot.normalize();
    return (dirShoot.dot(position) >= .5f) ? true : false;
}

void EntityAI::behaviourUpdate()
{
    if (canSeePlayer() || hasBeenAttacked)
        currentBehaviour = ATTACK;
    else currentBehaviour = WANDER;
}

Vector3 get_center_dir(Vector3 pos)
{
    return Vector3(-pos.x, 0, -pos.z).normalize();
}

void takeAction(EntityAI* entity, Vector3 position, float elapsed_time)
{
    isBoss = (dynamic_cast<EntityBoss*>(entity) ? true : false);
    multiShootSpd = 2500.f;
    normalShootSpd = 3000.f;
    if (b = dynamic_cast<EntityBoss*>(entity)) if(b->isHurt) b->hurtFrames += elapsed_time;

    switch (entity->currentBehaviour)
    {
    case ATTACK:
        entity->move_dir = World::get_instance()->player->model.getTranslation() - entity->model.getTranslation();
        entity->shotCdTime += elapsed_time;
        if (entity->shotCdTime > entity->cdShot)
        {
            if (b = dynamic_cast<EntityBoss*>(entity))
            {
                multishot(b->model, multiShootSpd, b->numBulletsShoot, b->dispersion, true);
            }
            else
                shoot(entity->model, normalShootSpd, entity->dispersion, true);
            entity->shotCdTime = 0.f;
        }
        normDist = entity->model.getTranslation().distance(World::get_instance()->player->model.getTranslation())/multiShootSpd;

        if (Game::instance->isHard || isBoss)
            entity->yaw += entity->model.getYawRotationToAimTo
            (
            World::get_instance()->player->model.getTranslation() + (World::get_instance()->player->velocity*0.5) * normDist
            );
        else
            entity->yaw += entity->model.getYawRotationToAimTo
            (
                World::get_instance()->player->model.getTranslation()
            );
        //
        if (isBoss)
        {
            //if (entity->move_dir.length() < 4000.f)
            if (entity->move_dir.length() < 4000.f)
                entity->move_dir = Vector3(0.f, 0.f, 0.f);
        }
        else
        {
            if (entity->move_dir.length() < 2500.f)
                entity->move_dir = Vector3(0.f, 0.f, 0.f);
        }
        break;
    case WANDER:
        //entity->speed = entity->speed - 800;
        if (entity->wanderChange > 2.f)
        {
            center = get_center_dir(entity->model.getTranslation());
            entity->move_dir = Vector3(center.x + get_random_dir(), 0.f, center.y + get_random_dir());
            entity->wanderChange = .0f;
        }
        entity->yaw += entity->model.getYawRotationToAimTo(position + entity->move_dir);
        //entity->speed = entity->speed + 800;
        break;
    default:
        break;
    }
    entity->move_dir.normalize();
}

void checkCollisions(EntityAI* entity, Vector3 position, float elapsed_time)
{
    collisions = std::vector <sCollisionData>();
    if (checkCollisions(position + entity->velocity * elapsed_time, collisions, entity, 20.f)) {
        //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        for (const sCollisionData& collisions : collisions) {
            //Vector3& velocity = velocity;
            newDir = Vector3(collisions.colNormal.x, 0.f, collisions.colNormal.z).normalize();
            entity->velocity.x += newDir.x * entity->speed;
            entity->velocity.z += newDir.z * entity->speed;
        }
    }
    /*else if(entity == dynamic_cast<EntityBoss*>(entity)){
        if (checkCollisions(position + entity->velocity * elapsed_time, collisions, entity, 62.5f)) {
            //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
            for (const sCollisionData& collisions : collisions) {
                //Vector3& velocity = velocity;
                Vector3 newDir = Vector3(collisions.colNormal.x, 0.f, collisions.colNormal.z).normalize();
                entity->velocity.x += newDir.x * entity->speed;
                entity->velocity.z += newDir.z * entity->speed;
            }
        }
    }*/
}

void checkBossCollisions(EntityBoss* entity, Vector3 position, float elapsed_time){
    if (checkCollisions(position + entity->velocity * elapsed_time, collisions, entity, 80.f)) {
        //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
        for (const sCollisionData& collisions : collisions) {
            //Vector3& velocity = velocity;
            newDir = Vector3(collisions.colNormal.x, 0.f, collisions.colNormal.z).normalize();
            entity->velocity.x += newDir.x * entity->speed;
            entity->velocity.z += newDir.z * entity->speed;
        }
    }
}

void EntityAI::update(float elapsed_time)
{
    wanderChange += elapsed_time;
    position = model.getTranslation();
    // yaw = degree between player and enemy; acos or asin? but that's inneficient
    behaviourUpdate();
    takeAction(this, position, elapsed_time);

    velocity = move_dir * speed;
    position = model.getTranslation();
    position.y = 0.0f; //el 51 es hardcodeado por la mesh del cubo (se tiene en cuenta el centro de la mesh)

    checkCollisions(this, position, elapsed_time);
    
    position = position + velocity * elapsed_time;
    
    velocity -= velocity * elapsed_time;

    model.setTranslation(position.x, position.y, position.z); // position.y = 51 harcoceado
    model.rotate(yaw, Vector3(0.0f, 1.0f, 0.0f));
    //color = Vector4(1, 1, 1, 1);
}


EntityBoss::EntityBoss(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, int hp, float speed, float cdShot, float dispersion, int numBulletsShoot)
    :EntityAI(model, mesh, shader, texture, hp, speed, cdShot, dispersion) {
        
    this->numBulletsShoot = numBulletsShoot;
    color = Vector4(1, 1, 1, 1);
    this->isHurt = false;
    this->hurtFrames = 0.f;
    this->maxhp = hp;
    this->HPbar = 500;
}

EntityBoss::~EntityBoss(){}

void EntityBoss::render(){
    // Enable shader and pass uniforms
    shader->enable();
    if (isHurt)
    {
        if (hurtFrames > .2f)
        {
            color = Vector4(1, 1, 1, 1);
            isHurt = false;
            hurtFrames = 0.f;
        }
    }
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render(GL_TRIANGLES);

    // Disable shader after finishing rendering
    shader->disable();
}

void EntityBoss::update(float elapsed_time)
{
    wanderChange += elapsed_time;
    position = model.getTranslation();
    // yaw = degree between player and enemy; acos or asin? but that's inneficient
    behaviourUpdate();
    takeAction(this, position, elapsed_time);

    velocity = move_dir * speed;
    position = model.getTranslation();
    position.y = 0.0f; //el 51 es hardcodeado por la mesh del cubo (se tiene en cuenta el centro de la mesh)

    checkBossCollisions(this, position, elapsed_time);
    
    position = position + velocity * elapsed_time;
    
    velocity -= velocity * elapsed_time;

    model.setTranslation(position.x, position.y, position.z); // position.y = 51 harcoceado
    model.rotate(yaw, Vector3(0.0f, 1.0f, 0.0f));
    //color = Vector4(1, 1, 1, 1);
}

EntityCollider::EntityCollider(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture):EntityMesh(model,mesh,shader,texture){
}

EntityCollider::~EntityCollider() {
    delete mesh;
    delete texture;
    delete shader;
}

void EntityCollider::render(){
    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();
}

void EntityCollider::update(float elapsed_time){
}

EntityProjectile::EntityProjectile(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, float speed, float dmg, Vector3 dir, bool isEnemy):EntityCollider(model, mesh, shader, texture){
    
    this->speed = speed;
    this->dmg = dmg;
    this->dir = dir;
    this->isEnemy = isEnemy;
}

EntityProjectile::~EntityProjectile(){
}

void EntityProjectile::render(){
    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();
}

struct sImpactData {
    Vector3 impPoint;
    Vector3 impNormal;
}; std::vector<sImpactData> impacts;

bool checkImpacts(const Vector3& target_pos,
    std::vector<sImpactData>& impacts, bool isEnemy) {
    center = target_pos + Vector3(0.f, 1.25f, 0.f);
    sphereRadius = 3.f;

    // For each collider entity “e” in root:
    //for(auto e:World::world->get_instance()->root->children){
    for (int i = 0; i < World::world->get_instance()->root->children.size(); i++) {
        if (p = dynamic_cast<EntityProjectile*>(World::world->get_instance()->root->children[i]))
        {
            if (p->isEnemy && isEnemy) continue;
            if (!p->isEnemy && !isEnemy) continue;
        }
        if (dynamic_cast<EntityPowerUp*>(World::world->get_instance()->root->children[i]))
            continue;
        if (dynamic_cast<EntityAI*>(World::world->get_instance()->root->children[i]))
            if (isEnemy) continue;
        if (c = dynamic_cast<EntityCollider*>(World::world->get_instance()->root->children[i])) {
            if (c->mesh->testSphereCollision(c->model, center, sphereRadius, colPoint, colNormal)) {
                return true;
            }
        }
        // End loop
    }
    return false;
}

void EntityProjectile::update(float elapsed_time){
    position = model.getTranslation();
    
    dir.normalize();
    velocity =  dir * speed;

    if (checkImpacts(position + velocity * elapsed_time, impacts, this->isEnemy)) {
        this->lifeTime = 0.f;
        World::world->get_instance()->root->removeChild(this);
        
    } else {
        if (this->lifeTime < elapsed_time){
            World::world->get_instance()->root->removeChild(this);
        }
    }
    position = position + velocity * elapsed_time;
    model.setTranslation(position.x, position.y, position.z);
}

EntityPowerUp::EntityPowerUp(Matrix44 model, Mesh* mesh, Shader* shader, Texture* texture, float lifeTime, powerUps effect):EntityMesh(model, mesh, shader, texture){
    this->lifeTime = lifeTime;
    this->lifeTimeTh = lifeTime/4;
    this->angle = 0;
    this->azimuth = 0;
    this->effect = effect;
    this->intermitent = true;
}

EntityPowerUp::~EntityPowerUp() {}

void EntityPowerUp::render(){
    model.setTranslation(model.getTranslation().x, 10*sin(azimuth), model.getTranslation().z);
    model.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

    // Enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewproj", Camera::current->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);


    // Render the mesh using the shader
    if(intermitent)
        mesh->render(GL_TRIANGLES);

    // Disable shader after finishing rendering
    shader->disable();
}

void checkCollisionsPowerUp(const Vector3& target_pos, EntityPowerUp* entity)
{
    center = target_pos + Vector3(0.f, 51.25f, 0.f);
    sphereRadius = 51.5f;

    if (World::world->get_instance()->player->mesh->testSphereCollision(World::world->get_instance()->player->model, center, sphereRadius, colPoint, colNormal))
    {
        World::world->get_instance()->player->addPowerUp(entity);
        World::world->get_instance()->root->removeChild(entity);
    }
}

void EntityPowerUp::update(float elapsed_time){
    angle += (float)elapsed_time * 20;
    azimuth += (float)elapsed_time * 0.8f;

    this->lifeTime -= elapsed_time;
    
    if(lifeTime <= lifeTimeTh){
        th -= elapsed_time;
        if (th <= 0.f)
        {
            th = 0.15f;
            intermitent = !intermitent;
        }
    }

    if(lifeTime <= 0.f){
        this->inWorld = false;
        World::get_instance()->root->removeChild(this);
    }
    checkCollisionsPowerUp(this->model.getTranslation(), this);
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

