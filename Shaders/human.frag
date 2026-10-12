#version 400 core
in vec3 colour;
in vec3 pos_world;
in vec3 normal_world;
in vec3 texture_uv;
uniform sampler2D tex;
uniform vec3 camPos_world, lightPos_world;
uniform float lightStr;
uniform int lightType;
uniform int useTexture;
uniform vec3 uvOffset;
uniform vec3 uvScale;
uniform vec3 lightColour, lightDirection, ambientColour;
uniform float ambientStrength, specularStrength, roughness;
out vec4 frag_colour;
vec3 safeNormalize(vec3 v) { return v / max(length(v), 0.00001); }
void main() {
    vec3 n = safeNormalize(normal_world);
    vec3 direction = dot(lightDirection, lightDirection) > 0.00001 ? lightDirection : vec3(0,-1,0);
    vec3 l = safeNormalize(lightType == 0 ? -direction : lightPos_world - pos_world);
    vec3 v = safeNormalize(camPos_world - pos_world);
    vec3 h = safeNormalize(l + v);
    vec3 albedo = useTexture != 0 ? texture(tex, texture_uv.xy * uvScale.xy + uvOffset.xy).rgb : colour;
    float diffuse = max(dot(n, l), 0.0);
    float exponent = mix(128.0, 4.0, clamp(roughness, 0.0, 1.0));
    float specular = diffuse > 0.0 ? pow(max(dot(n, h), 0.0), exponent) * specularStrength : 0.0;
    vec3 ambient = albedo * ambientColour * ambientStrength;
    frag_colour = vec4(ambient + lightColour * lightStr * (albedo * diffuse + specular), 1.0);
}