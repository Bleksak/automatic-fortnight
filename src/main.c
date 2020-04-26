#include "Windows.h"

#include "GL.h"
#include "Map.h"

#include "Blocks.h"
#include "Map.h"

static void error_callback(int error_code, const char* error) {
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
    struct GLOption GlOption = gl_init("Automatic Fortnight");
    if(!GlOption.ok) {
        fprintf(stderr, "%s\n", GlOption.error_message);
        return 0;
    }

    struct GL* gl = unwrap(void*, GlOption);

    // struct GLOption MapOption = LoadMap("world/world.map");
    // if(!MapOption.ok)
    // {
    //     fprintf(stderr, "%s\n", MapOption.error_message);
    // }

    // struct Map* map = MapOption.result_ptr;

    struct Map* map = map_generate("name.map", 25);
    
    // struct GLOption objOpt = LoadBlocks("blocks/objects.atf");
    // if(!objOpt.ok)
    // {
    //     fprintf(stderr, "%s\n", objOpt.error_message);
    //     return 0;
    // }

    // struct Blocks* blocks = objOpt.result_ptr;

    glClearColor(0.15f, 0.0f, 0.15f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while(!glfwWindowShouldClose(gl->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT

        float currentFrame = (float)glfwGetTime();
        gl->delta_time = currentFrame - gl->last_time;
        gl->last_time = currentFrame;
        
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
        // ShaderSetUniformInt(gl->shader->id, "texSampler", 0);
        // DrawBlock(blocks, 4);
        map_draw(map, gl->shader);
        
        camera_update_view_matrix(gl->camera);
        glUniformMatrix4fv(gl->shader->view_position, 1, GL_FALSE, &gl->camera->view[0][0]);

        glfwSwapBuffers(gl->window);
        glfwPollEvents();
    }

    free(gl);
    //delete stone;
    //delete stone2;

    return 0;
}
