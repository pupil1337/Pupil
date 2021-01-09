#pragma once

#include"pupil/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pupil/Renderer/GraphicsContext.h"

namespace Pupil {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// windows attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	};

}