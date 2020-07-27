#include "Octree.h"
using namespace Simplex;

uint Octree::m_uOctantCount = 0;
void Simplex::Octree::Init()
{
	m_uID = 0;
	m_uLevel = 0;
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pParent = nullptr;
}

void Simplex::Octree::Release()
{
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
}

Simplex::Octree::Octree(uint octantLevels, uint maxSub)
{
	Init();
	m_uLevel = octantLevels;
	if (octantLevels > maxSub) octantLevels = maxSub;

	//create root
	m_uOctantCount++;
	m_pRoot = this;
	m_fSize = 34.0f;

	m_v3Min = vector3(-m_fSize,
					  -m_fSize,
					  -m_fSize);

	m_v3Max = vector3(m_fSize,
					  m_fSize,
					  m_fSize);

	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		m_EntityList.push_back(m_pEntityMngr->GetEntityIndex(m_pEntityMngr->GetUniqueID(i)));
	}

	for (int i = 0; i < octantLevels; i++)
	{
		Subdivide();
	}
	
	m_pEntityMngr->ClearDimensionSetAll();
	AssignIDtoEntity();
}

Simplex::Octree::Octree(vector3 center, float size)
{
	m_uOctantCount++;
	m_uID = 0;
	m_uLevel = 0;
	m_uChildren = 0;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pParent = nullptr;

	m_v3Center = center;
	m_fSize = size;

	m_v3Min = vector3(m_v3Center.x - m_fSize, m_v3Center.y - m_fSize, m_v3Center.z - m_fSize);

	m_v3Max = vector3(m_v3Center.x + m_fSize, m_v3Center.y + m_fSize, m_v3Center.z + m_fSize);
}

Simplex::Octree::Octree(Octree const& other)
{
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;

	m_fSize = other.m_fSize;

	m_pMeshMngr = other.m_pMeshMngr;
	m_pEntityMngr = other.m_pEntityMngr;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_pParent = other.m_pParent;
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}

	m_EntityList = other.m_EntityList;

	m_pRoot = other.m_pRoot;
}

Octree& Simplex::Octree::operator=(Octree const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octree temp(other);
		Swap(temp);
	}
	return *this;
}

Simplex::Octree::~Octree(){	Release();}

void Simplex::Octree::Swap(Octree& other) 
{
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pChild, other.m_pChild);

	std::swap(m_EntityList, other.m_EntityList);

	std::swap(m_pRoot, other.m_pRoot);
}

float Octree::GetSize(){ return m_fSize;}
vector3 Octree::GetCenterGlobal(){ return m_v3Center;}
vector3 Octree::GetMinGlobal(){ return m_v3Min;}
vector3 Octree::GetMaxGlobal(){ return m_v3Max;}

bool Simplex::Octree::IsColliding(uint a_uRBIndex)
{
	MyRigidBody* temp = m_pEntityMngr->GetRigidBody(a_uRBIndex);
	vector3 tempMax = temp->GetMaxGlobal();
	vector3 tempMin = temp->GetMinGlobal();

	//set collision to true
	bool bColliding = true;
	//check ARBB collision
	if (m_v3Max.x < tempMin.x) //this to the right of other
		bColliding = false;
	if (m_v3Min.x > tempMax.x) //this to the left of other
		bColliding = false;

	if (m_v3Max.y < tempMin.y) //this below of other
		bColliding = false;
	if (m_v3Min.y > tempMax.y) //this above of other
		bColliding = false;

	if (m_v3Max.z < tempMin.z) //this behind of other
		bColliding = false;
	if (m_v3Min.z > tempMax.z) //this in front of other
		bColliding = false;

	return bColliding;
}

void Simplex::Octree::Display(uint octreeID)
{
	if (octreeID == -1)
	{
		matrix4 temp = glm::translate(m_v3Center) * glm::scale(m_v3Max - m_v3Min);
		m_pMeshMngr->AddWireCubeToRenderList(temp, C_YELLOW);

		for (int i = 0; i < m_lChild.size(); i++)
		{
			if (m_lChild[i]->IsLeaf())
			{
				matrix4 temp = glm::translate(m_lChild[i]->m_v3Center) * glm::scale(m_lChild[i]->m_v3Max - m_lChild[i]->m_v3Min);
				m_pMeshMngr->AddWireCubeToRenderList(temp, C_YELLOW);
			}
		}
	}
	else if (octreeID == 0)
	{
		matrix4 temp = glm::translate(m_v3Center) * glm::scale(m_v3Max - m_v3Min);
		m_pMeshMngr->AddWireCubeToRenderList(temp, C_YELLOW);
	}
	else
	{
		matrix4 temp = glm::translate(m_lChild[octreeID - 1]->m_v3Center) * glm::scale(m_lChild[octreeID - 1]->m_v3Max - m_lChild[octreeID - 1]->m_v3Min);
		m_pMeshMngr->AddWireCubeToRenderList(temp, C_YELLOW);
	}
}

void Simplex::Octree::ClearEntityList()
{
	m_EntityList.clear();
}

void Simplex::Octree::Subdivide()
{
	if (IsLeaf())
	{
		vector3 center = vector3(0.0f);
		float size = m_fSize / 2.0f;

		//Set each child
		//change center
		center = vector3(m_v3Center.x + size,
						 m_v3Center.y + size,
						 m_v3Center.z + size);

		m_pChild[0] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x + size,
						 m_v3Center.y + size,
						 m_v3Center.z - size);

		m_pChild[1] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x + size,
						 m_v3Center.y - size,
						 m_v3Center.z + size);

		m_pChild[2] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x + size,
						 m_v3Center.y - size,
						 m_v3Center.z - size);

		m_pChild[3] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x - size,
						 m_v3Center.y + size,
						 m_v3Center.z + size);

		m_pChild[4] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x - size,
						 m_v3Center.y + size,
						 m_v3Center.z - size);

		m_pChild[5] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x - size,
						 m_v3Center.y - size,
						 m_v3Center.z + size);

		m_pChild[6] = new Octree(center, size);

		//change center
		center = vector3(m_v3Center.x - size,
						 m_v3Center.y - size,
						 m_v3Center.z - size);

		m_pChild[7] = new Octree(center, size);

		//set parent
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i]->m_pParent = this;
			m_pChild[i]->m_pRoot = this->m_pRoot;
			m_pChild[i]->m_uLevel = this->m_uLevel + 1;

			for (int j = 0; j < m_EntityList.size(); j++)
			{
				if (m_pChild[i]->IsColliding(m_EntityList[j]))
				{
					m_pChild[i]->m_EntityList.push_back(m_EntityList[j]);
				}
			}

			m_pChild[i]->m_uID = pow(8, m_pChild[i]->m_uLevel - 1) * this->m_uID + i + 1;

			m_uChildren++;
			m_pRoot->m_lChild.push_back(m_pChild[i]);
		}
	}
	else
	{
		for (int i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->Subdivide();
		}
	}
}

Octree* Simplex::Octree::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

Octree* Simplex::Octree::GetParent(){ return m_pParent;}

bool Simplex::Octree::IsLeaf()
{
	return m_uChildren == 0;
}

void Simplex::Octree::AssignIDtoEntity()
{
	if (IsLeaf())
	{
		for (int i = 0; i < m_EntityList.size(); i++)
		{
			m_pEntityMngr->AddDimension(m_EntityList[i], m_uID);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
}

uint Simplex::Octree::GetOctantCount()
{
	return pow(8, m_uLevel);
}
