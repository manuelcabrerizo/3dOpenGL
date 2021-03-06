#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader.h"
#include "utility.h"
#include "math.h"
#include "terrain.h"
#include "player.h"
#include <stdlib.h>
#include <time.h>


float cube[] = {
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f   
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); 
}


static float mouse_x_offset          = 0;
static float mouse_y_offset          = 0;
static float mouse_x_last_offset     = 400;
static float mouse_y_last_offset     = 300;
static float mouse_x_relative_offest = 0;
static float mouse_y_relative_offest = 0;

static Player player;

void update_camera_vectors(PlayerCamera* camera)
{ 
    float xoffset = -mouse_x_relative_offest * camera->sensitivity; 
    float yoffset = -mouse_y_relative_offest * camera->sensitivity;  
    process_player_camera(&player, camera, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_x_relative_offest += xpos - mouse_x_last_offset;
    mouse_y_relative_offest += ypos - mouse_y_last_offset;
    mouse_x_last_offset = xpos;
    mouse_y_last_offset = ypos;
}

void processInput(GLFWwindow* window, PlayerCamera* camera)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.current_speed = SPEED; 
    }
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.current_speed = -SPEED;
    }
    else
    {
        player.current_speed = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if(player.is_jumping == false)
            player.up_speed = JUMP_POWER;
        player.is_jumping = true; 
    }

}

int main()
{
    srand(time(0));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL)
    {
        printf("FAILED to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("FAILED to initialize GLAD\n");
        return -1;
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);  
        
    // set shaders
    Shader test_shader;
    shader_load(&test_shader, "./code/vertex_shader.vert", "./code/fragment_shader.frag");
    Shader light_shader;
    shader_load(&light_shader, "./code/lightShader.vert", "./code/lightShader.frag");
    Shader terrain_shader;
    shader_load(&terrain_shader, "./code/terrain_vertexShader.vert", "./code/terrain_fragmentShader.frag");

    Mesh tree;
    LoadOBJFileIndex(&tree, "./tree.obj", "./tree4.bmp");
    Mesh grass;
    LoadOBJFileIndex(&grass, "./grassModel.obj", "./grassTexture.bmp");
    Mesh fern;
    LoadOBJFileIndex(&fern, "./fern.obj", "./fern.bmp");


    Vec3 player_init_pos = {0.0f, 0.0f, 0.0f};
    init_player(&player, player_init_pos, "./naruto.obj", "./naruto.bmp");
    
    // light loading steps...
    unsigned  int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    uint32_t lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    PlayerCamera camera; 
    Matrix proj = get_projection_perspective_matrix(45.0f*PI/180.0f, 800.0f / 600.0f, 0.1f, 100.0f);


    shader_use(&terrain_shader);

// testing terrain functions...
    Terrain test_terrain;
    Vec3 terrain_pos = {0.0f, 0.0f, 0.0f};
    init_terrain(&test_terrain, terrain_pos, 256, 256, 10);
    load_terrain(&test_terrain, "./grass0.bmp",
                                "./mud.bmp",
                                "./grassFlowers.bmp", 
                                "./path.bmp",
                                "./blend.bmp",
                                "./terrain.bmp");

    unsigned  int texture1Loc = glGetUniformLocation(terrain_shader.ID, "texture1");
    unsigned  int texture2Loc = glGetUniformLocation(terrain_shader.ID, "texture2");
    unsigned  int texture3Loc = glGetUniformLocation(terrain_shader.ID, "texture3");
    unsigned  int texture4Loc = glGetUniformLocation(terrain_shader.ID, "texture4");
    unsigned  int texture5Loc = glGetUniformLocation(terrain_shader.ID, "texture5");
    glUniform1i(texture1Loc, 0);
    glUniform1i(texture2Loc, 1);
    glUniform1i(texture3Loc, 2);
    glUniform1i(texture4Loc, 3);
    glUniform1i(texture5Loc, 4);

    unsigned int t_mtrlAmbientLocation   = glGetUniformLocation(terrain_shader.ID, "material.ambient");
    unsigned int t_mtrlDiffuseLocation   = glGetUniformLocation(terrain_shader.ID, "material.diffuse");
    unsigned int t_mtrlSpecularLocation  = glGetUniformLocation(terrain_shader.ID, "material.specular");
    unsigned int t_mtrlShininessLocation = glGetUniformLocation(terrain_shader.ID, "material.shininess");
    glUniform3f(t_mtrlAmbientLocation,  1.0f, 1.0f, 1.0f);
    glUniform3f(t_mtrlDiffuseLocation,  1.0f, 1.0f, 1.0f);
    glUniform3f(t_mtrlSpecularLocation, 1.0f, 1.0f, 1.0f);
    glUniform1f(t_mtrlShininessLocation, 32.0f);

    unsigned int t_lightPosLocation       = glGetUniformLocation(terrain_shader.ID, "light.position");
    unsigned int t_lightAmbientLocation   = glGetUniformLocation(terrain_shader.ID, "light.ambient");
    unsigned int t_lightDiffuseLocation   = glGetUniformLocation(terrain_shader.ID, "light.diffuse");
    unsigned int t_lightSpecularLocation  = glGetUniformLocation(terrain_shader.ID, "light.specular");
    unsigned int t_lightConstantLocation   = glGetUniformLocation(terrain_shader.ID, "light.constant");
    unsigned int t_lightLinearLocation   = glGetUniformLocation(terrain_shader.ID, "light.linear");
    unsigned int t_lightQuadraticLocation  = glGetUniformLocation(terrain_shader.ID, "light.quadratic");
    glUniform3f(t_lightPosLocation, 0.0f, 0.0f, 0.0f);
    glUniform3f(t_lightAmbientLocation, 0.5f, 0.2f, 0.1f);
    glUniform3f(t_lightDiffuseLocation, 1.0f, 0.5f, 0.2f);
    glUniform3f(t_lightSpecularLocation, 1.0f, 0.5f, 0.2f);
    glUniform1f(t_lightConstantLocation,  1.0f);
    glUniform1f(t_lightLinearLocation,    0.09f);
    glUniform1f(t_lightQuadraticLocation, 0.032f);

    unsigned int t_worldLocation = glGetUniformLocation(terrain_shader.ID, "world");
    unsigned int t_viewLocation  = glGetUniformLocation(terrain_shader.ID, "view");
    unsigned int t_projLocation  = glGetUniformLocation(terrain_shader.ID, "proj");
    unsigned int t_cameraLocation = glGetUniformLocation(terrain_shader.ID, "viewPos");
    glUniformMatrix4fv(t_projLocation, 1, GL_FALSE, proj.m[0]);
    glUseProgram(0);
    


    shader_use(&test_shader);
    unsigned  int texture1Location = glGetUniformLocation(test_shader.ID, "texture1");
    glUniform1i(texture1Location, 0);

    unsigned int transparentLocation = glGetUniformLocation(test_shader.ID, "transparent");
    glUniform1i(transparentLocation, 0);

    unsigned int mtrlAmbientLocation   = glGetUniformLocation(test_shader.ID, "material.ambient");
    unsigned int mtrlDiffuseLocation   = glGetUniformLocation(test_shader.ID, "material.diffuse");
    unsigned int mtrlSpecularLocation  = glGetUniformLocation(test_shader.ID, "material.specular");
    unsigned int mtrlShininessLocation = glGetUniformLocation(test_shader.ID, "material.shininess");
    glUniform3f(mtrlAmbientLocation,  1.0f, 1.0f, 1.0f);
    glUniform3f(mtrlDiffuseLocation,  1.0f, 1.0f, 1.0f);
    glUniform3f(mtrlSpecularLocation, 1.0f, 1.0f, 1.0f);
    glUniform1f(mtrlShininessLocation, 32.0f);

    unsigned int lightPosLocation       = glGetUniformLocation(test_shader.ID, "light.position");
    unsigned int lightAmbientLocation   = glGetUniformLocation(test_shader.ID, "light.ambient");
    unsigned int lightDiffuseLocation   = glGetUniformLocation(test_shader.ID, "light.diffuse");
    unsigned int lightSpecularLocation  = glGetUniformLocation(test_shader.ID, "light.specular");
    unsigned int lightConstantLocation   = glGetUniformLocation(test_shader.ID, "light.constant");
    unsigned int lightLinearLocation   = glGetUniformLocation(test_shader.ID, "light.linear");
    unsigned int lightQuadraticLocation  = glGetUniformLocation(test_shader.ID, "light.quadratic");
    glUniform3f(lightPosLocation, 0.0f, 0.0f, 0.0f);
    glUniform3f(lightAmbientLocation, 0.5f, 0.2f, 0.1f);
    glUniform3f(lightDiffuseLocation, 1.0f, 0.5f, 0.2f);
    glUniform3f(lightSpecularLocation, 1.0f, 0.5f, 0.2f);
    glUniform1f(lightConstantLocation,  1.0f);
    glUniform1f(lightLinearLocation,    0.09f);
    glUniform1f(lightQuadraticLocation, 0.032f);

    unsigned int worldLocation = glGetUniformLocation(test_shader.ID, "world");
    unsigned int viewLocation  = glGetUniformLocation(test_shader.ID, "view");
    unsigned int projLocation  = glGetUniformLocation(test_shader.ID, "proj");
    unsigned int cameraLocation = glGetUniformLocation(test_shader.ID, "viewPos");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, proj.m[0]);
    glUseProgram(0);
 
    Matrix view = get_view_matrix(camera.pos, player.position, camera.up);

    shader_use(&light_shader);
    unsigned int lightWorldLocation = glGetUniformLocation(light_shader.ID, "world");
    unsigned int lightViewLocation  = glGetUniformLocation(light_shader.ID, "view");
    unsigned int lightProjLocation  = glGetUniformLocation(light_shader.ID, "proj");
    glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, view.m[0]);
    glUniformMatrix4fv(lightProjLocation, 1, GL_FALSE, proj.m[0]);

    unsigned int colorLoction = glGetUniformLocation(light_shader.ID, "lightColor");
    glUniform3f(colorLoction, 1.0f, 0.5f, 0.2f);
    glUseProgram(0);
    glEnable(GL_DEPTH_TEST); 



    // setup trees locations
    Vec2 treesPositions[256 * 256];
    for(int z = 0; z < test_terrain.num_vert_col; z += 2)
    {
        for(int x = 0; x < test_terrain.num_vert_row; x += 2)
        {  
            treesPositions[(z * test_terrain.num_vert_row) + x].x = (rand() % 256) + 1;
            treesPositions[(z * test_terrain.num_vert_row) + x].y = (rand() % 256) + 1;
        }
    }

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
        // update functions...
        process_player_y_position(&player, &test_terrain, &camera);
        process_player_movement(&player, &test_terrain);
        processInput(window, &camera);
        update_camera_vectors(&camera);
        mouse_y_relative_offest = 0.0f;
        mouse_x_relative_offest = 0.0f;

        Vec3 camera_target = {player.position.x, player.position.y + 1.0f, player.position.z};
        view = get_view_matrix(camera.pos, camera_target, camera.up);
        Matrix world = get_identity_matrix();
        Matrix scale = get_identity_matrix();
        Matrix rotation = get_identity_matrix();
        Matrix traslation = get_identity_matrix(); 
        Vec3 trans = {0.0f, 0.0f, 0.0f};




        shader_use(&terrain_shader);
        glUniform3f(t_lightPosLocation, 0.0f, 2.0f, 0.0f);
        glUniform3f(t_cameraLocation, camera.pos.x, camera.pos.y, camera.pos.z);
        glUniformMatrix4fv(t_viewLocation, 1, GL_FALSE, view.m[0]); 
        glUniformMatrix4fv(t_worldLocation, 1, GL_FALSE, world.m[0]);
        glBindVertexArray(test_terrain.vao);   
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, test_terrain.texId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, test_terrain.rId);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, test_terrain.gId);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, test_terrain.bId);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, test_terrain.blendMapId);
        glDrawElements(GL_TRIANGLES, test_terrain.num_triangles * 3, GL_UNSIGNED_INT, 0);
         
        

 
        shader_use(&test_shader);
        glUniform1i(transparentLocation, 0);
        glUniform3f(cameraLocation, camera.pos.x, camera.pos.y, camera.pos.z);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.m[0]);  
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, world.m[0]);
        glUniform3f(lightPosLocation, 0.0f, 2.0f,0.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(tree.vao);
        glBindTexture(GL_TEXTURE_2D, tree.texId);
        Vec3 scaleVec = {2.0f, 2.0f, 2.0f};
        scale = get_scale_matrix(scaleVec);
        for(int z = 0; z < test_terrain.num_vert_col; z += 2)
        {
            for(int x = 0; x < test_terrain.num_vert_row; x += 2)
            {
                
                trans.x = treesPositions[(z * test_terrain.num_vert_row) + x].x * test_terrain.cell_spacing;
                trans.z = treesPositions[(z * test_terrain.num_vert_row) + x].y * test_terrain.cell_spacing;
                
                float y = getMapHeight(&test_terrain, trans.z / test_terrain.cell_spacing, trans.x / test_terrain.cell_spacing);
                trans.y = y;


                traslation = get_translation_matrix(trans);
                world = scale * rotation * traslation;
                glUniformMatrix4fv(worldLocation, 1, GL_FALSE, world.m[0]);
                glDrawElements(GL_TRIANGLES, tree.numIndex * 3, GL_UNSIGNED_INT, 0);
            }
        }
        glUniform1i(transparentLocation, 1);
        glDisable(GL_CULL_FACE);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(grass.vao);
        glBindTexture(GL_TEXTURE_2D, grass.texId);
        scaleVec = {1.0f, 1.0f, 1.0f};
        scale = get_scale_matrix(scaleVec);
        for(int z = 0; z < test_terrain.num_vert_col; z += 2)
        {
            for(int x = 0; x < test_terrain.num_vert_row; x += 2)
            {
                trans.x = treesPositions[(z * test_terrain.num_vert_row) + x].x * test_terrain.cell_spacing;
                trans.z = treesPositions[(z * test_terrain.num_vert_row) + x].y * test_terrain.cell_spacing;
                
                float y = getMapHeight(&test_terrain, trans.z / test_terrain.cell_spacing, trans.x / test_terrain.cell_spacing);
                trans.y = y - 0.1f;


                traslation = get_translation_matrix(trans);
                world = scale * rotation * traslation;
                glUniformMatrix4fv(worldLocation, 1, GL_FALSE, world.m[0]);
                glDrawElements(GL_TRIANGLES, grass.numIndex * 3, GL_UNSIGNED_INT, 0);
            }
        } 
        glBindVertexArray(fern.vao);
        glBindTexture(GL_TEXTURE_2D, fern.texId);
        scaleVec = {0.5f, 0.5f, 0.5f};
        scale = get_scale_matrix(scaleVec);
        for(int z = 0; z < test_terrain.num_vert_col; z += 8)
        {
            for(int x = 0; x < test_terrain.num_vert_row; x += 8)
            {
                
                trans.x = x * test_terrain.cell_spacing / 4;
                trans.z = z * test_terrain.cell_spacing / 4;
                
                float y = getMapHeight(&test_terrain, trans.z / test_terrain.cell_spacing, trans.x / test_terrain.cell_spacing);
                trans.y = y - 0.1f;


                traslation = get_translation_matrix(trans);
                world = scale * traslation;
                glUniformMatrix4fv(worldLocation, 1, GL_FALSE, world.m[0]);
                glDrawElements(GL_TRIANGLES, fern.numIndex * 3, GL_UNSIGNED_INT, 0);
            }
        } 
        glUniform1i(transparentLocation, 0);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);



        // DRAW::PLAYER::STUFF...
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, player.world.m[0]);
        glBindVertexArray(player.mesh.vao);
        glBindTexture(GL_TEXTURE_2D, player.mesh.texId);
        glDrawElements(GL_TRIANGLES, player.mesh.numIndex * 3, GL_UNSIGNED_INT, 0);




        shader_use(&light_shader); 
        glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, view.m[0]);
        glBindVertexArray(lightVAO);
        trans = {0.0f, 2.0f, 0.0f};
        traslation = get_translation_matrix(trans);
        world = traslation;
        glUniformMatrix4fv(lightWorldLocation, 1, GL_FALSE, world.m[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


