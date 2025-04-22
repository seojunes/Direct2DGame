	#include "TSceneGameIn.h"
#include "GameCore.h"
#include "TGame.h"

TSceneGameIn::TSceneGameIn(TGame* p)
{
	m_pOwner = p;
}
TSceneGameIn::~TSceneGameIn() {}

// 게임 씬 변환부분.
void TSceneGameIn::ProcessAction(Object* pObj)
{
	if (m_bNextScene == true)
	{
		m_pOwner->m_pAction->Release();
		m_pOwner->SetTransition(TSceneEvent::EVENT_NEXT_SCENE);
		m_bPrevScene = false;
		m_bNextScene = false;
		return;
	}
	if (m_bPrevScene == true)
	{
		m_pOwner->m_pAction->Release();
		m_pOwner->SetTransition(TSceneEvent::EVENT_PREV_SCENE);
		m_bPrevScene = false;
		m_bNextScene = false;
		return;
	}
}
// 마우스의 위치를 받아서 반환
TVector2 TSceneGameIn::GetWorldMousePos()
{
	TVector2 vPos = { (float)g_ptMouse.x, (float)g_ptMouse.y };
	vPos.x += m_vCamera.x - g_ptClientSize.x * 0.5f;
	vPos.y += m_vCamera.y - g_ptClientSize.y * 0.5f;
	return vPos;
}

bool TSceneGameIn::GameDataLoad(W_STR filename)
{
	TCHAR pBuffer[512] = { 0 };
	TCHAR pTemp[512] = { 0 };

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
		// UV texture
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
		if (iType == 1)			// 여러개의 이미지가 한개의 스프라이트
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
// 맵생성 부분.
bool TSceneGameIn::CreateMap()
{
	TRect rt;
	rt.SetP(0.0f, 0.0f, 1280.0f * 9.0f, 900 * 2.0f);
	m_pMap = std::make_shared<TMapObj>(rt, 1, 1);
	if (m_pMap->Create(m_pWorld.get()))
	{
		Texture* pTex = I_Tex.Load(L"../../data/texture/Map7.png");
		m_pMap->SetTexture(pTex).SetShader().SetLayout();
	}
	return true;
}

// 보스맵 생성 부분
bool TSceneGameIn::CreateBossMap()
{
	TRect rt;
	rt.SetS(12800.0f, 0.0f, 1280.0f, 900.0f);
	m_pBossMap = std::make_shared<TMapObj>(rt, 1, 1);
	if (m_pBossMap->Create(m_pWorld.get()))
	{
		Texture* pTex = I_Tex.Load(L"../../data/texture/bossMap.png");
		m_pBossMap->SetTexture(pTex).SetShader().SetLayout();
	}
	return true;
}

bool TSceneGameIn::CreateObject()
{
	TLoadResData resData;
	TVector2 tStart;
	TVector2 tEnd;
	//보스 생성시 이미지 
	m_pBossCreate = std::make_shared<TBossCreate>();
	tStart = { 13800.0f, 100.0f };
	tEnd = { 13830.0f , 280.0f };
	resData.texPathName = L"../../data/texture/Boss2.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pBossCreate->m_pMeshRender = &GameCore::m_MeshRender;
	if (m_pBossCreate->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pBossCreate->m_iCollisionType = CollisionType::T_Overlap;
	}

	// 보스룸으로 이동하는 portal
	m_pPortal = std::make_shared<TPortal>();
	m_pPortal->SetData(m_rtSpriteList);
	tStart = { 11272.0f,1023.0f };
	tEnd = { 11432.0f, 1200.0f };
	//m_pPortal->m_pWorld = m_pWorld.get();
	resData.texPathName = L"../../data/texture/portal.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pPortal->m_pMeshRender = &GameCore::m_MeshRender;
	if (m_pPortal->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pPortal->m_iCollisionType = CollisionType::T_Overlap;
	}

	// 엔딩 이미지
	m_pVictory = std::make_shared<TVictory>();
	tStart = { 13100.0f,-300.0f };
	tEnd = { 13780.0f, 0.0f };
	//m_pPortal->m_pWorld = m_pWorld.get();
	resData.texPathName = L"../../data/texture/victory.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pVictory->m_pMeshRender = &GameCore::m_MeshRender;
	if (m_pVictory->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pVictory->m_iCollisionType = CollisionType::T_Overlap;
	}

	// 사다리 생성 생성시 Alpha값 0으로 투명
	m_pRadder = std::make_shared<TRadderObj>();
	tStart = { 3760.0f,720.0f };
	tEnd = { 3780.0f, 850.0f };
	m_pRadder->m_fAlpha = 0.0f;
	//m_pPortal->m_pWorld = m_pWorld.get();
	resData.texPathName = L"../../data/texture/Radder.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pRadder->m_pMeshRender = &GameCore::m_MeshRender;
	if (m_pRadder->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pRadder->m_iCollisionType = CollisionType::T_Overlap;
	}
	return true;
}
// Character생성단
bool TSceneGameIn::CreateHero()
{
	m_pHero = std::make_shared<THeroObj>();
	m_pHero->SetData(m_rtSpriteList);
	m_pHero->SetMap(m_pMap.get());
	m_pHero->SetRadder(m_pRadder.get());
	TVector2 tStart = { 400.0f,300.0f };//m_pHero->m_fGroundY };
	TVector2 tEnd = { tStart.x + 85.0f, tStart.y + 110.0f };
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/character.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pHero->m_pMeshRender = &GameCore::m_MeshRender;
	m_pHero->m_pWorld = m_pWorld.get();
	if (m_pHero->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pHero->m_iCollisionType = CollisionType::T_Overlap;
	}
	return true;
}
// 바닥과의 충돌을 계산하는 rect생성. 
bool TSceneGameIn::CreateRect()
{
	rectArea = {

		{{ 0.0f  , 720.0f },{3720.0f,1020.0f} },
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
		{{12800.0f,0.0f},{12880.3f,1800.0f}},
		{{14004.0f,0.0f},{14080.0f,1800.0f}},
		{{12800.0f,843.5f},{14080.0f,900.0f}},
	};
	for (auto area : rectArea)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto m_pRect = std::make_shared<TCollisionManager>();
		m_pRect->m_pWorld = m_pWorld.get();
		m_pRect->SetMap(m_pMap.get());
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/collision.png";
		resData.texShaderName = L"../../data/shader/Default.txt";

		m_pRect->m_pMeshRender = &GameCore::m_MeshRender;

		//if (m_pWorld)
		//{ // TWorld 포인터가 유효한지 확인
		//	m_pWorld->AddCollisionExecute(obj.get(), std::bind(&TProjectile::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
		//}
		if (m_pRect->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pRect->m_iCollisionType = CollisionType::T_Overlap;
			m_ColList.emplace_back(m_pRect);
		}

	}
	return true;
}
// 몬스터와 hp바 같이 관리
bool TSceneGameIn::CreateNPC()
{
	Mon1Area = { {{1085.0f,620.0f},{1185.0f,720.0f}},
		{{885.0f,620.0f},{985.0f,720.0f}},
		{{2383.0f,620.0f},{2483.0f,720.0f}},
		{{3682.0f,1279.0f},{3782.0f,1379.0f}},
		{{4182.0f,1279.0f},{4282.0f,1379.0f}},
		{{5300.0f,1457.0f},{5400.0f,1557.0f}},
		{{7372.0f,1218.0f},{7472.0f,1318.0f}},
	};
	Mon2Area = { {{1878.5f,440.0f},{1996.0f,540.0f}},
		{{2982.5f,440.0f},{3100.0f,540.0f}},
		{{4432.5,1457.0f},{4550.0f,1557.0f}},
		{{6782.5f,1277.0f},{6900.0f,1377.0f}},
		{{9382.5f,1277.0f},{9500.0f,1377.0f}},
	};
	Mon3Area = { {{3356,320.0f},{3456.0f,420.0f}},
		//{{4247.0f,1100.0f},{4347.0f,1200.0f}},
		//{{8082.0f,1280.0f},{8082.0f,1380.0f}},
		{{5700.0f,1280.0f},{5800.0f,1380.0f}},
		{{6200.0f,1158.5f},{6300.0f,1258.5f}},
		{{9900.0f,1218.5f},{10000.0f,1318.5f}},
	};
	TNpcObj::CreateActionFSM();
	// npc
	//TRect rtWorldMap = m_pMap->m_rtScreen;


	for (auto area : Mon1Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj1 = std::make_shared<TMonster1>((tStart + tEnd) / 2.0f);
		npcobj1->m_pMeshRender = &GameCore::m_MeshRender;
		npcobj1->SetMap(m_pMap.get());
		npcobj1->SetData(m_rtSpriteList);
		npcobj1->SetFSM(&m_fsm);
		npcobj1->m_pHero = m_pHero.get(); // Hero 연결
		npcobj1->m_pWorld = m_pWorld.get();
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/monster.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj1->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj1->m_iCollisionType = CollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj1);
		}
		auto m_pHP = std::make_shared<HpBar>(npcobj1->m_HP);
		m_pHP->m_pWorld = m_pWorld.get();
		m_pHP->SetMap(m_pMap.get());
		m_pHP->SetNpc(npcobj1.get());
		//TLoadResData resData;
		resData.texPathName = L"../../data/ui/HpBar.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		tStart.y -= 30.0f;
		tStart.x += 25.0f;
		tEnd.y -= 110.0f;
		tEnd.x -= 25.0f;
		m_pHP->m_pMeshRender = &GameCore::m_MeshRender;
		if (m_pHP->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pHP->m_iCollisionType = CollisionType::T_Overlap;
			m_pHP->m_vInitialScale = m_pHP->m_rtScreen.vh;
			m_HPList.emplace_back(m_pHP);
		}
	}
	for (auto area : Mon2Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj2 = std::make_shared<TMonster2>();
		npcobj2->m_pMeshRender = &GameCore::m_MeshRender;
		npcobj2->SetMap(m_pMap.get());
		npcobj2->SetData(m_rtSpriteList);
		npcobj2->SetFSM(&m_fsm);
		npcobj2->m_pHero = m_pHero.get(); // Hero 연결
		npcobj2->m_pWorld = m_pWorld.get();
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/monster2.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj2->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj2->m_iCollisionType = CollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj2);
		}
		auto m_pHP = std::make_shared<HpBar>(npcobj2->m_HP);
		m_pHP->m_pWorld = m_pWorld.get();
		m_pHP->SetMap(m_pMap.get());
		m_pHP->SetNpc(npcobj2.get());
		//TLoadResData resData;
		resData.texPathName = L"../../data/ui/HpBar.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		tStart.y -= 30.0f;
		tStart.x += 40.0f;
		tEnd.y -= 110.0f;
		tEnd.x -= 40.0f;
		m_pHP->m_pMeshRender = &GameCore::m_MeshRender;
		if (m_pHP->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pHP->m_iCollisionType = CollisionType::T_Overlap;
			m_pHP->m_vInitialScale = m_pHP->m_rtScreen.vh;
			m_HPList.emplace_back(m_pHP);
		}
	}
	for (auto area : Mon3Area)
	{
		TVector2 tStart = area.first;
		TVector2 tEnd = area.second;
		auto npcobj3 = std::make_shared<TMonster3>();
		npcobj3->m_pMeshRender = &GameCore::m_MeshRender;
		npcobj3->SetMap(m_pMap.get());
		npcobj3->SetData(m_rtSpriteList);
		npcobj3->SetFSM(&m_fsm);
		npcobj3->m_pHero = m_pHero.get(); // Hero 연결
		npcobj3->m_pWorld = m_pWorld.get();
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/Mon3.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		if (npcobj3->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			//npcobj1->m_fSpeed = 50.0f + (rand() % 200);
			npcobj3->m_iCollisionType = CollisionType::T_Overlap;
			m_NpcList.emplace_back(npcobj3);
		}
		auto m_pHP = std::make_shared<HpBar>(npcobj3->m_HP);
		m_pHP->m_pWorld = m_pWorld.get();
		m_pHP->SetMap(m_pMap.get());
		m_pHP->SetNpc(npcobj3.get());
		resData.texPathName = L"../../data/ui/HpBar.png";
		resData.texShaderName = L"../../data/shader/Default.txt";
		tStart.y -= 30.0f;
		tStart.x += 20.0f;
		tEnd.y -= 110.0f;
		tEnd.x -= 20.0f;
		m_pHP->m_pMeshRender = &GameCore::m_MeshRender;
		if (m_pHP->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pHP->m_iCollisionType = CollisionType::T_Overlap;
			m_pHP->m_vInitialScale = m_pHP->m_rtScreen.vh;
			m_HPList.emplace_back(m_pHP);
		}
	}
	return true;
}
bool TSceneGameIn::CreateBoss()
{
	TNpcObj::CreateActionFSM();
	TVector2 tStart = { 13700.0f, 743.0f };
	TVector2 tEnd = { 13900.0f, 843.0f };
	m_pBoss = std::make_shared<TBossObj>((tStart + tEnd) / 2.0f);
	m_pBoss->m_pMeshRender = &GameCore::m_MeshRender;
	m_pBoss->SetMap(m_pMap.get());
	m_pBoss->SetFSM(&m_fsm);
	m_pBoss->SetData(m_rtSpriteList);
	m_pBoss->m_pHero = m_pHero.get(); // Hero 연결
	m_pBoss->m_pWorld = m_pWorld.get();
	TLoadResData resData;
	resData.texPathName = L"../../data/texture/Boss2.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	m_pBoss->m_pMeshRender = &GameCore::m_MeshRender;
	m_pBoss->m_pWorld = m_pWorld.get();
	if (m_pBoss->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		m_pBoss->m_iCollisionType = CollisionType::T_Overlap;
		//m_NpcList.emplace_back(m_pBoss);
	}
	return true;
}

bool TSceneGameIn::CreateUI()
{
	ButtonGUI::CreateActionFSM();

	TLoadResData resData;
	/*resData.texPathName = L"../../data/ui/Character.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto hero_tile1 = std::make_shared<UIHpTile>();
	hero_tile1->m_pMeshRender = &GameCore::m_MeshRender;
	hero_tile1->SetFSM(&m_GuiFSM);
	TVector2 vStart1 = { 0.0f, 720.0f };
	TVector2 vEnd1 = { 50.0f, 770.0f };
	if (hero_tile1->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		hero_tile1->m_iCollisionType = CollisionType::T_Overlap;
		m_UiList.emplace_back(hero_tile1);
	}*/


	resData.texPathName = L"../../data/ui/HeroEnergytile.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto hero_tile2 = std::make_shared<UIHpTile>();
	hero_tile2->m_pMeshRender = &GameCore::m_MeshRender;
	hero_tile2->SetFSM(&m_GuiFSM);
	TVector2 vStart1 = { 50.0f, 20.0f };
	TVector2 vEnd1 = { 550.0f, 70.0f };
	if (hero_tile2->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		hero_tile2->m_iCollisionType = CollisionType::T_Overlap;
		m_UiList.emplace_back(hero_tile2);
	}

	resData.texPathName = L"../../data/ui/HpBar.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto hero_hp = std::make_shared<UiHpBar>(HPBAR_OWNER::BAROWNER_HERO);
	hero_hp->SetHero(m_pHero.get());
	hero_hp->SetHeroHp(m_pHero->m_HeroHP);
	hero_hp->m_pMeshRender = &GameCore::m_MeshRender;
	hero_hp->SetFSM(&m_GuiFSM);
	vStart1 = { 118.37f, 31.0f };
	vEnd1 = { 545.0f, 64.0f };
	if (hero_hp->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		hero_hp->m_iCollisionType = CollisionType::T_Overlap;
		hero_hp->m_vInitialScale = hero_hp->m_rtScreen.vh;
		m_UiList.emplace_back(hero_hp);
	}

	/*resData.texPathName = L"../../data/ui/boss.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto boss_tile1 = std::make_shared<UIHpTile>();
	boss_tile1->m_pMeshRender = &GameCore::m_MeshRender;
	boss_tile1->SetFSM(&m_GuiFSM);
	vStart1 = { 40.0f, 150.0f };
	vEnd1 = { 115.0f, 225.0f };
	if (boss_tile1->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		boss_tile1->m_iCollisionType = CollisionType::T_Overlap;
		m_UiList.emplace_back(boss_tile1);
	}*/

	resData.texPathName = L"../../data/ui/BossEnergytile.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto boss_tile2 = std::make_shared<UIHpTile>();
	boss_tile2->m_pMeshRender = &GameCore::m_MeshRender;
	boss_tile2->SetFSM(&m_GuiFSM);
	vStart1 = { 115.0f, 150.0f };
	vEnd1 = { 1115.0f, 225.0f };
	if (boss_tile2->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		boss_tile2->m_iCollisionType = CollisionType::T_Overlap;
		m_UiList.emplace_back(boss_tile2);
	}

	resData.texPathName = L"../../data/ui/BossHpBar.png";
	resData.texShaderName = L"../../data/shader/Default.txt";

	auto boss_hp = std::make_shared<UiHpBar>(HPBAR_OWNER::BAROWNER_BOSS);
	boss_hp->SetBoss(m_pBoss.get());
	boss_hp->SetBossHp(m_pBoss->m_HP);
	boss_hp->m_pMeshRender = &GameCore::m_MeshRender;
	boss_hp->SetFSM(&m_GuiFSM);
	vStart1 = { 251.75f, 166.5f };
	vEnd1 = { 1106.5f, 214.0f };
	if (boss_hp->Create(m_pWorld.get(), resData, vStart1, vEnd1))
	{
		boss_hp->m_iCollisionType = CollisionType::T_Overlap;
		boss_hp->m_vBInitialScale = boss_hp->m_rtScreen.vh;
		m_UiList.emplace_back(boss_hp);
	}
	return true;
}
//bool TSceneGameIn::CreateEffect()
//{
//	auto pObject3 = std::make_shared<EffectObj>();
//	TVector2 tStart = { 640.0f,0.0f };
//	TVector2 tEnd2 = { tStart.x + 100.0f, tStart.y + 100.0f };
//	AddEffect(tStart, tEnd2);
//	return true;
//}
//
void   TSceneGameIn::AddEffect(TVector2 tStart, TVector2 tEnd)
{
	auto pObject3 = std::make_shared<EffectObj>();
	pObject3->m_pMeshRender = &GameCore::m_MeshRender;
	pObject3->m_vVertexList = pObject3->m_pMeshRender->m_vVertexList;
	TLoadResData resData;
	resData.texPathName = L"../../data/effect/attacked.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	EffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 0.3f;
	UINT iSprite = 0;
	data.m_iType = 0;// rand() % m_szSpriteList[0].size();
	if (data.m_iType == 0)
	{
		data.m_iNumAnimFrame = m_rtSpriteList[11].size();
		data.m_rtList = m_rtSpriteList[11];
	}
	if (data.m_iType == 1)
	{
		data.m_iNumAnimFrame = m_szSpriteList[0].size();
		data.m_szList = m_szSpriteList[0];
	}
	pObject3->SetData(data);
	pObject3->m_bCollisionCheck = false;
	if (pObject3->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		pObject3->m_pCurrentTexture = pObject3->m_pTexture;
		m_EffectList.emplace_back(pObject3);
	}
}

void   TSceneGameIn::AddItem(TVector2 tStart, TVector2 tEnd)
{
	auto pObject3 = std::make_shared<TItem>();
	pObject3->m_pMeshRender = &GameCore::m_MeshRender;
	pObject3->m_vVertexList = pObject3->m_pMeshRender->m_vVertexList;
	TLoadResData resData;
	resData.texPathName = L"../../data/Texture/item_H.png";
	resData.texShaderName = L"../../data/shader/Default.txt";
	EffectData data;
	data.m_bLoop = true;
	data.m_fLifeTime = 0.5f;
	UINT iSprite = 0;
	data.m_iType = 0;// rand() % m_szSpriteList[0].size();
	if (data.m_iType == 0)
	{
		data.m_iNumAnimFrame = m_rtSpriteList[26].size();
		data.m_rtList = m_rtSpriteList[26];
	}
	if (data.m_iType == 1)
	{
		data.m_iNumAnimFrame = m_szSpriteList[0].size();
		data.m_szList = m_szSpriteList[0];
	}
	if (pObject3->Create(m_pWorld.get(), resData, tStart, tEnd))
	{
		pObject3->m_iCollisionType = CollisionType::T_Overlap;
	}
}

void TSceneGameIn::AddDrop()
{
	DropArea = {
		{{12900.0f, 100.0f},{13050.0f, 200.0f}},
		{{13050.0f, 100.0f},{13200.0f, 200.0f}},
		{{13200.0f, 100.0f},{13350.0f, 200.0f}},
		{{13350.0f, 100.0f},{13500.0f, 200.0f}},
		{{13500.0f, 100.0f},{13650.0f, 200.0f}},
		{{13650.0f, 100.0f},{13800.0f, 200.0f}},
		{{13800.0f, 100.0f},{13950.0f, 200.0f}},
	};
	while (m_iDrop.size() < 4)
	{
		int droparea = rand() % 7;
		m_iDrop.insert(droparea);
	}
	std::vector<int> m_iDropv(m_iDrop.begin(), m_iDrop.end());

	for (int i = 0; i < 4; ++i)
	{
		TVector2 tStart = DropArea[m_iDropv[i]].first;
		TVector2 tEnd = DropArea[m_iDropv[i]].second;
		auto m_pRect = std::make_shared<TDropObj>();
		m_pRect->m_pWorld = m_pWorld.get();
		m_pRect->SetData(m_rtSpriteList);
		TLoadResData resData;
		resData.texPathName = L"../../data/texture/Boss2.png";
		resData.texShaderName = L"../../data/shader/Default.txt";

		m_pRect->m_pMeshRender = &GameCore::m_MeshRender;

		//if (m_pWorld)
		//{ // TWorld 포인터가 유효한지 확인
		//	m_pWorld->AddCollisionExecute(obj.get(), std::bind(&TProjectile::HitOverlap, this, std::placeholders::_1, std::placeholders::_2));
		//}
		if (m_pRect->Create(m_pWorld.get(), resData, tStart, tEnd))
		{
			m_pRect->m_iCollisionType = CollisionType::T_Overlap;
			m_DropList.emplace_back(m_pRect);
		}
	}

}

void   TSceneGameIn::Init()
{
	m_vCamera.x = 640.0f;
	m_vCamera.y = 400.0f;
	//NPC
	m_fsm.AddStateTransition(STATE_STAND, EVNET_DIE, STATE_DEAD);
	m_fsm.AddStateTransition(STATE_MOVE, EVNET_DIE, STATE_DEAD);
	m_fsm.AddStateTransition(STATE_ATTACK, EVNET_DIE, STATE_DEAD);
	//GUI 기능 추가 예정.
	m_GuiFSM.AddStateTransition(T_DEFAULT, EVENT_SELECT, T_HOVER);
	m_GuiFSM.AddStateTransition(T_HOVER, EVENT_DEFAULT, T_DEFAULT);
	m_GuiFSM.AddStateTransition(T_HOVER, EVENT_SELECT, T_SELECTED);
	m_GuiFSM.AddStateTransition(T_SELECTED, EVENT_DEFAULT, T_DEFAULT);

	TScene::CreateSound();
	m_pInGame->Play();

	m_pBitmap1Mask = I_Tex.Load(L"../../data/texture/bitmap2.bmp");
	GameDataLoad(L"SpriteData.txt");

	m_pWorld = std::make_shared<TWorld>(this);
	CreateMap();
	CreateBossMap();
	CreateRect();
	CreateObject();
	CreateHero();
	CreateNPC();
	CreateBoss();
	CreateUI();
}



void   TSceneGameIn::Frame()
{
	TScene::Frame();
	TVector2 vMouse = GetWorldMousePos();
	srand(static_cast<unsigned>(time(0)));

	// 디버그 모드 변환 벽충돌 표시, 카메라 수동이동
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

	// 영웅 관리, 밖으로 빼야됨
	if (m_pHero->m_HeroHP <= 0)
	{
		m_bNextScene = true;
	}

	// 점프 사운드, 슈팅 사운드
	SoundManager::GetInstance().Frame();
	if (m_pHero->m_iJumpingCount < 3 /*&& m_pHero->m_iJumpingCount >= 0*/ && g_GameKey.dwWkey == KEY_PUSH && m_pHero->m_CurrentState != HeroState::Radder)
	{
		m_pJumpSound->PlayEffect();
	}
	if (g_GameKey.dwSpace == KEY_UP)
	{
		m_pShotSound->PlayEffect();
	}

	m_pMap->Frame();
	if (g_GameKey.dwKkey == KEY_PUSH)
	{
		m_bPrevScene = true;
	}

	//기본 바닥높이 설정.
	//m_pHero->m_fGroundY = 1800;
	



	if (m_Debug == Debug::Normal)
	{
		for (auto data : m_ColList)
		{
			data->m_fAlpha = 0.0f;
		}
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

		if (m_pHero->m_BossMoving == BossRoomMovingState::STATE_ABLE && g_GameKey.dwSkey == KEY_PUSH)
		{
			m_pInGame->Stop();
			m_pBossSound->Play();
			m_vCamera.x = 13440.0f;
			m_vCamera.y = 500.0f;
			m_pHero->m_vPos = { 13440.0f, 500.0f };
			m_pHero->m_fGroundY = 900.0f;
			m_MapAction = MapAction::STATE_CREATEBOSS;
			m_pHero->m_BossMoving = BossRoomMovingState::STATE_UNABLE;
		}
	}
	else
	{
		for (auto data : m_ColList)
		{
			data->m_fAlpha = 1.0f;
		}
		if (g_GameKey.dwLeftbutton == KEY_HOLD)
		{
			m_vCamera.x -= 5.0f;
		}
		else if (g_GameKey.dwRightbutton == KEY_HOLD)
		{
			m_vCamera.x += 5.0f;
		}
		else if (g_GameKey.dwUpbutton == KEY_HOLD)
		{
			m_vCamera.y -= 5.0f;
		}
		else if (g_GameKey.dwDownbutton == KEY_HOLD)
		{
			m_vCamera.y += 5.0f;
		}
	}


	m_pBossCreate->GetState(m_MapAction == MapAction::STATE_CREATEBOSS);
	if (m_pBossCreate->m_bCreate == true)
	{
		m_MapAction = MapAction::STATE_BOSS;
	}
	m_pBoss->GetState(m_MapAction == MapAction::STATE_BOSS);

	if (!m_pBoss->m_bDead)
	{
		if (m_pBoss->m_bAttacked == true)
		{
			TVector2 dd = m_pBoss->Attacked(m_pBoss->m_vPos);
			TVector2 cc = { 50.0f,50.0f };
			TVector2 aa = dd - cc;
			TVector2 bb = dd + cc;
			AddEffect(aa, bb);
		}
		m_pBoss->FrameState(m_pHero.get());
		m_pBoss->Frame();
	}
	
	m_pBossCreate->Frame();

	if (m_pBoss->m_bBossA2)
	{
		m_pBossA2->PlayEffect();
		m_pBoss->m_bBossA2 = false;
	}


	// NPC 상태 업데이트
	for (auto& npc : m_NpcList)
	{
		if (!npc->m_bDead)
		{
			if (npc->m_bAttacked == true)
			{
				TVector2 dd = npc->Attacked(npc->m_vPos);
				TVector2 cc = { 50.0f,50.0f };
				TVector2 aa = dd - cc;
				TVector2 bb = dd + cc;
				AddEffect(aa, bb);
			}
			if (npc->m_bBefired)
			{
				m_pMon2M->PlayEffect();
				npc->m_bBefired = false;
			}
			if (npc->m_bBoom)
			{
				m_pMon3M->PlayEffect();
				npc->m_bBoom = false;
			}
			if (npc->m_bCrash)
			{
				m_pCrashSound->PlayEffect();
				npc->m_bCrash = false;
				npc->m_bCrashCheck = false;
			}
			npc->FrameState(m_pHero.get()); // Hero와 NPC 상호작용
			npc->Frame();
		}
	}
	// HP바 상태 업데이트
	for (auto& hp : m_HPList)
	{
		if (!hp->m_bDead)
		{
			hp->Frame();
		}
	}
	// Hero 상태 업데이트
	m_pHero->Frame();
	if (m_pHero->m_bAttacked)
	{
		m_pHurt->PlayEffect();
		m_pHero->m_bAttacked = false;
	}

	TSphere s;
	s.vCenter = vMouse;
	s.fRadius = 100.0f;


	for (UINT iNpc1 = 0; iNpc1 < m_NpcList.size(); iNpc1++)
	{
		if (m_NpcList[iNpc1]->m_bDead)
		{
			m_pWorld->DeleteCollisionExecute(m_NpcList[iNpc1].get());
		};
	}
	// 이펙트 관리.
	for (auto iter = std::begin(m_EffectList);
		iter != m_EffectList.end();)
	{
		EffectObj* pObj = (EffectObj*)iter->get();
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
			data->Frame();
		}
	}

	m_pWorld->Frame();
	m_pPortal->Frame();
	if (m_pPortal->m_bAble == true)
	{
		m_pTeleport->Play();
	}
	else
	{
		if (m_pTeleport)		m_pTeleport->Stop();
	}

	m_pRadder->Frame();
	// 보스피가 빠지면 보스죽었을때 상태로 이동.
	if (m_pBoss->m_HP <= 0)
	{
		//m_pBoss->m_bDead = true;
		m_bBossDefeated = true;
		m_pVictory->Frame();
	}
	// 보스죽었을때 1번만 실행
	if (m_bBossDefeated)
	{
		m_pHero->m_bEnd = true;
		for (auto data : m_DropList)
		{
			data->m_bDead = true;
		}
		m_pHero->m_CurrentState = HeroState::Victory;
		m_pBossSound->Stop();
		m_pClearSound->PlayEffect();
		m_pHero->m_bInvincible = false;
		m_pHero->m_vPos = { 13440.0f, 550.0f };
		m_pHero->m_fGravity = 0.0f;
		m_pHero->m_fVerticalSpeed = 0.0f;
		m_pHero->SetScale({ 150.0f,150.0f });
		m_bBossDefeated = false;
		m_pHero->m_bKeyinput = false;
	}

	if (m_pBoss->m_bM2Fire == true)
	{
		AddDrop();
		m_pBossDrop->PlayEffect();
		m_pBoss->m_bM2Fire = false;
	}

	for (auto& drop : m_DropList)
	{
		if (!drop->m_bDead)
		{
			drop->Frame();
		}
	}

	for (auto rectlist : m_ColList)
	{
		TRect Herorect = m_pHero->m_rtScreen;
		TRect Colrect = rectlist->m_rtScreen;
		if (TCollision::CheckRectToRect(Colrect, Herorect))
		{
			if (Herorect.vc.x < Colrect.vc.x)																		// 왼쪽에서 올라올때
			{
				if (Herorect.v2.y - 0.1f > Colrect.v1.y)																// 왼쪽아래서 올라올때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y<Colrect.v1.y && Herorect.v2.x - m_offsetdis > Colrect.v1.x)
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
				if (Herorect.v2.y - 0.1f > Colrect.v1.y)																 // 오른쪽에서 올라올때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y < Colrect.v1.y && Herorect.v1.x + m_offsetdis < Colrect.v2.x)
					{
						m_pHero->m_vPos.y = Colrect.v1.y - Herorect.vh.y;
						m_pHero->GetGroundH(Colrect.v1.y - Herorect.vh.y);
					}
				}
				else																								 // 오른쪽 위에서 내려갈때
				{
					if (Herorect.v2.y > Colrect.v1.y && Herorect.v1.y < Colrect.v1.y && Herorect.v1.x - m_offsetdis < Colrect.v2.x)
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
			if (m_pHero->m_CurrentState != HeroState::Victory && m_pHero->m_CurrentState != HeroState::Radder)
			{
				m_pHero->m_CurrentState = HeroState::Jump;
			}
		}
	}
}
void   TSceneGameIn::Render()
{
	SoundManager::GetInstance().Render();
	m_pMap->Transform(m_vCamera);
	m_pMap->Render();
	m_pBossMap->Transform(m_vCamera);
	m_pBossMap->Render();
	if (m_pBossCreate->m_bCreate == false)
	{
		m_pBossCreate->Transform(m_vCamera);
		m_pBossCreate->Render();
	}

	Device::m_pd3dContext->PSSetSamplers(0, 1, DxState::m_pPointSS.GetAddressOf());
	Device::m_pd3dContext->PSSetShaderResources(1, 1, &m_pBitmap1Mask->m_pTexSRV);

	for (auto data : m_NpcList)
	{
		if (data->m_bDead)
		{
			continue;
		}
		data->Transform(m_vCamera);
		data->Render();
	}


	m_pPortal->Transform(m_vCamera);
	m_pPortal->Render();
	m_pRadder->Transform(m_vCamera);
	m_pRadder->Render();
	m_pRadder->Fade();


	if (m_pBoss->m_HP <= 0)
	{
		m_pVictory->Transform(m_vCamera);
		m_pVictory->Render();
	}

	m_pHero->Transform(m_vCamera);
	m_pHero->Render();

	if (m_MapAction == MapAction::STATE_BOSS && !m_pBoss->m_bDead)
	{
		m_pBoss->Transform(m_vCamera);
		m_pBoss->Render();
	}


	for (auto data : m_ColList)
	{
		if (data->m_bDead)	continue;
		data->Transform(m_vCamera);
		data->Render();
		data->Fade();

	}
	for (auto data : m_HPList)
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
	for (auto data : m_DropList)
	{
		if (data->m_bDead)	continue;
		data->Transform(m_vCamera);
		data->Render();
		m_iDrop.clear();
	}

	for (int i = 0; i < 2; ++i)
	{
		if (!m_UiList[i]->m_bDead && m_pBoss->m_HP > 0)
		{
			m_UiList[i]->Transform(m_vCamera);
			m_UiList[i]->Render();
		}
	}


	if (m_MapAction == MapAction::STATE_BOSS && m_pBoss->m_HP > 0)
	{
		for (int i = 2; i < 4; ++i)
			if (!m_UiList[i]->m_bDead)
			{
				m_UiList[i]->Transform(m_vCamera);
				m_UiList[i]->Render();
			}
	}


}
void   TSceneGameIn::Release()
{
	if (m_pInGame)			m_pInGame->Stop();
	if (m_pBossSound)		m_pBossSound->Stop();
	if (m_pClearSound)		m_pClearSound->Stop();

	// 현재는 Object의 Release에서 아무런 작업을 하고있지 않지만, 추후 관리를 위해서 생성.
	for (auto data : m_NpcList)
	{
		data->Release();
		data = nullptr;
	}
	m_NpcList.clear();
	for (auto data : m_EffectList)
	{
		data->Release();
		data = nullptr;
	}

	for (auto data : m_DropList)
	{
		if (data->m_bDead)	continue;
		data->Release();
		data = nullptr;
	}

	m_EffectList.clear();
	for (auto data : m_UiList)
	{
		data->Release();
		data = nullptr;
	}
	m_UiList.clear();
	for (auto data : m_ColList)
	{
		data->Release();
		data = nullptr;
	}
	m_ColList.clear();
	for (auto data : m_HPList)
	{
		data->Release();
		data = nullptr;
		m_MapAction = MapAction::STATE_STAGE;
	}
	m_HPList.clear();

	if (m_pHero)
	{
		m_pHero->Release();
		m_pHero = nullptr;
	}
	if (m_pRadder)
	{
		m_pRadder->Release();
		m_pRadder = nullptr;
	}
	if (m_pPortal)
	{
		m_pPortal->Release();
		m_pPortal = nullptr;
	}
	if (m_pVictory)
	{
		m_pVictory->Release();
		m_pVictory = nullptr;
	}
	if (m_pBossMap)
	{
		m_pBossMap->Release();
		m_pBossMap = nullptr;
	}
	if (m_pMap)
	{
		m_pMap->Release();
		m_pMap = nullptr;
	}
	if (m_pBoss)
	{
		m_pBoss->Release();
		m_pBoss = nullptr;
	}
	if (m_pBossCreate)
	{
		m_pBossCreate->Release();
		m_pBossCreate = nullptr;
	}
}
