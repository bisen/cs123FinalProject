#version 330 core

in vec3 color;
in vec2 texc;
in vec4 lightDir;
in vec4 position_cameraSpace;
in vec4 normal_cameraSpace;
in vec3 pu;
out vec4 fragColor;

uniform bool isBackFace;
uniform bool useCelShading = true;
uniform bool useBumpMapping = true;
uniform bool blendTexture = true;

uniform sampler2D tex;
uniform int useTexture = 0;
uniform int textureWidth;
uniform int textureHeight;
uniform float blend;

uniform vec3 lightColor = vec3(1.0f,1.0f,0.8f);

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color = vec3(0.4f);
uniform vec3 specular_color;
uniform float shininess;

void main(){
    if(isBackFace && useCelShading) {
        fragColor = vec4(0.16,0.16,0.04,1);
    } else {
        vec3 texColor = texture(tex, texc).rgb;
        //texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
        vec4 normal = normal_cameraSpace;
        if(useTexture > 0 && useBumpMapping) {
            vec3 pv = cross(vec3(normal_cameraSpace),pu);
            float eu = 1.0/textureWidth;
            float ev = 1.0/textureHeight;
            float fu = (texture(tex,vec2(texc.x+eu,texc.y)).x - texture(tex,vec2(texc.x-eu,texc.y)).x)/(50*eu);
            float fv = (texture(tex,vec2(texc.x,texc.y+ev)).x - texture(tex,vec2(texc.x,texc.y-ev)).x)/(50*ev);
            normal = vec4(normalize(vec3(normal) + (fu * pu) - (fv * pv)),0);
        }

        vec3 color = ambient_color;
        if(useCelShading) {
            float intensity = 0;
            intensity += max(0.0, dot(lightDir, normal));
            if(intensity > 0.95) {
                color += diffuse_color;
            } else if(intensity > 0.8) {
                color += (2.0/3.0) * diffuse_color;
            } else if(intensity > 0.4) {
                color += (0.8/3.0) * diffuse_color;
            } else if(intensity > 0.2) {
                color += (0.4/3.0) * diffuse_color;
            } else {
                color += (0.2/3.0) * diffuse_color;
            }
        } else {
            if(lightColor != vec3(0)) {
                // Add diffuse component
                float diffuseIntensity = max(0.0, dot(lightDir, normal));
                color += max(vec3(0), lightColor * diffuse_color * diffuseIntensity);

                // Add specular component
                vec4 lightReflection = normalize(-reflect(lightDir, normal));
                vec4 eyeDirection = normalize(vec4(0,0,0,1) - position_cameraSpace);
                float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), 5);
                color += max (vec3(0), lightColor * specular_color * specIntensity);
            }
        }

        fragColor = vec4(color.xyz,1);
        if(blendTexture)
            fragColor = vec4(clamp(color*(1-blend) + blend*texColor, vec3(0),vec3(1)), 1);
    }
}
