#pragma once

#include "input.h"
#include "world.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

enum stageId { TITLE, GAME, MENU, ENDING};

class Stage{
public:
    
    float t = 0.0f;
    bool fin;
    
    Matrix44 model;
    Camera* camera;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    
    Stage();
    //Stage* current_stage;
    virtual void render();
    virtual void update(float elapsed_time);
        
    //void onEnter(Stage* new_stage);
    //void onLeave(Stage* next_stage);
    
};

class TitleStage : public Stage {
    
public:
    
    int width;
    int height;
    
    TitleStage();
    
    stageId getId();
    
    void render();
    void update(float elapsed_time);
    
};

class PlayStage : public Stage {
public:
        

    PlayStage();
    
    stageId getId();

    
    void render();
    void update(float elapsed_time);
    
    void onKeyDown( SDL_KeyboardEvent event );
    void onMouseWheel(SDL_MouseWheelEvent event);
    void onResize(int width, int height);
    
};

class MenuStage : public Stage {
public:
    
    MenuStage();
    
    stageId getId();
    
    void render();
    void update(float elapsed_time);
    
};

class EndStage : public Stage {
public:
    
    EndStage();
    
    stageId getId();
    
    void render();
    void update(float seconds_elapsed);
};

class StageManager : public Stage {
    
    static StageManager* instance;
    
public:
    
    void changeStage(Stage* new_stage);

};
/**
 MenuStage
        width = game::instance->window_width;
 
 PlayStage
 

 
 
 playButton = new EntityGUIelement;
 
 
 //hacer un shader que el vertice lo aplicas en clip space
 background (quad) = new EntityGUIelement(
        Vector2(0.0 , 0.0),
        vector2(0.5, 0.5),
        Texture:Get("path")
 //hacer un shader con camara ortografica --->opcion 1
 //viewprojection * el vector
background = newENtityGUIelement(
        //en world gui_camera = new camera
        //gui_camera->setOrthographic
                Vecotr2(width/2 * height)
                Vector2(200,60
                Texture::GEt
 )
 
 //mouse input to rotate the cam
 if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
 {
     camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
     camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
 }

 //async input to move the camera around
 if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
 if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
 if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
 if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
 if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);

 //to navigate with the mouse fixed in the middle
 if (mouse_locked)
     Input::centerMouse();
 
 */

