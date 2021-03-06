#include <stdio.h>
#include <stdlib.h>

#include "Textures.h"
#include "Image.h"
#include <string.h>

// static void closef(FILE** fp)
// {
//     if(*fp)
//         fclose(*fp);
// }

// struct GLOption LoadTextures(const char* filename)
// {
//     __attribute__((cleanup(closef))) FILE* fp = fopen(filename, "rb");

//     if(!fp)
//     {
//         return (struct GLOption)
//         {
//             .ok = false,
//             .error_message = "Failed to load textures",
//         };
//     }

//     struct Textures* textures = calloc(1, sizeof(struct Textures));

//     char line[512];

//     while(fgets(line, 512, fp))
//     {
//         char line_type[8];
//         int offset;

//         if(sscanf(line, "%s%n", line_type, &offset) != 1)
//         {
//             continue;
//         }

//         if(line_type[0] == ';')
//         {
//             continue;
//         }

//         char* line_value = line + offset + 1;

//         switch(line_type[0])
//         {
//             case 'n': //newmtl
//             {
//                 textures->textures = realloc(textures->textures, (textures->length + 1) * sizeof(struct Texture));
                
//                 glGenTextures(1, &textures->textures[textures->length].id);
//                 glBindTexture(GL_TEXTURE_2D, textures->textures[textures->length].id);
                
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//                 ++textures->length;
//             } break;

//             case 'K':
//             {
//                 switch(line_value[1])
//                 {
//                     case 'a':
//                     {
//                         sscanf(line_value, "%f %f %f",  &textures->textures[textures->length - 1].ambient[0],
//                                                         &textures->textures[textures->length - 1].ambient[1],
//                                                         &textures->textures[textures->length - 1].ambient[2]
//                         );
//                     } break;

//                     case 'd':
//                     {
//                         sscanf(line_value, "%f %f %f",  &textures->textures[textures->length - 1].diffuse[0],
//                                                         &textures->textures[textures->length - 1].diffuse[1],
//                                                         &textures->textures[textures->length - 1].diffuse[2]
//                         );
//                     } break;

//                     case 's':
//                     {
//                         sscanf(line_value, "%f %f %f",  &textures->textures[textures->length - 1].specular[0],
//                                                         &textures->textures[textures->length - 1].specular[1],
//                                                         &textures->textures[textures->length - 1].specular[2]
//                         );
//                     } break;
//                 }
//             } break;

//             case 'N':
//             {
//                 switch(line_type[1])
//                 {
//                     case 'i':
//                     {
//                         sscanf(line_value, "%f", &textures->textures[textures->length - 1].optical_density);
//                     } break;

//                     case 's':
//                     {
//                         sscanf(line_value, "%f", &textures->textures[textures->length - 1].specular_exponent);
//                     } break;
//                 }
//             } break;

//             case 'd':
//             {
//                 sscanf(line_value, "%f", &textures->textures[textures->length - 1].transparency);
//             } break;

//             case 'm':
//             {
//                 int width, height, channels;

//                 strtok(line_value, "\n"); // remove trailing newline from fgets

//                 unsigned char* data = image_read(line_value, &width, &height, &channels, 0, true);

//                 if(!data)
//                 {
//                     free(textures->textures);
//                     free(textures);
                    
                    
//                     return (struct GLOption)
//                     {
//                         .ok = false,
//                         .error_message = "Failed to load textures",
//                     };
//                 }
                
//                 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//                 glGenerateMipmap(GL_TEXTURE_2D);
                
//                 image_free(data);
//             } break;
//         }
//     }

//     return (struct GLOption)
//     {
//         .ok = true,
//         .result_ptr = textures,
//     };
// }

struct GLOption textures_block_load(char* data[6]) {
    GLuint texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for(unsigned int i = 0; i < 6; ++i) {
        int width, height, channels;
        unsigned char* img_data = image_read(data[i], &width, &height, &channels, 0, false);
        if(!img_data) {
            return Err("Failed to load textures");
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
        image_free(img_data);
    }

    return Ok(texture);
}

__attribute__((always_inline)) void BindTexture(GLuint* textures, unsigned int id) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[id]);
}
