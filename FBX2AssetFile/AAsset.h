#pragma once
#include "AActor.h"
#define MAX_BONE_MATRICES 255
struct TScene
{
	int		iStartFrame = 0;
	int		iLastFrame = 0;
	int		iFrameSpeed = 30;
};
struct TFbxNode
{
	BOOL         m_bMesh = FALSE;
	WCHAR        m_szName[32];
	WCHAR        m_szParentName[32];
};
struct TTrack
{
	int  iFrame;
	int  iTick;  // 1초=30프레임, 1프레임=160 틱, 1초=4800tick
	float fTime;
	T::TVector3 v, s;
	T::TQuaternion q;
	T::TMatrix matScale, matTrans, matRotate;
	TTrack() = default;
	TTrack(int f, T::TVector3& s,
		T::TQuaternion& r,
		T::TVector3& t)
	{
		iFrame = f;
		iTick = f * 160;
		fTime = (float)f;// / 30.0f;
		this->s = s;
		v = t;
		q = r;
	}
	TTrack(int f, T::TMatrix& s,
		T::TMatrix& r,
		T::TMatrix& t)
	{
		iFrame = f;
		iTick = f * 160;
		fTime = (float)f;// / 30.0f;
		matScale = s;
		matRotate = r;
		matTrans = t;
	}
};

// 정점 단위로 생성한다.
struct TWeight
{
	std::vector<int>   Index;
	std::vector<float> weight;
	int Insert(int iIndex, float fWeight)
	{
		size_t i = 0;
		for (i = 0; i < Index.size(); ++i)
		{
			if (fWeight > weight[i])
			{
				for (size_t j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iIndex;
				weight[i] = fWeight;
				break;
			}
		}
		return i;
		/*int   iTemp = iIndex;
		float fTemp = fWeight;

		for (size_t i = 0; i < Index.size(); i++)
		{
			if (fWeight > weight[i])
			{
				iTemp = Index[Index.size() - 1];
				fTemp = weight[Index.size() - 1];
				for (size_t j = Index.size()-1; j > i; j--)
				{
					Index[j] = Index[j-1];
					weight[j] = weight[j-1];
				}
				Index[i] = iIndex;
				weight[i] = fWeight;
				break;
			}
		}
		Index.push_back(iTemp);
		weight.push_back(fTemp);*/
	}
	TWeight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};
struct TAssetFileHeader
{
	int				iVersion = 100;
	int				iNumNodeCounter = 0;
	int				iMaxWeight = 0;
	int				isMesh = 0;
	int				isSubMesh = 0; // 서브메터리얼 유무
	int				iStartFrame = 0;
	int				iLastFrame = 0;
	int				iFrameSpeed = 30;
	int				iLength = 0;
	int				iNumTrack = 0;
	int				iBindposeMatrix=0;
	int				iChildNodeCounter = 0;
	int				iSubVertexBufferCounter = 0;
	int				iSubIndexBufferCounter = 0;
	int				iSubIWVertexBufferCounter = 0;
	WCHAR			szName[32] = { 0, };
	T::TMatrix		matWorld;
};
struct TKgcFileTrack
{
	std::vector<T::TMatrix> pAnimationMatrix;
};
struct TTexFileHeader
{
	int				iLength = 0;
	int				iType = 0;
};
struct  TBoneMatrix
{
	T::TMatrix  matBone[MAX_BONE_MATRICES];
};
// fbx -> load -> render
// fbx -> load -> asset -> import -> render
class AAsset : public AActor
{
public:
	TAssetFileHeader		m_Header;
	std::wstring			m_szFileName;
	T::TMatrix				m_matWorld;
	std::vector<std::wstring> m_szTexFileList;
	using vList = std::vector<PNCT_VERTEX>;
	using iList = std::vector<DWORD>;
	using iwList = std::vector<IW_VERTEX>;
	vList			m_vVertexList;		 // 프레임 화면 정보
	iList			m_vIndexList;
	iwList			m_vIWVertexList;
	std::vector<TFbxNode>   m_expFbxNodes;
	std::vector<vList>		m_vSubMeshVertexList;
	std::vector<iList>		m_vSubMeshIndexList;
	std::vector<TMatrix> m_pAnimationMatrix;
	std::vector<std::shared_ptr<AAsset>> m_ChildList;
	// 모든 행렬에 대한 에니메이션 프레임 저장
	using boneFrameMatrix = std::vector<TMatrix>;
		boneFrameMatrix m_pAnimationMatrixList;// 정적에니메이션	
	std::vector<boneFrameMatrix> m_pBoneAnimMatrix;
	// Skinning : 전체 트리 노드가 본 좌표계로 변환되는 행렬
	std::vector<TMatrix>		m_matBindPose;
	std::vector<int>			m_pUsedBoneIndexList;
	std::vector<TFbxNode>		m_ptNodeList;
	std::vector<TWeight>        m_WeightList;
	std::vector<iwList>			m_vSubMeshIWVertexList;
	public:
		static bool Export(AAsset* tFile,
							std::wstring szFileName);
		static bool Import( std::wstring szFileName,
							std::shared_ptr<AAsset>& tFbxModel);
};

