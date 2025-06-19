#include "pch.h"
#include "MirageAPI.h"

namespace MirageAPI {
    GraphicsWrapper::GraphicsWrapper() {
        nativeRenderer = new NativeRenderer(); 
    }
    
    GraphicsWrapper::~GraphicsWrapper() {
        if (nativeRenderer) {
            delete nativeRenderer;
            nativeRenderer = nullptr;
        }
    }
    
    void GraphicsWrapper::Initialize() {
        nativeRenderer->Init();
    }
    
    void GraphicsWrapper::RenderFrame(float deltaTime) {
        nativeRenderer->Render(deltaTime);
    }
}