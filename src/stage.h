//
//  stage.h
//  TJE_XCODE
//
//  Created by manu on 24/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#ifndef stage_h
#define stage_h

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

#endif /* stage_h */
