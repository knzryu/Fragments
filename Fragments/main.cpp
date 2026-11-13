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
#include"CombatManager.h"
#include "PlayerController.h"
#include "Camera.h"

// ゲームの状態をリセットする関数
void ResetGame(
    GameManager& gameManager,
    PlayerController& playerController,
    const CourseManager& courseManager,
    float& nextObstacleZ,
    bool& gameOver,
    int& previousTime)
{
    //ゲーム全体を初期状態に戻す
    gameManager.ResetRun(courseManager);

    // ダッシュの状態を初期化
    playerController.Reset();

    // 次の障害物の生成位置を初期化
    nextObstacleZ = 50.0f;

    // ゲームオーバーを解除
    gameOver = false;

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
    SetGraphMode(1280, 720, 32);

    //Dxlib初期化
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    //----------------------------------------------------------
    // Skyboxモデルの読み込み
    int skyboxHandle = MV1LoadModel(
        "Resource/Model/SkyBox.mv1"
    );

    if (skyboxHandle == -1)
    {
        MessageBox(NULL, "Skybox Load Failed",
            "Error", MB_OK);
        DxLib_End();
        return -1;
    }

    // テクスチャの読み込み
    int skyboxTexture = LoadGraph(
        "Resource/Model/Tx.png"
    );

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

	// BulletManagerを取得 
    BulletManager& bulletManager = gameManager.GetBulletManager();

    PlayerController playerController;

    // 戦闘管理オブジェクト
    CombatManager combatManager;

    Camera camera;
    
   
    //能力選択の状態
    bool isSelectingAbility = false;
    int abilityChoice = -1;


    int previousTime = GetNowCount();

 

    float obstacleSpacing = 25.0f;
    float nextObstacleZ = 50.0f;

	// ゲームオーバー状態を管理する変数
    bool gameOver = false;

	//  左クリックの押下状態を記録する変数
    bool wasLeftMousePressed = false;

    ResetGame(
        gameManager,
        playerController,
        courseManager,
        nextObstacleZ,
        gameOver,
        previousTime
    );

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

        
        gameManager.Update(deltaTime, courseManager);

        playerController.Update(player, deltaTime);

        // プレイヤーの進行位置に合わせてコースを追加生成
        courseManager.UpdateCourse(player.GetCourseZ());

		// プレイヤーの進行位置に合わせて古い制御点を削除 
        courseManager.RemoveOldPoints(player.GetCourseZ());

        // 横移動を反映してワールド座標を更新
        //これがないと、横移動したのにxとzが前の位置のままになってしまう。
        player.UpdateCoursePosition(courseManager);

        //毎フレーム、アニメーションの時間を進める
        player.UpdateAnimation(deltaTime);

        // プレイヤーの後方にある障害物を削除する
        obstacleManager.RemovePassedObstacles(
            player.GetCourseZ()
        );

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


        // マウスのボタン状態を取得
        int mouseInput = GetMouseInput();

        // 左クリックで弾を発射
        if (mouseInput & MOUSE_INPUT_LEFT)
        {
            // 左クリックした瞬間だけ発射
            if (!wasLeftMousePressed)
            {
                bulletManager.SpawnBullet(
                    player.GetCourseZ() + 1.0f,
                    player.GetLaneOffset(),
                    1.0f,
                    courseManager
                );
            }

			// 左クリックが押されている状態を記録
            wasLeftMousePressed = true;
        }
        else
        {
            // 左クリックを離したら、次の発射を許可
            wasLeftMousePressed = false;
        }

        
        //左クリックを押した瞬間だけ true
        bool leftClickDown = (
            mouseInput & MOUSE_INPUT_LEFT) != 0 &&
            (previousMouseInput & MOUSE_INPUT_LEFT) == 0;

        //能力選択画面
        if (isSelectingAbility && leftClickDown)
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
                isSelectingAbility = false;
            }
        }

        //これでボタンを押しっぱなしにしている間、何度も能力を選んでしまうことを防ぐ
        previousMouseInput = mouseInput;

       

        // 弾の位置を更新
        bulletManager.UpdateAll(deltaTime, courseManager);

        //戦闘処理をCombatManagerに依頼
        bool isLevelUp = combatManager.Update(
            enemyManager,
            bulletManager,
            progress
        );

        //レベルアップしたら能力選択画面へ
        if (isLevelUp)
        {
            isSelectingAbility = true;
        }


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
                gameOver = true;
                break;
            }
        }

       
     
       
        // 画面を消去する
        ClearDrawScreen();

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

        // CourseManagerから制御点を取得
        const std::vector<CoursePoint>& points =
            courseManager.GetControlPoints();

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
      
        //敵を描画
       // 生存している敵をすべて描画
		enemyManager.DrawAll();

        // 敵の数を確認
        DrawFormatString(
            20, 95,
            GetColor(255, 255, 0),
            "Enemy Count: %d",
            enemyManager.GetCount()
        );

        // 各敵の座標とHPを確認
        for (int i = 0; i < enemyManager.GetCount(); i++)
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
        }

        //障害物を配置
		obstacleManager.DrawAll();
        
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

        if (isSelectingAbility)
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

        // 衝突したらゲームオーバーを表示
        if (gameOver)
        {
            DrawString(
                600, 400,
                "GAME OVER",
                GetColor(255, 0, 0)
            );
        }
       
        bulletManager.DrawAll();

        // 描画結果を画面に反映
        ScreenFlip();

        // ゲームオーバーならキー入力を待つ
        if (gameOver)
        {
            int input = WaitKey();

            // Escapeならゲームを終了
            if (input == KEY_INPUT_ESCAPE)
            {
                break;
            }

            // それ以外のキーなら再スタート

            // コースを初期状態に戻す
            courseManager.Initialize();

            ResetGame(
                gameManager,
                playerController,
                courseManager,
                nextObstacleZ,
                gameOver,
                previousTime
            );

            continue;
        }

    }
    
    DxLib_End();

    return 0;
}
