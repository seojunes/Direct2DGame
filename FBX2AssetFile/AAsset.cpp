#include "AAsset.h"
/// <summary>
/*
TAssetFileHeader: 모든 블록 단위의 정보를 담는 구조체(메타데이터 역할).
Import() : 이진 파일로부터 메시 / 애니메이션 / 본 데이터를 로드하여 AAsset 오브젝트로 구성함.
Export() : 현재 AAsset의 데이터를 바이너리로 저장함.
UStaticMeshComponent : 메시 본체.
UPrimitiveComponent : Child Mesh 또는 SubMesh 단위.
Vertex / Index / Skinning Data를 모두 저장 및 복원 가능.*/
/// </summary>

bool AAsset::Import(std::wstring szFileName, std::shared_ptr<AAsset>& asset)
{
	FILE* fp = nullptr;
	//이진 파일 read모드로 파일 열기
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"rb");
	if (err != 0) return false;

	//헤더 읽기 (파일구조/버전 등 메타정보 포함)
	TAssetFileHeader fileHeader;	
	fread(&fileHeader, sizeof(TAssetFileHeader), 1, fp);

	//저장된 문자열 길이대로 파일명 읽기
	asset->m_szFileName.resize(fileHeader.iLength);
	fread((void*)asset->m_szFileName.data(), sizeof(wchar_t), fileHeader.iLength, fp);
	
	// FBX 노드 목록 로드
	asset->m_ptNodeList.resize(fileHeader.iNumNodeCounter);
	fread(&asset->m_ptNodeList.at(0), sizeof(TFbxNode), fileHeader.iNumNodeCounter, fp);

	auto mesh = std::make_shared<UStaticMeshComponent>();
	for (int iMesh =0; iMesh < fileHeader.iNumNodeCounter; iMesh++)
	{
		//각 child는 UPrimitiveComponent이고, mesh의 실제 geometry 혹은 submesh를 가짐.
		auto child = std::make_shared<UPrimitiveComponent>();

		TAssetFileHeader childheader;
	//  fread(목적지, 크기, 개수 , 파일포인터)
		fread(&childheader, sizeof(TAssetFileHeader), 1, fp);				
		child->m_bRenderMesh = childheader.isMesh;
		if (childheader.isMesh > 0)		//child가 메쉬이면 bindpose matrix 열기
		{
			child->m_matBindPose.resize(childheader.iBindposeMatrix);
			fread(&child->m_matBindPose.at(0),
				sizeof(Matrix),
				childheader.iBindposeMatrix, fp);
		}

		if (childheader.iNumTrack > 0)	//애니메이션 트랙이 있으면, 애니메이션 행렬 넘겨줌.
		{
			child->m_AnimList.resize(childheader.iNumTrack);
			fread(&child->m_AnimList.at(0),
				sizeof(Matrix), childheader.iNumTrack, fp);
		}
		// SubMesh를 가진 복합메시인지 확인
		if (childheader.isSubMesh > 0)
		{
			// 만약 SubMesh가 있으면, subchild에 버텍스, 인덱스, iW리스트 정보를 넣어줌.
			for (int isubchild = 0; isubchild < childheader.iSubVertexBufferCounter; isubchild++)
			{
				auto subchild = std::make_shared<UPrimitiveComponent>();
				
				int iSize;
				fread(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					subchild->m_vVertexList.resize(iSize);
					fread(&subchild->m_vVertexList.at(0), sizeof(PNCT_VERTEX), iSize, fp);
				}

				fread(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					subchild->m_vIndexList.resize(iSize);
					fread(&subchild->m_vIndexList.at(0), sizeof(DWORD), iSize, fp);
				}

				fread(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					subchild->m_vIWList.resize(iSize);
					fread(&subchild->m_vIWList.at(0), sizeof(IW_VERTEX), iSize, fp);
				}	
				//child에 서브메쉬를 추가.
				child->m_SubChilds.emplace_back(subchild);
			}
		}
		else
		{
			// 없으면 단일 메쉬니까 한 덩어리로 읽어옴.
			int iSize;
			fread(&iSize, sizeof(int), 1, fp);
			if (iSize > 0)
			{
				mesh->m_vVertexList.resize(iSize);
				fread(&mesh->m_vVertexList.at(0),
					sizeof(PNCT_VERTEX), iSize, fp);
			}

			fread(&iSize, sizeof(int), 1, fp);
			if (iSize  > 0)
			{
				mesh->m_vIndexList.resize(iSize);
				fread(&mesh->m_vIndexList.at(0), sizeof(DWORD), iSize, fp);
			}

			
			fread(&iSize, sizeof(int), 1, fp);
			if (iSize > 0)
			{
				mesh->m_vIWList.resize(iSize);
				fread(&mesh->m_vIWList.at(0), sizeof(IW_VERTEX), iSize, fp);
			}
		}
		//하나의 UStaticMeshComponent(mesh)가 여러 child (bone, mesh node 등)를 갖는 구조 
		mesh->m_Childs.emplace_back(child);
	}
	asset->SetMesh(mesh);

	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();

	return true;
}


bool  AAsset::Export(AAsset* tFile, std::wstring szFileName)
{
	//파일 열기 wb <- 이진 쓰기 모드.
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"wb");
	if (err != 0) return false;

	// 상위메쉬 가져오기 + 최상위 헤더 구성.
	auto mesh = tFile->GetMesh(); //상위 메쉬 얻기.
	//fileHeader는 전체 에셋의 정보를 담는 최상위 메타정보
	TAssetFileHeader fileHeader = tFile->m_Header;
	fileHeader.iLength = tFile->m_szFileName.size();		// 파일명 크기
	fileHeader.iNumNodeCounter = mesh->m_Childs.size();		// 하위 child수
	fileHeader.iVersion = 100;								// 버전
	fileHeader.matWorld = tFile->m_matWorld;				// 월드행렬
	fileHeader.iNumTrack = mesh->m_AnimList.size();			// 상위 anim 개수
	fileHeader.iStartFrame = fileHeader.iStartFrame;		
	fileHeader.iLastFrame = fileHeader.iLastFrame;
	fileHeader.iFrameSpeed = 30;							
	fileHeader.iBindposeMatrix = tFile->GetMesh()->m_matBindPose.size();
	//최상위 헤더/파일명/노드정보 저장 
	//구조체 파일명 노드정보 순서대로 저장해서 나중에 Import할때, excat하게 fread로 맞춰 읽게됨.
	fwrite(&fileHeader, sizeof(TAssetFileHeader), 1, fp);
	fwrite(tFile->m_szFileName.c_str(), sizeof(wchar_t), fileHeader.iLength, fp);	
	fwrite(&tFile->m_expFbxNodes.at(0), sizeof(TFbxNode), fileHeader.iNumNodeCounter, fp);

	for (auto& mesh : tFile->GetMesh()->m_Childs)
	{
		//개별 헤더 저장
		TAssetFileHeader header;
		ZeroMemory(&header, sizeof(header));	
		_tcscpy_s(header.szName, 32,mesh->m_szName.c_str());
		header.isMesh = mesh->m_bRenderMesh;
		header.iBindposeMatrix = mesh->m_matBindPose.size();
		header.isSubMesh = (mesh->m_SubChilds.size() > 0) ? 1 : 0;
		header.iSubVertexBufferCounter = mesh->m_SubChilds.size();
		header.iSubIndexBufferCounter = 0;
		header.iSubIWVertexBufferCounter = mesh->m_SubChilds.size();
		header.iNumTrack = mesh->m_AnimList.size();
		// 각child마다 쓰는 로컬 헤더
		fwrite(&header, sizeof(TAssetFileHeader), 1, fp);

		// 메쉬가 있으면 bindpose정보 전달.
		if (header.isMesh > 0)
		{
			fwrite( &mesh->m_matBindPose.at(0),
					sizeof(Matrix),
				mesh->m_matBindPose.size(), fp);
		}
		// 애니메이션 정보
		if (header.iNumTrack > 0)
		{
			fwrite(&mesh->m_AnimList.at(0), sizeof(Matrix),
				header.iNumTrack, fp);
		}
		
		/*for (auto tex : mesh-> )
		{
			TTexFileHeader texHead;
			texHead.iLength = tex.size();
			fwrite(&texHead, sizeof(TTexFileHeader), 1, fp);
			fwrite(tex.c_str(), sizeof(wchar_t), texHead.iLength, fp);
		}*/

		//SubMesh 여부에 따라 저장 방식 분기. 위와 동일.
		if (header.isSubMesh > 0)
		{
			for (auto sub : mesh->m_SubChilds)
			{
				int iSize = sub->m_vVertexList.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					fwrite(&sub->m_vVertexList.at(0),
						sizeof(PNCT_VERTEX), iSize, fp);
				}
			
				iSize = sub->m_vIndexList.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					fwrite(&sub->m_vIndexList.at(0), sizeof(DWORD), iSize, fp);
				}
			
				iSize = sub->m_vIWList.size();
				fwrite(&iSize, sizeof(int), 1, fp);
				if (iSize > 0)
				{
					fwrite(&sub->m_vIWList.at(0), sizeof(IW_VERTEX), iSize, fp);
				}
			}
		}
		else
		{
			int iSize = mesh->m_vVertexList.size();
			fwrite(&iSize, sizeof(int), 1, fp);
			if (iSize > 0)
			{
				fwrite(&mesh->m_vVertexList.at(0),
					sizeof(PNCT_VERTEX), iSize, fp);
			}

			iSize = mesh->m_vIndexList.size();
			fwrite(&iSize, sizeof(int), 1, fp);
			if (iSize > 0)
			{
				fwrite(&mesh->m_vIndexList.at(0), sizeof(DWORD), iSize, fp);
			}

			iSize = mesh->m_vIWList.size();
			fwrite(&iSize, sizeof(int), 1, fp);
			if (iSize > 0)
			{
				fwrite(&mesh->m_vIWList.at(0), sizeof(IW_VERTEX), iSize, fp);
			}
		}
	}
	err = fclose(fp);
	if (err != 0) return false;
	_fcloseall();
	return true;
}