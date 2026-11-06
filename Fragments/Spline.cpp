#include "Spline.h"

// Catmull-Rom‹Èüã‚ÌˆÊ’u‚ğæ“¾
CoursePoint Spline::CatmullRom(
    CoursePoint p0,
    CoursePoint p1,
    CoursePoint p2,
    CoursePoint p3,
    float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    CoursePoint result;

    result.x = 0.5f * (
        2.0f * p1.x +
        (-p0.x + p2.x) * t +
        (2.0f * p0.x - 5.0f * p1.x
            + 4.0f * p2.x - p3.x) * t2 +
        (-p0.x + 3.0f * p1.x
            - 3.0f * p2.x + p3.x) * t3
        );

    result.z = 0.5f * (
        2.0f * p1.z +
        (-p0.z + p2.z) * t +
        (2.0f * p0.z - 5.0f * p1.z
            + 4.0f * p2.z - p3.z) * t2 +
        (-p0.z + 3.0f * p1.z
            - 3.0f * p2.z + p3.z) * t3
        );

    return result;
}

// ‹Èü‚Ìis•ûŒü‚ğæ“¾
CoursePoint Spline::CatmullRomTangent(
    CoursePoint p0,
    CoursePoint p1,
    CoursePoint p2,
    CoursePoint p3,
    float t)
{
    float t2 = t * t;

    CoursePoint result;

    result.x = 0.5f * (
        (-p0.x + p2.x) +
        2.0f * (2.0f * p0.x - 5.0f * p1.x
            + 4.0f * p2.x - p3.x) * t +
        3.0f * (-p0.x + 3.0f * p1.x
            - 3.0f * p2.x + p3.x) * t2
        );

    result.z = 0.5f * (
        (-p0.z + p2.z) +
        2.0f * (2.0f * p0.z - 5.0f * p1.z
            + 4.0f * p2.z - p3.z) * t +
        3.0f * (-p0.z + 3.0f * p1.z
            - 3.0f * p2.z + p3.z) * t2
        );

    return result;
}