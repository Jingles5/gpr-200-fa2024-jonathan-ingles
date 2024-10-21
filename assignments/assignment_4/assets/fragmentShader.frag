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
void main()
{
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}