#pragma once

#include"Core.h"
#include"Layer.h"

namespace Pupil {

	class PP_API LayerStack {

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlayer(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlayer(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return  m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return  m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;

	};
}