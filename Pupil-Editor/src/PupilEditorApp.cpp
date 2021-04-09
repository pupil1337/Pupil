#include "Pupil.h"
#include "pupil/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Pupil {

	class PupilEditor : public Application {

	public:
		PupilEditor(): Application("Pupil Editor") {
			PushLayer(new EditorLayer());
		}
		
		~PupilEditor() {

		}

	};

	Application* CreateApplication() {
		return new PupilEditor();
	}
}