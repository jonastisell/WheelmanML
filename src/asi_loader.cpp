#include <windows.h>
#include <string>

#include "asi_loader.h"
#include "logger.h"

std::string WideToUtf8(const std::wstring& wideString)
{
  if (wideString.empty())
  {
    return "";
  }

  int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), static_cast<int>(wideString.size()), nullptr, 0, nullptr, nullptr);

  if (sizeNeeded == 0)
  {
    return "";
  }

  std::string result(sizeNeeded, '\0');

  WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), static_cast<int>(wideString.size()), &result[0], sizeNeeded, nullptr, nullptr);

  return result;
}

void LoadASIPlugins()
{
  wchar_t gamePath[MAX_PATH];

  GetModuleFileNameW(NULL, gamePath, MAX_PATH);

  std::wstring directory(gamePath);

  size_t pos = directory.find_last_of(L"\\/");

  if (pos != std::wstring::npos)
  {
    directory = directory.substr(0, pos + 1);
  }

  std::wstring searchPath = directory + L"*.asi";

  WIN32_FIND_DATAW findData;

  HANDLE findHandle = FindFirstFileW(searchPath.c_str(), &findData);

  if (findHandle == INVALID_HANDLE_VALUE)
  {
    Log("No ASI plugins found");
    return;
  }

  Log("Searching for ASI plugins ...");

  do
  {
    if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      std::wstring asiPath = directory + findData.cFileName;
      std::wstring wideAsiName = findData.cFileName;
      std::string asiName = WideToUtf8(wideAsiName);

      Log("Loading ASI plugin: " + asiName);

      HMODULE asiModule = LoadLibraryW(asiPath.c_str());

      if(asiModule){
        Log("Successfully loaded: " + asiName);
      }else{
        DWORD error = GetLastError();
        Log("FAILED to load: " + asiName + " (Error " + std::to_string(error) + ")");
      }
    }

  } while (FindNextFileW(findHandle, &findData));

  FindClose(findHandle);
}