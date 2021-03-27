#pragma once

#include "pupil/Events/Event.h"


namespace Pupil {

	class PP_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		virtual ~KeyEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode): m_KeyCode(keycode) { }
		int m_KeyCode;

	};

	class PP_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			:KeyEvent(keycode), m_RepeatCount(repeatCount) { }
		virtual ~KeyPressedEvent() = default;

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;

	};

	class PP_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode) { }
		virtual ~KeyReleasedEvent() = default;

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};

	class PP_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode)
			:KeyEvent(keycode) { }
		virtual ~KeyTypedEvent() = default;

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)

	private:
		int m_RepeatCount;

	};
}