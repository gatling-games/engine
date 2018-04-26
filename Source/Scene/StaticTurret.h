#pragma once

#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "Scene/Helicopter.h"

class StaticTurret : public Component
{
public:
    StaticTurret(GameObject* gameObject);

    //void drawProperties() override;

    //void serialize(PropertyTable &table) override;

    void update(float deltaTime) override;

    std::vector<Helicopter*> choppers;

private:

    Transform* transform_;

    float rotationCap_ = 45.0f;
    float rotationSpeed_ = 90.0f;
};