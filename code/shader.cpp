#include "shader.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void shader_load(Shader* shader, const char* vertex_path, const char* fragment_path)
{
    FILE* vertex_file;
    FILE* fragment_file;
    char* vertex_buffer;
    char* fragment_buffer;
    unsigned int vertex;
    unsigned int fragment;
    int success;
    char info_log[512];

    // vertes shader loading stage
    vertex_file = fopen(vertex_path, "rb");
    if(vertex_file != NULL)
    {
        fseek(vertex_file, 0, SEEK_END);
        int vertex_file_size = ftell(vertex_file);
        fseek(vertex_file, 0, SEEK_SET);
        vertex_buffer = (char*)malloc(vertex_file_size + 1);
        fread((void*)vertex_buffer, vertex_file_size, 1, vertex_file);
        vertex_buffer[vertex_file_size] = 0;
        fclose(vertex_file);
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_buffer, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            printf("ERROR::COMPILING::VERTEX::SHADER %s\n", info_log);
        }
        free(vertex_buffer);
    }
    else
    {
        printf("ERROR::OPENING::VERTEX::SHADER\n");
    }

    fragment_file = fopen(fragment_path, "rb");
    if(fragment_file != NULL)
    {
        fseek(fragment_file, 0, SEEK_END);
        int fragment_file_size = ftell(fragment_file);
        fseek(fragment_file, 0, SEEK_SET);
        fragment_buffer = (char*)malloc(fragment_file_size + 1);
        fread((void*)fragment_buffer, fragment_file_size, 1, fragment_file);
        fragment_buffer[fragment_file_size] = 0;
        fclose(fragment_file);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_buffer, NULL);
        glCompileShader(fragment);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            printf("ERROR::COMPILING::FRAGMENT::SHADER %s\n", info_log);
        }
        free(fragment_buffer);
    }
    else
    {
        printf("ERROR::OPENING::FRAGMENT::SHADER\n");
    }

    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader->ID, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED %s\n", info_log);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void shader_use(Shader* shader)
{
    glUseProgram(shader->ID);
}
