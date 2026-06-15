#include "assert.h"
#include "raylib.h"

void *LoadLibraryW(const wchar_t *lpLibFileName);
int FreeLibrary(void * hLibModule);
void *GetProcAddress(void *hModule, const char *lpProcName);


int CopyFileW(
              const wchar_t *lpExistingFileName,
              const wchar_t *lpNewFileName,
              int bFailIfExists
              );
typedef enum _GET_FILEEX_INFO_LEVELS {
    GetFileExInfoStandard,
    GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

typedef struct _FILETIME {
    int dwLowDateTime;
    int dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
    int dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    int nFileSizeHigh;
    int nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

int GetFileAttributesExW(const wchar_t *lpFileName, 
                         GET_FILEEX_INFO_LEVELS fInfoLevelId,
                         void *lpFileInformation);


unsigned long long GetWriteTimeW(wchar_t *FileName){
    WIN32_FILE_ATTRIBUTE_DATA Data = {0};
    GetFileAttributesExW(FileName, GetFileExInfoStandard, &Data);
    return (unsigned long long)Data.ftLastWriteTime.dwLowDateTime | 
    ((unsigned long long)Data.ftLastWriteTime.dwHighDateTime << 32);
}

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(1920,1080,"Raylib Test");
    SetTargetFPS(60);
    if(!IsWindowReady()){
        return -1;
    }
    
    int IsRunning = 1;
    float dt = 0;
    
    
    void(*UpdateApp)(float*) = 0;
    void *AppDLL = 0;
    
    
    unsigned long long LastWriteTime = 0;
    LastWriteTime = GetWriteTimeW(L"app.dll"); 
    
    assert(CopyFileW(L"App.dll", L"App_tmp.dll", 0));
    AppDLL = LoadLibraryW(L"App_tmp.dll");
    assert(AppDLL);
    UpdateApp = (void(*)(float*)) GetProcAddress(AppDLL,"UpdateApp");
    
    
    
    
    
    
    while(!WindowShouldClose()){
        if(LastWriteTime != GetWriteTimeW(L"app.dll")){
            WIN32_FILE_ATTRIBUTE_DATA Ignored = {0};
            if(!GetFileAttributesExW(L"lock.tmp", GetFileExInfoStandard, &Ignored)){
                FreeLibrary(AppDLL);
                AppDLL = 0;
                LastWriteTime = GetWriteTimeW(L"app.dll"); 
                assert(CopyFileW(L"App.dll", L"App_tmp.dll", 0));
                AppDLL = LoadLibraryW(L"App_tmp.dll");
                assert(AppDLL);
                UpdateApp = (void(*)(float*)) GetProcAddress(AppDLL,"UpdateApp");
            }
        }
        UpdateApp(&dt);
    }
    
}
