#version 330 core
out vec3 FragColor;
  
in vec2 UV;

uniform sampler2D texSampler;

void main()
{
    FragColor = texture(texSampler, UV).rgb;
    // FragColor = vec3(1.0, 0.0, 0.0);
}