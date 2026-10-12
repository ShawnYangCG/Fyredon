#version 400 core
in vec2 screenPosition;
out vec4 frag_colour;
uniform int backgroundMode;
uniform vec3 solidColour, topColour, horizonColour, bottomColour;
uniform mat4 inverseProjection, inverseView;
void main() {
    vec4 viewRay = inverseProjection * vec4(screenPosition, 1.0, 1.0);
    vec3 direction = normalize(mat3(inverseView) * (viewRay.xyz / viewRay.w));
    float elevation = direction.y;
    vec3 sky = elevation >= 0.0
        ? mix(horizonColour, topColour, smoothstep(0.0, 0.8, elevation))
        : mix(horizonColour, bottomColour, smoothstep(0.0, 0.8, -elevation));
    frag_colour = vec4(backgroundMode == 0 ? solidColour : sky, 1.0);
}
