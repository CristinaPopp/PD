#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SERVICES_KEY "SYSTEM\\CurrentControlSet\\Services"
#define IMAGE_PATH "ImagePath"
#define TYPE "Type"

void PrintImagePathForServices(int filterDrivers)
{
    HKEY hKey;
    DWORD index = 0;
    char subKeyName[256];
    DWORD subKeySize = sizeof(subKeyName);
    FILETIME lastWriteTime;


    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, SERVICES_KEY, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        printf("Nu s-a putut deschide cheia registry!\n");
        return;
    }


    while (RegEnumKeyEx(hKey, index, subKeyName, &subKeySize, NULL, NULL, NULL, &lastWriteTime) == ERROR_SUCCESS) {
        HKEY hSubKey;
        char imagePath[512];
        DWORD imagePathSize = sizeof(imagePath);
        DWORD typeValue = 0, typeSize = sizeof(typeValue);
        DWORD type;


        if (RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            int isDriver = 0;


            if (filterDrivers) {
                if (RegQueryValueEx(hSubKey, TYPE, NULL, &type, (LPBYTE)&typeValue, &typeSize) == ERROR_SUCCESS) {
                    if (typeValue == 0x01 || typeValue == 0x02) {
                        isDriver = 1;
                    }
                }
            }
            else {
                isDriver = 1;
            }


            if (isDriver && RegQueryValueEx(hSubKey, IMAGE_PATH, NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS) {
                if (type == REG_SZ) {
                    printf("Serviciu: %s\n", subKeyName);
                    printf("ImagePath: %s\n\n", imagePath);
                }
            }


            RegCloseKey(hSubKey);
        }

        subKeySize = sizeof(subKeyName);
        index++;
    }

    RegCloseKey(hKey);
}

int main(int argc, char* argv[])
{
    int filterDrivers = 0;

    if (argc > 1 && strcmp(argv[1], "drivers") == 0) {
        filterDrivers = 1;
        printf("Afisare doar pentru drivere:\n\n");
    }
    else {
        printf("Afisare pentru toate serviciile:\n\n");
    }

    PrintImagePathForServices(filterDrivers);
    return 0;
}
