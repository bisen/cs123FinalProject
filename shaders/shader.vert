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
out vec4 normal_worldSpace;
out vec4 normal_objectSpace;

// Transformation matrices
uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;

// Light data
uniform vec3 lightPosition_worldSpace;

uniform bool useLighting;     // Whether to calculate lighting using lighting equation
uniform bool isBackFace;
uniform bool useCelShading = true;
uniform vec3 allBlack = vec3(1);

uniform int textureWidth;
uniform int textureHeight;

//out vec3 position_worldSpace; // position of vertex in world space
//flat out vec3 flat_normal_cameraSpace; // normal of vertex in camera space
//out vec3 eyeDirection_cameraSpace; // vector from vertex to eye
//out vec3 lightPosition_cameraSpace; // direction of light in camera space

uniform float radius;
uniform float size;
uniform bool wrap = false;

vec4 transform_to_cylinder(vec3 inpos) {

    float circumference = 2 * M_PI * radius;
    float u = inpos.x / circumference;
    float theta = u * 2 * M_PI;

    vec4 outpos = vec4(0.0);
    float r = -radius + position.z;

    float newx = r * cos(theta);
    float newz = r * sin(theta);

    outpos = vec4(newx, inpos.y, newz, 1.0);
    if(inpos.y > 0.5) {
//        vec4 flatpos = vec4(-cos(theta) * (0.5 - (0.5 * (inpos.y - 0.5))), inpos.y, -sin(theta) * (0.5 - (0.5 * (inpos.y - 0.5))), 1.0);
//        vec4 norm = normalize(vec4(cos(theta), 1.0, sin(theta), 0.0));
//        return (inpos.z * norm) + flatpos;
        outpos = outpos + vec4( ((0.34) * (inpos.y - 0.5)), 0.0, ((0.34) * (inpos.y - 0.5)), 0.0);
    } else if(inpos.y < -0.5) {
//        vec4 flatpos = vec4(-cos(theta) * (0.5 + (0.5 * (inpos.y + 0.5))), inpos.y, -sin(theta) * (0.5 + (0.5 * (inpos.y + 0.5))), 1.0);
//        vec4 norm = normalize(vec4(cos(theta), -1.0, sin(theta), 0.0));
//        return (inpos.z * norm) + flatpos;
        outpos = outpos - vec4( ((0.34) * (inpos.y + 0.5)), 0.0, ((0.34) * (inpos.y + 0.5)), 0.0);
    }

    return outpos;
}

vec4 inverse_transform(vec3 outpos) {
    float circumference = 2 * M_PI * radius;
    float theta = atan(outpos.z / outpos.x);

    float u = theta / (2 * M_PI);

    float x = circumference * u;

    float r = x / cos(theta);
    float z = r + radius;

    return vec4(x, outpos.y, z, 0);
}

void main(){

    if(wrap) {
        texc = texCoord;

        vec4 cyl_position = transform_to_cylinder(position);
        //vec4 cyl_position2 = transform_to_cylinder(position + normal);
        vec4 cyl_normal = transform_to_cylinder(normal);

        normal_objectSpace = normalize(vec4(cyl_normal.xyz, 0.0));

//        vec3 position_worldSpace = (m * vec4(position, 1.0)).xyz;
//        vec3 new_position = (inverse(m) * vec4(position_worldSpace, 1.0)).xyz;
        position_cameraSpace = (v * m * cyl_position);
        normal_worldSpace = normalize((inverse(v) * normal_objectSpace));
        normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal_objectSpace.xyz), 0);
        lightDir = normalize(v * vec4(lightPosition_worldSpace, 1) - (position_cameraSpace));

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

        gl_Position =  mvp*cyl_position;
    } else {
        if(isBackFace && useCelShading) {
            if(abs(position.y) < 0.499f) {
                gl_Position = mvp * vec4(position*1.025, 1.0);
            } else {
                gl_Position= mvp * vec4(position, 1.0);
            }
        } else {
            texc = texCoord; //vec2((texCoord.x*2)-floor(texCoord.x*3),texCoord.y);

            position_cameraSpace = v * m * vec4(position, 1.0);
            normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);
            pu = normalize(mat3(transpose(inverse(v * m))) * tangent);

            normal_worldSpace = normalize(inverse(m) * vec4(normal.xyz, 0));
            normal_objectSpace = vec4(normal,0);

            gl_Position = mvp * vec4(position, 1.0);

            vec4 vertexToLight = vec4(0);
            // Point Light
            lightDir = normalize(v * vec4(lightPosition_worldSpace, 1) - (position_cameraSpace));
        }
    }
}
