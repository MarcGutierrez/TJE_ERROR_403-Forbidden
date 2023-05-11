//
//  entity.h
//  TJE_XCODE
//
//  Created by manu on 11/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#ifndef entity_h
#define entity_h


#endif /* entity_h */

class Entity {

    public:
        Entity(string name, Matrix44 model);             // Constructor
        virtual ~Entity();     // Destructor
         
        // Some attributes
        std::string name;
        Matrix44 model;
         
        // Methods that should be overwritten
        // by derived classes
        virtual void render();
        virtual void update(float elapsed_time);

        // Pointers to children
        std::vector<Entity*> children;
         
        void addChild(Entity* child);
        void removeChild(Entity* child);

        // Get transform in world coordinates
        Matrix44 getGlobalMatrix();

};

void Entity::render() {

    // Render code if EntityMesh
    // ...

    for(int i = 0; i < children.size(); i++)
    children[i]->render();
}

Matrix44 Entity::getGlobalMatrix() {

    if (parent)
    return model * parent->getGlobalMatrix();

    return model;
}



class EntityMesh : public Entity {

    public:

        // Attributes of the derived class
        Mesh* mesh = nullptr;
        Texture* texture = nullptr;
        Shader* shader = nullptr;
        Vector4 color;

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
    shader->setUniform("u_viewproj", camera->vp);
    shader->setTexture("u_texture", texture );


    // Render the mesh using the shader
    mesh->render( GL_TRIANGLES );

    // Disable shader after finishing rendering
    shader->disable();
};


