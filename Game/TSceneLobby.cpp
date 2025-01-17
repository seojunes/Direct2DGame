#include "TSceneLobby.h"
#include "TGameCore.h"
#include "TGame.h"
TSceneLobby::TSceneLobby(TGame* p)
{
    m_pOwner = p;
}
TSceneLobby::~TSceneLobby() {}
void TSceneLobby::ProcessAction(TObject* pObj)
{
    if (m_bNextScene == false)
    {
        for (auto data : m_UiList)
        {
            if (data->m_fAlpha <= 1.0f)
            {
                data->m_fAlpha += g_fSPF * 0.5f;
                if (data->m_fAlpha >= 1.0f)
                {
                    data->m_fAlpha = 1.0f;
                }
            }
        }
    }
    if (m_bNextScene == true)
    {
        for (auto data : m_UiList)
        {
            data->m_fAlpha -= g_fSPF * 0.5f;
            if (data->m_fAlpha <= 0.0f)
            {
                m_bLoadNextScene = true;
            }
        }
       
    }
    if (m_bLoadNextScene == true)
    {
        m_pRobby->Stop();
        m_pOwner->SetTransition(TSceneEvent::EVENT_NEXT_SCENE);
        m_pOwner->m_pAction->Init();
       
        m_bNextScene = false;
        m_bLoadNextScene = false;
        return;
    }
}

void   TSceneLobby::Init()
{
    //TSoundManager::GetInstance().Frame();
    TScene::CreateSound();
    TScene::m_pRobby->Play();

    TButtonGUI::CreateActionFSM();
    m_pWorld = std::make_shared<TWorld>(this);
    m_GuiFSM.AddStateTransition(T_DEFAULT, EVENT_SELECT, T_HOVER);
    m_GuiFSM.AddStateTransition(T_HOVER, EVENT_DEFAULT, T_DEFAULT);
    m_GuiFSM.AddStateTransition(T_HOVER, EVENT_SELECT, T_SELECTED);

    TLoadResData resData;
    resData.texPathName = L"../../data/Texture/lobby.png";
    resData.texShaderName = L"../../data/shader/Default.txt";

    auto ui1 = std::make_shared<TImageGUI>();
    ui1->m_pMeshRender = &TGameCore::m_MeshRender;
    ui1->SetFSM(&m_GuiFSM);
    TVector2 vStart1 = { 0.0f, 0.0f };
    TVector2 vEnd1 = { 1280.0f, 800.0f };
    if (ui1->Create(m_pWorld.get(), resData, vStart1, vEnd1))
    {
        ui1->m_iCollisionType = TCollisionType::T_Overlap;
        //ui1->SetScale(100.0f, 50.0f);
        //ui1->SetRotation(T_Pi * 0.25f);
        m_UiList.emplace_back(ui1);
    }

    resData.texPathName = L"../../data/ui/pressStart.png";

    auto ui = std::make_shared<TButtonGUI>();
    ui->m_pMeshRender = &TGameCore::m_MeshRender;
    ui->SetFSM(&m_GuiFSM);
    TVector2 vStart = { 328.0f, 680.0f };
    TVector2 vEnd = { 953.5f, 724.0f };

    ui->m_pMeshRender = &TGameCore::m_MeshRender;
    if (ui->Create(m_pWorld.get(), resData, vStart, vEnd))
    {
        ui->m_iCollisionType = TCollisionType::T_Overlap;
        //ui->SetScale(300.0f, 300.0f );
        //ui->SetRotation(T_Pi*0.25f);
        m_UiList.emplace_back(ui);
    }
}
void   TSceneLobby::Frame()
{
    TScene::Frame();
    for (auto data : m_UiList)
    {
        if (!data->m_bDead)
        {
            //data->FrameState(nullptr);
            data->Frame();
        }
    }

    m_UiList[1]->FrameState(nullptr);
    m_pWorld->Frame();
}
void   TSceneLobby::Render()
{

    for (auto data : m_UiList)
    {
        if (!data->m_bDead)
        {
            data->Fade();
            data->Transform({ 0.0f,0.0f });
            data->Render();
        }
    }
    if (m_UiList[1]->m_iSelectState == T_SELECTED)
    {
        m_bNextScene = true;
    }
}
void   TSceneLobby::Release()
{
    for (auto data : m_UiList)
    {
        data->Release();
    }
   
}