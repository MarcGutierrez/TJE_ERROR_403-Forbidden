//
//  interface.h
//  TJE_XCODE
//
//  Created by manu on 24/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#pragma once
#include "camera.h"
#include "shader.h"
#include "mesh.h"

class UI
{
public:
    
    Camera camera2D;
    Shader* shader;
    Mesh quad;
    Texture* texture;
    Vector4 color;
    UI(Mesh quad, Texture* texture, float x, float y, float w, float h, Vector4 color);
    
    void render();
    void update(float elapsed_time);
};
