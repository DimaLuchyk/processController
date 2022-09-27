#pragma once

#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <Windows.h>
#include <fstream>

class Controller
{
public:
	Controller(const std::wstring& progPaht)
		: m_progPath{ progPaht },
		m_timeToRestart{true}
	{
		//Zero memory of STARTUPINFO and PROCESS_INFORMATION
		ZeroMemory(&m_si, sizeof(m_si));
		m_si.cb = sizeof(m_si);
		ZeroMemory(&m_pi, sizeof(m_pi));

		//m_logFile.open("logFileNew.txt", std::ios::out | std::ios::in);

	}

	bool StartProcess()
	{
		if (!CreateProcess(
			m_progPath.c_str(),           //Program name
			NULL,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&m_si,            // Pointer to STARTUPINFO structure
			&m_pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			CloseHandle(m_pi.hProcess);
			CloseHandle(m_pi.hThread);
			return false;
		}

		return true;

	}



	void ThreadEntry()
	{
		for (int i = 0; true; i++)
		{
			if (m_timeToRestart)
			{
				Log("about to restart the process");
				std::this_thread::sleep_for(std::chrono::microseconds(60000));
				//std::this_thread::sleep_for(std::chrono::milliseconds(5000));

				m_timeToRestart = false;
				TerminateProcess(m_pi.hProcess, (UINT)0);
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));

				if (!StartProcess())
				{
					Log("Process failed to start");
					break;
				}
			}

			if (!(i % 150))
			{
				Log("ThreadEntry is alive");
			}
			
			if (!(i % 18000))
			{
				m_timeToRestart = true;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}


			

	}

	void Log(const std::string& message)
	{
		std::stringstream timeStream;
		auto time1 = std::chrono::system_clock::now();
		std::time_t timeConverted = std::chrono::system_clock::to_time_t(time1);
		timeStream << std::ctime(&timeConverted) << " | " << message << std::endl;

		std::cout << timeStream.str();
	}

	~Controller()
	{
		//m_logFile.close();
	}

private:
	std::wstring m_progPath;
	STARTUPINFO m_si;
	PROCESS_INFORMATION m_pi;


	//std::fstream m_logFile;

	std::atomic<bool> m_timeToRestart;
};

