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

// ゲームの状態をリセットする関数
void ResetGame(
    GameManager& gameManager,
    PlayerController& playerController,
    const CourseManager& courseManager,
    float& nextObstacleZ,
    int& previousTime)
{
    //ゲーム全体を初期状態に戻す
    gameManager.ResetRun(courseManager);

    //ゲーム状態をPlayingに戻す
    gameManager.SetGameState(GameState::Playing);

    // ダッシュの状態を初期化
    playerController.Reset();

    // 次の障害物の生成位置を初期化
    nextObstacleZ = 50.0f;


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

    

    //プレイヤークラスを参照
    Player& player = gameManager.GetPlayer();

    //プレイヤーの成長・進行状況を参照
    PlayerProgress& progress = gameManager.GetPlayerProgress();

    //障害物管理オブジェクトを参照
    ObstacleManager& obstacleManager = gameManager.GetObstacleManager();

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

 
    float obstacleSpacing = 25.0f;
    float nextObstacleZ = 50.0f;



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
            // ボスの出現・アニメーションを更新
            bossManager.Update(
                player.GetCourseZ(),
                deltaTime,
                courseManager
            );

            // ボスが生存していなければ前進可能
            bool canMoveForward = !bossManager.IsBossActive();

            // ゲーム全体を更新
            gameManager.Update(
                deltaTime,
                courseManager,
                canMoveForward
            );

            // プレイヤーの入力・左右移動
            playerController.Update(player, deltaTime);

            // プレイヤーの進行位置に合わせてコースを追加生成
            courseManager.UpdateCourse(player.GetCourseZ());

            // プレイヤーの進行位置に合わせて古い制御点を削除 
            courseManager.RemoveOldPoints(player.GetCourseZ());

            // 横移動を反映してワールド座標を更新
            //これがないと、横移動したのにxとzが前の位置のままになってしまう。
            player.UpdateCoursePosition(courseManager);

            // 敵の自動生成を更新
            enemyManager.Update(player.GetCourseZ(), courseManager);

            //毎フレーム、アニメーションの時間を進める
            player.UpdateAnimation(deltaTime);

            // プレイヤーの後方にある障害物を削除する
            obstacleManager.RemovePassedObstacles(player.GetCourseZ());


            // 次の障害物まで20以内になったら生成する
            if (player.GetCourseZ() + 20.0f >= nextObstacleZ)
            {
                // スプラインから生成位置を取得
                obstacleManager.SpawnObstacle(
                    nextObstacleZ,
                    courseManager
                );

                // 次の生成位置を更新
                nextObstacleZ += obstacleSpacing;
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

        //タイトル画面のSTARTボタン
        if (gameManager.GetGameState() == GameState::Title)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            //STARTボタンの範囲
            bool isMouseOverButton =
                mouseX >= 810 && mouseX <= 1110 &&
                mouseY >= 700 && mouseY <= 800;

            //ボタン内で左クリックした瞬間
            if (isMouseOverButton && leftClickDown)
            {
                ResetGame(
                    gameManager,
                    playerController,
                    courseManager,
                    nextObstacleZ,
                    previousTime
                );

                clickConsumed = true;
            }
        }

        // リザルト画面のボタン入力
        if (gameManager.GetGameState() == GameState::Result && leftClickDown)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // RETRYボタン
            if (mouseX >= 700 && mouseX <= 950 &&
                mouseY >= 900 && mouseY <= 980)
            {
                // コースを初期状態に戻す
                courseManager.Initialize();

                // ゲーム全体をリセットして再開
                ResetGame(
                    gameManager,
                    playerController,
                    courseManager,
                    nextObstacleZ,
                    previousTime
                );

                clickConsumed = true;
            }

            // TITLEボタン
            else if (mouseX >= 970 && mouseX <= 1220 &&
                mouseY >= 900 && mouseY <= 980)
            {
                // コースを初期状態に戻す
                courseManager.Initialize();

                // ゲームデータをリセット
                gameManager.ResetRun(courseManager);

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

      
        

        //能力選択画面
        if (gameManager.GetGameState() == GameState::AbilitySelect && leftClickDown)
        {
            int mouseX, mouseY;
            GetMousePoint(&mouseX, &mouseY);

            // 攻撃力カード
            if (mouseX >= 300 && mouseX <= 500 &&
                mouseY >= 280 && mouseY <= 480)
            {
                abilityChoice = 1;
            }
            // 移動速度カード
            else if (mouseX >= 540 && mouseX <= 740 &&
                mouseY >= 280 && mouseY <= 480)
            {
                abilityChoice = 2;
            }
            // 弾速カード
            else if (mouseX >= 780 && mouseX <= 980 &&
                mouseY >= 280 && mouseY <= 480)
            {
                abilityChoice = 3;
            }

            if (abilityChoice != -1)
            {
                if (abilityChoice == 1)
                {
                    player.SetAttackPower(player.GetAttackPower() + 1);
                }
                else if (abilityChoice == 2)
                {
                    player.SetForwardSpeed(12.0f);
                }
                else if (abilityChoice == 3)
                {
                    player.SetBulletSpeed(player.GetBulletSpeed() + 10.0f);
                }

                abilityChoice = -1;
                gameManager.SetGameState(GameState::Playing);
            }
        }

        //これでボタンを押しっぱなしにしている間、何度も能力を選んでしまうことを防ぐ
        previousMouseInput = mouseInput;

        if (gameManager.GetGameState() == GameState::Playing)
        {
            // GameManagerに弾の更新と戦闘処理を依頼
            bool isLevelUp = gameManager.UpdateBulletsAndCombat(
                deltaTime,
                courseManager
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
        }
        
        // GameOverからResultへ遷移
        if (gameManager.GetGameState() == GameState::GameOver)
        {
            // 保存された結果データをリザルト画面に渡す
            resultScene.Initialize(gameManager.GetResultData());

            // ゲームステートを変更
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
            const int buttonTop = 700;
            const int buttonRight = 1110;
            const int buttonBottom = 800;

   
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

            // STARTボタン
            DrawBox(
                buttonLeft, buttonTop,
                buttonRight, buttonBottom,
                buttonColor,
                TRUE
            );

            DrawString(
                925, 740,
                " START",
                GetColor(255, 255, 255)
            );
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
            gameManager.Draw();

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

            /*/ 制御点のX座標を確認するデバッグ表示
            for (int i = 0; i < (int)points.size() && i < 12; i++)
            {
                DrawFormatString(
                    20,
                    200 + i * 20,
                    GetColor(255, 255, 0),
                    "Point %d : X = %.3f Z = %.1f",
                    i,
                    points[i].x,
                    points[i].z
                );
            }
            /*/
            // すべてのスプライン区間を描画
            for (int i = 0; i < (int)points.size() - 3; i++)
            {
                for (int j = 0; j < 100; j++)
                {
                    float t0 = j / 100.0f;
                    float t1 = (j + 1) / 100.0f;

                    CoursePoint a = Spline::CatmullRom(
                        points[i],
                        points[i + 1],
                        points[i + 2],
                        points[i + 3],
                        t0
                    );

                    CoursePoint b = Spline::CatmullRom(
                        points[i],
                        points[i + 1],
                        points[i + 2],
                        points[i + 3],
                        t1
                    );

                    DrawLine3D(
                        VGet(a.x, 0.1f, a.z),
                        VGet(b.x, 0.1f, b.z),
                        GetColor(0, 255, 255)
                    );
                }
            }

            //デバック
            // 各敵の座標とHPを確認
            /*/for (int i = 0; i < enemyManager.GetCount(); i++)
            {
                Enemy& enemy = enemyManager.GetEnemy(i);

                DrawFormatString(
                    20, 120 + i * 20,
                    GetColor(255, 255, 0),
                    "Enemy %d: CourseZ=%.2f Offset=%.2f X=%.2f Z=%.2f",
                    i,
                    enemy.courseZ,
                    enemy.laneOffset,
                    enemy.x,
                    enemy.z
                );
            }/*/

            //障害物を配置
            obstacleManager.DrawAll();

            //敵を描画
            enemyManager.DrawAll();

            // ボスの描画
            bossManager.Draw();

            // コースの最後の制御点を確認


            DrawFormatString(
                20, 160,
                GetColor(255, 255, 255),
                "Last Control Z: %.2f",
                points.back().z
            );

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
                //DrawLine3D(leftNear, leftFar, GetColor(0, 255, 255));
                //DrawLine3D(rightNearPos, rightFarPos, GetColor(0, 255, 255));
            }

            //-----------------------------------------

            //敵の撃破数を表示
            DrawFormatString(
                20, 20,
                GetColor(255, 255, 255),
                "Defeated: %d",
                progress.GetDefeatedEnemyCount()
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

            if (gameManager.GetGameState() == GameState::AbilitySelect)
            {
                // 暗い背景
                DrawBox(
                    250, 180, 1030, 530,
                    GetColor(20, 20, 40), TRUE
                );

                DrawFormatString(
                    440, 215,
                    GetColor(255, 255, 255),
                    "LEVEL UP! Choose an ability"
                );
                // 攻撃力カード
                DrawBox(
                    300, 280, 500, 480,
                    GetColor(100, 45, 55), TRUE
                );
                DrawFormatString(
                    330, 330,
                    GetColor(255, 180, 180),
                    "ATTACK +1"
                );

                // 移動速度カード
                DrawBox(
                    540, 280, 740, 480,
                    GetColor(35, 85, 60), TRUE
                );
                DrawFormatString(
                    565, 330,
                    GetColor(160, 255, 190),
                    "SPEED +2"
                );

                // 弾速カード
                DrawBox(
                    780, 280, 980, 480,
                    GetColor(35, 60, 100), TRUE
                );
                DrawFormatString(
                    810, 330,
                    GetColor(170, 210, 255),
                    "BULLET +10"
                );
            }


            bulletManager.DrawAll();
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


    DxLib_End();

    return 0;
}
