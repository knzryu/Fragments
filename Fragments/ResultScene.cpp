
#include "DxLib.h"
#include "ResultScene.h"

//リザルト画面の初期化
void  ResultScene::Initialize(const ResultData& data)
{
	//結果データを保存
	resultData = data;
}

//リザルト画面の更新
void ResultScene::Draw()
{
    // 画面全体を暗くする背景
    DrawBox(
        0, 0, 1920, 1080,
        GetColor(10, 15, 30),
        TRUE
    );

    //タイトル
    DrawString(
        820, 100,
        "MISSION RESULT",
        GetColor(255, 80, 80)
    );

    DrawString(
        850, 150,
        "GAME OVER",
        GetColor(255, 80, 80)
    );

    // 結果表示パネル
    DrawBox(
        400, 230, 1520, 480,
        GetColor(25, 35, 55),
        TRUE
    );

    DrawBox(
        400, 230, 1520, 480,
        GetColor(80, 160, 200),
        FALSE
    );

    // 走行距離
    DrawString(
        500, 280,
        "DISTANCE",
        GetColor(150, 200, 230)
    );

    DrawFormatString(
        500, 330,
        GetColor(255, 255, 255),
        "%.1f m",
        resultData.distance
    );

    // 敵撃破数
    DrawString(
        850, 280,
        "DEFEATED",
        GetColor(150, 200, 230)
    );

    DrawFormatString(
        850, 330,
        GetColor(255, 255, 255),
        "%d",
        resultData.defeatedEnemyCount
    );

    // 生存時間
    DrawString(
        1200, 280,
        "SURVIVAL TIME",
        GetColor(150, 200, 230)
    );

    DrawFormatString(
        1200, 330,
        GetColor(255, 255, 255),
        "%.1f sec",
        resultData.survivalTime
    );

    // 獲得アイテム欄
    DrawString(
        450, 530,
        "ACQUIRED ITEMS",
        GetColor(100, 220, 255)
    );

    // アイテム表示用パネル
    DrawBox(
        400, 580, 1520, 850,
        GetColor(20, 30, 45),
        TRUE
    );

    DrawBox(
        400, 580, 1520, 850,
        GetColor(80, 160, 200),
        FALSE
    );

    //アイテム表示用の空スロット
    const int slotSize = 120;
    const int slotGap = 25;

    const int startX = 500;
    const int startY = 640;

    const int columCount = 6;

    for (int i = 0; i < 6; i++)
    {
        int x = startX + i * (slotSize + slotGap);

        DrawBox(
            x, startY,
            x + slotSize, startY + slotSize,
            GetColor(35, 50, 70),
            TRUE
        );

        DrawBox(
            x, startY,
            x + slotSize, startY + slotSize,
            GetColor(90, 130, 160),
            FALSE
        );
    }
    //アイテムがない場合の表示
    DrawString(
        850, 790,
        "NO ITEMS",
        GetColor(120, 140, 160)
    );

}