#pragma once
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum WrapMode
{
  CLAMP = 0
};
enum FilterMode
{
  Bilinear = 0
};

/* Cached
----------*/
// To Draw
D3D11_BUFFER_DESC bufferDesc = {
    0,                      // ByteWidth
    D3D11_USAGE_DYNAMIC,    // USAGE: write access access by CPU and GPU
    0,                      // BindFlags
    D3D11_CPU_ACCESS_WRITE, // allow CPU to write in buffer
    0,                      // MiscFlags
    0                       //StructureByteStride
};
D3D11_SUBRESOURCE_DATA subResourceData;
UINT stride = sizeof(Vertex);
UINT offset = 0;
ID3D11Buffer *vertexBuffer; // the pointer to the vertex buffer
ID3D11Buffer *indexBuffer;  // the pointer to the index buffer
/* Declarations
----------------*/
class Texture2D
{
private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  ID3D11ShaderResourceView *textureView;
  unsigned short pHeight, pWidth;

public:
  // WrapMode wrapMode, FilterMode filterMode
  Texture2D(const char *filePath, ISize imageSize, int bpp) : pWidth(imageSize.width), pHeight(imageSize.height)
  {
    unsigned char *rgb_image = stbi_load(filePath, &imageSize.width, &imageSize.height, &bpp, 4u);
    assert(rgb_image);
    /* Texture
    -----------*/
    // Description
    D3D11_TEXTURE2D_DESC sysTexDesc = {
        (UINT)imageSize.width,
        (UINT)imageSize.height,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        {1, 0},
        D3D11_USAGE_DYNAMIC,
        D3D11_BIND_SHADER_RESOURCE,
        D3D11_CPU_ACCESS_WRITE,
        0};
    // SubResource
    D3D11_SUBRESOURCE_DATA subResource = {};
    subResource.pSysMem = rgb_image;
    subResource.SysMemPitch = imageSize.width * 4u;
    subResource.SysMemSlicePitch = 0;
    // Create Texture
    ID3D11Texture2D *texture;
    graphics.device->CreateTexture2D(&sysTexDesc, &subResource, &texture);
    stbi_image_free(rgb_image);
    // ViewDescription
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = sysTexDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    // Create Resource View
    graphics.device->CreateShaderResourceView(texture, &srvDesc, &textureView);
    texture->Release();
  }
  ~Texture2D()
  {
    textureView->Release();
  }

public:
  const unsigned short &height = pHeight;
  const unsigned short &width = pWidth;
  void clear()
  {
    vertices.clear();
    indices.clear();
  }
  void draw()
  {
    // Create Vertex Buffer
    bufferDesc.ByteWidth = vertices.size() * sizeof(Vertex); // size is the VERTEX struct
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;         // use as a vertex buffer
    subResourceData = {vertices.data(), 0u, 0u};
    graphics.device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer); // create the buffer
    // Create Index Buffer
    bufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subResourceData = {indices.data(), 0u, 0u};
    graphics.device->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer);
    // Set Buffers
    graphics.deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset);
    graphics.deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0u);
    graphics.deviceContext->PSSetShaderResources(0u, 1u, &textureView);
    // Draw
    graphics.deviceContext->DrawIndexed(indices.size(), 0u, 0u);
    // Clear
    vertexBuffer->Release();
    indexBuffer->Release();
    clear();
  }
  /* Shapes
  ----------*/
  void triangle(
      const Vertex vertice0,
      const Vertex vertice1,
      const Vertex vertice2)
  {
    unsigned short vertI = vertices.size();
    // Triangle
    indices.push_back(vertI++);
    indices.push_back(vertI++);
    indices.push_back(vertI);
    // Vertices
    vertices.push_back(vertice0);
    vertices.push_back(vertice1);
    vertices.push_back(vertice2);
  }
  void quadrilateral(
      const Vertex topLeft,
      const Vertex topRight,
      const Vertex bottomRight,
      const Vertex bottomLeft)
  {
    unsigned short vertI0 = vertices.size();
    unsigned short vertI1 = vertI0 + 1;
    unsigned short vertI2 = vertI0 + 2;
    unsigned short vertI3 = vertI0 + 3;
    // Triangle
    indices.push_back(vertI0);
    indices.push_back(vertI1);
    indices.push_back(vertI2);
    // Triangle
    indices.push_back(vertI1);
    indices.push_back(vertI3);
    indices.push_back(vertI2);
    // Vertices
    vertices.push_back(topLeft);
    vertices.push_back(topRight);
    vertices.push_back(bottomLeft);
    vertices.push_back(bottomRight);
  }
  void rectangle(Rect verticesMap, Rect textureMap)
  {
    quadrilateral(
        {Vector3(verticesMap.xo, verticesMap.yo), Vector2(textureMap.xo, textureMap.yo)},
        {Vector3(verticesMap.xf, verticesMap.yo), Vector2(textureMap.xf, textureMap.yo)},
        {Vector3(verticesMap.xf, verticesMap.yf), Vector2(textureMap.xf, textureMap.yf)},
        {Vector3(verticesMap.xo, verticesMap.yf), Vector2(textureMap.xo, textureMap.yf)});
  }
};