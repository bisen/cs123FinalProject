#version 330 core

in vec3 position; // Position of the vertex
in vec3 normal; // normal of the vertex

out vec3 position_worldSpace; // position of vertex in world space
out vec3 position_cameraSpace; // position of vertex in camera space
out vec3 normal_cameraSpace; // normal of vertex in camera space
flat out vec3 flat_normal_cameraSpace; // normal of vertex in camera space
out vec3 eyeDirection_cameraSpace; // vector from vertex to eye
out vec3 lightPosition_cameraSpace; // direction of light in camera space

uniform mat4 mvp; // Modelview Projection matrix. This maps the vertices in model (object) space to screen coordinates
uniform mat4 v; // viewing matrix (world -> camera)
uniform mat4 m; // model matrix (object -> world)
uniform vec3 lightPosition_worldSpace;

void main(){
        position_worldSpace = (m * vec4(position, 1.0)).xyz;
        position_cameraSpace = (v * m * vec4(position, 1.0)).xyz;

        vec3 vertexPosition_cameraSpace = (v*m * vec4(position,1)).xyz;
        eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace;

        lightPosition_cameraSpace = (v * vec4(lightPosition_worldSpace, 1)).xyz;

        flat_normal_cameraSpace = normal_cameraSpace = ( inverse(transpose(v * m)) * vec4(normalize(normal), 0)).xyz;

        gl_Position =  mvp*vec4(position, 1.0);
}
