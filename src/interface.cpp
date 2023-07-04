//
//  interface.cpp
//  TJE_XCODE
//
//  Created by manu on 3/7/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include <stdio.h>
#include "game.h"
#include "interface.h"

UI::UI(Mesh* quad){
    quad->createQuad(0, 0, 2, 2, true);
    this->quad = quad;
    shader = Shader::Get("data/shaders/ortoshader.vs", "data/shaders/texture.fs");

    
}

void UI::render(){
    camera2D.view_matrix = Matrix44();
    camera2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, -1, 1);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    
    shader->enable();

    shader->setUniform("u_viewprojection", camera2D.viewprojection_matrix);
    shader->setUniform("u_texture", World::get_instance()->powerUpTexture, 0);
    //shader->setUniform("u_color", Vector4(1, 1, 1, 1));
    
    quad->render(GL_TRIANGLES);

    shader->disable();
}

void UI::update(float elapsed_time){
        
}
