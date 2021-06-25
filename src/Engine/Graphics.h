#pragma once
#include "Engine.h"
/* Declarations
----------------*/
class Graphics
{
public:
  ID3D11Device *device;
  ID3D11DeviceContext *deviceContext;
  IDXGISwapChain *swapchain;
  ID3D11RenderTargetView *backbuffer;
  // Constructor
  void Inicialize(HWND &hWnd, USSize &screen)
  {
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    // fill the swap chain description struct
    scd.BufferCount = 1;                                // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
    scd.BufferDesc.Width = screen.width;         // set the back buffer width
    scd.BufferDesc.Height = screen.height;       // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
    scd.OutputWindow = hWnd;                     // the window to be used
    scd.SampleDesc.Count = 4;                           // how many multisamples
    scd.Windowed = TRUE;                                // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(0,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  0,
                                  0,
                                  0,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &device,
                                  0,
                                  &deviceContext);
    // Create Backbuffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);
    // use the back buffer address to create the render target
    device->CreateRenderTargetView(pBackBuffer, 0, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    deviceContext->OMSetRenderTargets(1, &backbuffer, 0);
    /* Setup Blend State for transperency
    -------------------------------------*/
    D3D11_BLEND_DESC BlendStateDescription;
    BlendStateDescription.RenderTarget[0].BlendEnable = TRUE;
    BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    BlendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

    BlendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
    BlendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    BlendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    ID3D11BlendState *blend;
    device->CreateBlendState(&BlendStateDescription, &blend);

    float blendFactor[] = {0, 0, 0, 0};
    deviceContext->OMSetBlendState(blend, blendFactor, 0xffffffff);
    blend->Release();
    /* Viewport
    ------------*/
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = screen.width;
    viewport.Height = screen.height;

    deviceContext->RSSetViewports(1, &viewport);
    // Shaders Inicialize
    ID3DBlob *blob;
    // Color Pixel Shader
    HRESULT hr = D3DReadFileToBlob(L"PixelShader.cso", &blob);
    ID3D11PixelShader *pixelShader; // the pointer to the pixel shader
    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &pixelShader);
    blob->Release();
    if (FAILED(hr))
    {
      printf("Failed to load pixel shader with error: %x\n", (unsigned int)hr);
      exit(1);
    }
    deviceContext->PSSetShader(pixelShader, 0, 0);
    pixelShader->Release();
    // Color Vortex Shader
    hr = D3DReadFileToBlob(L"VertexShader.cso", &blob);
    if (FAILED(hr))
    {
      printf("Failed to load vertex shader with error: %x\n", (unsigned int)hr);
      exit(1);
    }
    ID3D11VertexShader *vortexShader; // the pointer to the vertex shader
    hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vortexShader);
    if (FAILED(hr))
    {
      printf("CreateVertexShader failed with error: %x\n", (unsigned int)hr);
      exit(1);
    }
    deviceContext->VSSetShader(vortexShader, 0, 0);
    vortexShader->Release();
    // create the input Layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}};
    ID3D11InputLayout *layout; // the pointer to the input colorLayout
    hr = device->CreateInputLayout(ied, 2, blob->GetBufferPointer(), blob->GetBufferSize(), &layout);
    blob->Release();
    if (FAILED(hr))
    {
      printf("CreateInputLayout failed with error: %x\n", (unsigned int)hr);
      exit(1);
    }
    deviceContext->IASetInputLayout(layout);
    layout->Release();
    // Create Texture2D sampler state
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    ID3D11SamplerState *sampleState;
    device->CreateSamplerState(&samplerDesc, &sampleState);
    deviceContext->PSSetSamplers(0, 1, &sampleState);

    sampleState->Release();
    deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  }
  void clearRender(fRGBA color)
  {
    deviceContext->ClearRenderTargetView(backbuffer, (float *)&color);
  }
  void render()
  {
    swapchain->Present(0u, 0u);
  }
  /* Deconstructor
  -----------------*/
  ~Graphics()
  {
    // close and release all existing COM objects
    swapchain->Release();
    backbuffer->Release();
    device->Release();
    deviceContext->Release();
  }
} graphics;