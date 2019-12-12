#ifndef MODELLOADER_H    
#define MODELLOADER_H

#include "Model.h"
#include <string>


Model loadFromFile(std::string file);

Model loadFromObj(std::string file);
Model loadFromDae(std::string file);

#endif