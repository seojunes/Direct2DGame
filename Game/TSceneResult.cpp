#include "TSceneResult.h"
#include "TGameCore.h"
#include "TGame.h"
TSceneResult::TSceneResult(TGame* p)
{
    m_pOwner = p;
}
TSceneResult::~TSceneResult() {}
void TSceneResult::ProcessAction(TObject* pObj)
{
    if (m_bNextScene == true)
    {
        m_pOwner->SetTransition(TSceneEvent::EVENT_NEXT_SCENE);
        m_bPrevScene = false;
        m_bNextScene = false;
        return;
    }
    if (m_bPrevScene == true)
    {
        m_pOwner->SetTransition(TSceneEvent::EVENT_PREV_SCENE);
        m_pOwner->m_pAction->Init();
        m_bPrevScene = false;
        m_bNextScene = false;
        return;
    }
}

void   TSceneResult::Init()
{

    //auto ui3 = std::make_shared<TNextBtn>();
    //ui3->m_pMeshRender = &TGameCore::m_MeshRender;
    //ui3->SetFSM(&m_GuiFSM);
    //if (ui3->Create(m_pWorld.get(), resData, { 700.0f, 0.0f }, { 800.0f, 100.0f }))
    //{
    //    ui3->m_iCollisionType = TCollisionType::T_Overlap;
    //    //ui2->SetScale(50.0f, 100.0f);
    //    //ui2->SetRotation(T_Pi * -0.25f);
    //    m_UiList.emplace_back(ui3);
    //}
    TButtonGUI::CreateActionFSM();
    m_pWorld = std::make_shared<TWorld>(this);
    m_GuiFSM.AddStateTransition(T_DEFAULT, EVENT_SELECT, T_HOVER);
    m_GuiFSM.AddStateTransition(T_HOVER, EVENT_DEFAULT, T_DEFAULT);
    m_GuiFSM.AddStateTransition(T_HOVER, EVENT_SELECT, T_SELECTED);

    TLoadResData resData;
    resData.texPathName = L"../../data/Texture/restart.png";
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

    resData.texPathName = L"../../data/ui/restart.png";

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
void   TSceneResult::Frame()
{
    for (auto data : m_UiList)
    {
        if (!data->m_bDead)
        {
            //data->FrameState(nullptr);
            data->Frame();
        }
    }

    m_pWorld->Frame();
}
void   TSceneResult::Render()
{
    for (auto data : m_UiList)
    {
        if (!data->m_bDead)
        {
            data->Transform({ 0.0f,0.0f });
            data->Render();
        }
    }
    if (m_UiList[1]->m_iSelectState == T_SELECTED)
    {
        m_bPrevScene = true;
    }
}
void   TSceneResult::Release()
{
    for (auto data : m_UiList)
    {
        data->Release();
    }
}