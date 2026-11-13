#include "Camera.h"
#include "DxLib.h"	

void Camera::Update(
	const Player& player,
	const CourseManager& courseManager)
{
	//コースの進行方向を取得
	CoursePoint direction =
		courseManager.GetDirection(player.GetCourseZ());

	//カメラの位置を計算
	VECTOR cameraPos = VGet(
		player.GetX() - direction.x * followDistance,
		cameraHeight,
		player.GetZ() - direction.z * followDistance
	);

	//カメラの注視店を計算
	VECTOR cameraTarget = VGet(
		player.GetX() + direction.x * lookAheadDistance,
		targetHeight,
		player.GetZ() + direction.z * lookAheadDistance
	);

	// カメラ位置を保存
	position = cameraPos;

	SetCameraPositionAndTarget_UpVecY(
		cameraPos,
		cameraTarget
	);
}

VECTOR Camera::GetPosition() const
{
	return position;
}

