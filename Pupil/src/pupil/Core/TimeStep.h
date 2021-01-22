#pragma once
#include "pupil/Core/Core.h"

namespace Pupil {

	class PP_API TimeStep {
	public:
		TimeStep(float time = 0.0f) : m_Time(time) {

		}

		operator float() const { return m_Time; }

		inline float GetSecond() { return m_Time; }
		inline float GetMilliSecond() { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}