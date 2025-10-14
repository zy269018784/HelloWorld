// 简化的修复版本
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <iostream>
#include <wrl.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <d3dx12.h>

// 必须链接这些库
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <iostream>
#include <vector>
#include <string>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

const uint32_t FrameCount = 2;
const uint32_t Width = 800;
const uint32_t Height = 600;

// 全局变量
GLFWwindow* g_Window = nullptr;
ComPtr<ID3D12Device> g_Device;
ComPtr<IDXGISwapChain3> g_SwapChain;
ComPtr<ID3D12CommandQueue> g_CommandQueue;
ComPtr<ID3D12DescriptorHeap> g_RtvHeap;
ComPtr<ID3D12Resource> g_RenderTargets[FrameCount];
ComPtr<ID3D12CommandAllocator> g_CommandAllocator;
ComPtr<ID3D12GraphicsCommandList> g_CommandList;
ComPtr<ID3D12Fence> g_Fence;
HANDLE g_FenceEvent = nullptr;
uint64_t g_FenceValue = 1;
uint32_t g_FrameIndex = 0;

// 三角形相关
ComPtr<ID3D12RootSignature> g_RootSignature;
ComPtr<ID3D12PipelineState> g_PipelineState;
ComPtr<ID3D12Resource> g_VertexBuffer;
D3D12_VERTEX_BUFFER_VIEW g_VertexBufferView;

struct Vertex {
    float position[3];
    float color[4];
};

bool CreateTriangleResources();
// 简化初始化
bool Init() {
    // 初始化GLFW
    if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_Window = glfwCreateWindow(Width, Height, "D3D12 Triangle", nullptr, nullptr);
    if (!g_Window) return false;

    // 启用调试层
#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
#endif

    // 创建设备
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_Device)))) {
        std::cerr << "Failed to create device" << std::endl;
        return false;
    }

    // 创建命令队列
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(g_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_CommandQueue)))) {
        return false;
    }

    // 创建交换链
    ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = Width;
    swapChainDesc.Height = Height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    HWND hwnd = glfwGetWin32Window(g_Window);
    if (FAILED(factory->CreateSwapChainForHwnd(g_CommandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain))) {
        return false;
    }
    swapChain.As(&g_SwapChain);
    g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();

    // 创建RTV堆
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    if (FAILED(g_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_RtvHeap)))) {
        return false;
    }

    // 创建RTV
    SIZE_T rtvDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < FrameCount; i++) {
        if (FAILED(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_RenderTargets[i])))) {
            return false;
        }
        g_Device->CreateRenderTargetView(g_RenderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }

    // 创建命令分配器和列表
    if (FAILED(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_CommandAllocator)))) {
        return false;
    }
    if (FAILED(g_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&g_CommandList)))) {
        return false;
    }
    g_CommandList->Close();

    // 创建围栏
    if (FAILED(g_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_Fence)))) {
        return false;
    }
    g_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    // 创建三角形
    return CreateTriangleResources();
}

bool CreateTriangleResources() {
    std::cout << "Creating triangle resources..." << std::endl;

    // 1. 创建空的根签名
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error))) {
        std::cerr << "Failed to serialize root signature" << std::endl;
        return false;
    }
    if (FAILED(g_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_RootSignature)))) {
        std::cerr << "Failed to create root signature" << std::endl;
        return false;
    }

    // 2. 编译着色器
    const char* vsCode = R"(
        struct VS_IN {
            float3 pos : POSITION;
            float4 col : COLOR;
        };
        struct PS_IN {
            float4 pos : SV_POSITION;
            float4 col : COLOR;
        };
        PS_IN main(VS_IN input) {
            PS_IN output;
            output.pos = float4(input.pos, 1.0);
            output.col = input.col;
            return output;
        }
    )";

    const char* psCode = R"(
        struct PS_IN {
            float4 pos : SV_POSITION;
            float4 col : COLOR;
        };
        float4 main(PS_IN input) : SV_Target {
            return input.col;
        }
    )";

    ComPtr<ID3DBlob> vertexShader, pixelShader;
    UINT compileFlags = 0;
#ifdef _DEBUG
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    if (FAILED(D3DCompile(vsCode, strlen(vsCode), nullptr, nullptr, nullptr, "main", "vs_5_0", compileFlags, 0, &vertexShader, &error))) {
        std::cerr << "VS compile failed: " << (char*)error->GetBufferPointer() << std::endl;
        return false;
    }
    if (FAILED(D3DCompile(psCode, strlen(psCode), nullptr, nullptr, nullptr, "main", "ps_5_0", compileFlags, 0, &pixelShader, &error))) {
        std::cerr << "PS compile failed: " << (char*)error->GetBufferPointer() << std::endl;
        return false;
    }

    // 3. 定义输入布局
    D3D12_INPUT_ELEMENT_DESC inputDescs[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // 4. 创建管线状态
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputDescs, _countof(inputDescs) };
    psoDesc.pRootSignature = g_RootSignature.Get();
    psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
    psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 重要：禁用背面剔除
    psoDesc.RasterizerState.DepthClipEnable = TRUE;
    psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
    psoDesc.BlendState.IndependentBlendEnable = FALSE;
    psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    if (FAILED(g_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_PipelineState)))) {
        std::cerr << "Failed to create PSO" << std::endl;
        return false;
    }

    // 5. 创建顶点缓冲区
    Vertex vertices[] = {
        { {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 上 - 红
        { {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 右下 - 绿
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }  // 左下 - 蓝
    };

    const UINT bufferSize = sizeof(vertices);

    // 创建上传堆
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = bufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    if (FAILED(g_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&g_VertexBuffer)))) {
        return false;
    }

    // 上传顶点数据
    void* pData = nullptr;
    if (FAILED(g_VertexBuffer->Map(0, nullptr, &pData))) {
        return false;
    }
    memcpy(pData, vertices, bufferSize);
    g_VertexBuffer->Unmap(0, nullptr);

    // 创建顶点缓冲区视图
    g_VertexBufferView.BufferLocation = g_VertexBuffer->GetGPUVirtualAddress();
    g_VertexBufferView.StrideInBytes = sizeof(Vertex);
    g_VertexBufferView.SizeInBytes = bufferSize;

    std::cout << "Triangle resources created successfully!" << std::endl;
    return true;
}

void WaitForGPU() {
    const UINT64 fence = g_FenceValue;
    g_CommandQueue->Signal(g_Fence.Get(), fence);
    g_FenceValue++;

    if (g_Fence->GetCompletedValue() < fence) {
        g_Fence->SetEventOnCompletion(fence, g_FenceEvent);
        WaitForSingleObject(g_FenceEvent, INFINITE);
    }
    g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
}

void Render() {
    // 重置命令
    g_CommandAllocator->Reset();
    g_CommandList->Reset(g_CommandAllocator.Get(), g_PipelineState.Get());

    // 资源屏障：Present -> Render Target
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = g_RenderTargets[g_FrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    g_CommandList->ResourceBarrier(1, &barrier);

    // 获取RTV
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_RtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += g_FrameIndex * g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // 清除为深灰色并设置渲染目标
    const float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    g_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    g_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // 设置管线状态
    g_CommandList->SetGraphicsRootSignature(g_RootSignature.Get());

    // 设置视口和裁剪矩形
    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)Width, (float)Height, 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, (LONG)Width, (LONG)Height };
    g_CommandList->RSSetViewports(1, &viewport);
    g_CommandList->RSSetScissorRects(1, &scissorRect);

    // 设置顶点缓冲区和绘制
    g_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_CommandList->IASetVertexBuffers(0, 1, &g_VertexBufferView);
    g_CommandList->DrawInstanced(3, 1, 0, 0);

    // 资源屏障：Render Target -> Present
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    g_CommandList->ResourceBarrier(1, &barrier);

    // 执行命令列表
    g_CommandList->Close();
    ID3D12CommandList* cmdLists[] = { g_CommandList.Get() };
    g_CommandQueue->ExecuteCommandLists(1, cmdLists);

    // 呈现
    g_SwapChain->Present(1, 0);
    WaitForGPU();
}

void Cleanup() {
    WaitForGPU();
    if (g_FenceEvent) CloseHandle(g_FenceEvent);
    if (g_Window) glfwDestroyWindow(g_Window);
    glfwTerminate();
}

int main() {
    if (!Init()) {
        Cleanup();
        return -1;
    }

    std::cout << "Running D3D12 Triangle Example. Press ESC to exit." << std::endl;

    while (!glfwWindowShouldClose(g_Window)) {
        glfwPollEvents();
        if (glfwGetKey(g_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
        Render();
    }

    Cleanup();
    return 0;
}