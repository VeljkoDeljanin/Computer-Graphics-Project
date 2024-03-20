#include "Renderer.h"

int main() {

    Render::Renderer &renderer = Render::Renderer::GetInstance();

    renderer.Init();

    while (renderer.IsRunning()) {
        renderer.Update();
    }

    return 0;
}