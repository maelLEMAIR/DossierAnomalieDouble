#ifndef TRANSFORM_SYSTEM_H_DEFINED
#define TRANSFORM_SYSTEM_H_DEFINED

#include "System.h"
#include <DirectXMath.h>

using namespace DirectX;

class TransformComponent;

class TransformSystem : public System
{
public:
	void OnInit() override;
	void Update(float dt) override;
	void SavePreviousPositions();

private:
	
	void UpdateTransform(Entity* pEntity,TransformComponent* _transformComponent, TransformComponent* _transformComponentParent = nullptr);
};

#endif