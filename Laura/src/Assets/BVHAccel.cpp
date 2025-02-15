#include "BVHAccel.h"

namespace Laura
{
	BVHAccel::BVHAccel(const std::vector<Tri>& mesh)
	: m_Mesh(mesh){
		Build();
	}

	void BVHAccel::Build()
	{
		m_NodesUsed = 0;
		const size_t N = m_Mesh.size(); // for convenience

		// preallocate the maximum number of nodes
		m_Nodes.resize(2 * N - 1);
		
		// generate index array - [0, 1, 2, ..., n] - identic mapping
		m_Indices.resize(N); 
		for (int i = 0; i < N; i++) 
			m_Indices[i] = i;

		// root as node at index 0
		Node& root = m_Nodes[m_NodesUsed++];
		// root contains full mesh
		root.leftChild_Or_FirstTri = 0;
		root.triCount = N;

		UpdateAABB(root);
		SubDivide(root);
	}

	void BVHAccel::UpdateAABB(Node& node)
	{
		node.min = glm::vec3(FLT_MAX);
		node.max = glm::vec3(-FLT_MAX);
		// iterate over primitives contained by the Node
		for (int i = 0; i < node.triCount; i++)
		{
			// get the real triangle index in the m_Mesh
			const int realTriIdx = m_Indices[node.leftChild_Or_FirstTri + i];
			const Tri& tri = m_Mesh[realTriIdx];
			// find minimum & maximum coordinates
			node.min = glm::min(node.min, tri.v0);
			node.min = glm::min(node.min, tri.v1);
			node.min = glm::min(node.min, tri.v2);
			node.max = glm::max(node.max, tri.v0);
			node.max = glm::max(node.max, tri.v1);
			node.max = glm::max(node.max, tri.v2);
		}
	}

	void BVHAccel::SubDivide(Node& node)
	{
		// Found Leaf Node
		if (node.triCount <= 2) 
			return;
		
		// SPLIT METHOD
		glm::vec3 AABB = node.max - node.min;
		int splitAxis = 0;
		if (AABB.y > AABB.x) splitAxis = 1;
		if (AABB.z > AABB.y) splitAxis = 2;
		int splitPoint = node.min[splitAxis] + AABB[splitAxis] * 0.5;
		// SPLIT METHOD

		uint32_t leftPtr = node.leftChild_Or_FirstTri; // points to the firstTri in node's triangles
		uint32_t rightPtr = node.leftChild_Or_FirstTri + node.triCount - 1; // points to the lastTri

		// partition/sort the triangles (quicksort partition)
		while (leftPtr <= rightPtr)
		{
			if (m_Mesh[m_Indices[leftPtr]].centroid[splitAxis] < splitPoint) 
				leftPtr++;
			else 
				Swap(leftPtr, rightPtr--); // swap and decrement right
		}

		int leftTriCount = leftPtr - node.leftChild_Or_FirstTri; // distance between firstTri and partition point
		
		// couldn't partition
		if (leftTriCount == 0 || leftTriCount == node.triCount) 
			return;

		// find indices for the new child nodes
		int leftChildIdx = m_NodesUsed++;
		int rightChildIdx = m_NodesUsed++;

		// populate children
		m_Nodes[leftChildIdx].leftChild_Or_FirstTri = node.leftChild_Or_FirstTri;
		m_Nodes[leftChildIdx].triCount = leftTriCount;
		m_Nodes[rightChildIdx].leftChild_Or_FirstTri = node.leftChild_Or_FirstTri + leftTriCount;
		m_Nodes[rightChildIdx].triCount = node.triCount - leftTriCount;
		
		// update current node
		node.triCount = 0; // Important! mark the node as non-leaf node
		node.leftChild_Or_FirstTri = leftChildIdx; // now points to the leftChild node

		UpdateAABB(m_Nodes[leftChildIdx]);
		UpdateAABB(m_Nodes[rightChildIdx]);
		
		SubDivide(m_Nodes[leftChildIdx]);
		SubDivide(m_Nodes[rightChildIdx]);
	}
}