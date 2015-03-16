#pragma once

#include "type.h"

namespace global
{

	class random
	{
	public:
		
		//	���� [0, 1] ֮���float64�����
		static float64 float64_0_1();

		//	���� [-1, 1] ֮���float64�����
		static float64 float64_M1_P1();

		//	���� [min_, max_]�ϵ�float64�����
		static float64 float64_range(float64 min_, float64 max_);

		//	���� {true, false} �ϵ�bool�����
		static bool bool_T_F();

		//	���� {true, false} �ϵ�bool�����, ����true�ĸ�����true_probָ��
		static bool bool_T_F(float64 true_prob);

		//	���� {0 , 1} �ϵ����������
		static int32 int32_0_1();

		//	���� [min_, max_]�ϵ����������
		static int32 int32_range(int32 min_, int32 max_);		

		//	���� {0 , 1} �ϵ����������
		static uint32 uint32_0_1();

		//	���� [min_, max_]�ϵ����������
		static uint32 uint32_range(uint32 min_, uint32 max_);

		// ������buf����С��sizeָ��������������򣬱���ԭ����{1, 2, 3}�������������{2, 1, 3}
		template<class T>
		static void arrange(T* buf, int32 size)
		{
			if (size > 0)
			{
				for(int32 i = 0; i < size ; ++ i)
				{
					int32 j = (int32_range(0, size - 1) % size);
					T temp = buf[i];
					buf[i] = buf[j];
					buf[j] = temp;
				}
			}
		}

		// ������buf����С��sizeָ�������������ֵ���Ҹ�ֵ������Ԫ�ظ�����ͬ�������Ƿ�ɹ�
		static bool unique_assign(int32* buf, int32 size, int32 min, int32 max);
					

	private:

		static bool initial_flag;

		static void initial();

		static int32 rand_int32();	

		static int32 const GLOBAL_RAND_MAX;

	
	};

}
