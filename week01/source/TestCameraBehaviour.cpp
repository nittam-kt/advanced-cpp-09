#pragma once

#include "TestCameraBehaviour.h"

#include <Transform.h>
#include <Input.h>
#include <UniDxTime.h>


using namespace DirectX;
using namespace UniDx;


void TestCameraBehaviour::OnEnable()
{
    transform->localPosition = Vector3(0, 0, -2);
}


void TestCameraBehaviour::Update()
{
    const float speed = 0.5f;

    Vector3 pos = transform->localPosition;
    if (Input::GetKey(Keyboard::A))
    {
        pos.x -= speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::D))
    {
        pos.x += speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::W))
    {
        pos.y += speed * Time::deltaTime;
    }
    if (Input::GetKey(Keyboard::S))
    {
        pos.y -= speed * Time::deltaTime;
    }
    transform->localPosition = pos;
}
