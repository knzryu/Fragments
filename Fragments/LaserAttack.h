#pragma once

#include "DxLib.h"
#include "CourseManager.h"

namespace
{
    // レーザー1本を描画する
    void DrawLaserBeam(
        VECTOR start,
        VECTOR end,
        float width,
        int textureHandle)
    {
        // 始点から終点への方向ベクトル
        VECTOR direction = VSub(end, start);

        // 正規化（長さを1にする）
        direction = VNorm(direction);

        // カメラの上方向
        VECTOR up = VGet(0.0f, 1.0f, 0.0f);

        // レーザーの横方向を計算
        VECTOR right = VCross(direction, up);
        right = VNorm(right);


        // 横方向に幅を持たせる
        VECTOR offset = VScale(right, width);

        // 四隅の座標
        VECTOR p0 = VSub(start, offset);
        VECTOR p1 = VAdd(start, offset);
        VECTOR p2 = VAdd(end, offset);
        VECTOR p3 = VSub(end, offset);

        // 法線
        VECTOR normal = VCross(direction, right);
        normal = VNorm(normal);

        // 頂点情報
        VERTEX3D vertex[6] = {};

        // 三角形1
        vertex[0].pos = p0;
        vertex[1].pos = p1;
        vertex[2].pos = p2;

        // 三角形2
        vertex[3].pos = p0;
        vertex[4].pos = p2;
        vertex[5].pos = p3;

        // テクスチャ座標
        vertex[0].u = 0.0f; vertex[0].v = 1.0f;
        vertex[1].u = 0.0f; vertex[1].v = 0.0f;
        vertex[2].u = 1.0f; vertex[2].v = 0.0f;

        vertex[3].u = 0.0f; vertex[3].v = 1.0f;
        vertex[4].u = 1.0f; vertex[4].v = 0.0f;
        vertex[5].u = 1.0f; vertex[5].v = 1.0f;

        // 全頂点に法線と色を設定
        for (int i = 0; i < 6; i++)
        {
            vertex[i].norm = normal;

            vertex[i].dif = GetColorU8(
                255, 255, 255, 255
            );

            vertex[i].spc = GetColorU8(
                0, 0, 0, 0
            );
        }
        // ライティングを無効化
        SetUseLighting(FALSE);

        // Zバッファへの書き込みを無効化
        SetWriteZBuffer3D(FALSE);

       

        // 2つの三角形を描画
        DrawPolygon3D(
            vertex,
            2,
            textureHandle,
            TRUE
        );
        // 描画設定を元に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // Zバッファへの書き込みを再び有効化
        SetWriteZBuffer3D(TRUE);
        // ライティングを元に戻す
        SetUseLighting(TRUE);
    }
}


//ボス攻撃
class LaserAttack
{
private:

    //レーザー描画用テクスチャ
    int textureHandle = -1;

    //レーザーテクスチャの見た目の半幅
    static constexpr float LASER_VISUAL_WIDTH = 2.0f;

    // レーザーの始点
    VECTOR startPosition = VGet(0.0f, 0.0f, 0.0f);

    // 3本のレーザーの終点
    VECTOR endPositions[3]{};

    // レーザーの長さ
    static constexpr float LASER_LENGTH = 50.0f;

    // レーザーの太さ（半径）
    static constexpr float LASER_RADIUS = 0.25f;

    // 左右に広げる角度
    static constexpr float LASER_ANGLE = 10.0f;

   

public:

    //テクスチャを設定
    void SetTextureHandle(int handle);

    // レーザーの方向を計算
    void SetDirection(
        VECTOR origin,
        CoursePoint forward,
        CoursePoint right
    );

    // レーザーがプレイヤーに命中したか判定
    bool IsHit(
        VECTOR playerMin,
        VECTOR playerMax
    ) const;

    // チャージ中のエネルギー球を描画
    void DrawCharge(float progress) const;

    // 予兆線を描画
    void DrawWarning() const;

    // 攻撃中のレーザーを描画
    void DrawAttack() const;

    // 攻撃終了時のレーザー消失演出
    void DrawEnd(float progress) const;

};