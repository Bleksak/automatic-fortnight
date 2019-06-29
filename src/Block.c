#include "Block.h"

struct Block* CreateBlock(vec3 position, struct Objects* objects, unsigned int block_id)
{
    if(block_id > BLOCK_ID_SIZE || !block_id)
    {
        return 0;
    }

    struct Block* block = calloc(1, sizeof(struct Block));
    glm_vec3_copy(position, block->position);

    glGenVertexArrays(1, &block->vao);
    glGenBuffers(1, &block->vbo_vertex);
    glGenBuffers(1, &block->vbo_texture);
    glGenBuffers(1, &block->vbo_normal);
    glGenBuffers(1, &block->ebo);

    glBindVertexArray(block->vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objects->objects[block_id - 1].f_size * sizeof(unsigned int), objects->objects[block_id - 1].f, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, block->vbo_vertex);
    glBufferData(GL_ARRAY_BUFFER, objects->objects[block_id - 1].f_size * sizeof(float) * 3, objects->objects[block_id - 1].v, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, block->vbo_texture);
    glBufferData(GL_ARRAY_BUFFER, objects->objects[block_id - 1].f_size * sizeof(float) * 2, objects->objects[block_id - 1].vt, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, block->vbo_normal);
    glBufferData(GL_ARRAY_BUFFER, objects->objects[block_id - 1].f_size * sizeof(float) * 3, objects->objects[block_id - 1].vn, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    
    glm_mat4_identity(block->model);
    glm_translate(block->model, position);

    block->objects = objects;
    block->object_idx = block_id - 1;

    return block;
}

void MoveBlockPosition(struct Block* block, vec3 position)
{
    glm_vec4_copy(block->position, position);
    glm_mat4_identity(block->model);
    glm_translate(block->model, position);
}

void MoveBlockVector(struct Block* block, vec3 vector)
{
    glm_vec3_add(vector, block->position, block->position);
    glm_mat4_identity(block->model);
    glm_translate(block->model, block->position);
}

void DrawBlock(struct Block* block, struct Shader* shader)
{

    if(!block)
        return;

    glBindVertexArray(block->vao);
    glUseProgram(shader->id);
    glUniformMatrix4fv(shader->model_position, 1, GL_FALSE, &block->model[0][0]);
    
    unsigned int drawn_sum = 0;

    for(unsigned int i = 0; i < block->objects->objects[block->object_idx].materials.length; ++i)
    {
        BindTexture(block->objects->textures, block->objects->objects[block->object_idx].materials.materials[i].id);
        glDrawElements(GL_TRIANGLES, block->objects->objects[block->object_idx].materials.materials[i].block_count, GL_UNSIGNED_INT, (const void*)(drawn_sum * sizeof(unsigned int)));
        drawn_sum += block->objects->objects[block->object_idx].materials.materials[i].block_count;
    }
}