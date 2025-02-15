// inspired by https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics
#pragma once

#include "lrpch.h"

namespace Laura 
{
	struct Tri{
		glm::vec3 v0, v1, v2, centroid;
	};

	class BVHAccel
	{
	public:
		struct Node {
			glm::vec3 min, max;
			/*	if primCount == 0: leftChild_Or_FirstTri == leftChild
				else leftChild_Or_FirstTri == firstTri */
			int leftChild_Or_FirstTri, triCount;
		};

		BVHAccel(const std::vector<Tri>& mesh);
		~BVHAccel() = default;

		std::span<const Node> hierarchy() const { return m_Nodes; }
		std::span<const uint32_t> indices() const { return m_Indices; } // size the same as mesh

	private:
		// Builds the Bounding Volume Hierarchy for a given Mesh using the UpdateAABB() & SubDivide() helper methods
		void Build();
		// Computes the Axis Aligned Bounding Box for a Node passed in using its triangles
		void UpdateAABB(Node& node);
		// Recursively splits the node using a split method, and sorts the triangle index array
		void SubDivide(Node& node);

		inline void Swap(int idx1, int idx2) {
			uint32_t tmpIdx1 = m_Indices[idx1];
			m_Indices[idx1] = m_Indices[idx2];
			m_Indices[idx2] = tmpIdx1;
		}
		
		const std::vector<Tri> m_Mesh;
		std::vector<Node> m_Nodes;
		std::vector<uint32_t> m_Indices;
		uint32_t m_NodesUsed = 0;
	};
}