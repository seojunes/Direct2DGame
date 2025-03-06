#pragma once
#include <string>
//_aligned_malloc
//�޸� �Ҵ��� �ּ��� ����
	//  16byte, 32byte, 64yte 
	//  [0~14,+2]-> [16+14,+2], [32, 
// ��ü�� �迭�� ��� -> 180 byte
	// A��ü(14byte) -> new -> 14����Ʈ �Ҵ��.
	// B��ü(14byte) -> new -> 14����Ʈ �Ҵ��.
	// �����ּ� : 0 ~ 14, 15~28

template<class T>
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 8,
		POOL_SIZE_MASK = POOL_MAX_SIZE-1,
	};
	//POOL_MAX_SIZE : 1000, POOL_SIZE_MASK : 0111
	//m_HeadPos = 0; pos=0 & POOL_SIZE_MASK -> 0
	//                0000 & 0111  -> 0 ���
	//                0001 & 0111  -> 1
	//                0010 & 0111  -> 2
	//                0011 & 0111  -> 3
	//                0100 & 0111  -> 4
	//                0101 & 0111  -> 5
	//                0110 & 0111  -> 6
	//                0111 & 0111  -> 7
	//                1000 & 0111  -> 0
	static void* operator new (size_t size)
	{
		// int pos = m_HeadPos++;
		long long pos = InterlockedIncrement64(&m_HeadPos) - 1;
		long long valPos = pos & POOL_SIZE_MASK;
		// ��ȯ��=m_mPool[valPos] -> nullptr ��ü�Ѵ�.
		void* pValue = InterlockedExchangePointer(&m_mPool[valPos], nullptr);
		if (pValue != nullptr)
		{
			return pValue;
		}
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete (void* obj)
	{
		// m_TailPos++
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;
		long long valPos = pos & POOL_SIZE_MASK;
		//'LONG64 _InterlockedExchange64(volatile LONG64 *,LONG64)': �μ� 1��(��) 'void **'���� 'volatile LONG64 *'(��)�� ��ȯ�� �� �����ϴ�.
		void* pValue = InterlockedExchangePointer(&m_mPool[valPos], obj);
		if (pValue != nullptr)
		{
			_aligned_free(pValue);
		}		
	}
private:
	static void* volatile m_mPool[POOL_MAX_SIZE];
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;
};
template<class T>
void* volatile TObjectPool<T>::m_mPool[POOL_MAX_SIZE] = {};
template<class T>
long long volatile TObjectPool<T>::m_HeadPos(0);
template<class T>
long long volatile TObjectPool<T>::m_TailPos(0);