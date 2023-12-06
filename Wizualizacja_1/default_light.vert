#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 3) in vec3 aNormal;

out vec3 Normal;
out vec3 Color;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main(){
Color = color;
gl_Position = proj*view*model*vec4(position, 1.0);
FragPos = vec3(model * vec4(position, 1.0));
Normal = mat3(transpose(inverse(model))) * aNormal;
}