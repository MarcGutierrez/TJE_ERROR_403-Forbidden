#include "loadScene.h"

struct sRenderData {
	Texture* texture = nullptr;
	Shader* shader = nullptr;
	std::vector<Matrix44> models;
};

std::map<std::string, sRenderData> meshes_to_load;

/*
	To use it: parseScene("data/myscene.scene");
*/

bool parseScene(const char* filename, Matrix44 model, Entity* root, int mapId)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors
	// meshes_to_load["meshes/example.obj"] = { Texture::Get("texture.tga"), Shader::Get("shader.vs", "shader.fs") };
	
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	Shader* shader;
	Texture* texture;
	if (!mapId)
	{
		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
		texture = Texture::Get("data/texture.tga");
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;
		render_data.shader = shader;
		render_data.texture = texture;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			InstancedEntityMesh* new_entity = new InstancedEntityMesh(model, Mesh::Get(mesh_name.c_str()), render_data.shader, render_data.texture);
			// Add all instances
			new_entity->models = render_data.models;
			// Add entity to scene root
			root->addChild(new_entity);
		}
		// Create normal entity
		else {
			EntityMesh* new_entity = new EntityMesh(model, Mesh::Get(mesh_name.c_str()), render_data.shader, render_data.texture);
			new_entity->model = render_data.models[0];
			// Add entity to scene root
			root->addChild(new_entity);
		}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}