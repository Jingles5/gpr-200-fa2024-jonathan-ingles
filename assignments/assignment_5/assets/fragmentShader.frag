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

uniform vec3 lightColor;
in vec3 FragPos;  
in vec3 Normal;  
uniform vec3 lightPos;
uniform vec3 ambientStrength;
uniform vec3 viewPos;  

void main()
{   


vec3 lightDir   = normalize(lightPos - FragPos);
vec3 viewDir    = normalize(viewPos - FragPos);
vec3 halfwayDir = normalize(lightDir + viewDir);

// Ambient
    vec3 ambient = ambientStrength * lightColor; 

    // Diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor; 

    //Specular
    float specularStrength = 0.5;
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = lightColor * spec;

    // Combine results
    vec4 textureColor = texture(texture1, TexCoord);
    vec3 result = (ambient + diffuse + specular) * textureColor.rgb;
    
    FragColor = vec4(result, 1.0);
}