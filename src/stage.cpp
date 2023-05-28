//
//  stage.cpp
//  TJE_XCODE
//
//  Created by manu on 26/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#include <stdio.h>
#include "stage.h"
#include "game.h"

Stage::Stage(){
    
}

void Stage::render(){
    
}

void Stage::update(float elapsed_time){
    
}

TitleStage::TitleStage(){
    this->width = Game::instance->window_width;
    this->height = Game::instance->window_height;
}

void TitleStage::render(){
    
}

void TitleStage::update(float elapsed_time){
    
}

PlayStage::PlayStage(){
    
}

void PlayStage::render(){
    this->world.render();
}

void PlayStage::update(float elapsed_time){
    //this->world.update(float elapsed_time);
}

MenuStage::MenuStage(){
    
}

void MenuStage::render(){
    
}

void MenuStage::update(float elapsed_time){
    
}

EndStage::EndStage(){
    
}

void EndStage::render(){
    
}

void EndStage::update(float elapsed_time){
    
}
