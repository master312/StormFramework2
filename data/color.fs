#version 130

uniform vec4 colorOverlay;
uniform sampler2D textureUnit;

in vec2 pass_UvCoordinates;
in vec4 pass_VertexColor;

void main() {
    vec4 tmpColor = pass_VertexColor * colorOverlay;
    tmpColor *= colorOverlay.w;

    gl_FragColor = texture2D(textureUnit, pass_UvCoordinates) * tmpColor;
}