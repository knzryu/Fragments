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

// ゲームの状態をリセットする関数
void ResetGame(
    GameManager& gameManager,
    float& nextObstacleZ,
    bool& gameOver,
    bool& isDashing,
    bool& dashUsed,
    float& dashTimer,
    int& dashDirection,
    int& previousTime)
{
    //ゲーム全体を初期状態に戻す
    gameManager.ResetRun();

    // ダッシュの状態を初期化
    isDashing = false;
    dashUsed = false;
    dashTimer = 0.0f;
    dashDirection = 0;

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

    // マウスカーソルを表示
    SetMouseDispFlag(TRUE);

    // 3Dの奥行き判定を有効にする
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // カメラに映す奥行きの範囲
    SetCameraNearFar(0.1f, 1000.0f);

    //プレイヤーの移動
    GameManager gameManager;
    gameManager.Initialize();

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

   
   
    //能力選択の状態
    bool isSelectingAbility = false;
    int abilityChoice = -1;


    int previousTime = GetNowCount();

    // ダッシュの設定
    float dashSpeed = 20.0f;       // ダッシュ中の移動量（1フレームあたり）
    float dashDuration = 0.4f;    // ダッシュの持続時間（秒）
    float dashTimer = 0.0f;       // ダッシュの経過時間

    bool isDashing = false;
    bool dashUsed = false;
    int dashDirection = 0;

    float obstacleSpacing = 10.0f;
    float nextObstacleZ = 50.0f;

	// ゲームオーバー状態を管理する変数
    bool gameOver = false;

	//  左クリックの押下状態を記録する変数
    bool wasLeftMousePressed = false;

    ResetGame(
        gameManager,
        nextObstacleZ,
        gameOver,
        isDashing,
        dashUsed,
        dashTimer,
        dashDirection,
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

        // プレイヤーを自動前進させる
        gameManager.Update(deltaTime);

        // プレイヤーの後方にある障害物を削除する
        obstacleManager.RemovePassedObstacles( player.GetZ());

        // 次の障害物まで20以内になったら生成する
        if (player.GetZ() + 20.0f >= nextObstacleZ)
        {
            obstacleManager.SpawnObstacle( nextObstacleZ);

            // 次の生成位置を更新する
            nextObstacleZ += obstacleSpacing;
        }

        // ダッシュ開始
        if (CheckHitKey(KEY_INPUT_SPACE) && !dashUsed)
        {
            if (CheckHitKey(KEY_INPUT_A))
            {
                dashDirection = -1;
                isDashing = true;
                dashTimer = 0.0f;
                dashUsed = true;
            }
            else if (CheckHitKey(KEY_INPUT_D))
            {
                dashDirection = 1;
                isDashing = true;
                dashTimer = 0.0f;
                dashUsed = true;
            }
        }

        // Spaceを離したら、次のダッシュを許可する
        if (!CheckHitKey(KEY_INPUT_SPACE))
        {
            dashUsed = false;
        }

        // ダッシュ中の処理
        if (isDashing)
        {
            // Spaceを離したら即終了
            if (!CheckHitKey(KEY_INPUT_SPACE))
            {
                isDashing = false;
            }
            else
            {
                player.SetX(
                    player.GetX() + dashDirection * dashSpeed * deltaTime
                );

                // 0.4秒経過したら終了
                if (dashTimer >= dashDuration)
                {
                    isDashing = false;
                }
            }
        }
        else
        {
            // 通常の左右移動
            if (CheckHitKey(KEY_INPUT_A))
            {
                player.MoveLeft(deltaTime);
            }

            if (CheckHitKey(KEY_INPUT_D))
            {
                player.MoveRight(deltaTime);
            }
        }
        // マウスのボタン状態を取得
        int mouseInput = GetMouseInput();

        // 左クリックで弾を発射
        if (mouseInput & MOUSE_INPUT_LEFT)
        {
            // 左クリックした瞬間だけ発射
            if (!wasLeftMousePressed)
            {
                bulletManager.SpawnBullet(
                    player.GetX(),
                    1.0f,
                    player.GetZ() + 1.0f
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

        // コースから外れないようにする
        if (player.GetX() < -8.0f)
        {
            player.SetX(-8.0f);
        }

        if (player.GetX() > 8.0f)
        {
			player.SetX(8.0f);
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

        // 敵が生存している場合だけ、弾との当たり判定を行う
        // すべての敵と弾の当たり判定を行う
        for (int e = 0; e < enemyManager.GetCount(); e++)
        {
		
            Enemy& enemy = enemyManager.GetEnemy(e);
            // 倒された敵は判定しない
            if (enemy.hp <= 0)
            {
                continue;
            }

            // 敵の当たり判定範囲
            float enemyMinX = enemy.x - 0.5f;
            float enemyMaxX = enemy.x + 0.5f;
            float enemyMinY = enemy.y;
            float enemyMaxY = enemy.y + 2.0f;
            float enemyMinZ = enemy.z - 0.5f;
            float enemyMaxZ = enemy.z + 0.5f;

            // すべての弾を後ろから確認する
            for (int i = bulletManager.GetCount() - 1; i >= 0; i--)
            {

                // 無効な弾は判定しない 
                if (!bulletManager.GetBullet(i).active)
                {
                    continue;
                }

                // 弾の当たり判定範囲
                float bulletMinX = bulletManager.GetBullet(i).x - 0.15f;
                float bulletMaxX = bulletManager.GetBullet(i).x + 0.15f;
                float bulletMinY = bulletManager.GetBullet(i).y - 0.15f;
                float bulletMaxY = bulletManager.GetBullet(i).y + 0.15f;
                float bulletMinZ = bulletManager.GetBullet(i).z - 0.15f;
                float bulletMaxZ = bulletManager.GetBullet(i).z + 0.15f;

                // 敵と弾が重なっているか
                bool isHit = IsCollidingAABB(
                    bulletMinX, bulletMaxX,
                    bulletMinY, bulletMaxY,
                    bulletMinZ, bulletMaxZ,
                    enemyMinX, enemyMaxX,
                    enemyMinY, enemyMaxY,
                    enemyMinZ, enemyMaxZ
                );

                if (isHit)
                {
                    //ダメージを与える前のHPを記録
                    int previousHP = enemy.hp;

                    //1ダメージを与える
                    enemy.TakeDamage(1);
                    
                    //HPが正の値から0になった瞬間だけ撃破数を増やす
                    if (previousHP > 0 && enemy.hp == 0)
                    {
                        // レベルアップ前のレベルを記録
                        int previousLevel = progress.GetPlayerLevel();

                        // 撃破数を増やす
                        progress.AddDefeatedEnemyCount();

                        // 経験値を獲得
                        progress.AddExperience(10);

                        // レベルが上がったか確認
                        if (progress.GetPlayerLevel() > previousLevel)
                        {
                            isSelectingAbility = true;
                        }
                    }
                    // 命中した弾を削除
                    bulletManager.GetBullet(i).active = false;
                }
            }
        }

        bulletManager.RemoveInactiveBullets();

        // カメラの位置と注視点を計算する
        VECTOR cameraPos =
            VGet(player.GetX(), 5.0f, player.GetZ() - 10.0f);

        VECTOR cameraTarget =
            VGet(player.GetX(), 1.0f, player.GetZ() + 5.0f);

        // カメラを設定する
        SetCameraPositionAndTarget_UpVecY(
            cameraPos,
            cameraTarget
        );

        // 画面を消去する
        ClearDrawScreen();

        //これでMain.cppは前進処理の詳細を知らなくても、GameManagerに更新を依頼できる。
        gameManager.Draw();

      
        //敵を描画
       // 生存している敵をすべて描画
		enemyManager.DrawAll();

        //障害物を配置
		obstacleManager.DrawAll();
        

        // 地面を描画する
        DrawCube3D(
            VGet(-20.0f, -0.5f, -100.0f),
            VGet(20.0f, 0.0f, 1000.0f),
            GetColor(80, 80, 80),
            GetColor(80, 80, 80),
            TRUE
        );

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
        
        bulletManager.UpdateAll(deltaTime);
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
            ResetGame(
                gameManager,
                nextObstacleZ,
                gameOver,
                isDashing,
                dashUsed,
                dashTimer,
                dashDirection,
                previousTime
            );

            continue;
        }

    }
    
    DxLib_End();

    return 0;
}
