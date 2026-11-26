#include "DxLib.h"
#include <vector>
#include"Player.h"
#include"Obstacle.h"
#include"Collision.h"
#include"Enemy.h"
#include"Bullet.h"
#include"ObstacleManager.h"
#include "GameManager.h"
#include <cstdio>
#include <math.h>
#include "Spline.h"
#include "CourseManager.h"
#include <algorithm>
#include "PlayerController.h"
#include "Camera.h"
#include "ResultScene.h"
#include "BossManager.h"
#include "RankingManager.h"

// ゲームの状態をリセットする関数
void ResetGame(
    GameManager& gameManager,
    PlayerController& playerController,
    BossManager& bossManager,
    const CourseManager& courseManager,
    float& nextObstacleZ,
    bool& isHazardWaiting,
    float& bossDefeatedCourseZ,
    int& previousTime)
{
    //ゲーム全体を初期状態に戻す
    gameManager.ResetRun(courseManager);

    // ボスの状態を初期化
    bossManager.Reset();


    // ボス撃破後の待機状態を初期化
    isHazardWaiting = false;

    // ボス撃破時の距離を初期化
    bossDefeatedCourseZ = 0.0f;

    //ゲーム状態をPlayingに戻す
    gameManager.SetGameState(GameState::Playing);

    // ダッシュの状態を初期化
    playerController.Reset();

    // 次の障害物の生成位置を初期化
    nextObstacleZ = 100.0f;


    // 前回時刻を更新して、時間差をリセット
    previousTime = GetNowCount();
}


// マウスの前フレームの入力状態
int previousMouseInput = 0;



int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    //window表示
    ChangeWindowMode(TRUE);
    SetGraphMode(1920, 1080, 32);

    //Dxlib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }


    //----------------------------------------------------------
    //タイトル背景画像の読み込み
    int titleBackgroundHandle = LoadGraph("Resource/Texture/TitleBackground.png");

    if (titleBackgroundHandle == -1)
    {
        MessageBox(
            NULL,
            "Title Background Load Failed",
            "Error",
            MB_OK
        );

        DxLib_End();
        return -1;
    }


    //----------------------------------------------------------
    // Skyboxモデルの読み込み
    int skyboxHandle = MV1LoadModel("Resource/Sky/SkyBox.mv1");

    if (skyboxHandle == -1)
    {
        MessageBox(NULL, "Skybox Load Failed",
            "Error", MB_OK);
        DxLib_End();
        return -1;
    }

    // テクスチャの読み込み
    int skyboxTexture = LoadGraph("Resource/Sky/Tx.png");

    if (skyboxTexture == -1)
    {
        MessageBox(NULL, "Skybox Texture Load Failed",
            "Error", MB_OK);
        MV1DeleteModel(skyboxHandle);
        DxLib_End();
        return -1;
    }

    // テクスチャをモデルに割り当て
    MV1SetTextureGraphHandle(
        skyboxHandle,
        0,
        skyboxTexture,
        FALSE
    );

    // マテリアル設定
    //Diffuse（拡散反射）
    MV1SetMaterialDifColor(
        skyboxHandle, 0,
        GetColorF(1.0, 1.0, 1.0, 1.0)
    );
    /*
    //Specular（鏡面反射）
    MV1SetMaterialSpcColor(
        skyboxHandle, 0,
        GetColorF(0, 0, 0, 1)
    );

    //Emissive (自己発光）
    MV1SetMaterialEmiColor(
        skyboxHandle, 0,
        GetColorF(1, 1, 1, 1)
    );

    //Ambient (環境反射）
    MV1SetMaterialAmbColor(
        skyboxHandle, 0,
        GetColorF(0, 0, 0, 1)
    );

    //SpecularPower (鏡面反射の強さ）
    MV1SetMaterialSpcPower(
        skyboxHandle, 0, 0.0f
    );*/
   

    // SkyBoxのスケール設定
    MV1SetScale(
        skyboxHandle,
        VGet(0.001f, 0.001f, 0.0015f)
    );


    // SkyBoxのメッシュ数を取得
    int meshNum = MV1GetMeshNum(skyboxHandle);

    // すべてのメッシュで裏面カリングを無効にする
    for (int i = 0; i < meshNum; i++)
    {
        MV1SetMeshBackCulling(skyboxHandle, i, FALSE);
    }


    //Zバッファを使用しない
    // Skyboxの奥行き判定を無効化
    MV1SetUseZBuffer(skyboxHandle, FALSE);

    // Skyboxの奥行き情報を書き込まない
    MV1SetWriteZBuffer(skyboxHandle, FALSE);

    //---------------------------------------------------------


    // マウスカーソルを表示
    SetMouseDispFlag(TRUE);

    // 3Dの奥行き判定を有効にする
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // カメラに映す奥行きの範囲
    SetCameraNearFar(0.1f, 2000.0f);


    //コースの管理
    CourseManager courseManager;
    courseManager.Initialize();

    // リザルト画面
    ResultScene resultScene;

    //プレイヤーの移動
    GameManager gameManager;

    gameManager.Initialize(courseManager);

    RankingManager rankingManager;

    rankingManager.Initialize();

    // ランキングリセットの確認状態
    bool rankingResetConfirm = false;

    //プレイヤークラスを参照
    Player& player = gameManager.GetPlayer();

    //プレイヤーの成長・進行状況を参照
    PlayerProgress& progress = gameManager.GetPlayerProgress();

    //障害物管理オブジェクトを参照
    ObstacleManager& obstacleManager = gameManager.GetObstacleManager();
    // 障害物モデルを読み込む
    if (!obstacleManager.LoadModel())
    {
        MessageBox(
            NULL,
            "Obstacle Model Load Failed",
            "Error",
            MB_OK
        );

        DxLib_End();
        return -1;
    }


    // EnemyManagerを取得
    EnemyManager& enemyManager = gameManager.GetEnemyManager();

    // BossManagerを生成
    BossManager bossManager;
    
    // 敵モデルを読み込む
    if (!enemyManager.LoadModel())
    {
        MessageBox(
            NULL,
            "Enemy Model Load Failed",
            "Error",
            MB_OK
        );

        DxLib_End();
        return -1;
    }

    // ボスモデルを読み込む
    if (!bossManager.LoadModel())
    {
        MessageBox(
            NULL,
            "Boss Model Load Failed",
            "Error",
            MB_OK
        );

        DxLib_End();
        return -1;
    }

	// BulletManagerを取得 
    BulletManager& bulletManager = gameManager.GetBulletManager();

    PlayerController playerController;


    Camera camera;
    
   
    //能力選択の状態
    
    int abilityChoice = -1;


    int previousTime = GetNowCount();

    //障害物の生成感覚
    float obstacleSpacing = 50.0f;
    //障害物初期生成位置
    float nextObstacleZ =100.0f;
    // ボス撃破後の敵・障害物の再開管理
    bool isHazardWaiting = false;

    // ボス撃破時のプレイヤー進行距離
    float bossDefeatedCourseZ = 0.0f;


    //Windowsから送られてくるメッセージを処理するためのDx関数
    while (ProcessMessage() == 0)
    {
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }
        // 経過時間を計算する（秒）
        int currentTime = GetNowCount();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // フレーム落ち時に極端な移動が起きないよう制限する
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        if (gameManager.GetGameState() == GameState::Playing)
        {

            // 更新前のボス進行状態を保存
            BossPhase previousPhase = bossManager.GetPhase();


            // ボスの出現・アニメーションを更新
            bossManager.Update(
                player.GetCourseZ(),
                deltaTime,
                courseManager
            );


            // 更新後のボス進行状態を取得
            BossPhase currentPhase = bossManager.GetPhase();
            // ボス撃破によってBattleからNormalに戻った瞬間
            if (previousPhase == BossPhase::Battle &&
                currentPhase == BossPhase::Normal)
            {
                isHazardWaiting = true;

                // 撃破時のプレイヤー位置を記録
                bossDefeatedCourseZ = player.GetCourseZ();

                // 敵と障害物を削除
                enemyManager.Clear();
                obstacleManager.Reset();
            }

            // NormalからWarningに切り替わった瞬間
            if (previousPhase == BossPhase::Normal &&
                currentPhase == BossPhase::Warning)
            {
                // 敵をすべて削除
                enemyManager.Clear();

                // 障害物をすべて削除
                obstacleManager.Reset();
            }


            // プレイヤーの入力・左右移動
            playerController.Update(player, deltaTime);

            // プレイヤーの進行位置に合わせてコースを追加生成
            courseManager.UpdateCourse(player.GetCourseZ());

            // プレイヤーの進行位置に合わせて古い制御点を削除 
            courseManager.RemoveOldPoints(player.GetCourseZ());

            // GameManagerにゲーム全体の更新を依頼
            bool canMoveForward =
                bossManager.GetPhase() != BossPhase::Battle;

            gameManager.Update(
                deltaTime,
                courseManager,
                canMoveForward
            );
            // ボス撃破後、30m進んだら敵と障害物の生成を再開
            if (isHazardWaiting &&
                player.GetCourseZ() >= bossDefeatedCourseZ + 30.0f)
            {
                isHazardWaiting = false;

                // 敵の生成位置をリセット
                enemyManager.ResetSpawnPosition(player.GetCourseZ());

                // 障害物の生成位置をリセット
                nextObstacleZ = player.GetCourseZ() + 100.0f;
            }

            
            // 通常ラン中だけ敵を自動生成する
            if (bossManager.GetPhase() == BossPhase::Normal &&
                !isHazardWaiting)
            {
                enemyManager.Update(
                    player.GetCourseZ(),
                    courseManager
                );
            }


            //毎フレーム、アニメーションの時間を進める
            player.UpdateAnimation(deltaTime);

            // ダメージ演出・無敵時間を更新
            player.UpdateDamageEffect(deltaTime);

            // プレイヤーの後方にある障害物を削除する
            obstacleManager.RemovePassedObstacles(player.GetCourseZ());


            // 通常ラン中、かつボス撃破後の待機が終了している場合だけ障害物を生成する
            if (bossManager.GetPhase() == BossPhase::Normal &&
                !isHazardWaiting)
            {
                // 次の障害物まで100m以内になったら生成する
                if (player.GetCourseZ() + 100.0f >= nextObstacleZ)
                {
                    obstacleManager.SpawnObstacle(
                        nextObstacleZ,
                        courseManager
                    );

                    nextObstacleZ += obstacleSpacing;
                }
            }

            // カメラを設定
            camera.Update(player, courseManager);
        }
        


        // マウスのボタン状態を取得
        int mouseInput = GetMouseInput();

        //左クリックを押した瞬間だけ true
        bool leftClickDown = (
            mouseInput & MOUSE_INPUT_LEFT) != 0 &&
            (previousMouseInput & MOUSE_INPUT_LEFT) == 0;


        bool clickConsumed = false;

        // タイトル画面のクリック処理
        if (gameManager.GetGameState() == GameState::Title && leftClickDown)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // リセット確認中
            if (rankingResetConfirm)
            {
                // YESボタン
                if (mouseX >= 1570 && mouseX <= 1680 &&
                    mouseY >= 970 && mouseY <= 1020)
                {
                    rankingManager.Clear();
                    rankingResetConfirm = false;
                }
                // NOボタン
                else if (mouseX >= 1710 && mouseX <= 1820 &&
                    mouseY >= 970 && mouseY <= 1020)
                {
                    rankingResetConfirm = false;
                }

                // 確認中のクリックをSTARTや射撃に使わせない
                clickConsumed = true;
            }
            else
            {
                // RESET RANKINGボタン
                if (mouseX >= 1660 && mouseX <= 1850 &&
                    mouseY >= 970 && mouseY <= 1020)
                {
                    rankingResetConfirm = true;
                    clickConsumed = true;
                }
                // STARTボタン
                else if (mouseX >= 810 && mouseX <= 1110 &&
                    mouseY >= 775 && mouseY <= 875)
                {
                    ResetGame(
                        gameManager,
                        playerController,
                        bossManager,
                        courseManager,
                        nextObstacleZ,
                        isHazardWaiting,
                        bossDefeatedCourseZ,
                        previousTime
                    );

                    clickConsumed = true;
                }
            }
        }

        // リザルト画面のボタン入力
        if (gameManager.GetGameState() == GameState::Result && leftClickDown)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // RETRYボタン
            if (mouseX >= 600 && mouseX <= 855 &&
                mouseY >= 825 && mouseY <= 930)
            {
                // コースを初期状態に戻す
                courseManager.Initialize();

                // ゲーム全体をリセットして再開
                ResetGame(
                    gameManager,
                    playerController,
                    bossManager,
                    courseManager,
                    nextObstacleZ,
                    isHazardWaiting,
                    bossDefeatedCourseZ,
                    previousTime
                );

                clickConsumed = true;
            }

            // TITLEボタン
            else if (mouseX >= 1065 && mouseX <= 1320 &&
                mouseY >= 825 && mouseY <= 930)
            {
                // コースを初期状態に戻す
                courseManager.Initialize();

                // ゲームデータをリセット
                gameManager.ResetRun(courseManager);

                // ボスの状態を初期化
                bossManager.Reset();

                // ボス撃破後の待機状態を初期化
                isHazardWaiting = false;

                // ボス撃破時の距離を初期化
                bossDefeatedCourseZ = 0.0f;

                // プレイヤーコントローラーもリセット
                playerController.Reset();

                // 障害物の生成位置をリセット
                nextObstacleZ = 50.0f;

                // 時間をリセット
                previousTime = GetNowCount();

                // タイトル画面に戻す
                gameManager.SetGameState(GameState::Title);

                //クリック入力
                clickConsumed = true;
            }
        }

      
        
        //能力選択画面（いずれ分離する予定）
        if (gameManager.GetGameState() == GameState::AbilitySelect && leftClickDown)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // 攻撃力カUP
            if (mouseX >= 560 && mouseX <= 820 &&
                mouseY >= 430 && mouseY <= 700)
            {
                abilityChoice = 1;
            }
            // 弾サイズUP
            else if (mouseX >= 840 && mouseX <= 1100 &&
                mouseY >= 430 && mouseY <= 700)
            {
                abilityChoice = 2;
            }
            // クールタイム短縮
            else if (mouseX >= 1120 && mouseX <= 1380 &&
                mouseY >= 430 && mouseY <= 700)
            {
                abilityChoice = 3;
            }

            //選択した能力選択を適用
            if (abilityChoice != -1)
            {
                if (abilityChoice == 1)
                {
                    player.SetAttackPower(
                        player.GetAttackPower() + 1
                    );
                }
                else if (abilityChoice == 2)
                {
                    player.SetBulletSize(
                        player.GetBulletSize() + 0.3f
                    );
                }
                else if (abilityChoice == 3)
                {
                    player.SetShotCooldown(
                        player.GetShotCooldown() - 0.5f
                    );
                }
                abilityChoice = -1;

                //能力選択画面終了
                gameManager.SetGameState(GameState::Playing);

                //選択クリックを射撃に適用させない
                clickConsumed = true;
            }
        }

        //これでボタンを押しっぱなしにしている間、何度も能力を選んでしまうことを防ぐ
        previousMouseInput = mouseInput;

        if (gameManager.GetGameState() == GameState::Playing)
        {
            // GameManagerに弾の更新と戦闘処理を依頼
            bool isLevelUp = gameManager.UpdateBulletsAndCombat(
                deltaTime,
                courseManager,
                bossManager
            );

            //レベルアップしたら能力選択画面へ
            if (isLevelUp)
            {
                gameManager.SetGameState(GameState::AbilitySelect);
            }
        }

        //障害物との衝突判定
        if (gameManager.GetGameState() == GameState::Playing)
        {
            // プレイヤーの当たり判定範囲
            float playerMinX = player.GetX() - 0.5f;
            float playerMaxX = player.GetX() + 0.5f;
            float playerMinY = 0.0f;
            float playerMaxY = 2.0f;
            float playerMinZ = player.GetZ() - 0.5f;
            float playerMaxZ = player.GetZ() + 0.5f;

            // すべての障害物をチェック
            for (int i = 0; i < obstacleManager.GetCount(); i++)
            {
                //i番目の障害物を取得
                Obstacle& obstacle = obstacleManager.GetObstacle(i);

                bool isColliding = IsCollidingAABB(
                    playerMinX, playerMaxX,
                    playerMinY, playerMaxY,
                    playerMinZ, playerMaxZ,
                    obstacle.minX, obstacle.maxX,
                    obstacle.minY, obstacle.maxY,
                    obstacle.minZ, obstacle.maxZ
                );

                if (isColliding)
                {
                    gameManager.SetGameState(GameState::GameOver);
                    break;
                }
            }
            // 敵との衝突判定
            if (gameManager.GetGameState() == GameState::Playing)
            {
                // すべての敵をチェック
                for (int i = 0; i < enemyManager.GetCount(); i++)
                {
                    // i番目の敵を取得
                    Enemy& enemy = enemyManager.GetEnemy(i);

                    // 倒されている敵は無視
                    if (enemy.IsDead())
                    {
                        continue;
                    }

                    // 敵の当たり判定範囲
                    float enemyMinX = enemy.x - 0.5f;
                    float enemyMaxX = enemy.x + 0.5f;

                    float enemyMinY = enemy.y + 1.0f;
                    float enemyMaxY = enemy.y + 3.0f;

                    float enemyMinZ = enemy.z - 0.5f;
                    float enemyMaxZ = enemy.z + 0.5f;

                    // プレイヤーと敵の衝突判定
                    bool isColliding = IsCollidingAABB(
                        playerMinX, playerMaxX,
                        playerMinY, playerMaxY,
                        playerMinZ, playerMaxZ,
                        enemyMinX, enemyMaxX,
                        enemyMinY, enemyMaxY,
                        enemyMinZ, enemyMaxZ
                    );

                    if (isColliding)
                    {
                        // プレイヤーに1ダメージ
                        player.TakeDamage(1);

                        // HPが0になったらゲームオーバー
                        if (player.GetHP() <= 0)
                        {
                            gameManager.SetGameState(GameState::GameOver);
                            break;
                        }
                    }
                }
            }
            // ボスのレーザーとの衝突判定
            if (gameManager.GetGameState() == GameState::Playing)
            {
                bool isLaserHit = bossManager.IsPlayerHitByLaser(
                    VGet(playerMinX, playerMinY, playerMinZ),
                    VGet(playerMaxX, playerMaxY, playerMaxZ)
                );

                if (isLaserHit)
                {
                    gameManager.SetGameState(GameState::GameOver);
                }
            }
        }
        
        // GameOverからResultへ遷移
        if (gameManager.GetGameState() == GameState::GameOver)
        {
            // リザルトデータを取得
            const ResultData& result = gameManager.GetResultData();

            // ランキングに登録
            rankingManager.AddRecord(result);

            // リザルト画面を初期化
            resultScene.Initialize(result);

            // リザルト画面へ移行
            gameManager.SetGameState(GameState::Result);
        }
       
        // 画面を消去する
        ClearDrawScreen();


        if (gameManager.GetGameState() == GameState::Title)
        {
            
            // 背景画像
            DrawExtendGraph(
                0, 0,1920, 1080,
                titleBackgroundHandle,
                TRUE
            );

            //STARTボタンの範囲
            const int buttonLeft = 810;
            const int buttonTop = 775;
            const int buttonRight = 1110;
            const int buttonBottom = 875;


   
            // マウス位置
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // ボタン上にマウスがあるか
            bool isMouseOverButton =
                mouseX >= buttonLeft && mouseX <= buttonRight &&
                mouseY >= buttonTop && mouseY <= buttonBottom;

            // ボタンの色を変更
            int buttonColor = isMouseOverButton
                ? GetColor(40, 150, 220)
                : GetColor(20, 80, 150);

            // STARTボタン描画
            DrawBox(
                buttonLeft, buttonTop,
                buttonRight, buttonBottom,
                buttonColor,
                TRUE
            );

            // START文字
            DrawExtendString(
                925, 815,
                1.1f, 1.1f,
                " START",
                GetColor(255, 255, 255)
            );

            // =====================================
            // RANKING
            // =====================================

            // ランキングパネル
            DrawBox(
                1370, 620, 1890, 1050,
                GetColor(15, 25, 45),
                TRUE
            );

            DrawBox(
                1370, 620, 1890, 1050,
                GetColor(80, 160, 200),
                FALSE
            );

            // ランキングタイトル
            DrawString(
                1540, 645,
                "  TOP 5 RANKING",
                GetColor(100, 220, 255)
            );

            // 列の見出し
            DrawString(1395, 700, "RANK", GetColor(150, 200, 230));
            DrawString(1470, 700, "ENEMY", GetColor(150, 200, 230));
            DrawString(1555, 700, "DISTANCE", GetColor(150, 200, 230));
            DrawString(1770, 700, "SCORE", GetColor(150, 200, 230));

            // ランキングデータを取得
            const std::vector<RankingRecord>& rankingRecords =
                rankingManager.GetRecords();

            // ランキングを表示
            for (int i = 0; i < static_cast<int>(rankingRecords.size()); i++)
            {
                const RankingRecord& record = rankingRecords[i];

                int drawY = 750 + i * 42;

                // 順位
                DrawFormatString(
                    1420, drawY,
                    GetColor(255, 220, 100),
                    "%d", i + 1
                );

                // 敵撃破数
                DrawFormatString(
                    1490, drawY,
                    GetColor(255, 255, 255),
                    "%d", record.defeatedEnemyCount
                );

                // 走行距離
                DrawFormatString(
                    1580, drawY,
                    GetColor(255, 255, 255),
                    "%.1f M", record.distance
                );

                // スコア
                DrawFormatString(
                    1780, drawY,
                    GetColor(100, 255, 180),
                    "%d", record.score
                );
            }
               
            if (rankingResetConfirm)
            {
                // 確認メッセージ
                DrawString(
                    1410, 925,
                    "CLEAR ALL RANKING?",
                    GetColor(255, 100, 100)
                );

                // YESボタンのホバー判定
                bool yesHover =
                    mouseX >= 1570 && mouseX <= 1680 &&
                    mouseY >= 970 && mouseY <= 1020;

                DrawBox(
                    1570, 970, 1680, 1020,
                    yesHover
                    ? GetColor(220, 60, 60)
                    : GetColor(150, 40, 40),
                    TRUE
                );

                DrawString(
                    1605, 986,
                    " YES",
                    GetColor(255, 255, 255)
                );

                // NOボタンのホバー判定
                bool noHover =
                    mouseX >= 1710 && mouseX <= 1820 &&
                    mouseY >= 970 && mouseY <= 1020;

                DrawBox(
                    1710, 970, 1820, 1020,
                    noHover
                    ? GetColor(70, 120, 180)
                    : GetColor(40, 70, 110),
                    TRUE
                );

                DrawString(
                    1750, 986,
                    "NO ",
                    GetColor(255, 255, 255)
                );
            }
            else
            {
                // リセットボタンのマウス位置
                int mouseX, mouseY;
                GetMousePoint(&mouseX, &mouseY);

                // リセットボタンのホバー判定
                bool resetHover =
                    mouseX >= 1660 && mouseX <= 1850 &&
                    mouseY >= 970 && mouseY <= 1020;


                DrawBox(
                    1660, 970, 1850, 1020,
                    resetHover
                    ? GetColor(180, 60, 60)
                    : GetColor(100, 35, 45),
                    TRUE
                );

                DrawString(
                    1680, 986,
                    "  RESET BUTTON",
                    GetColor(255, 255, 255)
                );
            }
                   
            
        }
        else if (gameManager.GetGameState() == GameState::Result)
        {
            // リザルト画面描画
            resultScene.Draw();
        }

        else
        {
            //ゲーム画面の描画処理
            //---------------------------------------


            // SkyBoxの描画
            // SkyBoxをカメラ位置に追従させる
            MV1SetPosition(skyboxHandle, camera.GetPosition());

            // SkyBoxはライティングの影響を受けない
            SetUseLighting(FALSE);

            // SkyBoxを描画
            MV1DrawModel(skyboxHandle);

            // ライティングを元に戻す
            SetUseLighting(TRUE);


            //---------------------------------------


            //これでMain.cppは前進処理の詳細を知らなくても、GameManagerに更新を依頼できる。
            //3Dオブジェクト
            gameManager.Draw();
            //障害物を配置
            obstacleManager.DrawAll();
            
            


            // Playing中の射撃処理
            if (gameManager.GetGameState() == GameState::Playing)
            {
                // START・RETRYで使用されていないクリックだけ射撃に使う
                if (leftClickDown && !clickConsumed)
                {
                    gameManager.TryShot(courseManager);
                }

                // クールタイム表示
                float cooldown = gameManager.GetShotCooldown();

                if (cooldown <= 0.0f)
                {
                    DrawString(
                        20, 95,
                        "SHOT: READY",
                        GetColor(0, 255, 0)
                    );
                }
                else
                {
                    DrawFormatString(
                        20, 95,
                        GetColor(255, 180, 0),
                        "SHOT: %.1f SEC",
                        cooldown
                    );
                }
            }


            // CourseManagerから制御点を取得
            const std::vector<CoursePoint>& points =
                courseManager.GetControlPoints();

            // ボスHPバー
            if (bossManager.IsBossActive())
            {
                // 現在HPと最大HPを取得
                int currentHP = bossManager.GetBossHP();
                int maxHP = bossManager.GetBossMaxHP();

                // HPの割合を計算
                float hpRate = 0.0f;

                if (maxHP > 0)
                {
                    hpRate =
                        static_cast<float>(currentHP) / maxHP;
                }

                // HP割合を0～1の範囲に制限
                if (hpRate < 0.0f)
                {
                    hpRate = 0.0f;
                }
                else if (hpRate > 1.0f)
                {
                    hpRate = 1.0f;
                }

                // HPバーの位置とサイズ
                const int barX = 660;
                const int barY = 75;
                const int barWidth = 600;
                const int barHeight = 26;


                // 現在のHPに応じたバーの長さ
                int currentWidth =
                    static_cast<int>(barWidth * hpRate);

                // ボス名
                DrawFormatString(
                    barX,
                    barY - 25,
                    GetColor(255, 255, 255),
                    "TITAN-01"
                );

                // HPバーの背景
                DrawBox(
                    barX,
                    barY,
                    barX + barWidth,
                    barY + barHeight,
                    GetColor(40, 40, 40),
                    TRUE
                );

                // 現在HPのバー
                if (currentWidth > 0)
                {
                    DrawBox(
                        barX,
                        barY,
                        barX + currentWidth,
                        barY + barHeight,
                        GetColor(220, 35, 45),
                        TRUE
                    );
                }

                // HPバーの枠線
                DrawBox(
                    barX,
                    barY,
                    barX + barWidth,
                    barY + barHeight,
                    GetColor(255, 255, 255),
                    FALSE
                );

                // HP数値
                DrawFormatString(
                    barX + barWidth + 15,
                    barY,
                    GetColor(255, 255, 255),
                    "%d / %d",
                    currentHP,
                    maxHP
                );
            }

            // コースの最後の制御点を確認
            /*DrawFormatString(
                20, 160,
                GetColor(255, 255, 255),
                "Last Control Z: %.2f",
                points.back().z
            );*/

            //---------------------------------------

            // スプラインに沿った道路を描画する
            const float roadWidth = 10.0f;
            const float roadHeight = 0.01f;
            const float segmentLength = 2.0f;

            const int roadColor = GetColor(80, 80, 80);

            // 描画するZ座標の範囲
            // スプラインが正常に計算できる範囲
            float minCourseZ = points[1].z;
            float maxCourseZ = points[points.size() - 2].z;

            // 道路の描画範囲
            float startZ = floorf(
                (player.GetCourseZ() - 100.0f) / segmentLength
            ) * segmentLength;

            float endZ = player.GetCourseZ() + 200.0f;

            // 有効な範囲に制限
            if (startZ < minCourseZ)
            {
                startZ = minCourseZ;
            }

            if (endZ > maxCourseZ)
            {
                endZ = maxCourseZ;
            }

            // Z方向に一定間隔で道路を分割
            for (float z = startZ; z < endZ; z += segmentLength)
            {
                float nextZ = z + segmentLength;

                // 手前側と奥側のコース中心を取得
                CoursePoint centerNear = courseManager.GetPosition(z);
                CoursePoint centerFar = courseManager.GetPosition(nextZ);

                // それぞれの地点の右方向を取得
                CoursePoint rightNear = courseManager.GetRight(z);
                CoursePoint rightFar = courseManager.GetRight(nextZ);

                // 手前側の左端
                VECTOR leftNear = VGet(
                    centerNear.x - rightNear.x * roadWidth,
                    roadHeight,
                    centerNear.z - rightNear.z * roadWidth
                );

                // 手前側の右端
                VECTOR rightNearPos = VGet(
                    centerNear.x + rightNear.x * roadWidth,
                    roadHeight,
                    centerNear.z + rightNear.z * roadWidth
                );

                // 奥側の左端
                VECTOR leftFar = VGet(
                    centerFar.x - rightFar.x * roadWidth,
                    roadHeight,
                    centerFar.z - rightFar.z * roadWidth
                );

                // 奥側の右端
                VECTOR rightFarPos = VGet(
                    centerFar.x + rightFar.x * roadWidth,
                    roadHeight,
                    centerFar.z + rightFar.z * roadWidth
                );

                // 三角形その1
                DrawTriangle3D(
                    leftNear,
                    rightNearPos,
                    leftFar,
                    roadColor,
                    TRUE
                );

                // 三角形その2
                DrawTriangle3D(
                    rightNearPos,
                    rightFarPos,
                    leftFar,
                    roadColor,
                    TRUE
                );

                //道路の左右にネオンラインを描画
                DrawLine3D(leftNear, leftFar, GetColor(0, 255, 255));
                DrawLine3D(rightNearPos, rightFarPos, GetColor(0, 255, 255));
            }

            //道路描画の後に
            //ボスの描画
            bossManager.Draw();
            // 弾描画
            bulletManager.DrawAll();


            //-----------------------------------------

            //敵の撃破数を表示
            DrawFormatString(
                20, 20,
                GetColor(255, 255, 255),
                "Defeated: %d",
                progress.GetDefeatedEnemyCount()
            );

            DrawFormatString(
                220, 20,
                GetColor(255, 255, 255),
                " %.0f M",
                player.GetCourseZ()
            );

            //経験値表示
            // プレイヤーのレベルを表示
            DrawFormatString(
                20, 45,
                GetColor(0, 255, 255),
                "Level: %d",
                progress.GetPlayerLevel()
            );

            // 累計経験値と次の目標を表示
            DrawFormatString(
                20, 70,
                GetColor(255, 255, 0),
                "EXP: %d / %d",
                progress.GetExperience(),
                progress.GetNextLevelExp()
            );

            // 現在HPと最大HPを取得
            int currentHP = player.GetHP();
            int maxHP = player.GetMaxHP();

            // HPの割合を計算
            float hpRate = 0.0f;

            if (maxHP > 0)
            {
                hpRate = static_cast<float>(currentHP) / maxHP;
            }

            // HP割合を0～1に制限
            if (hpRate < 0.0f)
            {
                hpRate = 0.0f;
            }
            else if (hpRate > 1.0f)
            {
                hpRate = 1.0f;
            }

            // HPバーの位置とサイズ
            const int barX = 20;
            const int barY = 145;
            const int barWidth = 200;
            const int barHeight = 18;

            // 現在HPに応じたバーの長さ
            int currentWidth =
                static_cast<int>(barWidth * hpRate);

            // HPに応じて色を変更
            int hpColor;

            if (hpRate > 0.6f)
            {
                hpColor = GetColor(40, 220, 100);
            }
            else if (hpRate > 0.3f)
            {
                hpColor = GetColor(255, 200, 40);
            }
            else
            {
                hpColor = GetColor(230, 50, 50);
            }

            // HP表示
            DrawFormatString(
                barX,
                barY - 22,
                GetColor(255, 255, 255),
                "HP: %d / %d",
                currentHP,
                maxHP
            );

            // HPバーの背景
            DrawBox(
                barX,
                barY,
                barX + barWidth,
                barY + barHeight,
                GetColor(40, 40, 40),
                TRUE
            );

            // 現在HPのバー
            if (currentWidth > 0)
            {
                DrawBox(
                    barX,
                    barY,
                    barX + currentWidth,
                    barY + barHeight,
                    hpColor,
                    TRUE
                );
            }

            // HPバーの枠線
            DrawBox(
                barX,
                barY,
                barX + barWidth,
                barY + barHeight,
                GetColor(255, 255, 255),
                FALSE
            );

            

            if (gameManager.GetGameState() == GameState::AbilitySelect)
            {
                

                // 選択画面のパネル
                DrawBox(
                    480, 250, 1440, 820,
                    GetColor(20, 25, 45),
                    TRUE
                );

                // パネルの枠線
                DrawBox(
                    480, 250, 1440, 820,
                    GetColor(80, 160, 200),
                    FALSE
                );

                // タイトル
                DrawString(
                    800, 320,
                    "              LEVEL UP!",
                    GetColor(100, 220, 255)
                );
     
               
                // ATTACK CARD
                DrawBox(
                    560, 430, 820, 700,
                    GetColor(100, 45, 55),
                    TRUE
                );

                DrawBox(
                    560, 430, 820, 700,
                    GetColor(220, 90, 100),
                    FALSE
                );

                DrawString(
                    620, 500,
                    "ATTACK",
                    GetColor(255, 180, 180)
                );
                DrawString(
                    620, 560,
                    "POWER +1",
                    GetColor(255, 255, 255)
                );
                // BULLET SIZE CARD
                DrawBox(
                    840, 430, 1100, 700,
                    GetColor(35, 85, 60),
                    TRUE
                );
                DrawBox(
                    840, 430, 1100, 700,
                    GetColor(100, 220, 140),
                    FALSE
                );
                DrawString(
                    900, 500,
                    "BULLET",
                    GetColor(160, 255, 190)
                );

                DrawString(
                    900, 560,
                    "SIZE +0.3",
                    GetColor(255, 255, 255)
                );
                // COOLDOWN CARD
                DrawBox(
                    1120, 430, 1380, 700,
                    GetColor(35, 60, 100),
                    TRUE
                );

                DrawBox(
                    1120, 430, 1380, 700,
                    GetColor(100, 150, 230),
                    FALSE
                );

                DrawString(
                    1170, 500,
                    "COOLDOWN",
                    GetColor(170, 210, 255)
                );
                DrawString(
                    1170, 560,
                    "-0.5 SEC",
                    GetColor(255, 255, 255)
                );
            }


            
        }

        // 描画結果を画面に反映
        ScreenFlip();

    }
    
    //読み込んだ画像を(メモリ）解放
    // タイトル 背景画像
    DeleteGraph(titleBackgroundHandle);

    //SkyBoxのテクスチャ
    DeleteGraph(skyboxTexture);

    //SkyBoxモデル
    MV1DeleteModel(skyboxHandle);

    // 障害物モデルを解放
    obstacleManager.ReleaseModel();

    DxLib_End();

    return 0;
}
