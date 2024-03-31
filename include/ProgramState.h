#pragma once

struct ProgramState {
    static inline bool flashlight = false;

    static inline bool VSync = true;

    static inline bool inversion = false;
    static inline bool grayscale = false;

    static inline bool sharpenKernel = false;
    static inline bool blurKernel = false;
    static inline bool edgeDetectionKernel = false;
};
