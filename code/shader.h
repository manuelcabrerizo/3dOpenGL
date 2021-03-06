#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

struct Shader 
{
    unsigned int ID;
};


void shader_load(Shader* shader, const char* vertex_path, const char* fragment_path);
void shader_use(Shader* shader);


#endif

