#pragma once

#include "ResultData.h"

class ResultScene
{
private:
	ResultData resultData;

	int fontHandle = -1;
public:
	void Initialize(const ResultData& data);

	void Update();

	void Draw();

	void Finalize();

};

