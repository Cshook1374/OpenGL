#version 330 core

in vec3 vertexColor;
in vec2 textureCoords;

uniform sampler2D textureSampler;

void main(void) {
    gl_FragColor = texture(textureSampler, textureCoords);
}