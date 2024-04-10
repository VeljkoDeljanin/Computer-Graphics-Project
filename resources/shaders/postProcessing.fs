#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS hdrBuffer;
uniform sampler2DMS bloomBlur;
uniform sampler2DMS screenTexture;

uniform int framebufferWidth;
uniform int framebufferHeight;

uniform bool grayscaleEnabled;

uniform bool sharpenKernelEnabled;
uniform bool blurKernelEnabled;
uniform bool edgeDetectionKernelEnabled;
uniform bool embossKernelEnabled;

uniform bool hdr;
uniform bool bloom;
uniform float exposure;

ivec2 offsets[9] = ivec2[](
    ivec2(-1,  1), // top-left
    ivec2( 0,  1), // top-center
    ivec2( 1,  1), // top-right
    ivec2(-1,  0), // center-left
    ivec2( 0,  0), // center-center
    ivec2( 1,  0), // center-right
    ivec2(-1, -1), // bottom-left
    ivec2( 0, -1), // bottom-center
    ivec2( 1, -1)  // bottom-right
);

float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

float edgeDetectionKernel[9] = float[](
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
);

float embossKernel[9] = float[](
    -2, -1,  0,
    -1,  1,  1,
     0,  1,  2
);

float identityKernel[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
);

vec3 CalculateColor(float[9] kernel);

void main() {
    vec3 color;

    if (sharpenKernelEnabled)
        color = CalculateColor(sharpenKernel);
    else if (blurKernelEnabled)
        color = CalculateColor(blurKernel);
    else if (edgeDetectionKernelEnabled)
        color = CalculateColor(edgeDetectionKernel);
    else if (embossKernelEnabled)
        color = CalculateColor(embossKernel);
    else
        color = CalculateColor(identityKernel);

    FragColor = vec4(color, 1.0f);
}

vec3 CalculateColor(float[9] kernel) {
    ivec2 viewportDim = ivec2(framebufferWidth, framebufferHeight);
    ivec2 coords = ivec2(viewportDim * TexCoords);

    vec3 sampleTexAA[9];
    vec3 sampleTexHDR[9];
    vec3 sampleTexBloom[9];
    vec3 finalColor[9];

    for (int i = 0; i < 9; i++) {
        vec3 sample0 = texelFetch(screenTexture, coords + offsets[i], 0).rgb;
        vec3 sample1 = texelFetch(screenTexture, coords + offsets[i], 1).rgb;
        vec3 sample2 = texelFetch(screenTexture, coords + offsets[i], 2).rgb;
        vec3 sample3 = texelFetch(screenTexture, coords + offsets[i], 3).rgb;

        sampleTexAA[i] = 0.25f * (sample0 + sample1 + sample2 + sample3);

        sample0 = texelFetch(hdrBuffer, coords + offsets[i], 0).rgb;
        sample1 = texelFetch(hdrBuffer, coords + offsets[i], 1).rgb;
        sample2 = texelFetch(hdrBuffer, coords + offsets[i], 2).rgb;
        sample3 = texelFetch(hdrBuffer, coords + offsets[i], 3).rgb;

        sampleTexHDR[i] = 0.25f * (sample0 + sample1 + sample2 + sample3);

        sample0 = texelFetch(bloomBlur, coords + offsets[i], 0).rgb;
        sample1 = texelFetch(bloomBlur, coords + offsets[i], 1).rgb;
        sample2 = texelFetch(bloomBlur, coords + offsets[i], 2).rgb;
        sample3 = texelFetch(bloomBlur, coords + offsets[i], 3).rgb;

        sampleTexBloom[i] = 0.25f * (sample0 + sample1 + sample2 + sample3);

        if (bloom)
            sampleTexHDR[i] += sampleTexBloom[i];

        vec3 hdrResult;
        if (hdr)
            hdrResult = vec3(1.0f) - exp(-sampleTexHDR[i] * exposure);
        else
            hdrResult = sampleTexHDR[i];

        if (grayscaleEnabled) {
            float grayscale = 0.2126f * sampleTexAA[i].r + 0.7152f * sampleTexAA[i].g + 0.0722f * sampleTexAA[i].b
                            + 0.2126f * hdrResult.r + 0.7152f * hdrResult.g + 0.0722f * hdrResult.b;
            finalColor[i] = vec3(grayscale);
        }
        else
            finalColor[i] = mix(sampleTexAA[i], hdrResult, 0.5f);
    }

    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++)
        color += finalColor[i] * kernel[i];

    return color;
}
