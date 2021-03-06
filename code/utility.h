#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

struct Camera 
{
    Vec3 pos      = {0.0f, 0.0f,  3.0f};
    Vec3 front    = {0.0f, 0.0f,  -1.0f};
    Vec3 up       = {0.0f, 1.0f,  0.0f};
    Vec3 right    = {0.0f, 0.0f,  0.0f};
    Vec3 world_up = {0.0f, 1.0f,  0.0f};
    Vec3 world_front = {0.0f, 0.0f, -1.0f};
    float yaw     = -90.0f;
    float pitch   = 0.0f;
    float speed   = 0.05f;
    float sensitivity = 0.1f;
};

#pragma pack(push, 1)
struct bitmapHeader
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t bitmapOffset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
};
#pragma pack(pop)

struct Texture
{
    uint32_t* pixels;
    int width;
    int height;
};

struct IndexBuffer
{
    int a, b, c;
};

struct VertexBuffer
{
    Vec3 vertice; 
    Vec3 normal; 
    Vec2 textureCoord;
};

struct Mesh
{
    uint32_t vao;
    int numVertices;
    int numTexCoords;
    int numNormals;
    int numIndex; 
    Vec3* vertices;
    Vec2* textureCoords;
    Vec3* normals;
    int* indices;
    IndexBuffer* vertexIndex;
    IndexBuffer* textureIndex;
    IndexBuffer* normalIndex;
    VertexBuffer* vertexBuffer;
    VertexBuffer* fVertexBuffer;
    Texture tex;
    uint32_t texId;
};


Texture LoadBMP(const char* filename);
void LoadOBJFile(Mesh* mesh, const char* filePhat, const char* texFileName);
void LoadOBJFileIndex(Mesh* mesh, const char* filePhat, const char* texFileName);

#endif
