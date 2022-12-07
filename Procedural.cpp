#include "Procedural.h"
#include "Engine/Math.h"
#include <climits>
#include <stdlib.h>
#include <random>
#include <vector>

namespace Procedural
{
	std::random_device ran;
	//変数
	long SEED = ran();		//シード値
	int DesidedValue;		//最終的に生成される値
	std::vector<int> Prime;	//素数群でかき混ぜる

	void SetSeed()
	{
		SEED = ran();
	}

	void SetSeed(int seed)
	{
		SEED = seed;
	}

	int FormValue()
	{
		Prime.clear();
		Prime.reserve(10);
		Prime = Math::MakePrime(10);
		for(int i = 0; i < 10; i++)
		{
			SEED += Prime.at(i);
			SEED *= Prime.at(i);
			if (SEED >= INT_MAX)	//intの最大値を超えた場合
			{
				SEED -= UINT_MAX;	//uintの最大値を引く
			}
			if (SEED <= INT_MIN)	//intの最小値を下回った場合
			{
				SEED += UINT_MAX;	//uintの最大値を足す
			}
			SEED -= Prime.at(i);
		}
		SEED -= Math::GetDigits(SEED, 1, 1);	//このまま流すと偶数しか発生しないので2桁目の値で引く
		DesidedValue = (int)SEED;
		return DesidedValue;
	}
}