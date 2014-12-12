#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec3 position; // Position of the vertex
in vec3 normal; // normal of the vertex

out vec3 position_worldSpace; // position of vertex in world space
out vec3 position_cameraSpace; // position of vertex in camera space
out vec3 normal_cameraSpace; // normal of vertex in camera space
flat out vec3 flat_normal_cameraSpace; // normal of vertex in camera space
out vec3 eyeDirection_cameraSpace; // vector from vertex to eye
out vec3 lightPosition_cameraSpace; // direction of light in camera space

uniform float radius;
uniform vec3 center;
uniform bool wrap = false;

uniform mat4 mvp; // Modelview Projection matrix. This maps the vertices in model (object) space to screen coordinates
uniform mat4 v; // viewing matrix (world -> camera)
uniform mat4 m; // model matrix (object -> world)
uniform vec3 lightPosition_worldSpace;

void main(){

    if(wrap) {
        vec3 newcenter = (m * vec4(center.xyz, 1.0)).xyz;
        vec3 old_position_worldSpace = (m * vec4(position, 1.0)).xyz;

        float circumference = 2 * M_PI * radius;

        float u = old_position_worldSpace.x / circumference;
        float theta = u * 2 * M_PI;

        float newx = radius * cos(theta);
        float newz = (radius-old_position_worldSpace.z) * sin(theta);

        vec4 cyl_position = vec4(newx, old_position_worldSpace.y, newz, 1.0) + vec4(center.xyz, 1.0);

        position_worldSpace = old_position_worldSpace;
        vec3 new_position = (inverse(m) * vec4(position_worldSpace, 1.0)).xyz;
        position_cameraSpace = (v * m * vec4(new_position, 1.0)).xyz;

        vec3 vertexPosition_cameraSpace = (v*m * vec4(new_position,1)).xyz;
        eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace;

        lightPosition_cameraSpace = (v * vec4(lightPosition_worldSpace, 1)).xyz;

        flat_normal_cameraSpace = normal_cameraSpace = -( inverse(transpose(v * m)) * vec4(normalize(normal), 0)).xyz;

        /*
      (0, max)
      ^
      | v
      |
      |     u
      *------>
      (0,0)    (max,0)
    */

        gl_Position =  mvp*inverse(m)*cyl_position;
    } else {

        position_worldSpace = (m * vec4(position, 1.0)).xyz;
        position_cameraSpace = (v * m * vec4(position, 1.0)).xyz;

        vec3 vertexPosition_cameraSpace = (v*m * vec4(position,1)).xyz;
        eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace;

        lightPosition_cameraSpace = (v * vec4(lightPosition_worldSpace, 1)).xyz;

        flat_normal_cameraSpace = normal_cameraSpace = ( inverse(transpose(v * m)) * vec4(normalize(normal), 0)).xyz;

        gl_Position =  mvp*vec4(position, 1.0);
    }
}
