#pragma once
#include "NativeMirageAPI.h" // Подключаем нативный заголовок

namespace MirageAPI {
	public ref class GraphicsWrapper {
	public:
		GraphicsWrapper();
		~GraphicsWrapper();
        
		void Initialize();
		void RenderFrame(float deltaTime);
        
	private:
		NativeRenderer* nativeRenderer; // Указатель на нативный объект
	};
}