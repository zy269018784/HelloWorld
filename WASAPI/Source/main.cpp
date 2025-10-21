#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ole32.lib")

// �豸��Ϣ�ṹ��
struct AudioDeviceInfo {
    std::wstring id;
    std::wstring name;
    std::wstring description;
    bool isDefault = false;
};

// ö����Ƶ�豸
std::vector<AudioDeviceInfo> EnumerateAudioDevices(EDataFlow dataFlow) 
{
    std::vector<AudioDeviceInfo> devices;

    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDeviceCollection* pCollection = NULL;
    IMMDevice* pDefaultDevice = NULL;
    LPWSTR pDefaultDeviceId = NULL;
    UINT count = 0;

    // ��ʼ��COM
    CoInitialize(NULL);

    // �����豸ö����
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

    // ��ȡĬ���豸ID
    hr = pEnumerator->GetDefaultAudioEndpoint(dataFlow, eConsole, &pDefaultDevice);
    if (SUCCEEDED(hr)) {
        hr = pDefaultDevice->GetId(&pDefaultDeviceId);
        pDefaultDevice->Release();
    }

    // ��ȡ�����豸����
    hr = pEnumerator->EnumAudioEndpoints(dataFlow, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) goto Exit;

    // ��ȡ�豸����
    hr = pCollection->GetCount(&count);
    if (FAILED(hr)) goto Exit;

    std::cout << "count "  << count << std::endl;

    // ���������豸
    for (UINT i = 0; i < count; i++) {
        IMMDevice* pDevice = NULL;
        IPropertyStore* pProps = NULL;
        PROPVARIANT varName, varDesc;
        LPWSTR pwszID = NULL;

        PropVariantInit(&varName);
        PropVariantInit(&varDesc);

        // ��ȡ�豸
        hr = pCollection->Item(i, &pDevice);
        if (FAILED(hr)) continue;

        // ��ȡ�豸ID
        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr)) {
            pDevice->Release();
            continue;
        }

        AudioDeviceInfo info;
        info.id = pwszID;
        info.isDefault = (pDefaultDeviceId && wcscmp(pwszID, pDefaultDeviceId) == 0);

        // ��ȡ�豸����
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (SUCCEEDED(hr)) {
            // ��ȡ�Ѻ�����
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
            if (SUCCEEDED(hr)) {
                info.name = varName.pwszVal;
            }
            std::cout << "into.name " << info.name.c_str() << std::endl;

            // ��ȡ�豸����
            hr = pProps->GetValue(PKEY_Device_DeviceDesc, &varDesc);
            if (SUCCEEDED(hr)) {
                info.description = varDesc.pwszVal;
            }

            pProps->Release();
        }

        devices.push_back(info);

        // ����
        CoTaskMemFree(pwszID);
        PropVariantClear(&varName);
        PropVariantClear(&varDesc);
        pDevice->Release();
    }

Exit:
    // ������Դ
    if (pDefaultDeviceId) CoTaskMemFree(pDefaultDeviceId);
    if (pCollection) pCollection->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();

    return devices;
}

int main() {
    // ö������豸
    std::wcout << L"����豸 (Render/Playback):\n";
    auto outputDevices = EnumerateAudioDevices(eRender);
    for (const auto& device : outputDevices) {
        std::wcout << L"  " << device.name << L" (" << device.description << L")";
        if (device.isDefault) std::wcout << L" [Ĭ���豸]";
        std::wcout << L"\n    ID: " << device.id << L"\n\n";
    }

    // ö�������豸
    std::wcout << L"\n�����豸 (Capture/Recording):\n";
    auto inputDevices = EnumerateAudioDevices(eCapture);
    for (const auto& device : inputDevices) {
        std::wcout << L"  " << device.name << L" (" << device.description << L")";
        if (device.isDefault) std::wcout << L" [Ĭ���豸]";
        std::wcout << L"\n    ID: " << device.id << L"\n\n";
    }

    return 0;
}