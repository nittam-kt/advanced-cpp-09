#pragma once

#include "Renderer.h"
#include "Texture.h"

namespace UniDx {

// --------------------
// CubeRendererクラス
// --------------------
class CubeRenderer : public MeshRenderer
{
public:
    template<typename TVertex>
    static std::unique_ptr<CubeRenderer> create(const std::wstring& shaderPath, const std::wstring& texturePath)
    {
        auto ptr = std::unique_ptr<CubeRenderer>(new CubeRenderer());
        ptr->addShaderTextue<TVertex>(shaderPath, texturePath);
        return ptr;
    }

    template<typename TVertex>
    void addShaderTextue(const std::wstring& shaderPath, const std::wstring& textuePath)
    {
        // マテリアルを追加
        materials.push_back(std::make_unique<Material>());

        // マテリアルをシェーダーを読み込んで初期化
        materials.back()->shader.compile<VertexPT>(shaderPath);

        // テクスチャを読み込んでマテリアルに追加
        if (textuePath.size() > 0)
        {
            std::unique_ptr<Texture> t = std::make_unique<Texture>();
            t->load(textuePath);
            materials.back()->addTexture(std::move(t));
        }
    }

protected:
    virtual void OnEnable() override;
};

} // namespace UniDx
