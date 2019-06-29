#include "Windows.h"

#include "GL.h"
#include "Map.h"
#include "Block.h"

//#include "Map.hpp"
//#include "Texture.hpp"

static void error_callback(int error_code, const char* error)
{
    fprintf(stderr, "An error occured: %d : %s\n", error_code, error);
}

#ifdef RELEASE

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)

#else
int main()
#endif
{
    glfwSetErrorCallback(error_callback);
    struct GLOption GlOption = GLInit("Automatic Fortnight");
    if(!GlOption.ok)
    {
        fprintf(stderr, "%s\n", GlOption.error_message);
        return 0;
    }

    struct GL* gl = GlOption.result_ptr;

    struct GLOption MapOption = LoadMap("world/world.map");
    if(!MapOption.ok)
    {
        fprintf(stderr, "%s\n", MapOption.error_message);
    }

    struct Map* map = MapOption.result_ptr;
    
    // struct Objects* obj = LoadObjects("blocks/default.obj").result_ptr;
    // struct Block* blok = CreateBlock((vec3){0.0f, 0.0f, 0.0f}, obj, 1);

    // int nbFrames = 0;

    glClearColor(0.15f, 0.0f, 0.15f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // GLuint vao, vbo, ebo;
    // glGenBuffers(1, &vbo);
    // glGenBuffers(1, &ebo);
    // glGenVertexArrays(1, &vao);

    // glBindVertexArray(vao);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // float vbo_data[] =
    // {
    //     1.0f, 0.0f, 0.0f,
    //     1.0f, 1.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    // };

    // unsigned int ebo_data[] = 
    // {
    //     0, 1, 2
    // };

    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_data), ebo_data, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(gl->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT

        float currentFrame = (float)glfwGetTime();
        gl->deltaTime = currentFrame - gl->lastTime;
        gl->lastTime = currentFrame;
        
        // nbFrames++;
        // if (gl->deltaTime >= 1.0f )
        // {
        //     printf("%f ms/frame\n", 1000.0f/(float)nbFrames);
        //     nbFrames = 0;
        //     gl->lastTime = currentFrame;
        // }

        // glBindVertexArray(vao);
        // glUseProgram(gl->shader->id);
        // glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // DrawBlock(blok, gl->shader);
        DrawMap(map, gl->shader);
        
        UpdateViewMatrix(gl->camera);
        glUniformMatrix4fv(gl->shader->view_position, 1, GL_FALSE, &gl->camera->view[0][0]);

        glfwSwapBuffers(gl->window);
        glfwPollEvents();
    }

    free(gl);
    //delete stone;
    //delete stone2;

    return 0;
}