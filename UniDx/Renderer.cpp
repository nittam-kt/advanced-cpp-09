#include "Renderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "D3DManager.h"
#include "Texture.h"
#include "Camera.h"
#include "Debug.h"

namespace UniDx{

// -----------------------------------------------------------------------------
// 頂点シェーダー側と共有する、モデルビュー行列の定数バッファ
//     UniDxではすべてのシェーダーでスロット0番に共通で指定する
// -----------------------------------------------------------------------------
struct VSConstantBuffer0
{
    Matrix world;
    Matrix view;
    Matrix projection;
};


// -----------------------------------------------------------------------------
// コンストラクタ
// -----------------------------------------------------------------------------
Material::Material() :
    Object([this]() { return shader.name; }),
    mainTexture(
        [this]() { return textures.size() > 0 ? textures.front().get() : nullptr; }
    ),
    vertexType(VertexTypeUnknown)
{
}

Material::Material(VertexType vt, const std::wstring& shPath) :
    Object([this]() { return shader.name; }),
    mainTexture(
        [this]() { return textures.size() > 0 ? textures.front().get() : nullptr; }
    ),
    vertexType(vt),
    shaderPath(shPath)
{
}

// -----------------------------------------------------------------------------
// レンダリング用にデバイスへ設定
// -----------------------------------------------------------------------------
bool Material::compileShader()
{
    switch (vertexType)
    {
    case VertexTypeP:
        return shader.compile<VertexP>(shaderPath);
    case VertexTypePN:
        return shader.compile<VertexPN>(shaderPath);
    case VertexTypePT:
        return shader.compile<VertexPT>(shaderPath);
    case VertexTypePC:
        return shader.compile<VertexPC>(shaderPath);
    case VertexTypePNT:
        return shader.compile<VertexPNT>(shaderPath);
    case VertexTypePNC:
        return shader.compile<VertexPNC>(shaderPath);
    case VertexTypeUnknown:
    default:
        assert(false);
        return false;
    }
}


// -----------------------------------------------------------------------------
// レンダリング用にデバイスへ設定
// -----------------------------------------------------------------------------
void Material::setForRender() const
{
    shader.setToContext();
    for (auto& tex : textures)
    {
        tex->setForRender();
    }
}


// -----------------------------------------------------------------------------
// テクスチャ追加
// -----------------------------------------------------------------------------
void Material::addTexture(std::unique_ptr<Texture> tex)
{
    textures.push_back(std::move(tex));
}


// -----------------------------------------------------------------------------
// 有効化
// -----------------------------------------------------------------------------
void Renderer::OnEnable()
{
    // 行列用の定数バッファ生成
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(VSConstantBuffer0);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    D3DManager::instance->GetDevice()->CreateBuffer(&desc, nullptr, constantBuffer0.GetAddressOf());
}


// -----------------------------------------------------------------------------
// 現在の姿勢とカメラをシェーダーの定数バッファに転送
// -----------------------------------------------------------------------------
void Renderer::updatePositionCameraCBuffer(const UniDx::Camera& camera) const
{
    // ─ ワールド行列を位置に合わせて作成
    VSConstantBuffer0 cb{};
    cb.world = transform->getLocalToWorldMatrix();
    cb.view = camera.GetViewMatrix();
    cb.projection = camera.GetProjectionMatrix(16.0f/9.0f);

    // 定数バッファ更新
    D3DManager::instance->GetContext()->UpdateSubresource(constantBuffer0.Get(), 0, nullptr, &cb, 0, 0);
}


// -----------------------------------------------------------------------------
// 現在の姿勢とカメラをシェーダーの定数バッファに転送
// -----------------------------------------------------------------------------
void Renderer::setShaderForRender() const
{
    for(auto& material : materials)
    {
        material->setForRender();

        ID3D11Buffer* cbs[1] = { constantBuffer0.Get() };
        D3DManager::instance->GetContext()->VSSetConstantBuffers(0, 1, cbs);
    }
}


// -----------------------------------------------------------------------------
// メッシュを使って描画
// -----------------------------------------------------------------------------
void MeshRenderer::Render(const Camera& camera) const
{
    // 現在のTransformとカメラの情報をシェーダーのConstantBufferに転送
    Renderer::updatePositionCameraCBuffer(camera);

    //-----------------------------
    // シェーダーをセット
    //-----------------------------
    setShaderForRender();

    //-----------------------------
    // 描画実行
    //-----------------------------
    mesh.Render();
}

}
