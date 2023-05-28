#pragma once

#include "entity.h"
#include "utils.h"
#include <fstream>
#include <map>

struct sRenderData;

bool parseScene(const char* filename, Matrix44 model, Entity* root, int mapId);



