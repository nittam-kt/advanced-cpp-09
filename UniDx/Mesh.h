#pragma once

#include <memory>
#include <span>
#include <vector>

#include <d3d11.h>
#include <SimpleMath.h>

#include "Object.h"
#include "Property.h"
#include "Shader.h"


namespace UniDx {

class Camera;
class Texture;

// --------------------
// SubMesh構造体
// --------------------
struct SubMesh
{
    D3D11_PRIMITIVE_TOPOLOGY topology;
    std::span<const Vector3> positions;
    std::span<const Vector3> normals;
    std::span<const Color> colors;
    std::span<const Vector2> uv;
    std::span<const Vector2> uv2;
    std::span<const Vector2> uv3;
    std::span<const Vector2> uv4;
    ComPtr<ID3D11Buffer> buffer;
    UINT stride;

    template<typename TVertex>
    size_t copyTo(std::span<TVertex> vertex)
    {
        assert(vertex.size() >= positions.size());

        // 位置のコピー
        for (int i = 0; i < positions.size(); ++i)
        {
            vertex[i].position = positions[i];
        }
        // 法線のコピー
        copyNormalTo(vertex);

        // カラーのコピー
        copyColorTo(vertex);

        // uv0のコピー
        copyUV0To(vertex);

        return positions.size();
    }

    // ID3D11Bufferの頂点バッファを作成
    // 戻ったバッファは破棄しても良い（DirectX12では仕様が変わる）
    template<typename TVertex>
    std::unique_ptr< std::vector<TVertex> > createVertexBuffer()
    {
        // メモリ上に頂点を確保
        std::unique_ptr < std::vector<TVertex> > buf = std::make_unique< std::vector<TVertex> >();
        buf->resize(positions.size());

        // 確保したメモリに各属性データをコピー
        copyTo(std::span<TVertex>(*buf));

        // ID3D11Buffer を作成
        stride = sizeof(TVertex);
        createBuffer(&buf->front());

        // メモリ上のデータを返す。DirextX11では即座に開放して良い
        return buf;
    }

    // GPUにバッファを作成
    void createBuffer(void* data);

    // 描画
    void Render() const;

    // 法線のコピー
    template<typename TVertex>
    void copyNormalTo(std::span<TVertex> vertex) {}
    template<> void copyNormalTo(std::span<VertexPN> vertex)
    { assert(normals.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].normal = normals[i]; }
    template<> void copyNormalTo(std::span<VertexPNT> vertex)
    { assert(normals.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].normal = normals[i]; }
    template<> void copyNormalTo(std::span<VertexPNC> vertex)
    { assert(normals.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].normal = normals[i]; }

    // カラーのコピー
    template<typename TVertex>
    void copyColorTo(std::span<TVertex> vertex) {}
    template<> void copyColorTo(std::span<VertexPC> vertex)
    { assert(colors.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].color = colors[i]; }
    template<> void copyColorTo(std::span<VertexPNC> vertex)
    { assert(colors.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].color = colors[i]; }

    // UV0のコピー
    template<typename TVertex>
    void copyUV0To(std::span<TVertex> vertex) {}
    template<> void copyUV0To(std::span<VertexPT> vertex)
    { assert(uv.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].uv0 = uv[i]; }
    template<> void copyUV0To(std::span<VertexPNT> vertex)
    { assert(uv.size() == positions.size()); for (int i = 0; i < positions.size(); ++i) vertex[i].uv0 = uv[i]; }
};


// --------------------
// Meshクラス
// --------------------
class Mesh : public Object
{
public:
    std::vector< std::unique_ptr<SubMesh> > submesh;

    Mesh() : Object([this]() {return name_;}) {}
    virtual ~Mesh() {}

    void Render() const
    {
        for (auto& sub : submesh)
        {
            sub->Render();
        }
    }

protected:
    std::wstring name_;
};




} // namespace UniDx
