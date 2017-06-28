#version 130

in vec2 vertexPosition;
in vec2 uvCoordinates;
in vec4 vertexColor;

uniform mat4 perspective;

out vec2 pass_UvCoordinates;
out vec4 pass_VertexColor;

void main() {
    
    gl_Position = perspective * vec4(vertexPosition, 0.2, 1.0);
    
    pass_VertexColor = vertexColor;
    pass_UvCoordinates = uvCoordinates;

}