#pragma once

#include "Laura.h"

namespace Laura
{

	class ProfilerPanel
	{
	public:
		ProfilerPanel() = default;
		~ProfilerPanel() = default;

		void OnImGuiRender(std::shared_ptr<const Profiler> profiler);
	};

	class RollingBuffer
	{
	public:
		RollingBuffer(size_t capacity)
			: m_Capacity(capacity), m_Data(){
			m_Data.reserve(m_Capacity);
		}

		~RollingBuffer() = default;

		inline void push_back(double item) {
			if (m_Data.size() == m_Capacity) m_Data.clear();
			m_Data.push_back(item);
		}

		inline const double* data() const {
			return m_Data.data();
		}

		inline const size_t size() const {
			return m_Data.size();
		}

	private:
		size_t m_Capacity = 0;
		std::vector<double> m_Data;
	};


	// Utility class for realtime plotting
	class CircularBuffer
	{
	public:
		CircularBuffer()
			: m_Data(), m_Capacity(0), m_Size(0), m_WriteIdx(0)
		{}

		~CircularBuffer() = default;

		inline void push_back(const double item){
			if (m_Capacity == 0) return; // Need to set the capacity first using resize()

			m_Data[m_WriteIdx] = item;
			m_Data[m_WriteIdx + m_Capacity] = item; // double buffering
			m_WriteIdx = (m_WriteIdx + 1) % m_Capacity; // increment
			if (m_Size < m_Capacity) m_Size++;
		}

		inline void setCapacity(const int cap){
			if (m_Capacity == cap) return; // no need to change anything if the capacity stays the same
			m_Data.clear();
			m_Data.resize(2 * cap);
			m_Capacity = cap;
			m_Size = 0;
			m_WriteIdx = 0;
		}

		inline const double* data() const {
			if (m_Capacity == 0) return nullptr;
			if (m_Size < m_Capacity) return &m_Data[0];
			return &m_Data[m_WriteIdx];
		}

		inline const int size() const { 
			return m_Size; 
		}

	private:
		std::vector<double> m_Data;
		int m_Capacity;
		int m_Size;
		int m_WriteIdx;
	};
}