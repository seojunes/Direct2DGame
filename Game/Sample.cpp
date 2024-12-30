#include "Sample.h"
#include <cmath>

bool Sample::CreateSound()
{
	TSoundManager& mgr = TSoundManager::GetInstance();
	m_pSound = mgr.Load(L"../../data/sound/Festival Theme.ogg");
	m_pJumpSound = mgr.Load(L"../../data/sound/Jump.ogg");
	m_pShotSound = mgr.Load(L"../../data/sound/Laser Fire.wav");
	m_pCrashSound = mgr.Load(L"../../data/sound/Crash.wav");
	m_pSound->Play();
	return true;
}

TVector2 Sample::GetWorldMousePos()
{
	TVector2 vPos = { (float)m_Input.m_ptMouse.x,
						  (float)m_Input.m_ptMouse.y };
	vPos.x += m_vCamera.x - g_ptClientSize.x * 0.5f;
	vPos.y += m_vCamera.y - g_ptClientSize.y * 0.5f;
	return vPos;
}

bool Sample::GameDataLoad(W_STR filename)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, filename.c_str(), _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, (unsigned int)_countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		int iType = 0;
		float fLife = 0.0f;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d %d %f"), pTemp,
			(unsigned int)_countof(pTemp), &iNumFrame, &iType, &fLife);
		//m_rtSpriteList[iCnt].resize(iNumFrame);

		if (iType == 0)
		{
			RECT rt;
			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(pBuffer, _countof(pBuffer), fp_src);
				_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
					&rt.left, &rt.top, &rt.right, &rt.bottom);
				m_rtSpriteList[iCnt].push_back(rt);
			}
		}
		if (iType == 1)
		{
			TCHAR pTexFileName[256] = { 0 };
			T_STR_VECTOR strVector;
			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(pBuffer, _countof(pBuffer), fp_src);
				_stscanf_s(pBuffer, _T("%s %s"),
					pTemp, (unsigned int)_countof(pTemp),
					pTexFileName, (unsigned int)_countof(pTexFileName));

				I_Tex.Load(pTexFileName);
				strVector.push_back(I_Tex.SplitPath(pTexFileName));
			}
			m_szSpriteList.push_back(strVector);
		}
	}
	fclose(fp_src);
	return true;
}
bool Sample::CreateMap()
{
	TRect rt;
	rt.SetP(0.0f, 0.0f, 5120.0f, 800.f);
	m_pMap = std::make_shared<TMapObj>(rt, 4, 1);
	if (m_pMap->Create(m_pWorld.get()))
	{
		TTexture* pTex = I_Tex.Load(L"../../data/texture/Map.png");
		m_pMap->SetTexture(pTex).
			SetShader().
			SetLayout();
	}
	return true;
}
bool Sample::CreateHero()
{
	m_pHero = std::make_shared<THeroObj>();
	m_pHero->SetData(m_rtSpriteList);
	//m_pHero->InitHero(100, 50.0f, 20.0f);
	m_pHero->SetMap(m_pMap.get());
	TVector2 tStart = { 640.0f,m_pHero->m_fGroundY };
	TVector2 tEnd = { tStart.x + 50.0f, tStart.y + 75.0f };
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/newMega.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pHero->m_pMeshRender = &TGameCore::m_MeshRender;
	m_pHero->m_pWorld = m_pWorld.get();
	if (m_pHero->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pHero->m_iCollisionType = TCollisionType::T_Overlap;
	}
	return true;

}
bool Sample::CreateNPC()
{
	// npc
	TRect rtWorldMap = m_pMap->m_rtScreen;
	for (int iNpc = 0; iNpc < m_Npccount; iNpc++)
	{
		auto npcobj = std::make_shared<TNpcObj>();
		npcobj->m_pMeshRender = &TGameCore::m_MeshRender;
		npcobj->SetMap(m_pMap.get());
		npcobj->StartFSM(&m_fsm);
		TVector2 vStart(
			rtWorldMap.v1.x + (rand() % (UINT)(rtWorldMap.vs.x - 100.0f)),
			rtWorldMap.v1.y + (rand() % (UINT)(rtWorldMap.vs.y - 100.0f)));
		TVector2 tEnd(vStart.x + 42.0f, vStart.y + 60.0f);
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/bitmap1.bmp";
		resData.texShaderName = L"../../data/shader/BlendMask.txt";
		if (npcobj->Create(m_pWorld.get(), resData, vStart, tEnd))
		{
			npcobj->m_fSpeed = 50.0f + (rand() % 200);
			npcobj->m_iCollisionType = TCollisionType::T_Overlap;
			//npcobj->SetScale(30.0f, 30.0f );
			//npcobj->SetRotation(T_Pi);
			m_NpcList.emplace_back(npcobj);
		}
	}
	return true;
}
bool Sample::CreateEffect()
{
	auto pObject3 = std::make_shared<TEffectObj>();
	TVector2 tStart = { 640.0f,0.0f };
	TVector2 tEnd2 = { tStart.x + 100.0f, tStart.y + 100.0f };
	AddEffect(tStart, tEnd2);
	return true;
}

void Sample::TransPivot()
{
}
void   Sample::Init()
{
	// STATE_STAND -> EVENT_PATROL (시간경과)    -> STATE_MOVE
// STATE_STAND -> EVENT_FINDTARGET(검색범위) -> STATE_ATTACK
// STATE_MOVE  -> EVENT_STOP		->STATE_STAND
// STATE_MOVE  -> EVENT_LOSTTARGET  ->STATE_STAND
// STATE_MOVE  -> EVENT_FINDTARGET  ->STATE_ATTACK
// STATE_ATTACK -> EVENT_LOSTTARGET ->STATE_STAND 
// STATE_ATTACK -> EVENT_STOP       ->STATE_STAND 
	m_fsm.AddStateTransition(STATE_STAND, EVENT_PATROL, STATE_MOVE);
	m_fsm.AddStateTransition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	m_fsm.AddStateTransition(STATE_MOVE, EVENT_STOP, STATE_STAND);
	m_fsm.AddStateTransition(STATE_MOVE, EVENT_LOSTTARGET, STATE_STAND);
	m_fsm.AddStateTransition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	m_fsm.AddStateTransition(STATE_ATTACK, EVENT_STOP, STATE_STAND);
	m_fsm.AddStateTransition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	//UINT iCurrent = T_ActionState::STATE_STAND;
	//UINT iEvent = T_ActionEvent::EVENT_PATROL;
	//// STATE_MOVE
	//UINT iActionState = m_fsm.GetOutputState(iCurrent, iEvent);
	//// STATE_ATTACK
	//iActionState = m_fsm.GetOutputState(iActionState,
	//    T_ActionEvent::EVENT_FINDTARGET);
	//iActionState = m_fsm.GetOutputState(iActionState,
	//    T_ActionEvent::EVENT_LOSTTARGET);

	CreateSound();
	m_pBitmap1Mask = I_Tex.Load(L"../../data/texture/bitmap2.bmp");
	GameDataLoad(L"SpriteData.txt");

	m_pWorld = std::make_shared<TWorld>();
	CreateMap();
	CreateHero();
	CreateNPC();
	//CreateEffect();
}
void  Sample::AddMissile(TVector2 tStart, TVector2 tEnd)
{
	auto mObject = std::make_shared<TMissileObj>();
	mObject->m_pMeshRender = &TGameCore::m_MeshRender;
	mObject->m_vVertexList = mObject->m_pMeshRender->m_vVertexList;
	mObject->SetHero(m_pHero.get());
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/Bullet.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	mObject->SetDirrection(m_pHero->m_CurrentView);
	if (mObject->Create(m_pWorld.get(),resData, tStart, tEnd))
	{
		m_MissileList.emplace_back(mObject);
	}
	
}
void   Sample::AddEffect(TVector2 tStart, TVector2 tEnd)
{
	auto pObject3 = std::make_shared<TEffectObj>();
	pObject3->m_pMeshRender = &TGameCore::m_MeshRender;
	pObject3->m_vVertexList = pObject3->m_pMeshRender->m_vVertexList;
	TLoadResData resData;
	/*resData.texPathName = L"../../data/texture/newMega.png";
	resData.texShaderName = L"../../data/shader/DefaultMask.txt";*/
	resData.texPathName = L"../../data/texture/newMega.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	TEffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 2.0f;
	//data.m_fOffsetTime = 0.5f;
	UINT iSprite = 0;
	data.m_iType = 0;// rand() % m_szSpriteList[0].size();
	if (data.m_iType == 0)
	{
		data.m_iNumAnimFrame = m_rtSpriteList[iSprite].size();
		data.m_rtList = m_rtSpriteList[iSprite];
	}
	if (data.m_iType == 1)
	{
		data.m_iNumAnimFrame = m_szSpriteList[0].size();
		data.m_szList = m_szSpriteList[0];
	}
	pObject3->SetData(data);
	if (pObject3->Create(m_pWorld.get(),resData, tStart, tEnd))
	{
		m_EffectList.emplace_back(pObject3);
	}
}

void   Sample::Frame()
{
	// 점프 사운드
	TSoundManager::GetInstance().Frame();
	if (m_pHero->m_iJumpingCount < 3 && m_pHero->m_iJumpingCount >= 0 && g_GameKey.dwWkey == KEY_PUSH)
	{
		m_pJumpSound->PlayEffect();
	}

	m_pMap->Frame();
	m_pHero->Frame();

	if (m_pHero->m_rtScreen.v1.x >= 640.0f && m_pHero->m_rtScreen.v1.x <= 4480.0f)
	{
		m_vCamera.x = m_pHero->m_vPos.x;
	}
	else if (m_pHero->m_rtScreen.v1.x < 640.0f)
	{
		m_vCamera.x = 640.0f;
	}
	else if (m_pHero->m_rtScreen.v1.x > 4480.0f)
	{
		m_vCamera.x = 4480.0f;
	}
	//m_vCamera.y = m_pHero->m_vPos.y;


	// m_vCamera.y = m_pHero->m_rtScreen.y - 225;// 초기 카메라 세팅
	  // 메가맨 위치와 카메라 위치 보정

	
	TVector2 vMouse = GetWorldMousePos();
	for (auto data : m_NpcList)
	{
		if (!data->m_bDead)
		{
			data->FrameState(m_pHero.get());
			data->Frame();
		}
	}

	
	TSphere s;
	s.vCenter = vMouse;
	s.fRadius = 100.0f;

	for (UINT iNpc1 = 0; iNpc1 < m_NpcList.size(); iNpc1++)
	{
		for (UINT iMissile = 0; iMissile < m_MissileList.size(); iMissile++)
		{
			if (m_MissileList[iMissile]->m_bDead || m_NpcList[iNpc1]->m_bDead) continue;
			if (TCollision::CheckRectToRect(
				m_NpcList[iNpc1]->m_rtScreen,
				m_MissileList[iMissile]->m_rtScreen))
			{
				m_NpcList[iNpc1]->m_bDead = true;
				m_MissileList[iMissile]->m_bDead = true;
				m_pCrashSound->PlayEffect();
				m_Npccount--;
			}
		}
	}


	if (g_GameKey.dwMiddleClick == KEY_HOLD)
	{
		TVector2 v1 = vMouse;
		v1.x = vMouse.x - 50.0f;
		v1.y = vMouse.y - 50.0f;
		TVector2 tEnd = { v1.x + 100.0f, v1.y + 100.0f };
		AddEffect(v1, tEnd);

		/*for (int iCell = 0; iCell < m_pMap->m_Cells.size(); iCell++)
		{
			if (TCollision::CheckRectToPoint(
				m_pMap->m_Cells[iCell].rt, m_Input.m_ptMouse))
			{
				m_pMap->m_Cells[iCell].iTexID = 2;
			}
		} 충돌부분*/

		/*UINT iType = rand() / 3;
		if(iType ==0 )
			AddEffectSingle(tStart, tEnd, std::shared_ptr<TE1>());
		if (iType == 1)
			AddEffectSingle(tStart, tEnd, std::shared_ptr<TE2>());
		if (iType == 2)
			AddEffectSingle(tStart, tEnd, std::shared_ptr<TE3>());*/
	}
	for (UINT iNpc1 = 0; iNpc1 < m_NpcList.size(); iNpc1++)
	{
		if (m_NpcList[iNpc1]->m_bDead)
		{
			m_pWorld->DeleteCollisionExecute(m_NpcList[iNpc1].get());
		};
	}
	for (auto iter = std::begin(m_EffectList);
		iter != m_EffectList.end();)
	{
		TEffectObj* pObj = (TEffectObj*)iter->get();
		if (pObj->m_bDead == false)
		{
			pObj->Frame();
			iter++;
		}
		else
		{
			pObj->Release();
			iter = m_EffectList.erase(iter);
		}
	}
	if (g_GameKey.dwSpace == KEY_PUSH)
	{
		TVector2 tStart = { m_pHero->m_rtScreen.vc.x, m_pHero->m_rtScreen.vc.y };
		TVector2 tEnd = { tStart.x + 40.0f, tStart.y + 30.0f };
		AddMissile(tStart, tEnd);
		m_pShotSound->PlayEffect();
	}
	for (auto iter = std::begin(m_MissileList);
		iter != m_MissileList.end();)
	{
		TMissileObj* pObj = (TMissileObj*)iter->get();
		if (pObj->m_bDead == false)
		{
			pObj->Frame();
			iter++;
		}
		else
		{
			pObj->Release();
			iter = m_MissileList.erase(iter);
		}
	}

	m_pWorld->Frame();

}
void   Sample::Render()
{
	TSoundManager::GetInstance().Render();
	m_pMap->Transform(m_vCamera);
	m_pMap->Render();

	TDevice::m_pd3dContext->PSSetSamplers(0, 1, TDxState::m_pPointSS.GetAddressOf());
	TDevice::m_pd3dContext->PSSetShaderResources(1, 1, &m_pBitmap1Mask->m_pTexSRV);
	m_pHero->Transform(m_vCamera);
	m_pHero->Render();

	for (auto data : m_NpcList)
	{
		if (data->m_bDead) continue;
		data->Transform(m_vCamera);
		data->Render();
	}
	for (auto data : m_EffectList)
	{
		data->Transform(m_vCamera);
		data->Render();
	}
	for (auto data : m_MissileList)
	{
		if (data->m_bDead) continue;
		data->Transform(m_vCamera);
		data->Render();
	}
	// 남은 NPC 출력
	// 문자열로 변환
	std::wstring npcCountText = std::to_wstring(m_Npccount);
	// 텍스트 출력 영역 정의
	D2D1_RECT_F rt = { 640.0f, 100.0f, 1280.0f, 800.0f };
	// 화면에 텍스트 그리기
	m_DxWrite.DirectDraw(rt, L"남은 NPC", D2D1::ColorF(D2D1::ColorF::Yellow));
	rt.top += 25, rt.left += 25;
	m_DxWrite.DirectDraw(rt, npcCountText, D2D1::ColorF(D2D1::ColorF::Yellow));
}
void   Sample::Release()
{
	// 현재는 Object의 Release에서 아무런 작업을 하고있지 않지만, 추후 관리를 위해서 생성.
	for (auto data : m_NpcList)
	{
		data->Release();
	}
	for (auto data : m_EffectList)
	{
		data->Release();
	}
	for (auto data : m_MissileList)
	{
		data->Release();
	}

	m_pHero->Release();
	m_pMap->Release();
}

//GameStart(800, 600);
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	Sample sample;
	sample.SetWindowClass(hInstance);
	sample.SetWindow();
	sample.GameRun();
}