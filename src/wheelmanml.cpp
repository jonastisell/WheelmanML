#include <windows.h>
#include "config.h"
#include "wheelmanml.h"
#include "asi_loader.h"
#include "logger.h"

DWORD WINAPI CustomCodeThread(LPVOID lpParam) {
  InitializeLog();
  Log(std::string(PROJECT_NAME) + " started");

  LoadASIPlugins();
  Log("Finished loading ASI plugins");

  return 0;
}