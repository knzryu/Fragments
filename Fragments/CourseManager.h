#pragma once

#include "Spline.h"
#include <vector>
#include <random>

class CourseManager
{
private:

    // コース区間の種類
    enum class CurveType
    {
        Straight,
        Left,
        Right
    };

    // 現在の区間
    CurveType currentCurveType = CurveType::Straight;

    // コースの制御点
    std::vector<CoursePoint> controlPoints;

    // 乱数生成器
    std::mt19937 randomEngine;

    // コースの横幅の範囲
    static constexpr float MIN_X = -20.0f;
    static constexpr float MAX_X = 20.0f;

    // カーブの移動距離
    static constexpr float MIN_CURVE_DISTANCE = 16.0f;
    static constexpr float MAX_CURVE_DISTANCE = 20.0f;

    // 区間の開始X座標
    float segmentStartX = 0.0f;

    // 区間の目標X座標
    float targetX = 0.0f;

    // 区間の制御点数
    int segmentPointCount = 4;

    // 区間内で生成した制御点数
    int generatedPointsInSegment = 0;

    // 最初のカーブかどうか
    bool isFirstCurve = true;

    // 次の区間を決定する
    void SelectNextCurve();

public:

    // コースの初期生成
    void Initialize();

    // コースの先を追加生成する
    void UpdateCourse(float playerCourseZ);

    // 制御点を追加する
    void GeneratePoint();

    // 制御点を取得する
    const std::vector<CoursePoint>& GetControlPoints() const;

    // 古い制御点を削除する
    void RemoveOldPoints(float playerCourseZ);

    // 指定したZ座標のコース位置を取得する
    CoursePoint GetPosition(float z) const;

    // 指定したZ座標の進行方向を取得する
    CoursePoint GetDirection(float z) const;

    // 指定したZ座標の右方向を取得する
    CoursePoint GetRight(float z) const;
};