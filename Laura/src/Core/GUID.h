#pragma once
#include "lrpch.h"

namespace Laura
{
	class GUID
	{
	public:
		GUID();
		GUID(uint64_t guid);
		GUID(const GUID& other);
		operator uint64_t() const { return m_GUID; }
		~GUID() = default;
	private:
		uint64_t m_GUID;
	};
}

namespace std
{
	template<>
	struct hash<Laura::GUID>
	{
		std::size_t operator()(const Laura::GUID& guid) const
		{
			return std::hash<uint64_t>()((uint64_t)guid);
		}
	};
}