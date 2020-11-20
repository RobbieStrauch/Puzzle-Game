#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}
	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//Idle Animations\\

	//Idle left
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle right
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());

	//Walk Animations\\

	//Walk left
	m_animController->AddAnimation(animations["WalkLeft"].get<Animation>());
	//Walk right
	m_animController->AddAnimation(animations["WalkRight"].get<Animation>());

	//Set default animation
	m_animController->SetActiveAnim(IDLELEFT);

}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}
	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;


	if (Input::GetKey(Key::T))
	{
		m_hasPhysics = !m_hasPhysics;
	}

	if (m_hasPhysics)
	{
		//Physics movement
		auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
		auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
		float speed = 1.f;
		b2Vec2 vel = b2Vec2(0.f, 0.f);

		if (Input::GetKey(Key::A))
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(-200000.f * speed, 0.f), true);
			m_facing = RIGHT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(200000.f * speed, 0.f), true);
			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKeyDown(Key::T))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
		if (canJump.m_canJump)
		{
			if (Input::GetKeyDown(Key::Space))
			{
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 100000.f), true);
				canJump.m_canJump = false;
			}
		}

	}

}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		activeAnimation = WALK;
	}

	else
	{
		activeAnimation = IDLE;
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
