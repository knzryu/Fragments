#ifndef CAMERA_H
#define CAMERA_H

#include "DxLib.h"
#include "Player.h"
#include "CourseManager.h"

class Camera
{
private:
	

	//カメラの設定値
	float followDistance = 5.0f;
	float lookAheadDistance = 6.0f;

	float cameraHeight = 4.5f;
	float targetHeight = 1.5f;

	// カメラの現在位置
	VECTOR position = { 0.0f, 0.0f, 0.0f };


public:
	void Update(
		const Player& player,
		const CourseManager& courseManager
	);

	VECTOR GetPosition() const;
};

				
#endif 
