#version 330 core


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;
in float visibility;

void main()
{
    vec4 blendMapColor = texture(texture5, TexCoord);
    float backgroundTexAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
    vec2 tiledCoords = TexCoord * 150.0;
    vec4 backgroundTexColor = texture(texture1, tiledCoords) * backgroundTexAmount;
    vec4 rTexColor = texture(texture2, tiledCoords) * blendMapColor.r;
    vec4 gTexColor = texture(texture3, tiledCoords) * blendMapColor.g;
    vec4 bTexColor = texture(texture4, tiledCoords) * blendMapColor.b;
    vec4 totalColor = backgroundTexColor + rTexColor + gTexColor + bTexColor;

    vec3 fog_color = vec3(1.0, 0.5, 0.2);

    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 objectColor = totalColor.xyz;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
    FragColor = mix(vec4(fog_color, 1.0), FragColor, visibility);
}
