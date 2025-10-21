#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ole32.lib")

// 设备信息结构体
struct AudioDeviceInfo {
    std::wstring id;
    std::wstring name;
    std::wstring description;
    bool isDefault = false;
};

// 枚举音频设备
std::vector<AudioDeviceInfo> EnumerateAudioDevices(EDataFlow dataFlow) 
{
    std::vector<AudioDeviceInfo> devices;

    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDeviceCollection* pCollection = NULL;
    IMMDevice* pDefaultDevice = NULL;
    LPWSTR pDefaultDeviceId = NULL;
    UINT count = 0;

    // 初始化COM
    CoInitialize(NULL);

    // 创建设备枚举器
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&pEnumerator);
    if (hr < 0)
    {
        std::cout << "CoCreateInstance failed" << std::endl;
        goto Exit;
    }

    // 获取默认设备ID
    hr = pEnumerator->GetDefaultAudioEndpoint(dataFlow, eConsole, &pDefaultDevice);
    if (SUCCEEDED(hr)) {
        hr = pDefaultDevice->GetId(&pDefaultDeviceId);
        pDefaultDevice->Release();
    }

    // 获取所有设备集合
    hr = pEnumerator->EnumAudioEndpoints(dataFlow, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) goto Exit;

    // 获取设备数量
    hr = pCollection->GetCount(&count);
    if (FAILED(hr)) goto Exit;

    std::cout << "count "  << count << std::endl;

    // 遍历所有设备
    for (UINT i = 0; i < count; i++) {
        IMMDevice* pDevice = NULL;
        IPropertyStore* pProps = NULL;
        PROPVARIANT varName, varDesc;
        LPWSTR pwszID = NULL;

        PropVariantInit(&varName);
        PropVariantInit(&varDesc);

        // 获取设备
        hr = pCollection->Item(i, &pDevice);
        if (FAILED(hr)) continue;

        // 获取设备ID
        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr)) {
            pDevice->Release();
            continue;
        }

        AudioDeviceInfo info;
        info.id = pwszID;
        info.isDefault = (pDefaultDeviceId && wcscmp(pwszID, pDefaultDeviceId) == 0);

        // 获取设备属性
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (SUCCEEDED(hr)) {
            // 获取友好名称
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
            if (SUCCEEDED(hr)) {
                info.name = varName.pwszVal;
            }
            std::cout << "into.name " << info.name.c_str() << std::endl;

            // 获取设备描述
            hr = pProps->GetValue(PKEY_Device_DeviceDesc, &varDesc);
            if (SUCCEEDED(hr)) {
                info.description = varDesc.pwszVal;
            }

            pProps->Release();
        }

        devices.push_back(info);

        // 清理
        CoTaskMemFree(pwszID);
        PropVariantClear(&varName);
        PropVariantClear(&varDesc);
        pDevice->Release();
    }

Exit:
    // 清理资源
    if (pDefaultDeviceId) CoTaskMemFree(pDefaultDeviceId);
    if (pCollection) pCollection->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();

    return devices;
}

int main() {
    // 枚举输出设备
    std::wcout << L"输出设备 (Render/Playback):\n";
    auto outputDevices = EnumerateAudioDevices(eRender);
    for (const auto& device : outputDevices) {
        std::wcout << L"  " << device.name << L" (" << device.description << L")";
        if (device.isDefault) std::wcout << L" [默认设备]";
        std::wcout << L"\n    ID: " << device.id << L"\n\n";
    }

    // 枚举输入设备
    std::wcout << L"\n输入设备 (Capture/Recording):\n";
    auto inputDevices = EnumerateAudioDevices(eCapture);
    for (const auto& device : inputDevices) {
        std::wcout << L"  " << device.name << L" (" << device.description << L")";
        if (device.isDefault) std::wcout << L" [默认设备]";
        std::wcout << L"\n    ID: " << device.id << L"\n\n";
    }

    return 0;
}