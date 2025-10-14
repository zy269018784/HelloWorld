#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <d3d9.h>
#include <iostream>

// 在包含 GLFW 后定义 Win32 函数
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "glfw3.lib")

using namespace std;

// D3D9 对象
IDirect3D9* g_pD3D = nullptr;
IDirect3DDevice9* g_pDevice = nullptr;

// 顶点缓冲区
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;

// 窗口尺寸
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 自定义顶点结构
struct CUSTOMVERTEX
{
    FLOAT x, y, z;    // 位置
    DWORD color;      // 颜色
};

// 自定义顶点格式
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// 错误回调函数
void error_callback(int error, const char* description)
{
    cerr << "GLFW Error " << error << ": " << description << endl;
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
        cerr << "Failed to initialize GLFW" << endl;
        return nullptr;
    }

    // 配置GLFW不使用OpenGL上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "D3D9 + GLFW Triangle", nullptr, nullptr);
    if (!window)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }

    glfwSetKeyCallback(window, key_callback);
    return window;
}

// 初始化D3D9设备
bool InitializeD3D9(GLFWwindow* window)
{
    // 获取窗口的HWND
    HWND hwnd = glfwGetWin32Window(window);
    if (!hwnd)
    {
        cerr << "Failed to get window handle" << endl;
        return false;
    }

    // 创建D3D9对象
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_pD3D)
    {
        cerr << "Failed to create D3D9 object" << endl;
        return false;
    }

    // 设备参数
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = WINDOW_WIDTH;
    d3dpp.BackBufferHeight = WINDOW_HEIGHT;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // 创建设备
    HRESULT hr = g_pD3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pDevice
    );

    if (FAILED(hr))
    {
        cerr << "Failed to create D3D9 device" << endl;
        return false;
    }

    return true;
}

// 创建顶点缓冲区
bool CreateVertexBuffer()
{
    // 创建顶点缓冲区
    HRESULT hr = g_pDevice->CreateVertexBuffer(
        3 * sizeof(CUSTOMVERTEX),
        0,
        D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT,
        &g_pVertexBuffer,
        nullptr
    );

    if (FAILED(hr))
    {
        cerr << "Failed to create vertex buffer" << endl;
        return false;
    }

    // 顶点数据 - 使用标准化设备坐标 (-1 到 1)
    CUSTOMVERTEX vertices[] =
    {
        { 0.0f, 0.5f, 0.5f, D3DCOLOR_XRGB(255, 0, 0) },    // 顶点0: 红色
        { 0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(0, 255, 0) },   // 顶点1: 绿色
        { -0.5f, -0.5f, 0.5f, D3DCOLOR_XRGB(0, 0, 255) }   // 顶点2: 蓝色
    };

    // 锁定顶点缓冲区并填充数据
    VOID* pVertices;
    hr = g_pVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
    if (FAILED(hr))
    {
        cerr << "Failed to lock vertex buffer" << endl;
        return false;
    }

    memcpy(pVertices, vertices, sizeof(vertices));
    g_pVertexBuffer->Unlock();

    return true;
}

// 手动创建单位矩阵
void SetIdentityMatrix(D3DMATRIX& matrix)
{
    ZeroMemory(&matrix, sizeof(D3DMATRIX));
    matrix._11 = matrix._22 = matrix._33 = matrix._44 = 1.0f;
}

// 手动创建视图矩阵
void SetViewMatrix(D3DMATRIX& matrix, float eyeX, float eyeY, float eyeZ,
    float lookAtX, float lookAtY, float lookAtZ,
    float upX, float upY, float upZ)
{
    D3DVECTOR eye = { eyeX, eyeY, eyeZ };
    D3DVECTOR lookAt = { lookAtX, lookAtY, lookAtZ };
    D3DVECTOR up = { upX, upY, upZ };

    D3DVECTOR zAxis = { lookAt.x - eye.x, lookAt.y - eye.y, lookAt.z - eye.z };
    // 归一化 zAxis
    float length = sqrtf(zAxis.x * zAxis.x + zAxis.y * zAxis.y + zAxis.z * zAxis.z);
    zAxis.x /= length; zAxis.y /= length; zAxis.z /= length;

    D3DVECTOR xAxis;
    xAxis.x = up.y * zAxis.z - up.z * zAxis.y;
    xAxis.y = up.z * zAxis.x - up.x * zAxis.z;
    xAxis.z = up.x * zAxis.y - up.y * zAxis.x;
    // 归一化 xAxis
    length = sqrtf(xAxis.x * xAxis.x + xAxis.y * xAxis.y + xAxis.z * xAxis.z);
    xAxis.x /= length; xAxis.y /= length; xAxis.z /= length;

    D3DVECTOR yAxis;
    yAxis.x = zAxis.y * xAxis.z - zAxis.z * xAxis.y;
    yAxis.y = zAxis.z * xAxis.x - zAxis.x * xAxis.z;
    yAxis.z = zAxis.x * xAxis.y - zAxis.y * xAxis.x;

    ZeroMemory(&matrix, sizeof(D3DMATRIX));
    matrix._11 = xAxis.x; matrix._12 = yAxis.x; matrix._13 = zAxis.x;
    matrix._21 = xAxis.y; matrix._22 = yAxis.y; matrix._23 = zAxis.y;
    matrix._31 = xAxis.z; matrix._32 = yAxis.z; matrix._33 = zAxis.z;
    matrix._41 = -(xAxis.x * eye.x + xAxis.y * eye.y + xAxis.z * eye.z);
    matrix._42 = -(yAxis.x * eye.x + yAxis.y * eye.y + yAxis.z * eye.z);
    matrix._43 = -(zAxis.x * eye.x + zAxis.y * eye.y + zAxis.z * eye.z);
    matrix._44 = 1.0f;
}

// 手动创建投影矩阵
void SetProjectionMatrix(D3DMATRIX& matrix, float fov, float aspect, float nearZ, float farZ)
{
    float yScale = 1.0f / tanf(fov / 2.0f);
    float xScale = yScale / aspect;

    ZeroMemory(&matrix, sizeof(D3DMATRIX));
    matrix._11 = xScale;
    matrix._22 = yScale;
    matrix._33 = farZ / (farZ - nearZ);
    matrix._34 = 1.0f;
    matrix._43 = -nearZ * farZ / (farZ - nearZ);
    matrix._44 = 0.0f;
}

// 设置渲染状态
void SetupRenderStates()
{
    // 设置世界矩阵为单位矩阵
    D3DMATRIX worldMatrix;
    SetIdentityMatrix(worldMatrix);
    g_pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);

    // 设置视图矩阵
    D3DMATRIX viewMatrix;
    SetViewMatrix(viewMatrix,
        0.0f, 0.0f, -2.0f,  // 摄像机位置
        0.0f, 0.0f, 0.0f,   // 观察点
        0.0f, 1.0f, 0.0f);  // 上向量
    g_pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

    // 设置投影矩阵
    D3DMATRIX projectionMatrix;
    SetProjectionMatrix(projectionMatrix,
        3.14159f / 4.0f,                    // 45度视野
        (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, // 宽高比
        0.1f,                               // 近平面
        100.0f);                            // 远平面
    g_pDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

    // 设置渲染状态
    g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);      // 禁用光照
    g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 禁用背面剔除
    g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);        // 启用深度测试
}

// 渲染函数
void Render()
{
    // 清除后台缓冲区和深度缓冲区
    g_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 51, 102), 1.0f, 0);

    // 开始场景
    if (SUCCEEDED(g_pDevice->BeginScene()))
    {
        // 设置数据流
        g_pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
        g_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

        // 绘制三角形
        g_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

        // 结束场景
        g_pDevice->EndScene();
    }

    // 呈现到屏幕
    g_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

// 清理资源
void Cleanup()
{
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pDevice) g_pDevice->Release();
    if (g_pD3D) g_pD3D->Release();
}

// 主函数
int main()
{
    cout << "Initializing GLFW..." << endl;
    GLFWwindow* window = InitializeGLFW();
    if (!window)
    {
        return -1;
    }

    cout << "Initializing D3D9..." << endl;
    if (!InitializeD3D9(window))
    {
        glfwTerminate();
        return -1;
    }

    cout << "Creating vertex buffer..." << endl;
    if (!CreateVertexBuffer())
    {
        Cleanup();
        glfwTerminate();
        return -1;
    }

    SetupRenderStates();

    cout << "D3D9 + GLFW Triangle Example Running..." << endl;
    cout << "Press ESC to exit" << endl;

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        Render();
        glfwPollEvents();
    }

    cout << "Cleaning up resources..." << endl;
    Cleanup();
    glfwTerminate();
    cout << "Application exited successfully." << endl;

    return 0;
}