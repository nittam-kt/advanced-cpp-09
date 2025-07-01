// CreateDefaultScene.cpp
// デフォルトのシーンを生成します

#include <UniDx.h>
#include <Scene.h>
#include <GameObject.h>
#include <Component.h>
#include <Transform.h>
#include <Camera.h>
#include <PrimitiveRenderer.h>

#include "TestRenderer.h"
#include "TestCameraBehaviour.h"

using namespace std;
using namespace UniDx;


unique_ptr<Scene> CreateDefaultScene()
{
    // キューブレンダラを作ってサイズを調整
    auto floor = make_unique<GameObject>(L"床",
        CubeRenderer::create<VertexPT>(L"Resource/Albedo.hlsl", L"Resource/floor.png"));
    floor->transform->localPosition = Vector3(0.0f, -1.0f, 0.0f);
    floor->transform->localScale = Vector3(2.0f, 0.5f, 2.0f);

    return make_unique<Scene>(

        make_unique<GameObject>(L"オブジェクトルート",
            make_unique<GameObject>(L"テストオブジェクト",
                make_unique<TestRenderer>()),
            std::move(floor)
        ),

        make_unique<GameObject>(L"カメラルート",
            make_unique<Camera>(),
            make_unique<TestCameraBehaviour>()
        )
    );
}
