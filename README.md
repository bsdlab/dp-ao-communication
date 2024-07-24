# AO Communication APP
This app communicates with the API of the Alpha Omega machine and provides
functionality for:
1. Stream data from the AOs API buffer
2. Control the stimulation

## Building
Note the API requires windows specific headers!
### For Windows
1. download powershell:
	https://sourceforge.net/projects/powershell.mirror/files/latest/download
2. download python (add python to system path, can be done when installing)
	https://www.python.org/downloads/
3. Install Chocolatey (https://chocolatey.org/install):
	Execute in powershell with admin premissions:
		Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
4. Run in powershell:
	choco install make
5. Install cmake (cmake-3.24.0-rc3-windows-x86_64.msi):
	https://cmake.org/download/
	Add cmake to system path (can be done when installing)
6. Download MinGW-64, unzip copy mingw64 folder into C:/:
	https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/sjlj/x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0.7z/download

7. Add C:\mingw64\bin to system path

8. Build lsl -> not need -> just copy https://github.com/sccn/liblsl/releases/download/v1.16.0/lslexamples-1.16.0-Win_amd64.zip into the directory and copy dll into build of AO_API
	1. Remove CMakeCache.txt
	2. cmake -S .  -B . -G "MinGW Makefiles"
	3. make

USE LSL-1.16 VERSION -> 1.15 can not handle multiple streams!!!

9. build AO API
    1. Remove build/CMakeCache.txt
    2. cmake -S .  -B ./build -G "MinGW Makefiles"
    3. cd build/
    4. make

## TODOS
- [ ] restructure directory
- [ ] remove python dependencies
