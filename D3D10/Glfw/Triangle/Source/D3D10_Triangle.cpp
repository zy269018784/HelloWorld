#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <D3D10_1.h>
#include <D3D10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>

// 在包含 GLFW 后定义 Win32 函数
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib, "d3d10_1.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "glfw3.lib")

using namespace DirectX;

// D3D10 对象
ID3D10Device1* g_pDevice = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D10RenderTargetView* g_pRenderTargetView = nullptr;

// 图形资源
ID3D10VertexShader* g_pVertexShader = nullptr;
ID3D10PixelShader* g_pPixelShader = nullptr;
ID3D10InputLayout* g_pInputLayout = nullptr;
ID3D10Buffer* g_pVertexBuffer = nullptr;

// 窗口尺寸
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 顶点结构
struct SimpleVertex
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

// 顶点着色器代码
const char* g_VertexShaderCode =
"struct VS_INPUT\n"
"{\n"
"    float3 Pos : POSITION;\n"
"    float4 Color : COLOR;\n"
"};\n"
"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float4 Color : COLOR;\n"
"};\n"
"VS_OUTPUT main(VS_INPUT input)\n"
"{\n"
"    VS_OUTPUT output;\n"
"    output.Pos = float4(input.Pos, 1.0f);\n"
"    output.Color = input.Color;\n"
"    return output;\n"
"}";

// 像素着色器代码
const char* g_PixelShaderCode =
"struct PS_INPUT\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float4 Color : COLOR;\n"
"};\n"
"float4 main(PS_INPUT input) : SV_Target\n"
"{\n"
"    return input.Color;\n"
"}";

// 错误回调函数
void error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// 键盘输入回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// 初始化GLFW窗口
GLFWwindow* InitializeGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // 配置GLFW不使用OpenGL上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "D3D10 + GLFW Triangle", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwSetKeyCallback(window, key_callback);
    return window;
}

// 初始化D3D10设备
bool InitializeD3D10(GLFWwindow* window)
{
    // 获取窗口的HWND
    HWND hwnd = glfwGetWin32Window(window);
    if (!hwnd)
    {
        std::cerr << "Failed to get window handle" << std::endl;
        return false;
    }

    // 交换链描述
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WINDOW_WIDTH;
    sd.BufferDesc.Height = WINDOW_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // 特性等级
    D3D10_FEATURE_LEVEL1 featureLevels[] = { D3D10_FEATURE_LEVEL_10_1, D3D10_FEATURE_LEVEL_10_0 };
    D3D10_FEATURE_LEVEL1 featureLevel;

    // 创建设备和交换链
    HRESULT hr = D3D10CreateDeviceAndSwapChain1(
        nullptr,                    // 默认适配器
        D3D10_DRIVER_TYPE_HARDWARE, // 硬件驱动
        nullptr,                    // 软件模块
        0,                          // 标志
        D3D10_FEATURE_LEVEL_10_1,   // 特性等级
        D3D10_1_SDK_VERSION,        // SDK版本
        &sd,                        // 交换链描述
        &g_pSwapChain,              // 交换链
        &g_pDevice                  // 设备
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to create D3D10 device and swap chain" << std::endl;
        return false;
    }

    return true;
}

// 创建渲染目标视图
bool CreateRenderTarget()
{
    ID3D10Texture2D* pBackBuffer = nullptr;

    HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get back buffer" << std::endl;
        return false;
    }

    hr = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
    {
        std::cerr << "Failed to create render target view" << std::endl;
        return false;
    }

    g_pDevice->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
    return true;
}

// 编译着色器
bool CompileShader(const char* shaderCode, const char* entryPoint, const char* target, ID3D10Blob** ppBlob)
{
    ID3D10Blob* pErrorBlob = nullptr;

    HRESULT hr = D3DCompile(
        shaderCode,
        strlen(shaderCode),
        nullptr,
        nullptr,
        nullptr,
        entryPoint,
        target,
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        ppBlob,
        &pErrorBlob
    );

    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            std::cerr << "Shader compilation error: " << (char*)pErrorBlob->GetBufferPointer() << std::endl;
            pErrorBlob->Release();
        }
        return false;
    }

    if (pErrorBlob) pErrorBlob->Release();
    return true;
}

// 创建着色器和输入布局
bool CreateShadersAndInputLayout()
{
    ID3D10Blob* pVSBlob = nullptr;
    ID3D10Blob* pPSBlob = nullptr;

    if (!CompileShader(g_VertexShaderCode, "main", "vs_4_0", &pVSBlob))
    {
        return false;
    }

    if (!CompileShader(g_PixelShaderCode, "main", "ps_4_0", &pPSBlob))
    {
        if (pVSBlob) pVSBlob->Release();
        return false;
    }

    HRESULT hr = g_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        pPSBlob->Release();
        return false;
    }

    hr = g_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), &g_pPixelShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        pPSBlob->Release();
        return false;
    }

    // 创建输入布局
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    hr = g_pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pInputLayout);

    pVSBlob->Release();
    pPSBlob->Release();

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

// 创建顶点缓冲区
bool CreateVertexBuffer()
{
    // 三角形的三个顶点
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },   // 顶点0: 红色
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },  // 顶点1: 绿色
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }  // 顶点2: 蓝色
    };

    D3D10_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 3;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D10_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    HRESULT hr = g_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create vertex buffer" << std::endl;
        return false;
    }

    return true;
}

// 设置视口
void SetupViewport()
{
    D3D10_VIEWPORT vp;
    vp.Width = WINDOW_WIDTH;
    vp.Height = WINDOW_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pDevice->RSSetViewports(1, &vp);
}

// 渲染函数
void Render()
{
    // 清除渲染目标为深蓝色
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    g_pDevice->ClearRenderTargetView(g_pRenderTargetView, clearColor);

    // 设置输入布局
    g_pDevice->IASetInputLayout(g_pInputLayout);

    // 设置顶点缓冲区
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    g_pDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 设置图元拓扑
    g_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 设置着色器
    g_pDevice->VSSetShader(g_pVertexShader);
    g_pDevice->PSSetShader(g_pPixelShader);

    // 绘制三角形
    g_pDevice->Draw(3, 0);

    // 呈现到屏幕
    g_pSwapChain->Present(1, 0);
}

// 清理资源
void Cleanup()
{
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pInputLayout) g_pInputLayout->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pDevice) g_pDevice->Release();
}

// 主函数
int main()
{
    std::cout << "Initializing GLFW..." << std::endl;
    GLFWwindow* window = InitializeGLFW();
    if (!window)
    {
        return -1;
    }

    std::cout << "Initializing D3D10..." << std::endl;
    if (!InitializeD3D10(window))
    {
        glfwTerminate();
        return -1;
    }

    std::cout << "Creating render target..." << std::endl;
    if (!CreateRenderTarget())
    {
        Cleanup();
        glfwTerminate();
        return -1;
    }

    SetupViewport();

    std::cout << "Creating shaders..." << std::endl;
    if (!CreateShadersAndInputLayout())
    {
        Cleanup();
        glfwTerminate();
        return -1;
    }

    std::cout << "Creating vertex buffer..." << std::endl;
    if (!CreateVertexBuffer())
    {
        Cleanup();
        glfwTerminate();
        return -1;
    }

    std::cout << "D3D10 + GLFW Triangle Example Running..." << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        Render();
        glfwPollEvents();
    }

    std::cout << "Cleaning up resources..." << std::endl;
    Cleanup();
    glfwTerminate();
    std::cout << "Application exited successfully." << std::endl;

    return 0;
}