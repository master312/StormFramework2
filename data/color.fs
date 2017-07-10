#version 130

uniform vec4 colorMultiply;
uniform vec3 colorAdd;
uniform sampler2D textureUnit;

in vec2 pass_UvCoordinates;
in vec4 pass_VertexColor;

void main() {
    vec4 tmpColor = pass_VertexColor * colorMultiply;
    vec4 txtColor = texture2D(textureUnit, pass_UvCoordinates);
    tmpColor.xyz *= txtColor.xyz;
    tmpColor.xyz += colorAdd.xyz;
    tmpColor *= (colorMultiply.w * txtColor.w);
    
    gl_FragColor = tmpColor;

}