#include "TSceneGameIn.h"
#include "TGameCore.h"
#include "TGame.h"

TSceneGameIn::TSceneGameIn(TGame* p)
{
	m_pOwner = p;
}
TSceneGameIn::~TSceneGameIn() {}

void TSceneGameIn::ProcessAction(TObject* pObj)
{
	m_pSound->Play();
	if (m_bNextScene == true)
	{
		m_pOwner->SetTransition(TSceneEvent::EVENT_NEXT_SCENE);
		m_bNextScene = false;
		return;
	}
	if (m_bPrevScene == true)
	{
		m_pOwner->SetTransition(TSceneEvent::EVENT_PREV_SCENE);
		m_bPrevScene = false;
		return;
	}
}

bool TSceneGameIn::CreateSound()
{
	TSoundManager& mgr = TSoundManager::GetInstance();
	m_pSound = mgr.Load(L"../../data/sound/Festival Theme.ogg");
	m_pJumpSound = mgr.Load(L"../../data/sound/Jump.ogg");
	m_pShotSound = mgr.Load(L"../../data/sound/Laser Fire.wav");
	m_pCrashSound = mgr.Load(L"../../data/sound/Crash.wav");
	return true;
}

TVector2 TSceneGameIn::GetWorldMousePos()
{
	TVector2 vPos = { (float)g_ptMouse.x, (float)g_ptMouse.y };
	vPos.x += m_vCamera.x - g_ptClientSize.x * 0.5f;
	vPos.y += m_vCamera.y - g_ptClientSize.y * 0.5f;
	return vPos;
}

bool TSceneGameIn::GameDataLoad(W_STR filename)
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
bool TSceneGameIn::CreateMap()
{
	TRect rt;
	rt.SetP(0.0f, 0.0f, 1280.0f * 9.0f, 900 * 2.0f);
	m_pMap = std::make_shared<TMapObj>(rt, 1, 1);
	if (m_pMap->Create(m_pWorld.get()))
	{
		TTexture* pTex = I_Tex.Load(L"../../data/texture/Map7.png");
		m_pMap->SetTexture(pTex).SetShader().SetLayout();
	}
	return true;
}
bool TSceneGameIn::CreateHero()
{
	m_pHero = std::make_shared<THeroObj>();
	m_pHero->SetData(m_rtSpriteList);
	m_pHero->SetMap(m_pMap.get());
	TVector2 tStart = { 400.0f,300.0f };//m_pHero->m_fGroundY };
	TVector2 tEnd = { tStart.x + 80.0f, tStart.y + 100.0f };
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/walk.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pHero->m_pMeshRender = &TGameCore::m_MeshRender;
	m_pHero->m_pWorld = m_pWorld.get();
	if (m_pHero->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pHero->m_iCollisionType = TCollisionType::T_Overlap;
	}
	return true;
}
bool TSceneGameIn::CreateRect()
{
	rectArea = {

		{{ 0.0f  , 720.0f },{3730.0f,1020.0f} },
		{{ 1786.0f  , 540.0f },{2106.0f,720.0f}},
		{{ 3246.0f  , 418.5f },{3566.0f,540.0f}},
		{{ 2760.0f   , 540.0f },{3566.0f,720.0f}},
		{{ 3812.0f  , 0.0f },{3900.0f,1200.0f}},
		{{ 3406.0f  , 1200.0f },{4380.0f,1260.0f}},
		{{ 2500.0f  , 900.0f },{2756.0f,1800.0f}},
		{{3082.0f  , 1379.0f},{4868.0f,1440.0f}},
		{{2758.0f  , 1557.0f},{11520.0f,1800.0f}},
		{{ 4134.0f  , 900.0f },{4214.0f,1200.0f}},
		{{ 4868.0f  , 1380.0f },{5024.0f,1557.0f}},
		{{ 5678.0f  , 1379.0f },{5838.0f,1557.0f}},
		{{ 6004.0f  , 1438.0f },{6486.0f,1557.0f}},
		{{ 6166.0f  , 1258.0f },{6324.0f,1438.0f}},
		{{ 6814.0f  , 1377.0f },{6888.0f,1433.0f}},
		{{ 9408.0f  , 1377.0f },{9492.0f,1433.0f}},
		{{ 7062.0f  , 1318.5f },{7782.0f,1557.0f}},
		{{ 9658.0f  , 1318.5f },{10380.0f,1557.0f}},
		{{ 8274.0f  , 1379.5f },{9082.0f,1557.0f}},
		{{ 10870.0f  , 1379.5f },{11520.0f,1557.0f}},
		{{ 8600.0f  , 1200.0f },{9080.0f,1379.5f}},
		{{ 11200.0f  , 1200.0f },{11520.0f,1379.5f}},
		{{ 3886.0f  , 0.0f },{11520.0f,900.0f}},
		{{ 15200.0f  , 900.0f },{15400.0f,1800.0f}},
		{{ 0.0f  , -50.0f },{3880.0f,0.0f}},
		{{ -50.0f  , 0.0f },{0.0f,900.0f} },
		{{11520.0f,0.0f},{11720.0f,1800.0f}},
	};
	for (auto area : rectArea)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto m_pRect = std::make_shared<TCollisionManager>();
		m_pRect->SetMap(m_pMap.get());
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/collision.png";
		resData.texShaderName = L"../../data/shader/Default.txt";

		m_pRect->m_pMeshRender = &TGameCore::m_MeshRender;
		m_pRect->m_pWorld = m_pWorld.get();
		//if (m_pWorld)
		//{ // TWorld 포인터가 유효한지 확인
		//	m_pWorld->AddCollisionExecute(obj.get(), std::bind(&TProjectile::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
		//}
		if (m_pRect->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pRect->m_iCollisionType = TCollisionType::T_Block;
			m_ColList.emplace_back(m_pRect);
		}
		
	}
	return true;
}

bool TSceneGameIn::CreateNPC()
{
	Mon1Area = { {{1400.0f,600.0f},{1442.0f,660.0f}},
		{{1700.0f,600.0f},{1742.0f,660.0f}},
		{{800.0f,600.0f},{842.0f,660.0f}},
		{{1100.0f,600.0f},{1142.0f,660.0f}},
	};
	Mon2Area = { {{2000.0f,600.0f},{2042.0f,660.0f}},
		{{2700.0f,600.0f},{2742.0f,660.0f}},
	};
	Mon3Area = { {{3000.0f,600.0f},{3042.0f,660.0f}},
		{{3700.0f,600.0f},{3742.0f,660.0f}},
	};
	TNpcObj::CreateActionFSM();
	// npc
	TRect rtWorldMap = m_pMap->m_rtScreen;
	for (auto area : Mon1Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj1 = std::make_shared<TMonster1>();
		npcobj1->m_pMeshRender = &TGameCore::m_MeshRender;
		npcobj1->SetMap(m_pMap.get());
		npcobj1->SetFSM(&m_fsm);
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/monster.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj1->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj1->m_iCollisionType = TCollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj1);
		}
	}
	for (auto area : Mon2Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj2 = std::make_shared<TMonster2>();
		npcobj2->m_pMeshRender = &TGameCore::m_MeshRender;
		npcobj2->SetMap(m_pMap.get());
		npcobj2->SetFSM(&m_fsm);
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/monster2.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj2->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj2->m_iCollisionType = TCollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj2);
		}
	}
	for (auto area : Mon3Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj3 = std::make_shared<TMonster3>();
		npcobj3->m_pMeshRender = &TGameCore::m_MeshRender;
		npcobj3->SetMap(m_pMap.get());
		npcobj3->SetFSM(&m_fsm);
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/monster3.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj3->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj3->m_iCollisionType = TCollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj3);
		}
	}
	return true;
}
bool TSceneGameIn::CreateUI()
{
	TButtonGUI::CreateActionFSM();

	TLoadResData resData;
	resData.texPathName = L"../../data/ui/main_start_nor.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto ui1 = std::make_shared<TPrevBtn>();
	ui1->m_pMeshRender = &TGameCore::m_MeshRender;
	ui1->SetFSM(&m_GuiFSM);
	TVector2 vStart1 = { 0.0f, 0.0f };
	TVector2 vEnd1 = { 100.0f, 100.0f };
	if (ui1->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		ui1->m_iCollisionType = TCollisionType::T_Overlap;
		//ui1->SetScale(100.0f, 50.0f);
		//ui1->SetRotation(T_Pi * 0.25f);
		m_UiList.emplace_back(ui1);
	}

	auto ui2 = std::make_shared<TNextBtn>();
	ui2->m_pMeshRender = &TGameCore::m_MeshRender;
	ui2->SetFSM(&m_GuiFSM);
	TVector2 vStart2 = { 700.0f, 0.0f };
	TVector2 vEnd2 = { 800.0f, 100.0f };
	if (ui2->Create(m_pWorld.get(), resData, vStart2, vEnd2))
	{
		ui2->m_iCollisionType = TCollisionType::T_Overlap;
		//ui2->SetScale(50.0f, 100.0f);
		//ui2->SetRotation(T_Pi * -0.25f);
		m_UiList.emplace_back(ui2);
	}

	return true;
}
bool TSceneGameIn::CreateEffect()
{
	auto pObject3 = std::make_shared<TEffectObj>();
	TVector2 tStart = { 640.0f,0.0f };
	TVector2 tEnd2 = { tStart.x + 100.0f, tStart.y + 100.0f };
	AddEffect(tStart, tEnd2);
	return true;
}

void   TSceneGameIn::AddEffect(TVector2 tStart, TVector2 tEnd)
{
	auto pObject3 = std::make_shared<TEffectObj>();
	pObject3->m_pMeshRender = &TGameCore::m_MeshRender;
	pObject3->m_vVertexList = pObject3->m_pMeshRender->m_vVertexList;
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/newMega.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	TEffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 2.0f;
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
	if (pObject3->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_EffectList.emplace_back(pObject3);
	}
}
void   TSceneGameIn::Init()
{
	//NPC
	//m_fsm.AddStateTransition(STATE_STAND, EVENT_PATROL, STATE_MOVE);
	//m_fsm.AddStateTransition(STATE_STAND, EVENT_FINDTARGET, STATE_MOVE);
	//m_fsm.AddStateTransition(STATE_MOVE, EVENT_STOP, STATE_STAND);
	//m_fsm.AddStateTransition(STATE_MOVE, EVENT_LOSTTARGET, STATE_STAND);
	//m_fsm.AddStateTransition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	//m_fsm.AddStateTransition(STATE_ATTACK, EVENT_STOP, STATE_STAND);
	//m_fsm.AddStateTransition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);
	m_fsm.AddStateTransition(STATE_STAND, EVNET_DIE, STATE_DEAD);
	m_fsm.AddStateTransition(STATE_MOVE, EVNET_DIE, STATE_DEAD);
	m_fsm.AddStateTransition(STATE_ATTACK, EVNET_DIE, STATE_DEAD);
	//GUI 기능 추가 예정.
	m_GuiFSM.AddStateTransition(T_DEFAULT, EVENT_SELECT, T_HOVER);
	m_GuiFSM.AddStateTransition(T_HOVER, EVENT_DEFAULT, T_DEFAULT);
	m_GuiFSM.AddStateTransition(T_HOVER, EVENT_SELECT, T_SELECTED);
	m_GuiFSM.AddStateTransition(T_SELECTED, EVENT_DEFAULT, T_DEFAULT);


	CreateSound();
	m_pBitmap1Mask = I_Tex.Load(L"../../data/texture/bitmap2.bmp");
	GameDataLoad(L"SpriteData.txt");

	m_pWorld = std::make_shared<TWorld>(this);
	CreateMap();
	CreateRect();
	CreateHero();
	CreateNPC();
	//CreateUI();
	//CreateEffect();
}



void   TSceneGameIn::Frame()
{
	// 점프 사운드
	TSoundManager::GetInstance().Frame();
	if (m_pHero->m_iJumpingCount < 3 && m_pHero->m_iJumpingCount >= 0 && g_GameKey.dwWkey == KEY_PUSH)
	{
		m_pJumpSound->PlayEffect();
	}
	if (g_GameKey.dwSpace == KEY_UP)
	{
		m_pShotSound->PlayEffect();
	}

	m_pMap->Frame();

	//m_pHero->m_fGravity = 980;
	m_pHero->m_fGroundY = 1800;
	for (auto rectlist : m_ColList)
	{
		TRect Herorect = m_pHero->m_rtScreen;
		TRect Colrect = rectlist->m_rtScreen;
		/*TRect Projectrect = m_pHero->m_pProjectile->m_rtScreen;*/
		//m_pHero->GetGroundH(1800.0f);
		if (TCollision::CheckRectToRect(Colrect, Herorect))
		{
			if (Herorect.vc.x < Colrect.vc.x)																		// 왼쪽에서 올라올때
			{
				if (Herorect.v2.y-0.1f > Colrect.v1.y)																// 왼쪽아래서 올라올때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y<Colrect.v1.y && Herorect.vc.x > Colrect.v1.x)
					{
						m_pHero->m_vPos.y = Colrect.v1.y - Herorect.vh.y;
						m_pHero->GetGroundH(Colrect.v1.y - Herorect.vh.y);
					}
				}
				else																								// 왼쪽 위에서 내려갈때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y<Colrect.v1.y && Herorect.v2.x + m_offsetdis > Colrect.v1.x)
					{
						m_pHero->m_vPos.y = Colrect.v1.y - Herorect.vh.y;
						m_pHero->GetGroundH(Colrect.v1.y - Herorect.vh.y);
					}
				}
				
			}
			else																									 // 오른쪽에서 올라올때
			{
				if (Herorect.v2.y-0.1f > Colrect.v1.y)																 // 오른쪽에서 올라올때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y<Colrect.v1.y && Herorect.vc.x < Colrect.v2.x)
					{
						m_pHero->m_vPos.y = Colrect.v1.y - Herorect.vh.y;
						m_pHero->GetGroundH(Colrect.v1.y - Herorect.vh.y);
					}
				}
				else																								 // 오른쪽 위에서 내려갈때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y<Colrect.v1.y && Herorect.v1.x - m_offsetdis < Colrect.v2.x)
					{
						m_pHero->m_vPos.y = Colrect.v1.y - Herorect.vh.y;
						m_pHero->GetGroundH(Colrect.v1.y - Herorect.vh.y);
					}
				}
			}

			if (Herorect.vc.x <Colrect.v2.x && Herorect.vc.x > Colrect.v1.x)									    // 머리 충돌할때
			{
				if (Herorect.v1.y > Colrect.vc.y && Herorect.v1.y < Colrect.v2.y)
				{
					m_pHero->m_vPos.y = Colrect.v2.y + Herorect.vh.y;
					m_pHero->m_fGravity = 980;
					m_pHero->m_fVerticalSpeed = 0;
				}
			}

			if (Herorect.v2.x > Colrect.v1.x && Herorect.v2.x - m_offsetdis < Colrect.v1.x && Herorect.v2.y + m_offsetdis >= Colrect.v1.y)		// 오른쪽 벽에 충돌할때
			{
				m_pHero->m_vPos.x = Colrect.v1.x - Herorect.vh.x;
			}
			if (Herorect.v1.x  < Colrect.v2.x && Herorect.v1.x + m_offsetdis > Colrect.v2.x && Herorect.v2.y + m_offsetdis >= Colrect.v1.y)		// 왼쪽 벽에 충돌할때
			{
				m_pHero->m_vPos.x = Colrect.v2.x + Herorect.vh.x;
			}
		}
		else
		{
			m_pHero->m_bIsJumping = true;
			m_pHero->m_CurrentState = HeroState::Jump;
		}

		for (auto& projectile : m_pHero->m_pProjectile->m_datalist)
		{
			auto pProjectile = dynamic_cast<TProjectileEffect*>(projectile.get());
			if (pProjectile && !pProjectile->m_bDead) {
				if (TCollision::CheckRectToRect(Colrect, pProjectile->m_rtScreen)) {
					pProjectile->m_bDead = true; // 충돌 시 발사체 제거
				}
			}
		}
		//m_pHero->m_bIsJumping = true;
	}


	/*m_vCamera.x = m_pHero->m_vPos.x;
	m_vCamera.y = m_pHero->m_vPos.y;*/

	if (g_GameKey.dwPkey == KEY_PUSH)
	{
		
		if (m_Debug == Debug::Normal)
		{
			m_Debug = Debug::Debug;
		}
		else
		{
			m_Debug = Debug::Normal;
		}
	}

	if (m_Debug == Debug::Normal)
	{
		if (m_pHero->m_rtScreen.vc.x >= 640.0f && m_pHero->m_rtScreen.vc.x <= 3240.0f && m_pHero->m_rtScreen.vc.y < 800.0f)
		{
			m_vCamera.x = m_pHero->m_vPos.x;
			m_vCamera.y = 400;
		}
		if (m_pHero->m_rtScreen.vc.x >= 3750.0f && m_pHero->m_rtScreen.vc.x < 5000.0f && m_pHero->m_rtScreen.vc.y > 600.0f)
		{
			m_vCamera.y = m_pHero->m_vPos.y - 200.0f;
			m_vCamera.x = 3240.0f;
		}
		if (m_pHero->m_rtScreen.vc.y < 1300.0f && m_pHero->m_rtScreen.vc.y >= 1000.0f && m_pHero->m_rtScreen.vc.x < 3880.0f && m_pHero->m_rtScreen.vc.x > 3200.0f)
		{
			m_vCamera.x = 3240.0f;
			m_vCamera.y = 1300.0f;
		}
		else if ((m_pHero->m_rtScreen.vc.y > 1300.0f || m_pHero->m_rtScreen.vc.x > 3840.0f) && m_pHero->m_rtScreen.vc.x < 10880.0f && m_pHero->m_rtScreen.vc.x >3240.0f)
		{
			m_vCamera.x = m_pHero->m_vPos.x;
			m_vCamera.y = 1300.0f;
		}
	}
	else
	{
		if (g_GameKey.dwLeftbutton == KEY_HOLD)
		{
			m_vCamera.x -= 1.0f;
		}
		else if (g_GameKey.dwRightbutton == KEY_HOLD)
		{
			m_vCamera.x += 1.0f;
		}
		else if (g_GameKey.dwUpbutton == KEY_HOLD)
		{
			m_vCamera.y -= 1.0f;
		}
		else if (g_GameKey.dwDownbutton == KEY_HOLD)
		{
			m_vCamera.y += 1.0f;
		}
	}
	



	/*else if (m_pHero->m_rtScreen.vc.x < 640.0f)
	{
		m_vCamera.x = 640.0f;
	}
	else if (m_pHero->m_rtScreen.vc.x > 4480.0f)
	{
		m_vCamera.x = 4480.0f;
	}*/
	//m_vCamera.y = m_pHero->m_vPos.y;


	// m_vCamera.y = m_pHero->m_rtScreen.y - 225;// 초기 카메라 세팅
	  // 메가맨 위치와 카메라 위치 보정


	TVector2 vMouse = GetWorldMousePos();
	for (auto& npc : m_NpcList) 
	{
		if (!npc->m_bDead) 
		{
			if (TCollision::CheckRectToRect(npc->m_rtScreen, m_pHero->m_rtScreen))
			{
				npc->HitOverlap(m_pHero.get(), THitResult{}); // NPC가 Hero와 충돌
				m_pHero->HitOverlap(npc.get(), THitResult{}); // Hero가 NPC와 충돌
			}
		}
	}

	// NPC와 미사일 충돌 처리
	for (auto& npc : m_NpcList)
	{
		if (!npc->m_bDead) 
		{
			for (auto& projectile : m_pHero->m_pProjectile->m_datalist)
			{
				auto pProjectile = dynamic_cast<TProjectileEffect*>(projectile.get());
				if (pProjectile && !pProjectile->m_bDead) 
				{
					if (TCollision::CheckRectToRect(npc->m_rtScreen, pProjectile->m_rtScreen))
					{
  						npc->m_HP -= pProjectile->m_Data.m_iDamage;
						pProjectile->m_bDead = true; // 미사일 소멸
						if (npc->m_HP <= 0)
						{
							/*npc->m_bDead = true;*/
							m_pCrashSound->PlayEffect();
						}
					}
				}
			}
		}
	}

	// NPC 상태 업데이트
	for (auto& npc : m_NpcList) 
	{
		if (!npc->m_bDead)
		{
			npc->FrameState(m_pHero.get()); // Hero와 NPC 상호작용
			npc->Frame();
		}
	}

	// Hero 상태 업데이트
	m_pHero->Frame();


	TSphere s;
	s.vCenter = vMouse;
	s.fRadius = 100.0f;




	//if (g_GameKey.dwMiddleClick == KEY_HOLD)
	//{
	//	TVector2 v1 = vMouse;
	//	v1.x = vMouse.x - 50.0f;
	//	v1.y = vMouse.y - 50.0f;
	//	TVector2 tEnd = { v1.x + 100.0f, v1.y + 100.0f };
	//	AddEffect(v1, tEnd);
	//}
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

	for (auto data : m_UiList)
	{
		if (!data->m_bDead)
		{
			//data->FrameState(m_pHero.get());
			data->Frame();
		}
	}
	for (auto data : m_ColList)
	{
		if (!data->m_bDead)
		{
			//data->FrameState(m_pHero.get());
			data->Frame();
		}
	}

	m_pWorld->Frame();

}
void   TSceneGameIn::Render()
{
	TSoundManager::GetInstance().Render();

	m_pMap->Transform(m_vCamera);
	m_pMap->Render();

	TDevice::m_pd3dContext->PSSetSamplers(0, 1, TDxState::m_pPointSS.GetAddressOf());
	TDevice::m_pd3dContext->PSSetShaderResources(1, 1, &m_pBitmap1Mask->m_pTexSRV);

	for (auto data : m_NpcList)
	{
		if (data->m_bDead)	continue;
		data->Transform(m_vCamera);
		data->Render();
	}

	m_pHero->Transform(m_vCamera);
	m_pHero->Render();


	for (auto data : m_ColList)
	{
		if (data->m_bDead)	continue;
		data->Transform(m_vCamera);
		data->Render();
	}
	for (auto data : m_EffectList)
	{
		data->Transform(m_vCamera);
		data->Render();
	}
	for (auto data : m_UiList)
	{
		if (!data->m_bDead)
		{
			data->Transform(m_vCamera);
			data->Render();
		}
	}

}
void   TSceneGameIn::Release()
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
	for (auto data : m_UiList)
	{
		data->Release();
	}
	for (auto data : m_ColList)
	{
		data->Release();
	}
	m_pHero->Release();
	m_pMap->Release();
}
