#pragma once

class PingPong : public GameScene
{

	public:

	PingPong() :GameScene(L"PingPong Scene"){};
	~PingPong() = default;
	PingPong(const PingPong& other) = delete;
	PingPong(PingPong&& other) noexcept = delete;
	PingPong& operator=(const PingPong& other) = delete;
	PingPong& operator=(PingPong&& other) noexcept = delete;

protected:

	enum class InputID
	{
		up,
		down,
		up1,
		down1,
		space
	};

	void Initialize() override;
	void Update() override;
	void Draw() override;
	

private:
	void Restart();


	GameObject* m_RightWall;
	GameObject* m_LeftWall;
	GameObject* m_Ball;

	bool m_IsGameOn;
};

