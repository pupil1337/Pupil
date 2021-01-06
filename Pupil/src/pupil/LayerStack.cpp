#include "pppch.h"
#include "LayerStack.h"

namespace Pupil {

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayerInsert, layer);
		++m_LayerInsert;
	}

	void LayerStack::PushOverlayer(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsert, layer);
		if (it != m_Layers.begin() + m_LayerInsert) {
			layer->OnDetach();
			m_Layers.erase(it);
			--m_LayerInsert;
		}
	}

	void LayerStack::PopOverlayer(Layer* overlay) {
		auto it = std::find(m_Layers.begin() + m_LayerInsert, m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}