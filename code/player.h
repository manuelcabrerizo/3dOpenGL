#ifndef PLAYER_H
#define PLAYER_H

#include "math.h"
#include "utility.h"
#include "terrain.h"
#include <stdio.h>

#define SPEED 0.3
#define ROTATION 2.5
#define GRAVITY 0.01
#define JUMP_POWER 0.2



struct PlayerCamera
{
    Vec3 pos      = {0.0f, 0.0f,  0.0f};
    Vec3 up       = {0.0f, 1.0f,  0.0f};
    float sensitivity = 0.1f;
    float distance = 90;
};

struct Player
{
    Vec3 position;
    float current_speed;
    float current_turn_speed;
    float up_speed = 0.0f;
    Matrix world;
    Mesh mesh;
    bool is_jumping = false;
    float height = 0;
};

void process_player_camera(Player* player, PlayerCamera* camera, float xoffset, float yoffset)
{
    player->current_turn_speed = player->current_turn_speed - (xoffset); 
    if(camera->pos.y < player->height)
        camera->pos.y = player->height;
    if(camera->pos.y > player->height + 10.0f)
        camera->pos.y = player->height + 10.0f; 
    if(camera->pos.y >= player->height && camera->pos.y <= player->height + 10.0f)
        camera->pos.y -= yoffset * camera->sensitivity;
    float new_xoffset = (camera->distance * sinf(to_radiant(player->current_turn_speed))) * camera->sensitivity;
    float new_zoffset = (camera->distance * cosf(to_radiant(player->current_turn_speed))) * camera->sensitivity;
    camera->pos.x = player->position.x - new_xoffset;
    camera->pos.z = player->position.z - new_zoffset;
}

void init_player(Player* player, Vec3 position, const char* mesh_file_paht, const char* tex_file_paht)
{
    player->position = position;
    LoadOBJFileIndex(&player->mesh, mesh_file_paht, tex_file_paht); 
}

void process_player_movement(Player* player, Terrain* terrain)
{      
    float height = getMapHeight(terrain, player->position.z / (float)terrain->cell_spacing,
                                         player->position.x / (float)terrain->cell_spacing);
    player->height = height; 

    Vec3 scaleVec = {0.01f, 0.01f, 0.01f};
    Matrix scale = get_scale_matrix(scaleVec);
    Matrix rot_x = get_rotation_x_matrix(to_radiant(-90));
    Matrix rot_y = get_rotation_y_matrix(to_radiant(player->current_turn_speed));
    Matrix rotation = rot_x * rot_y;
    float new_x_position = player->current_speed * sinf(to_radiant(player->current_turn_speed));
    float new_z_position = player->current_speed * cosf(to_radiant(player->current_turn_speed));
    player->position = {player->position.x + new_x_position,
                        player->position.y,
                        player->position.z + new_z_position};

    Matrix translation = get_translation_matrix(player->position);
    player->world = scale * rotation * translation;    
}

void process_player_y_position(Player* player, Terrain* terrain, PlayerCamera* camera)
{    
    if(player->is_jumping)
    {
        player->up_speed -=  GRAVITY;
        camera->pos.y += player->up_speed;
        player->position.y += player->up_speed; 
        if(player->position.y < player->height)
        {
            player->position.y = player->height;
            player->is_jumping = false;
        } 
    }
    else
    {
        float height = getMapHeight(terrain, player->position.z / (float)terrain->cell_spacing,
                                             player->position.x / (float)terrain->cell_spacing);
        player->position.y = height;
    }
}

#endif
