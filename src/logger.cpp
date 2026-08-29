#include <windows.h>
#include <fstream>
#include <string>

#include "logger.h"
#include "config.h"

std::string logPath;


void InitializeLog()
{
  char modulePath[MAX_PATH];

  GetModuleFileNameA(NULL, modulePath, MAX_PATH);

  std::string directory(modulePath);

  size_t pos = directory.find_last_of("\\/");

  if (pos != std::string::npos)
  {
    directory = directory.substr(0, pos + 1);
  }

  logPath = directory + "asi_log.txt";

  std::ofstream logFile(logPath, std::ios::out | std::ios::trunc);

  if (logFile.is_open())
  {
    logFile << PROJECT_NAME << " Log\n";
    logFile << "====================\n";
    logFile.close();
  }
}


void Log(const std::string& message)
{
  std::ofstream logFile(logPath, std::ios::out | std::ios::app);

  if (logFile.is_open())
  {
    logFile << message << "\n";
    logFile.close();
  }
}