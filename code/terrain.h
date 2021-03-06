#ifndef TERRAIN_H
#define TERRAIN_H

#include "utility.h"

struct Terrain 
{    
    Vec3 position;
    int num_vertices;
    int num_vert_row;
    int num_vert_col;
    int num_cell_row;
    int num_cell_col;
    int cell_spacing;
    int num_triangles;
    VertexBuffer* vertex_buffer;
    int* indices;
    uint8_t* height_map;    
    uint32_t vao;
    uint32_t texId;
    Texture tex;
    uint32_t rId;
    Texture rTex;
    uint32_t gId;
    Texture gTex;
    uint32_t bId;
    Texture bTex;
    uint32_t blendMapId;
    Texture blendMap;
};


float get_height(Terrain* terrain, int x, int z);
void init_terrain(Terrain* terrain, Vec3 position, int rows, int cols, int cell_spacing);
void load_terrain(Terrain* terrain,
                  const char* tex_path,
                  const char* r_path,
                  const char* g_path,
                  const char* b_path,
                  const char* blendMap_path,
                  const char* heightmap_path);
float getMapHeight(Terrain* terrain, float x, float z);



#endif
