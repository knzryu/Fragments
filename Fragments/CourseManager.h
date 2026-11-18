#pragma once

#include "Spline.h"
#include <vector>
#include <random>

class CourseManager
{
private:

    //カーブ生成の設定
    enum class CurveType
    {
        Straight, //直線
        Left,
        Right
    };

    //現在の区間の種類
    CurveType currentCurveType = CurveType::Straight;

    // コースの制御点を複数保存する配列
    std::vector<CoursePoint> controlPoints;

    // 乱数生成器
    std::mt19937 randomEngine;

    //現在のカーブの傾き
    float currentSlope = 0.0f;

    //目標とするカーブの傾き
    float targetSlope = 0.0f;

    //現在の区間に残っている制御点数
    int remainingPoints = 0;

    //次のカーブ区間を決定する
    void SelectNextCurve();

public:

    // コースの初期生成
    void Initialize();

    // コースの先を追加生成する
    void UpdateCourse(float playerCourseZ);


    // ランダムな制御点を追加
    void GeneratePoint();

    // 制御点を取得
    const std::vector<CoursePoint>& GetControlPoints() const;

    // 古い制御点を削除する
    void RemoveOldPoints(float playerCourseZ);

    // 指定したZ座標のコース上の位置を取得
    CoursePoint GetPosition(float z) const;

    // 指定したZ座標のコースの進行方向を取得
    CoursePoint GetDirection(float z) const;

    // 指定したZ座標のコースの右方向を取得
    CoursePoint GetRight(float z) const;
};

