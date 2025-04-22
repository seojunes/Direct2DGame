#include "AAsset.h"
bool AAsset::Import(std::wstring szFileName,
	std::shared_ptr<AAsset>& asset)
{
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"rb");
	if (err != 0) return false;

	TAssetFileHeader fileHeader;	
	fread(&fileHeader, sizeof(TAssetFileHeader), 1, fp);


	asset->m_szFileName.resize(fileHeader.iLength);
	asset->m_ptNodeList.resize(fileHeader.iNumNodeCounter);

	fread((void*)asset->m_szFileName.data(), sizeof(wchar_t),
						fileHeader.iLength, fp);	
	fread(&asset->m_ptNodeList.at(0), sizeof(TFbxNode),
						fileHeader.iNumNodeCounter, fp);

	auto mesh = std::make_shared<UStaticMeshComponent>();
	for (int iMesh =0; iMesh < fileHeader.iNumNodeCounter; iMesh++)
	{
		auto child = std::make_shared<UPrimitiveComponent>();

		TAssetFileHeader childheader;
		fread(&childheader, sizeof(TAssetFileHeader), 1, fp);				
		child->m_bRenderMesh = childheader.isMesh;
		if (childheader.isMesh > 0)
		{
			child->m_matBindPose.resize(childheader.iBindposeMatrix);
			fread(&child->m_matBindPose.at(0),
				sizeof(TMatrix),
				childheader.iBindposeMatrix, fp);
		}
		if (childheader.iNumTrack > 0)
		{
			child->m_AnimList.resize(childheader.iNumTrack);
			fread(&child->m_AnimList.at(0),
				sizeof(TMatrix), childheader.iNumTrack, fp);
		}

		if (childheader.isSubMesh > 0)
		{
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
				child->m_SubChilds.emplace_back(subchild);
			}
		}
		else
		{
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
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, szFileName.c_str(), L"wb");
	if (err != 0) return false;
	auto mesh = tFile->GetMesh();
	TAssetFileHeader fileHeader = tFile->m_Header;
	fileHeader.iLength = tFile->m_szFileName.size(); // 파일명 크기
	fileHeader.iNumNodeCounter = mesh->m_Childs.size();
	fileHeader.iVersion = 100;
	fileHeader.matWorld = tFile->m_matWorld;
	fileHeader.iNumTrack = mesh->m_AnimList.size();
	fileHeader.iStartFrame = fileHeader.iStartFrame;
	fileHeader.iLastFrame = fileHeader.iLastFrame;
	fileHeader.iFrameSpeed = 30;
	fileHeader.iBindposeMatrix = tFile->GetMesh()->m_matBindPose.size();

	fwrite(&fileHeader, sizeof(TAssetFileHeader), 1, fp);
	fwrite(tFile->m_szFileName.c_str(), sizeof(wchar_t), fileHeader.iLength, fp);	
	fwrite(&tFile->m_expFbxNodes.at(0), sizeof(TFbxNode), fileHeader.iNumNodeCounter, fp);

	for (auto& mesh : tFile->GetMesh()->m_Childs)
	{
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
		fwrite(&header, sizeof(TAssetFileHeader), 1, fp);

		// bindpose
		if (header.isMesh > 0)
		{
			fwrite( &mesh->m_matBindPose.at(0),
					sizeof(TMatrix),
				mesh->m_matBindPose.size(), fp);
		}
		if (header.iNumTrack > 0)
		{
			fwrite(&mesh->m_AnimList.at(0), sizeof(TMatrix),
				header.iNumTrack, fp);
		}
		
		/*for (auto tex : mesh-> )
		{
			TTexFileHeader texHead;
			texHead.iLength = tex.size();
			fwrite(&texHead, sizeof(TTexFileHeader), 1, fp);
			fwrite(tex.c_str(), sizeof(wchar_t), texHead.iLength, fp);
		}*/
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