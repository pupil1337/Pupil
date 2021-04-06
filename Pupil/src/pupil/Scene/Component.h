#pragma once
#include "pupil/Core/Core.h"

#include <glm/glm.hpp>
#include "pupil/Scene/SceneCamera.h"
#include "pupil/Scene/ScriptEntity.h"
#include "pupil/Core/TimeStep.h"

namespace Pupil {

	struct TagComponent {
		TagComponent() = default;
		TagComponent(const std::string& name) : Name(name) { }

		TagComponent(const TagComponent& Tag) = default;

		/// data ///
		std::string Name;
	};

	struct TransformComponent {
		TransformComponent() = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) { }
		TransformComponent(const TransformComponent& Transform) = default;

		glm::mat4 GetTransform() {
					// translate
			return glm::translate(glm::mat4(1.0f), translation)
					// rotate
				 * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
				 * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
				 * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
					// scale
				 * glm::scale(glm::mat4(1.0f), scale);
				
		}

		/// data ///
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation    = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale       = { 1.0f, 1.0f, 1.0f };
	};


	struct ColorComponent {
		ColorComponent() = default;
		ColorComponent(const glm::vec4& color): Color(color) { }

		ColorComponent(const ColorComponent& Color) = default;

		ColorComponent& operator=(ColorComponent& rhs) {
			Color = rhs.Color;
			return *this;
		}

		/// data ///
		glm::vec4 Color{1.0f};
	};

	struct CameraComponent {
		CameraComponent() = default;
		CameraComponent(const CameraComponent& rhs) = default;

		/// data ///
		SceneCamera Camera;
		bool Primary = true;
		bool FixAspectRatio = true;
	};

	struct NativeScriptComponent {
		
		/// data ///
		ScriptEntity* Instance = nullptr;

		ScriptEntity* (*CreateNativeScriptComp)();
		void (*DestroyNativeScriptComp)(NativeScriptComponent* nsc);

		template<typename T>
		void Bind() {
			CreateNativeScriptComp = []() { return static_cast<ScriptEntity*>(new T()); };
			DestroyNativeScriptComp = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			// todo // ���ڴ�й©(������ֱ����������������������Ȳ��ܡ���˵���Ƿ���ȷ��
		}
	};

}