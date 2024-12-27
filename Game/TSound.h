#pragma once
#include "TStd.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")
class TSound
{
	std::wstring m_csName;
public:
	FMOD::System*  m_pSystem = nullptr;
	FMOD::Sound*   m_pSound = nullptr;
	FMOD::Channel* m_pChannel = nullptr;

	float  m_fVolume = 0.5f;
	std::wstring  m_csBuffer;
public:
	void	Play(bool bLoop = true);
	void	PlayEffect(bool bLoop = false);
	void	Stop();
	void	Paused();
	void	VolumeUp(float fVolume);
	void	VolumeDown(float fVolume);
public:
	virtual bool		Load(  FMOD::System* pSystem,
								std::wstring filename);
	virtual void		Init();
	virtual void		Frame();
	virtual void		Render();
	virtual void		Release();
public:
	TSound() : m_csName(L"none") {}
	TSound(std::wstring key) : m_csName(key) {}
};

class TSoundManager
{
	FMOD::System* m_pSystem = nullptr;
public:
	//static TSoundManager mgr;
	// ΩÃ±€≈Ê ∆–≈œ
	static TSoundManager& GetInstance()
	{
		static TSoundManager mgr;
		return mgr;
	}	
	TSound* Load(std::wstring filename);
	TSound* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, TSound*> maplist;
	TSoundManager() {}
public:
	~TSoundManager();
};
#define I_Sound TSoundManager::GetInstance()