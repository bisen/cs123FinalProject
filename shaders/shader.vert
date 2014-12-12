#version 330 core

in vec3 position; // Position of the vertex
in vec3 normal; // Normal of the vertex
in vec3 tangent; // Tangent at the vertex (in +u direction)
in vec2 texCoord; // UV texture coordinates

out vec4 lightDir; // Light directions
out vec2 texc;
out vec4 position_cameraSpace;
out vec4 normal_cameraSpace;
out vec3 pu;

// Transformation matrices
uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;

// Light data
uniform vec3 lightPosition_worldSpace;

uniform bool useLighting;     // Whether to calculate lighting using lighting equation
uniform bool isBackFace;
uniform vec3 allBlack = vec3(1);

uniform int textureWidth;
uniform int textureHeight;

void main(){
    if(isBackFace) {
        gl_Position = mvp * vec4(position*1.05, 1.0);
    } else {
        texc = texCoord; //vec2((texCoord.x*2)-floor(texCoord.x*3),texCoord.y);

        position_cameraSpace = v * m * vec4(position, 1.0);
        normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);
        pu = normalize(mat3(transpose(inverse(v * m))) * tangent);

        gl_Position = mvp * vec4(position, 1.0);

        vec4 vertexToLight = vec4(0);
        // Point Light
        lightDir = normalize(v * vec4(lightPosition_worldSpace, 1) - position_cameraSpace);
    }
}
