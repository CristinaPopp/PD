#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")


void ListUSBDevices() {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD i;

    std::wcout << L"\nLista dispozitivelor USB conectate:\n";

    deviceInfoSet = SetupDiGetClassDevs(
        nullptr, L"USB", nullptr,
        DIGCF_ALLCLASSES | DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare la SetupDiGetClassDevs" << std::endl;
        return;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        TCHAR buffer[1000];
        DWORD buffersize = 0;

        if (SetupDiGetDeviceRegistryProperty(
            deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC,
            nullptr, (PBYTE)buffer, sizeof(buffer), &buffersize)) {
            std::wcout << L" - Dispozitiv: " << buffer << std::endl;
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}


void AllocatePages() {
    int N;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;

    std::cout << "\n Alocare memorie cu VirtualAlloc:" << std::endl;
    std::cout << "Dimensiunea unei pagini: " << pageSize << " bytes" << std::endl;
    std::cout << "Introdu numarul de pagini de alocat: ";
    std::cin >> N;

    SIZE_T totalSize = N * pageSize;
    LPVOID allocated = VirtualAlloc(
        nullptr,            
        totalSize,           
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE       
    );

    if (allocated == nullptr) {
        std::cerr << "Eroare la VirtualAlloc: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Memorie alocata cu succes pentru " << N << " pagini (" << totalSize << " bytes)." << std::endl;
        VirtualFree(allocated, 0, MEM_RELEASE);
    }
}

int main() {

    ListUSBDevices();    
    AllocatePages();     

    return 0;
}
