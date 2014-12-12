#version 330 core
#define M_PI 3.1415926535897932384626433832795

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

//out vec3 position_worldSpace; // position of vertex in world space
//flat out vec3 flat_normal_cameraSpace; // normal of vertex in camera space
//out vec3 eyeDirection_cameraSpace; // vector from vertex to eye
//out vec3 lightPosition_cameraSpace; // direction of light in camera space

uniform float radius;
uniform vec3 center;
uniform bool wrap = false;

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

        vec3 position_worldSpace = old_position_worldSpace;
        vec3 new_position = (inverse(m) * vec4(position_worldSpace, 1.0)).xyz;
        position_cameraSpace = (v * m * vec4(new_position, 1.0));

        /*
        vec3 vertexPosition_cameraSpace = (v*m * vec4(new_position,1)).xyz;
        eyeDirection_cameraSpace = vec3(0,0,0) - vertexPosition_cameraSpace;

        lightPosition_cameraSpace = (v * vec4(lightPosition_worldSpace, 1)).xyz;

        flat_normal_cameraSpace = normal_cameraSpace = -( inverse(transpose(v * m)) * vec4(normalize(normal), 0)).xyz;
        */

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
        if(isBackFace) {
            gl_Position = mvp * vec4(position*1.02, 1.0);
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
}
