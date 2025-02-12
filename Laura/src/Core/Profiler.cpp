#include "Profiler.h"

namespace Laura 
{	
	Profiler::ScopeTimer::ScopeTimer(Profiler* profiler, const char* label)
		: m_Profiler(profiler), m_Label(label)
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	Profiler::ScopeTimer::~ScopeTimer()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		double elapsed_ms = std::chrono::duration<double, std::milli>(end - m_Start).count();
		m_Profiler->add(m_Label, elapsed_ms);
	}

	Profiler::Profiler()
		: m_Labels(), m_Durations(), m_LabelsLastFrame(), m_DurationsLastFrame() {}

	void Profiler::beginFrame()
	{
		m_Labels.clear();
		m_Durations.clear();
	}

	void Profiler::endFrame()
	{
		m_LabelsLastFrame = m_Labels;
		m_DurationsLastFrame = m_Durations;
	}

	const std::shared_ptr<Profiler::ScopeTimer> Profiler::getTimer(const char* label) {
		return std::make_shared<Profiler::ScopeTimer>(this, label);
	}

	void Profiler::add(const char* label, double elapsed_ms) {
		m_Labels.push_back(label);
		m_Durations.push_back(elapsed_ms);
	};
}