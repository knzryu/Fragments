#pragma once

// コース上の座標
struct CoursePoint
{
    float x;
    float z;
};

class Spline
{
public:

    // Catmull-Rom曲線上の位置を取得
    static CoursePoint CatmullRom(
        CoursePoint p0,
        CoursePoint p1,
        CoursePoint p2,
        CoursePoint p3,
        float t
    );

    // 曲線の進行方向を取得
    static CoursePoint CatmullRomTangent(
        CoursePoint p0,
        CoursePoint p1,
        CoursePoint p2,
        CoursePoint p3,
        float t
    );
};