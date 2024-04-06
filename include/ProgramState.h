#pragma once

struct ProgramState {
    static inline bool flashlight = false;

    static inline bool VSync = true;

    static inline bool grayscale = false;

    static inline bool sharpenKernel = false;
    static inline bool blurKernel = false;
    static inline bool edgeDetectionKernel = false;
    static inline bool embossKernel = false;

    static inline bool antiAliasing = true;

    static inline bool normalMapping = true;
    static inline bool parallaxMapping = true;
};
