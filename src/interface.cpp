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

UI::UI(Mesh quad, Texture* texture, float x, float y,float w, float h, Vector4 color){
    quad.createQuad(x, y, w, h, true);
    this->quad = quad;
    this->texture = texture;
    this->color = color;
    shader = Shader::Get("data/shaders/ortoshader.vs", "data/shaders/texture.fs");

    
}

void UI::render(){
    camera2D.view_matrix = Matrix44();
    camera2D.setOrthographic(0, Game::instance->window_width, 0, Game::instance->window_height, -1, 1);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    shader->enable();

    shader->setUniform("u_viewprojection", camera2D.viewprojection_matrix);
    shader->setUniform("u_texture", texture, 0);
    shader->setUniform("u_color", color);
    
    quad.render(GL_TRIANGLES);

    shader->disable();
}

void UI::update(float elapsed_time){
        
}
