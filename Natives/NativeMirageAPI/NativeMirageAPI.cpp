#include "pch.h"
#include <iostream>

#include "NativeMirageAPI.h"

auto NativeRenderer::Init() -> void
{
    std::cout << "NativeRenderer::Init";
}

auto NativeRenderer::Render(float dt) -> void
{
    std::cout << "NativeRenderer::Render";
}
