#include "terrain.h"
#include <math.h>
#include <glad/glad.h>


static void load_texture_to_terrain(Texture& tex, uint32_t& texId, const char* path)
{
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    texId = texture;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    tex = LoadBMP(path);
    if(tex.pixels != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height,
                                    0, GL_BGR, GL_UNSIGNED_BYTE, tex.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("ERROR::LOADING::BMP::FILE\n");
    }
}


float get_height(Terrain* terrain, int x, int z)
{
    if(x >= terrain->num_vert_row)
        x = terrain->num_vert_row - 1;
    if(z >= terrain->num_vert_col)
        z = terrain->num_vert_col - 1;
    if(x < 0)
        x = 0;
    if(z < 0)
        z = 0; 
    return (float)terrain->height_map[(z * terrain->num_vert_row) + x];
}

void init_terrain(Terrain* terrain, Vec3 position, int rows, int cols, int cell_spacing)
{
    terrain->position = position;
    terrain->num_vert_row = rows;
    terrain->num_vert_col = cols;
    terrain->cell_spacing = cell_spacing;
    terrain->num_vertices = rows * cols;
    terrain->num_cell_row = rows - 1;
    terrain->num_cell_col = cols - 1;
    terrain->num_triangles = 2 * (terrain->num_cell_row * terrain->num_cell_col);    
}

void load_terrain(Terrain* terrain,
                  const char* tex_path,
                  const char* r_path,
                  const char* g_path,
                  const char* b_path,
                  const char* blendMap_path,
                  const char* heightmap_path)
{
    
    terrain->vertex_buffer = (VertexBuffer*)malloc(terrain->num_vertices * sizeof(VertexBuffer));
    terrain->height_map = (uint8_t*)malloc(terrain->num_vertices * sizeof(uint8_t));
    terrain->indices       = (int*)malloc((terrain->num_triangles * 3) * sizeof(int));

    // get heightmap info...
    Texture height_map = LoadBMP(heightmap_path);
    // generate vertices...
    for(int z = 0; z < terrain->num_vert_col; z++)
    {
        for(int x = 0; x < terrain->num_vert_row; x++)
        {
            uint32_t pixel = (uint32_t)height_map.pixels[(z * terrain->num_vert_row) + x];
            uint8_t height = (uint8_t)(pixel & 0xFFFFFF);
            terrain->height_map[(z * terrain->num_vert_row) + x] = height;

            float x_position = terrain->position.x + ((float)x * (float)terrain->cell_spacing);
            float y_position = terrain->position.y + height;
            float z_position = terrain->position.z + ((float)z * (float)terrain->cell_spacing);

            Vec3 vertex = { x_position, y_position, z_position};
            terrain->vertex_buffer[(z * terrain->num_vert_row) + x].vertice = vertex;
            Vec2 texCoord;
            texCoord.x = x * (1.0f / terrain->num_cell_row);
            texCoord.y = z * (1.0f / terrain->num_cell_col);
            terrain->vertex_buffer[(z * terrain->num_vert_row) + x].textureCoord = texCoord; 
        }
    }

    // generate normals...
    for(int z = 0; z < terrain->num_vert_col; z++)
    {
        for(int x = 0; x < terrain->num_vert_row; x++)
        {
            float a = get_height(terrain, x, z);
            float b = get_height(terrain, x + 1, z);
            float c = get_height(terrain, x, z + 1);
            Vec3 u = {(float)terrain->cell_spacing, b - a, 0.0f};
            Vec3 v = {0.0f, c - a, (float)terrain->cell_spacing};
            Vec3 normal = normaliza_vec3(vec3_cross(u, v));
            terrain->vertex_buffer[(z * terrain->num_vert_row) + x].normal = -normal;
        }
    }
    // generate indices...
    int index = 0;
    for(int z = 0; z < terrain->num_cell_col; z++)
    {
        for(int x = 0; x < terrain->num_cell_row; x++)
        { 
            terrain->indices[index + 0] = ((z + 1) * terrain->num_vert_row) + x;
            terrain->indices[index + 1] = (z * terrain->num_vert_row) + (x + 1);
            terrain->indices[index + 2] = (z * terrain->num_vert_row) + x;
            
            terrain->indices[index + 3] = ((z + 1) * terrain->num_vert_row) + (x + 1); 
            terrain->indices[index + 4] = (z * terrain->num_vert_row) + (x + 1);
            terrain->indices[index + 5] = ((z + 1) * terrain->num_vert_row) + x;
            index += 6;
        }
    }
    
    glGenVertexArrays(1, &terrain->vao);
    glBindVertexArray(terrain->vao);
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, terrain->num_vertices * sizeof(VertexBuffer), terrain->vertex_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBuffer), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (terrain->num_triangles * 3) * sizeof(int), terrain->indices, GL_STATIC_DRAW);
    
    load_texture_to_terrain(terrain->tex, terrain->texId, tex_path);
    load_texture_to_terrain(terrain->rTex, terrain->rId, r_path);
    load_texture_to_terrain(terrain->gTex, terrain->gId, g_path);
    load_texture_to_terrain(terrain->bTex, terrain->bId, b_path);
    load_texture_to_terrain(terrain->blendMap, terrain->blendMapId, blendMap_path);

    free(terrain->tex.pixels);
    free(terrain->rTex.pixels);
    free(terrain->gTex.pixels);
    free(terrain->bTex.pixels);
    free(terrain->blendMap.pixels);
    free(terrain->vertex_buffer);
    free(terrain->indices);

    glBindVertexArray(0);
}



float getMapHeight(Terrain* terrain, float x, float z)
{
	float col = floorf(x);
	float row = floorf(z);

	float A = get_height(terrain, row,   col);
	float B = get_height(terrain, row,   col+1);
	float C = get_height(terrain, row+1, col);
	float D = get_height(terrain, row+1, col+1);

	float dx = x - col;
	float dz = z - row;

	float height = 0.0f;
	if(dz < 1.0f - dx)  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C

		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}




