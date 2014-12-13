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
uniform bool useCelShading = true;
uniform vec3 allBlack = vec3(1);

uniform int textureWidth;
uniform int textureHeight;

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
        outpos = outpos - vec4( ((inpos.y - 0.5) * newx), 0.0, ((inpos.y - 0.5) * newz), 0.0);
    } else if(inpos.y < -0.5) {
        outpos = outpos + vec4( ((inpos.y + 0.5) * newx), 0.0, ( (inpos.y + 0.5) * newz), 0.0);
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
        vec4 cyl_normal = transform_to_cylinder(normal);

        vec4 normal_objectSpace = normalize(vec4(cyl_normal.xyz, 0.0));

        position_cameraSpace = (v * m * cyl_position);
        normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal_objectSpace.xyz), 0);
        lightDir = normalize(v * vec4(lightPosition_worldSpace, 1) - (position_cameraSpace));

        gl_Position =  mvp*cyl_position;
    } else {
        if(isBackFace && useCelShading) {
            if(abs(position.y) < 0.499f) {
                gl_Position = mvp * vec4(position*1.025, 1.0);
            } else {
                gl_Position= mvp * vec4(position, 1.0);
            }
        } else {
            texc = texCoord;
            position_cameraSpace = v * m * vec4(position, 1.0);
            normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);
            pu = normalize(mat3(transpose(inverse(v * m))) * tangent);

            gl_Position = mvp * vec4(position, 1.0);

            vec4 vertexToLight = vec4(0);
            // Point Light
            lightDir = normalize(v * vec4(lightPosition_worldSpace, 1) - (position_cameraSpace));
        }
    }
}
