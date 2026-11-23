#pragma once

class CourseManager;

// ボスの行動状態
enum class BossAction
{
    Idle,       // 待機
    Warning,    // 攻撃予兆
    Attacking,  // 攻撃中
    Recovery,   // 攻撃後の硬直
    Dead        // 撃破
};


class Boss
{
private:

	
    // ボスの初期HP
    static constexpr int INITIAL_HP = 1;

    int hp = INITIAL_HP;

	bool active = false;


    // 現在の行動状態
    BossAction action = BossAction::Idle;

    // 現在の行動状態が続いた時間
    float actionTimer = 0.0f;

	//座標
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;


    // ボスのコース上の位置
    float bossCourseZ = 0.0f;

    // 現在のレーンオフセット
    float laneOffset = 0.0f;

    // 移動先のレーンオフセット
    float targetLaneOffset = 0.0f;

    // 現在のレーン番号（0:左、1:中央、2:右）
    int currentLane = 1;

    // レーン移動速度
    float laneMoveSpeed = 4.0f;


    // モデルハンドル
    int modelHandle = -1;

    // アニメーションのアタッチ番号
    int animAttachIndex = -1;

    // アニメーションの総再生時間
    float animTotalTime = 0.0f;

    // 現在の再生時間
    float animTime = 0.0f;


    


public:

    // モデルとアニメーションの読み込み
    bool LoadModel();

    // モデルとアニメーションの解放
    void ReleaseModel();

    // ボスの更新
    void Update(float deltaTime, const CourseManager& courseManager);

    // ボスの描画
    void Draw();


    // 出現
    void Spawn(float startCourseZ, float startLaneOffset,const CourseManager& courseManager);

    void UpdateLane(float deltaTime);

    // ダメージを受ける
    void TakeDamage(int damage);

    // 弾がボスに命中したか判定
    bool IsHit(
        float bulletPreviousCourseZ,
        float bulletCourseZ,
        float bulletLaneOffset,
        float bulletSize
    ) const;

    // 撃破判定
    bool IsDead() const;

    // 出現状態を取得
    bool IsActive() const;

    // HPを取得
    int GetHP() const;

    // 現在の行動状態を取得
    BossAction GetAction() const;

    // ボスの状態を初期化
    void Reset();

};
