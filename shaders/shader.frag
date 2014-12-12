#version 330 core


in vec3 position_worldSpace; // position of vertex in world space
in vec3 position_cameraSpace; // position of vertex in camera space
in vec3 normal_cameraSpace; // normal of vertex in camera space
flat in vec3 flat_normal_cameraSpace; // normal of vertex in camera space
in vec3 eyeDirection_cameraSpace; // vector from vertex to eye
in vec3 lightPosition_cameraSpace; // direction of light in camera space

uniform mat4 mvp; // Modelview Projection matrix. This maps the vertices in model (object) space to world coordinates
uniform mat4 v; // viewing matrix (world -> camera)
uniform mat4 m; // model matrix (object -> world)
uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 color;
uniform vec3 lightPosition_worldSpace;

uniform bool useLighting;
uniform bool smoothShading;

out vec4 fragColor; //output color

void main(){
    //Convenience code for setting a single color
    vec3 diffuseColor;
    vec3 ambientColor;
    if (dot(color, vec3(1,1,1)) > 0){
        diffuseColor = color;
        ambientColor = vec3(.1,.1,.1);
    } else{
        diffuseColor = diffuse_color;
        ambientColor = ambient_color;
    }

    if (useLighting){
        vec3 normal = (smoothShading) ? normal_cameraSpace : flat_normal_cameraSpace ;
        vec3 vertex_to_light_cameraSpace = lightPosition_cameraSpace - position_cameraSpace;
        float lam = dot(vertex_to_light_cameraSpace, normal);
        float distance = length(lightPosition_worldSpace - position_worldSpace);
        float light_power = 1;

        fragColor =  vec4(ambientColor + light_power*lam*diffuseColor/(distance*distance),1);
    } else{
        fragColor = vec4(ambientColor + diffuseColor,1);
    }
}
