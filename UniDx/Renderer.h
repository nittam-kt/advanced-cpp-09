#pragma once

#include <memory>
#include <map>

#include <SimpleMath.h>

#include "Component.h"
#include "Shader.h"
#include "Mesh.h"


namespace UniDx {

class Camera;
class Texture;

// --------------------
// Materialクラス
// --------------------
class Material : public Object
{
public:
    Shader shader;
    ReadOnlyProperty<Texture*> mainTexture;

    // コンストラクタ
    Material();
    Material(VertexType vt, const std::wstring& shPath);

    // シェーダのコンパイル
    // あらかじめ頂点タイプとシェーダパスを設定して使う
    bool compileShader();

    // マテリアル情報設定。Render()内で呼び出す
    void setForRender() const;

    // テクスチャ追加
    void addTexture(std::unique_ptr<Texture> tex);

protected:
    ComPtr<ID3D11Buffer> constantBuffer;
    std::vector<std::unique_ptr<Texture>> textures;
    VertexType vertexType;
    std::wstring shaderPath;
};


// --------------------
// Rendererクラス
// --------------------
class Renderer : public Component
{
public:
    std::vector< std::unique_ptr<Material> > materials;

    virtual void Render(const Camera& camera) const {}

protected:
    ComPtr<ID3D11Buffer> constantBuffer0;

    virtual void OnEnable() override;
    virtual void updatePositionCameraCBuffer(const UniDx::Camera& camera) const;
    virtual void setShaderForRender() const;
};


// --------------------
// MeshRendererクラス
// --------------------
class MeshRenderer : public Renderer
{
public:
    Mesh mesh;

    // メッシュを使って描画
    virtual void Render(const Camera& camera) const override;
};


} // namespace UniDx
