#pragma once

#include "lrpch.h"

namespace Laura
{

	class Profiler
	{
	public:
		class ScopeTimer
		{
		public:
			ScopeTimer(Profiler* profiler, const char* label);
			~ScopeTimer();
		private:
			Profiler* m_Profiler;
			const char* m_Label;
			std::chrono::high_resolution_clock::time_point m_Start;
		};

		Profiler();
		~Profiler() = default;

		void beginFrame();
		void endFrame();

		const std::shared_ptr<ScopeTimer> getTimer(const char* label);
		void add(const char* label, double elapsed_ms);

		inline const char* const* getLabels() const { return m_LabelsLastFrame.data(); }
		inline const double* getDurations() const { return m_DurationsLastFrame.data(); }
		inline const uint16_t getCount() const { return m_DurationsLastFrame.size(); }

	private:
		std::vector<const char*> m_Labels;
		std::vector<double> m_Durations;

		std::vector<const char*> m_LabelsLastFrame;
		std::vector<double> m_DurationsLastFrame;
	};

}