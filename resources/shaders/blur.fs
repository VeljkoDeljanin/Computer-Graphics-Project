#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS image;

uniform int framebufferWidth;
uniform int framebufferHeight;

uniform bool horizontal;
const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main() {
    ivec2 viewportDim = ivec2(framebufferWidth, framebufferHeight);
    ivec2 coords = ivec2(viewportDim * TexCoords);

    vec3 sample0 = texelFetch(image, coords, 0).rgb;
    vec3 sample1 = texelFetch(image, coords, 1).rgb;
    vec3 sample2 = texelFetch(image, coords, 2).rgb;
    vec3 sample3 = texelFetch(image, coords, 3).rgb;

    vec3 result = (0.25f * (sample0 + sample1 + sample2 + sample3)) * weight[0];

    if (horizontal) {
        for (int i = 1; i < 5; i++) {
            sample0 = texelFetch(image, coords + ivec2(i, 0.0f), 0).rgb;
            sample1 = texelFetch(image, coords + ivec2(i, 0.0f), 1).rgb;
            sample2 = texelFetch(image, coords + ivec2(i, 0.0f), 2).rgb;
            sample3 = texelFetch(image, coords + ivec2(i, 0.0f), 3).rgb;

            result += (0.25f * (sample0 + sample1 + sample2 + sample3)) * weight[i];

            sample0 = texelFetch(image, coords - ivec2(i, 0.0f), 0).rgb;
            sample1 = texelFetch(image, coords - ivec2(i, 0.0f), 1).rgb;
            sample2 = texelFetch(image, coords - ivec2(i, 0.0f), 2).rgb;
            sample3 = texelFetch(image, coords - ivec2(i, 0.0f), 3).rgb;

            result += (0.25f * (sample0 + sample1 + sample2 + sample3)) * weight[i];
        }
    }
    else {
        for (int i = 1; i < 5; i++) {
            sample0 = texelFetch(image, coords + ivec2(0.0f, i), 0).rgb;
            sample1 = texelFetch(image, coords + ivec2(0.0f, i), 1).rgb;
            sample2 = texelFetch(image, coords + ivec2(0.0f, i), 2).rgb;
            sample3 = texelFetch(image, coords + ivec2(0.0f, i), 3).rgb;

            result += (0.25f * (sample0 + sample1 + sample2 + sample3)) * weight[i];

            sample0 = texelFetch(image, coords - ivec2(0.0f, i), 0).rgb;
            sample1 = texelFetch(image, coords - ivec2(0.0f, i), 1).rgb;
            sample2 = texelFetch(image, coords - ivec2(0.0f, i), 2).rgb;
            sample3 = texelFetch(image, coords - ivec2(0.0f, i), 3).rgb;

            result += (0.25f * (sample0 + sample1 + sample2 + sample3)) * weight[i];
        }
    }

    FragColor = vec4(result, 1.0f);
}