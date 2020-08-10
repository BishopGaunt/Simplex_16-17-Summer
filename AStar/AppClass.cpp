#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Anthony LaRosa - al8249@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(5.0f, 4.0f, 17.0f), //Position
		vector3(5.0f, 3.5f, 16.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light(0 is reserved for global light)

	srand(time(NULL));

	//creeper
	m_pCreeper = new Model();
	m_pCreeper->Load("Minecraft\\Creeper.obj");
	m_pCreeperRB = new MyRigidBody(m_pCreeper->GetVertexList());

	//steve
	m_pSteve = new Model();
	m_pSteve->Load("Minecraft\\Steve.obj");
	m_pSteveRB = new MyRigidBody(m_pSteve->GetVertexList());


	uint size = tileMngr->GetArraySize();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			blocks.push_back(new Model());
			blocks[i * size + j]->Load("Minecraft\\Cube.obj");
		}
	}

	tileMngr->AStar();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Set model matrix to the creeper
	//matrix4 mCreeper = glm::translate(tileMngr->GetStart()->GetPosition());
	//m_pCreeper->SetModelMatrix(mCreeper);
	//m_pCreeperRB->SetModelMatrix(mCreeper);
	//m_pMeshMngr->AddAxisToRenderList(mCreeper);

	//Set model matrix to Steve
	matrix4 mSteve = glm::translate(tileMngr->GetEnd()->GetPosition());
	m_pSteve->SetModelMatrix(mSteve);
	m_pSteveRB->SetModelMatrix(mSteve);
	m_pMeshMngr->AddAxisToRenderList(mSteve);

	//Set model matrix to Blocks
	matrix4 mBlock = glm::translate(vector3(0.0f));

	uint size = tileMngr->GetArraySize();
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (tileMngr->tiles[i][j] != nullptr)
			{
				mBlock = glm::translate(tileMngr->tiles[i][j]->GetPosition()) * glm::translate(vector3(-0.5f, -1.1f, -0.5f));
				blocks[i * size + j]->SetModelMatrix(mBlock);
				blocks[i * size + j]->AddToRenderList();
			}
		}
	}

	bool bColliding = m_pCreeperRB->IsColliding(m_pSteveRB);

	/*m_pCreeper->AddToRenderList();
	m_pCreeperRB->AddToRenderList();*/

	m_pSteve->AddToRenderList();
	m_pSteveRB->AddToRenderList();

	m_pMeshMngr->Print("Colliding: ");
	if (bColliding)
	{
		m_pMeshMngr->PrintLine("YES!", C_RED);

		tileMngr->SetStart(tileMngr->GetEnd());

		int ranRow = rand() % tileMngr->GetArraySize();
		int ranCol = rand() % tileMngr->GetArraySize();

		if (tileMngr->tiles[ranRow][ranCol] == nullptr)
		{
			ranRow = 1;
			ranCol = 1;
		}

		tileMngr->SetEnd(tileMngr->tiles[ranRow][ranCol]);
		
		tileMngr->AStar();
	}	
	else
	{
		m_pMeshMngr->PrintLine("no", C_YELLOW);
	}

	matrix4 mTemp = glm::translate(vector3(0.0f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, -1.5708f, AXIS_X);

	for (int i = 0; i < tileMngr->GetShortPath().size(); i++)
	{
		mTemp = glm::translate(tileMngr->GetShortPath()[i]->GetPosition()) * glm::rotate(IDENTITY_M4, glm::radians(-90.0f), AXIS_X);
		m_pMeshMngr->AddPlaneToRenderList(mTemp, C_GREEN);
	}
	
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	vector3 v3Start; //start point
	vector3 v3End; //end point
	static uint route = 0; //current route
	bool bColliding = m_pCreeperRB->IsColliding(m_pSteveRB);
	if (bColliding)
	{
		route = 0;
	}

	v3Start = tileMngr->GetShortPath()[route]->GetPosition(); //start at the current route
	if (route + 1 >= tileMngr->GetShortPath().size())
	{
		v3End = tileMngr->GetShortPath()[route]->GetPosition();
	}
	else
	{
		v3End = tileMngr->GetShortPath()[route + 1]->GetPosition(); //end at route +1 (if overboard will restart from 0)
	}
	 

	//get the percentace
	float fTimeBetweenStops = 0.5;//in seconds
	//map the value to be between 0.0 and 1.0
	float fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	//calculate the current position
	vector3 v3CurrentPos = glm::lerp(v3Start, v3End, fPercentage);
	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos);

	//if we are done with this route
	if (fPercentage >= 1.0f)
	{
		route++; //go to the next route
		fTimer = m_pSystem->GetDeltaTime(uClock);//restart the clock
		//route %= tileMngr->GetShortPath().size();//make sure we are within boundries
		if (route >= tileMngr->GetShortPath().size())
			route = tileMngr->GetShortPath().size() - 1;
	}

	m_pCreeper->SetModelMatrix(m4Model);
	m_pCreeperRB->SetModelMatrix(m4Model);

	m_pCreeper->AddToRenderList();
	m_pCreeperRB->AddToRenderList();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}

void Application::Release(void)
{

	//release the model
	SafeDelete(m_pCreeper);

	//release the rigid body for the model
	SafeDelete(m_pCreeperRB);

	//release the model
	SafeDelete(m_pSteve);

	//release the rigid body for the model
	SafeDelete(m_pSteveRB);

	//release GUI
	ShutdownGUI();
}