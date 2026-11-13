#include "Spline.h"
#include <vector>
#include <random>

class CourseManager
{
private:

    // コースの制御点を複数保存する配列
    std::vector<CoursePoint> controlPoints;

    // 乱数生成器
    std::mt19937 randomEngine;

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

