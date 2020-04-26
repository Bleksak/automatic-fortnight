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

    struct Map* map = map_generate("name.map", 25);
    
    glClearColor(0.15f, 0.0f, 0.15f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // glfwSwapInterval(0);

    while(!glfwWindowShouldClose(gl->window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT

        float currentFrame = (float)glfwGetTime();
        gl->delta_time = currentFrame - gl->last_time;
        gl->last_time = currentFrame;

        map_draw(map, gl->shader);
        
        camera_update_view_matrix(gl->camera);
        glUniformMatrix4fv(gl->shader->view_position, 1, GL_FALSE, &gl->camera->view[0][0]);

        input_update(gl);
        glfwSwapBuffers(gl->window);
        glfwPollEvents();
    }

    free(gl);

    return 0;
}
