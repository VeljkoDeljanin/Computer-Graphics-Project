#pragma once

struct ProgramState {
    static inline bool guiEnabled = false;

    static inline float guiWindowWidth = 500.0f;
    static inline float guiWindowHeight = 500.0f;

    static inline bool VSync = true;

    static inline float movementSpeed = 2.5f;
    static inline bool cursorEnabled = false;
    static inline bool flyCamera = false;

    static inline bool flashlight = false;
    static inline bool lampLights = true;
    static inline bool billboardLights = true;

    static inline bool antiAliasing = true;
    static inline bool grayscale = false;
    static inline int kernelSelected = 0;
    static inline bool sharpenKernel = false;
    static inline bool blurKernel = false;
    static inline bool edgeDetectionKernel = false;
    static inline bool embossKernel = false;
    static inline bool hdr = true;
    static inline bool bloom = true;
    static inline float exposure = 1.0f;

    static inline bool normalMapping = true;
    static inline bool parallaxMapping = true;
    static inline float heightScale = 0.1f;
};
