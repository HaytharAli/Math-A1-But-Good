#include "AnimationSpritePlayground.h"
#include "Utilities.h"

AnimationSpritePlayground::AnimationSpritePlayground(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(0.f, -10.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void AnimationSpritePlayground::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera
	{
		//Create Camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup Hello World Sign
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//Setup BOX
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-30.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	}

	//Setup static BOX
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2000, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	//Setup Player
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<Player>(entity);

		//Set up components
		std::string fileName = "spritesheets/spritesheet.png";
		std::string animations = "Ghost.json";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 16, 16, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhysBody = ECS::GetComponent<PhysicsBody>(entity);
		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhysBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void AnimationSpritePlayground::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	Scene::AdjustScrollOffset();
	player.Update();
}

void AnimationSpritePlayground::KeyboardHold()
{
}

void AnimationSpritePlayground::KeyboardDown()
{
}

void AnimationSpritePlayground::KeyboardUp()
{
}
