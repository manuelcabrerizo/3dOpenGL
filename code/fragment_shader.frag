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

out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;
in float visibility;
uniform sampler2D texture1;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform int transparent;

void main()
{
    vec3 fog_color = vec3(1.0, 0.5, 0.2);

    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(normal);
    if(transparent == 1)
    {
        norm = normalize(vec3(0.0, 1.0, 0.0));
    }
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 objectColor = texture(texture1, TexCoord).xyz;
    if(transparent == 1)
    {
        if(objectColor.r >= 0.2)
        {
            if(objectColor.g >= 0.2)
            {
                if(objectColor.b >= 0.2)
                {
                    discard;
                }
            }
        }
    }


    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular) * objectColor; 
    FragColor = vec4(result, 1.0);
    FragColor = mix(vec4(fog_color, 1.0), FragColor, visibility);
}
