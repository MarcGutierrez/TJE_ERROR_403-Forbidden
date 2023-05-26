//
//  world.cpp
//  TJE_XCODE
//
//  Created by manu on 17/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include <stdio.h>
#include "world.h"
#include "entity.h"

World::World(){
    root = new Entity("root", this->model);
    
}
/*
 bool WOrld::checkLineOfSight(const Matrix44& obs, const Matrix44& target)
    Vecror3 front = normalize (obs.frontVector())
    Vector3 toTarget = normalize(target.getTranslation() - obs.getTranslation())
 
    Vector3 ray_origin =  obs.getTranslation()
    Vector3 ray_direction = toTarget;
    float distance = toTarget.length();
    
    
 
 if(toTarget.dot(front) > 0.5 //90 grados
    for(auto e:root.children)
        EntityColider* ec = dynamic_cast<EntityColider*>(e);
        if(!ec) continue
        
        ec->testRayColision(
            ec->model
            ray_origin
            ray_direction
            Vector3(),
            Vector3(),
            distance
        )){
    return true
 
 return false
 */
