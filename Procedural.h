#pragma once
namespace Procedural
{
	void SetSeed();			//引数無しならランダムな値が入る
	void SetSeed(int seed);	//シード値を手動で入れる
	int FormValue();		//与えられたシード値を基に値を生成する
};