#pragma once
#include "Std.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")
class Sound
{
	std::wstring m_csName;
public:
	FMOD::System* m_pSystem = nullptr;
	FMOD::Sound* m_pSound = nullptr;
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
	virtual bool		Load(FMOD::System* pSystem,
		std::wstring filename);
	virtual void		Init();
	virtual void		Frame();
	virtual void		Render();
	virtual void		Release();
public:
	Sound() : m_csName(L"none") {}
	Sound(std::wstring key) : m_csName(key) {}
};

class SoundManager
{
	FMOD::System* m_pSystem = nullptr;
public:
	//static SoundManager mgr;
	// ΩÃ±€≈Ê ∆–≈œ
	static SoundManager& GetInstance()
	{
		static SoundManager mgr;
		return mgr;
	}
	Sound* Load(std::wstring filename);
	Sound* GetPtr(std::wstring key);
	std::wstring SplitPath(std::wstring file);
	void		Frame();
	void		Render();
private:
	std::map<std::wstring, Sound*> maplist;
	SoundManager() {}
public:
	~SoundManager();
};
#define I_Sound SoundManager::GetInstance()