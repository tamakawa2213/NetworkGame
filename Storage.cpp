#include "Storage.h"

namespace Storage
{
	char Difficulty;


	void SetDifficulty(char dif)
	{
		Difficulty = dif;
	}
	char GetDifficulty()
	{
		return Difficulty;
	}
}