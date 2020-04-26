#version 330 core
out vec4 FragColor;
  
in vec3 UV;

uniform samplerCube texSampler;

void main()
{
    FragColor = texture(texSampler, UV);
    // FragColor = vec3(1.0, 0.0, 0.0);
}