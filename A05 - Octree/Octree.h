#pragma once

#include <vector>
#include "Definitions.h"
#include "MyEntityManager.h"
#include "MyRigidBody.h"

namespace Simplex
{
	class Octree
	{
		static uint m_uOctantCount; //will store the number of octants instantiated
		//static uint m_uMaxLevel;//will store the maximum level an octree can go to

		uint m_uID = 0; //Will store the current ID for this octree
		uint m_uLevel = 0; //Will store the current level of the octree
		uint m_uChildren = 0;// Number of children on the octree (either 0 or 8)

		float m_fSize = 0.0f; //half the length of one side (initialy will be the radius of the entity sphere)

		MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
		MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

		vector3 m_v3Center; //Will store the center point of the octant
		vector3 m_v3Min; //Will store the minimum vector of the octant
		vector3 m_v3Max; //Will store the maximum vector of the octant

		Octree* m_pParent = nullptr;// Will store the parent of current octant
		Octree* m_pChild[8];//Will store the children of the current octant

		std::vector<uint> m_EntityList; //List of Entities under this octant (Index in Entity Manager)

		Octree* m_pRoot = nullptr;//Root octant
		std::vector<Octree*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

	private:
		void Init();
		void Release();

	public:
		Octree(uint octantLevels, uint maxSub); //construct octree
		Octree(vector3 center, float size);

		Octree(Octree const& other);
		Octree& operator=(Octree const& other);
		~Octree();
		void Swap(Octree& other);
		
		float GetSize();

		vector3 GetCenterGlobal();
		vector3 GetMinGlobal();
		vector3 GetMaxGlobal();
		
		bool IsColliding(uint a_uRBIndex);
		
		void Display(uint octreeID);
		
		void ClearEntityList();
		
		void Subdivide();
		
		Octree* GetChild(uint a_nChild);
		
		Octree* GetParent();
		
		bool IsLeaf();
		
		void AssignIDtoEntity();

		uint GetOctantCount(void);
	};
}
