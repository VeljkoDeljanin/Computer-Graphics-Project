#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool inversionEnabled;
uniform bool grayscaleEnabled;

uniform bool sharpenKernelEnabled;
uniform bool blurKernelEnabled;
uniform bool edgeDetectionKernelEnabled;

const float offset = 1.0f / 300.0f;
vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
);

float identityKernel[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
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

vec3 CalculateColor(float[9] kernel);

void main() {
    vec3 color;

    if (sharpenKernelEnabled)
        color = CalculateColor(sharpenKernel);
    else if (blurKernelEnabled)
        color = CalculateColor(blurKernel);
    else if (edgeDetectionKernelEnabled)
        color = CalculateColor(edgeDetectionKernel);
    else if (inversionEnabled)
        color = vec3(1.0f - texture(screenTexture, TexCoords));
    else if (grayscaleEnabled) {
        vec4 texColor = texture(screenTexture, TexCoords);
        float avg = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
        color = vec3(avg);
    }
    else
        color = CalculateColor(identityKernel);

    FragColor = vec4(color, 1.0f);
}

vec3 CalculateColor(float[9] kernel) {
    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
#if 0
        if (inversionEnabled)
            sampleTex[i] = vec3(1.0f - texture(screenTexture, TexCoords));
        else if (grayscaleEnabled) {
            vec4 texColor = texture(screenTexture, TexCoords);
            float avg = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
            sampleTex[i] = vec3(avg);
        }
        else
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
#else
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
#endif
    }

    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];

    return color;
}
