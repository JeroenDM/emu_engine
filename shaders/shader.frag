#version 400 core

precision mediump float;

in vec4 Color;

void main() {
    gl_FragColor = Color;
}