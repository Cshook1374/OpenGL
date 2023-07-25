#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textures;

out vec3 vertexColor;
out vec2 textureCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main(void) {
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    vertexColor = vec3(position + 0.5);
    textureCoords = textures;
}
