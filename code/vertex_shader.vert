#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out float visibility;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

const float density = 0.007;
const float gradient = 1.5;

void main()
{
    vec4 vertPosRelativeToCamera = view * world * vec4(aPos, 1.0);
    float dist = length(vertPosRelativeToCamera.xyz);
    visibility = exp(-pow((dist * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    gl_Position = proj * view * world * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(world))) * aNormal;
    fragPos = vec3(world * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
}
