#include "pppch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template <>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Pupil {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, const Entity& entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "EntityID" << YAML::Value << "2500698263";// ToDo entity id

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag.Name;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << transform.translation;
			out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& camera = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType"   << YAML::Value << (int)camera.Camera.GetProjectionType();
			out << YAML::Key << "Primary"          << YAML::Value << camera.Primary;
			out << YAML::Key << "FixAspectRatio"   << YAML::Value << camera.FixAspectRatio;
			out << YAML::Key << "AspectRatio"      << YAML::Value << camera.Camera.GetAspectRatio();
			out << YAML::Key << "PerspectiveFov"   << YAML::Value << camera.Camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear"  << YAML::Value << camera.Camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar"   << YAML::Value << camera.Camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.Camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.Camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar"  << YAML::Value << camera.Camera. GetOrthographicFar();
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ColorComponent>()) {
			auto& color = entity.GetComponent<ColorComponent>();
			out << YAML::Key << "ColorComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << color.Color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filePath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "UnDefine Name";// ToDo for name
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			PP_CORE_INFO("entityid: {0}", (uint32_t)entityID);
			Entity entity{ entityID, &m_Scene.get()->m_Registry };
			if (!entity) return;
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream outFile(filePath);
		outFile << out.c_str();
	}

	bool SceneSerializer::DeSerialize(const std::string& filePath) {
		YAML::Node data = YAML::LoadFile(filePath);
		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PP_CORE_INFO("DeSerialize the Scene: {0}", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				std::string entityName;
				uint64_t uuid = entity["EntityID"].as<uint64_t>();
				
				auto tagComponent = entity["TagComponent"];
				if (tagComponent) entityName = tagComponent["Tag"].as<std::string>();
				PP_CORE_TRACE("Create Entity: id: {0}, name: {1}", uuid, entityName);

				Entity deserializedEntity = m_Scene->CreateEntity(entityName);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent) {
					auto& tc = deserializedEntity.AddComponent<TransformComponent>();

					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixAspectRatio = cameraComponent["FixAspectRatio"].as<bool>();
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraComponent["ProjectionType"].as<int>());
					cc.Camera.SetPerspectiveFOV(cameraComponent["PerspectiveFov"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraComponent["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraComponent["PerspectiveFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraComponent["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraComponent["OrthographicFar"].as<float>());
				}

				auto colorComponent = entity["ColorComponent"];
				if (colorComponent) {
					auto& cc = deserializedEntity.AddComponent<ColorComponent>();

					cc.Color = colorComponent["Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}

}