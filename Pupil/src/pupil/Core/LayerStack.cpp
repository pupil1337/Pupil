#include "pppch.h"
#include "LayerStack.h"

namespace Pupil {

	LayerStack::~LayerStack() {
		PP_PROFILE_FUNCTION();

		for (Layer* layer : m_Layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		PP_PROFILE_FUNCTION();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsert, layer);
		++m_LayerInsert;
		layer->OnAttach();
	}

	void LayerStack::PushOverlayer(Layer* overlay) {
		PP_PROFILE_FUNCTION();

		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer) {
		PP_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsert, layer);
		if (it != m_Layers.begin() + m_LayerInsert) {
			layer->OnDetach();
			m_Layers.erase(it);
			--m_LayerInsert;
		}
		layer->OnAttach();
	}

	void LayerStack::PopOverlayer(Layer* overlay) {
		PP_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin() + m_LayerInsert, m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			overlay->OnDetach();
			m_Layers.erase(it);
		}
		overlay->OnAttach();
	}

}