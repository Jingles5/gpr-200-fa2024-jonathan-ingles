#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec4 Color;
uniform float uTime = 1;
uniform vec4 uColor;
uniform sampler2D ourTexture;

in vec3 lightColor;
in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 lightPos;  


void main()
{
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * ourColor;
    FragColor = vec4(result, 1.0);
}