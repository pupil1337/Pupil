#pragma once

#include"Event.h"


namespace Pupil {

	class PP_API WindowResizedEvent : public Event {
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		virtual ~WindowResizedEvent() = default;

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int m_Width, m_Height;
	};

	class PP_API WindowClosedEvent : public Event {
	public:
		WindowClosedEvent() { }
		virtual ~WindowClosedEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)

	};

	class PP_API AppTickEvnet : public Event {
	public:
		AppTickEvnet() { }
		virtual ~AppTickEvnet() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)

	};

	class PP_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() { }
		virtual ~AppUpdateEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)

	};

	class PP_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() { }
		virtual ~AppRenderEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)

	};

}