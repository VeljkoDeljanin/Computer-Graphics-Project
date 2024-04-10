#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 lightColor;
uniform sampler2D image;

void main() {
    FragColor = vec4(lightColor * vec3(texture(image, fs_in.TexCoords)), 1.0f);

    float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if (brightness > 1.0f)
        BrightColor = vec4(FragColor.rgb, 1.0f);
    else
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}