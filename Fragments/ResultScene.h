#pragma once

#include "ResultData.h"

class ResultScene
{
private:
	ResultData resultData;


public:
	void Initialize(const ResultData& data);

	void Update();

	void Draw();

};

