#version 130

uniform vec4 colorMultiply;
uniform vec3 colorAdd;
uniform sampler2D textureUnit;

in vec2 pass_UvCoordinates;
in vec4 pass_VertexColor;

void main() {
    vec4 tmpColor = pass_VertexColor * colorMultiply;
    tmpColor.xyz += colorAdd.xyz;
    tmpColor *= colorMultiply.w;
    tmpColor *= texture2D(textureUnit, pass_UvCoordinates);
    
    gl_FragColor = tmpColor;

}