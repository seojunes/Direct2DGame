#include "TSound.h"
void	TSound::Play(bool bLoop)
{
	bool  bPlay = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&bPlay);
	}
	if (bPlay == false)
	{
		FMOD_RESULT hr = m_pSystem->playSound(m_pSound, nullptr, 
			false,
			&m_pChannel);
		if (hr == FMOD_OK)
		{
			m_pChannel->setVolume(m_fVolume);
			if (bLoop)
				m_pChannel->setMode(FMOD_LOOP_NORMAL);
			else
				m_pChannel->setMode(FMOD_LOOP_OFF);
		}

	}
}
void	TSound::PlayEffect(bool bLoop)
{
	FMOD_RESULT hr =m_pSystem->playSound(m_pSound, nullptr,
			false,
			&m_pChannel);
	if (hr == FMOD_OK)
	{
		if (hr == FMOD_OK)
		{
			m_pChannel->setVolume(m_fVolume);
			if (bLoop)
				m_pChannel->setMode(FMOD_LOOP_NORMAL);
			else
				m_pChannel->setMode(FMOD_LOOP_OFF);
		}
	}
}
void	TSound::Stop()
{
	if (m_pChannel)
	{
		m_pChannel->stop();
		m_pChannel = nullptr;
	}
}
void	TSound::Paused()
{
	bool bPlay = false;
	m_pChannel->isPlaying(&bPlay);
	if (bPlay)
	{
		bool paused;
		m_pChannel->getPaused(&paused);
		m_pChannel->setPaused(!paused);
	}
}
void	TSound::VolumeUp(float fVolume)
{
	if (m_pChannel)
	{
		float fCurrentVolume;
		m_pChannel->getVolume(&fCurrentVolume);
		m_fVolume = fCurrentVolume + fVolume;
		m_fVolume = max(0.0f, m_fVolume);
		m_fVolume = min(1.0f, m_fVolume);
		m_pChannel->setVolume(m_fVolume);
	}
}
void	TSound::VolumeDown(float fVolume)
{
	if (m_pChannel)
	{
		float fCurrentVolume;
		m_pChannel->getVolume(&fCurrentVolume);
		m_fVolume = fCurrentVolume - fVolume;
		m_fVolume = max(0.0f, m_fVolume);
		m_fVolume = min(1.0f, m_fVolume);
		m_pChannel->setVolume(m_fVolume);
	}
}
bool		TSound::Load(FMOD::System* pSystem, std::wstring filename)
{
	m_pSystem = pSystem;
	FMOD_RESULT hr = m_pSystem->createSound(
		to_wm(filename).c_str(),
		FMOD_DEFAULT, 0,
		&m_pSound);	
	if (hr == FMOD_OK)
	{
		return true;
	}	
	return false;
}
void		TSound::Init()
{
		
}
void		TSound::Frame()
{
	m_pSystem->update();
	unsigned int ms = 0;
	unsigned int size = 0;		 
	m_pSound->getLength(&size, FMOD_TIMEUNIT_MS);
	m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);

	wchar_t szBuffer[256] = { 0, };
	_stprintf_s(szBuffer,
		_T("전체시간[%02d:%02d]:경과시간[%02d:%02d]"),
		size / 1000 / 60,
		size / 1000 % 60,		
		ms / 1000 / 60,
		ms / 1000 % 60);
	m_csBuffer = szBuffer;
}
void		TSound::Render()
{
	//static float fTmpTimer = 0.0f;
	//fTmpTimer += g_fSPF;
	//if (fTmpTimer > 1.0)
	//{
	//	OutputDebugString(m_csBuffer.c_str());
	//	fTmpTimer = 0.0f;
	//}
}
void		TSound::Release()
{
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}

	
}
void TSoundManager::Frame()
{
	for (auto sound : maplist)
	{
		sound.second->Frame();
	}
}
void TSoundManager::Render()
{
	for (auto sound : maplist)
	{
		sound.second->Render();
	}
}

TSound* TSoundManager::Load(std::wstring filename)
{
	if (m_pSystem == nullptr)
	{
		FMOD_RESULT hr = FMOD::System_Create(
			&m_pSystem);
		if (hr == FMOD_OK)
		{
			m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
		}
	}

	auto key = SplitPath(filename);
	auto data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}
	
	TSound* pSound = new TSound(key);
	
	if (pSound->Load(m_pSystem, filename))
	{
		maplist.insert(std::make_pair(key, pSound));
	}
	else
	{
		delete pSound;
		pSound = nullptr;
	}
	return pSound;
}
std::wstring TSoundManager::SplitPath(std::wstring file)
{
	TCHAR szFileName[256];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath_s(file.c_str(), Drive, Dir, FName, Ext);

	std::wstring key = FName;
	key += Ext;
	return key;
}
TSound* TSoundManager::GetPtr(std::wstring key)
{
	auto data = maplist.find(key);
	if (data != maplist.end())
	{
		return data->second;
	}
	return nullptr;
}

TSoundManager::~TSoundManager()
{
	for (auto sound : maplist)
	{
		sound.second->Release();
		delete sound.second;
	}
	maplist.clear();
	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
	}
}