#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "entity.h"
#include "stage.h"


#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;
//World* World::world = NULL;

Matrix44 model;

Vector4 color = Vector4(1,1,1,1);

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
    cdGamepad = 0.f;
	mouse_locked = false;
    
    camera = new Camera();
    camera->lookAt(Vector3(0.f,1000.f, 1000.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
    camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
    //std::vector<Stage*> stages;
    //PlayStage* play_stage = new PlayStage();
    TitleStage* title_stage = new TitleStage();

    
    /*stages.push_back(play_stage);
    stages.push_back(title_stage);
     
    
    current_stage = stages[0];*/
    current_stage = title_stage;
    musicCd = 64.f;
    audioChannel = NULL;
    isHard = true;
    Audio::Init();
    

	/*//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	texture->load("data/texture.tga");

	// example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/box.ASE");
    Mesh* test;
    test = Mesh::Get("data/test_scene.obj");
    Mesh* plane = new Mesh();
    plane->createSubdividedPlane(1000.0f);

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    
    
    //TESTEST/////////////
    
    //root = new Entity("root", model);
    //EntityMesh* entityCaja = new EntityMesh("caja", model, mesh, texture, shader, color);
    //EntityPlayer* player = new EntityPlayer("player", model, mesh, texture, shader, color, camera);

    EntityPlayer* player = new EntityPlayer(model, mesh, shader, texture, camera);
    
    EntityMesh* test_scene = new EntityMesh(model, test, shader, texture);
    
    EntityMesh* test_plane = new EntityMesh(model, plane, shader, texture);
    
    player->model.translate(0.0f, 100.0f, 0.0f);
    
    world->root->addChild(player);
    //world->root->addChild(test_scene);
    world->root->addChild(test_plane);
    
	//parseScene("data/scenes/test_scene.scene", model, world->root, NULL);*/
    
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	/*//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube
	Matrix44 m;
	//m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

	if(shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", time);

		//do the draw call
		//mesh->render( GL_TRIANGLES );
        
        //render stage here
        current_stage->render();
        

		//disable shader
		shader->disable();
	}*/

	//Draw the floor grid
    current_stage->render();
	//drawGrid();

	//render the FPS, Draw Calls, etc
	//drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);

}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
    //mouse input to rotate the cam
    cdGamepad += seconds_elapsed;
    if (cdGamepad > 1.f)
    {
        cdGamepad = 0.f;
        updateGamepads();
    }

    //if(current_stage->free_cam == true){
    //    if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
    //    {
    //        camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
    //        camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
    //    }
    //    
    //    //async input to move the camera around
    //    if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
    //    if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
    //    if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
    //    if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
    //    if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
    //    
    //    //to navigate with the mouse fixed in the middle
    //    if (mouse_locked)
    //        Input::centerMouse();
    //}
    musicCd += seconds_elapsed;

    if(TitleStage* s = dynamic_cast<TitleStage*>(current_stage)){
        if (current_stage->fin == true){
            current_stage = new PlayStage();
            Audio::Stop(audioChannel);
            musicCd = 160.f;
        }
        else if (musicCd >= 64.f)
        {
            audioChannel = Audio::PlayM("data/audio/forbidden_theme_test.wav");
            musicCd = 0.f;
        }
        if(s->tutorial){
            current_stage = new MenuStage();
        }
    }
    if(MenuStage* s = dynamic_cast<MenuStage*>(current_stage)){
        if(current_stage->fin == true)
            current_stage = new TitleStage();
    }
    if(PlayStage* s = dynamic_cast<PlayStage*>(current_stage)){
        if (current_stage->fin == true){
            current_stage = new EndStage();
            Audio::Stop(audioChannel);
            musicCd = 64.f;
        }
    }
    if(EndStage* s = dynamic_cast<EndStage*>(current_stage)){
        if(s->restart){
            current_stage = new TitleStage();
            Audio::Stop(audioChannel);
            musicCd = 64.f;
        }
        if(s->retry){
            current_stage = new PlayStage();
            Audio::Stop(audioChannel);
            musicCd = 160.f;
        }
        else if (musicCd >= 64.f)
        {
            audioChannel = Audio::PlayM("data/audio/forbidden_theme_test.wav");
            musicCd = 0.f;
        }
    }
    current_stage->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

