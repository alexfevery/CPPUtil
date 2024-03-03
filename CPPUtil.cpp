
#include "CPPUtil.h"

using namespace std;


void Util::LogErrorTerminate_Debug(std::wstring ErrorMessage)
{
	__debugbreak();
	TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}
void Util::LogErrorTerminate_Release(std::wstring ErrorMessage)
{
	GlobalError = true;
	std::wstring logFilePath = GetLogFilePath(L"ErrorLog");
	Log(ConvertFunctionNameToWide(__FUNCTION__) + L" (Line: " + std::to_wstring(__LINE__) + L")\n" + ErrorMessage, L"ErrorLog", 30 * 86400);
	int msgboxID = MessageBoxW(GetForegroundWindow(), L"A fatal error occurred. Would you like to view the error log?", L"Fatal Error", MB_ICONERROR | MB_YESNO | MB_DEFBUTTON2);
	if (msgboxID == IDYES) { ShellExecuteW(NULL, L"open", logFilePath.c_str(), NULL, NULL, SW_SHOWNORMAL); }
	TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}


float Util::lerp(float a, float b, float t) {
	return a + t * (b - a);
}


void Util::PreventDuplicateAppInstance(wstring AppName)
{
	HANDLE hMutex = CreateMutexW(NULL, FALSE, AppName.c_str());
	Util_Assert(hMutex != nullptr, L"hMutex was null.");
	if (hMutex != nullptr)
	{
		DWORD dwMutexWaitResult = WaitForSingleObject(hMutex, 0);
		if (dwMutexWaitResult != WAIT_OBJECT_0)
		{
			MessageBox(HWND_DESKTOP, TEXT("This application is already running"), TEXT("Information"), MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
			CloseHandle(hMutex);
			TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
		}
	}
}

bool Util::usesIME(const std::wstring& languageName) 
{
	const std::vector<std::wstring> imeLanguages = {
		L"chinese",
		L"japanese",
		L"korean",
	};
	std::wstring lowerLanguageName = ToLower(languageName);
	for (const auto& imeLanguage : imeLanguages) 
	{
		if (StringContains(imeLanguage, languageName)) { return true; }
	}
	return false;
}

std::vector<std::tuple<std::wstring, HKL>> Util::GetInstalledKeyboardLayouts()
{
	int layoutCount = GetKeyboardLayoutList(0, nullptr);
	std::vector<HKL> layoutHandles(layoutCount);
	std::vector<std::tuple<std::wstring, HKL>> layouts;

	if (layoutCount > 0) {
		GetKeyboardLayoutList(layoutCount, layoutHandles.data());

		for (HKL layoutHandle : layoutHandles) {
			WCHAR layoutName[KL_NAMELENGTH];
			if (GetKeyboardLayoutName(layoutName)) 
			{
				WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
				if (LCIDToLocaleName(MAKELCID((DWORD)(uintptr_t)layoutHandle & 0x0000FFFF, SORT_DEFAULT), localeName, LOCALE_NAME_MAX_LENGTH, 0)) 
				{
					WCHAR languageName[LOCALE_NAME_MAX_LENGTH];
					if (GetLocaleInfoEx(localeName, LOCALE_SENGLISHLANGUAGENAME, languageName, LOCALE_NAME_MAX_LENGTH)) {
						layouts.push_back(std::make_tuple(std::wstring(languageName), layoutHandle));
					}
				}
			}
		}
	}
	return layouts;
}

bool Util::SetKeyboardLanguage(const std::wstring name) 
{
	auto installedLayouts = GetInstalledKeyboardLayouts();
	std::wstring lowerName = ToLower(name);
	for (const auto& layout : installedLayouts) {
		std::wstring lowerLayoutName = ToLower(std::get<0>(layout));
		if (StringContains(lowerLayoutName, lowerName))
		{
			SendMessageW(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(std::get<1>(layout)));
			return true;
		}
	}
	return false;
}

void Util::DisableIme() {
	auto installedLayouts = GetInstalledKeyboardLayouts();
	for (const auto& layout : installedLayouts) {
		if (!usesIME(std::get<0>(layout))) {
			SendMessageW(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, reinterpret_cast<LPARAM>(std::get<1>(layout)));
			break;
		}
	}
}


wstring Util::GetExecutionLocation()
{
	WCHAR exePath[MAX_PATH];
	wstring exeLocation;

	if (GetModuleFileNameW(NULL, exePath, MAX_PATH)) {
		exeLocation = wstring(exePath);
	}
	return exeLocation;
}

void Util::CreateShortcut(const std::wstring& targetPath, const std::wstring& shortcutPath)
{
	Util_Assert(SUCCEEDED(CoInitialize(NULL)), L"Failed to initialize COM library.");
	IShellLink* pShellLink = nullptr;
	Util_Assert(SUCCEEDED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink)), L"Failed to create instance of IShellLink.");
	pShellLink->SetPath(targetPath.c_str());
	IPersistFile* pPersistFile = nullptr;
	Util_Assert(SUCCEEDED(pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile)), L"Failed to query IPersistFile interface.");
	Util_Assert(SUCCEEDED(pPersistFile->Save(shortcutPath.c_str(), TRUE)), L"Failed to save shortcut.");
	pPersistFile->Release();
	pShellLink->Release();
	CoUninitialize();
}

std::wstring Util::StringWhere(const std::wstring& wstr, std::function<bool(wchar_t)> func)
{
	std::wstring result;
	for (wchar_t c : wstr)
	{
		if (func(c)) { result += c; }
	}
	return result;
}

std::wstring Util::StringExcept(const std::wstring& wstr, std::function<bool(wchar_t)> func)
{
	std::wstring result;
	for (wchar_t c : wstr)
	{
		if (!func(c)) { result += c; }
	}
	return result;
}

bool Util::IsAll(const std::wstring& wstr, std::function<bool(wchar_t)> func) {
	for (wchar_t ch : wstr) {
		if (!func(ch)) {
			return false;
		}
	}
	return true;
}

bool Util::IsAny(const std::wstring& wstr, std::function<bool(wchar_t)> func) {
	for (wchar_t ch : wstr) {
		if (func(ch)) {
			return true;
		}
	}
	return false;
}

std::wstring Util::GetApplicationDataPath(const std::wstring& appname, const std::wstring& folder)
{
	PWSTR appDataPath = NULL;
	std::wstring fullPath;

	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath)))
	{
		Util_Assert(appDataPath, L"AppDataPath is NULL");

		fullPath = std::wstring(appDataPath) + L"\\" + appname + L"\\" + folder;

		if (!CreateDirectoryW(fullPath.c_str(), NULL)) {
			DWORD error = GetLastError();
			if (error != ERROR_ALREADY_EXISTS) {
				std::wstringstream ss;
				ss << L"Failed to create directory: " << fullPath << L" Error: " << error;
				Util_LogErrorTerminate(ss.str());
			}
		}
		CoTaskMemFree(appDataPath);
	}
	else {
		Util_LogErrorTerminate(L"SHGetKnownFolderPath failed");
	}

	return fullPath;
}



string Util::execCommand(const string& command)
{
	wstring wcommand = utf8_to_wstring(command);
	vector<wchar_t> wcommandBuffer(wcommand.begin(), wcommand.end());
	wcommandBuffer.push_back(L'\0');

	SECURITY_ATTRIBUTES securityAttr = {};
	securityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttr.bInheritHandle = TRUE;
	securityAttr.lpSecurityDescriptor = NULL;

	HANDLE readPipe, writePipe;
	if (!CreatePipe(&readPipe, &writePipe, &securityAttr, 0)) {
		return "";
	}

	STARTUPINFOW startupInfo = { 0 };
	startupInfo.cb = sizeof(STARTUPINFOW);
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.wShowWindow = SW_HIDE;
	startupInfo.hStdOutput = writePipe;
	startupInfo.hStdError = writePipe;

	PROCESS_INFORMATION processInfo;
	if (!CreateProcessW(NULL, &wcommandBuffer[0], NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo)) {
		CloseHandle(readPipe);
		CloseHandle(writePipe);
		return "";
	}

	const int bufferSize = 4096;
	DWORD bytesRead;
	char buffer[bufferSize] = {};
	string output;
	while (true) {
		DWORD bytesAvailable = 0;
		if (!PeekNamedPipe(readPipe, NULL, 0, NULL, &bytesAvailable, NULL)) {
			break;
		}

		if (bytesAvailable > 0) {
			if (ReadFile(readPipe, buffer, bufferSize - 1, &bytesRead, NULL) && bytesRead > 0) {
				buffer[bytesRead] = '\0';
				output.append(buffer);
			}
		}
		else {
			DWORD exitCode;
			GetExitCodeProcess(processInfo.hProcess, &exitCode);
			if (exitCode != STILL_ACTIVE) {
				break;
			}
			Sleep(100);
		}
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	CloseHandle(readPipe);
	CloseHandle(writePipe);

	return output;
}

bool Util::downloadFile(const std::wstring& url, const std::wstring& destinationPath)
{
	// Convert the URLs to UTF8
	std::string encoded_url = Util::wstring_to_utf8(url);
	std::string utf8_destination_path = Util::wstring_to_utf8(destinationPath);

	// Check if the file already exists
	if (PathFileExistsW(destinationPath.c_str())) { return true; }

	// Construct the command to check HTTP status without downloading the content
	std::string check_status_command = "curl -s -o /dev/null -w \"%{http_code}\" -H \"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0 Safari/537.36\" \"" + encoded_url + "\"";
	std::string response_code = Util::execCommand(check_status_command);

	// Only download if the response code is 200 (OK)
	if (response_code == "200") {
		// Construct the command to download the file
		std::string download_command = "curl -s -H \"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0 Safari/537.36\" -o " + utf8_destination_path + " \"" + encoded_url + "\"";
		Util::execCommand(download_command);

		// Check if the file was downloaded successfully
		std::ifstream file(destinationPath, std::ios::binary);
		Util_Assert(file, L"File download failed. file: " + destinationPath);
		file.close();
	}
	else
	{
		return false;
	}
	return true;
}


string Util::wstring_to_urlencoded(const wstring& wstring)
{
	ostringstream escaped;
	escaped.fill('0');
	escaped << hex;

	for (wchar_t wc : wstring) {
		int length = WideCharToMultiByte(CP_UTF8, 0, &wc, 1, nullptr, 0, nullptr, nullptr);
		string utf8_char(length, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wc, 1, &utf8_char[0], length, nullptr, nullptr);

		for (char c : utf8_char) {
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
			}
			else {
				escaped << '%' << setw(2) << static_cast<int>(static_cast<unsigned char>(c));
			}
		}
	}

	return escaped.str();
}


string Util::getDeviceIdentifier() {
	// Get the volume serial number
	DWORD volumeSerialNumber;
	if (!GetVolumeInformationA("C:\\", NULL, 0, &volumeSerialNumber, NULL, NULL, NULL, 0)) {
		return "";
	}
	// Get the computer name
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName);
	if (!GetComputerNameA(computerName, &size)) {
		return "";
	}
	// Get the processor information
	int cpuInfo[4] = { 0 };
	__cpuid(cpuInfo, 0);
	stringstream cpuId;
	cpuId << hex << cpuInfo[0] << "-" << cpuInfo[1] << "-" << cpuInfo[2] << "-" << cpuInfo[3];
	// Combine components into a unique identifier
	stringstream identifier;
	identifier << "VSN-" << hex << volumeSerialNumber
		<< "-CN-" << computerName
		<< "-CPU-" << cpuId.str();

	return identifier.str();
}


wstring Util::uploadFile(const wstring& wstr, const string serverUrl) {
	string utf8Str = wstring_to_utf8(wstr);

	// Retrieve the path to the system's temporary directory
	TCHAR tempPath[MAX_PATH];
	DWORD result = GetTempPath(MAX_PATH, tempPath);
	if (result == 0 || result > MAX_PATH) { return L"failed to retrieve temporary directory path"; }

	// Create the temporary file path
	wstring tempFilePath = wstring(tempPath) + L"temp.txt";
	remove(wstring_to_utf8(tempFilePath).c_str());
	// Create a temporary file to save the data
	ofstream file;
	file.open(tempFilePath);
	if (!file.is_open()) { return L"failed to create temporary file"; }
	file << utf8Str;
	file.close();

	string body = "mac=" + getDeviceIdentifier() + "&file=temp.txt";
	string command = "curl -X POST -s -H -o /dev/null \"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0 Safari/537.36\" -H \"Content-Type: multipart/form-data\" --form \"file=@" + wstring_to_utf8(tempFilePath) + "\" --form \"mac=" + getDeviceIdentifier() + "\" \"" + serverUrl + "\"";

	string response = execCommand(command);
	// Cleanup: Remove the temporary file
	remove(wstring_to_utf8(tempFilePath).c_str());
	// Convert the response string to wstring
	wstring wresponse = utf8_to_wstring(response);

	return wresponse;
}

wstring Util::UploadMessage(wstring wstr, const string serverUrl)
{
	string command = "curl -s -H \"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0 Safari/537.36\" \"" + serverUrl + "?mac=" + getDeviceIdentifier();
	string str = wstring_to_utf8(wstr);
	command += "&message=" + str;

	command += "\"";
	string response = Util::execCommand(command);
	wstring wresponse = Util::utf8_to_wstring(response);
	return wresponse;
}


wstring Util::ConvertFunctionNameToWide(const char* FunctionName)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, FunctionName, -1, NULL, 0);
	wstring functionNameWide(bufferSize - 1, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, FunctionName, -1, &functionNameWide[0], bufferSize);

	return functionNameWide;
}

std::wstring Util::GetFileName(const std::wstring& path, bool withExtension)
{
	size_t pos = path.find_last_of(L"/\\");
	std::wstring filename = (pos == std::wstring::npos) ? path : path.substr(pos + 1);
	if (!withExtension)
	{
		size_t extPos = filename.find_last_of(L".");
		if (extPos != std::wstring::npos) { filename = filename.substr(0, extPos); }
	}
	return filename;
}

int Util::Max(int a, int b) {
	return (a > b) ? a : b;
}

int Util::Min(int a, int b) {
	return (a < b) ? a : b;
}

float Util::Max(float a, float b) {
	return (a > b) ? a : b;
}

float Util::Min(float a, float b) {
	return (a < b) ? a : b;
}

int Util::Clamp(int value, int minVal, int maxVal) {
	return Max(minVal, Min(value, maxVal));
}

float Util::Clamp(float value, float minVal, float maxVal) {
	return Max(minVal, Min(value, maxVal));
}

void Util::OpenLog(const std::wstring& logFileName)
{
	wstring logFilePath = Util::GetLogFilePath(logFileName);
	ShellExecuteW(NULL, L"open", logFilePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

wstring Util::GetLogFilePath(const wstring& logFileName)
{
	PWSTR appDataPath = NULL;
	wstring logFilePath;

	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath)))
	{
		wstring exeFullPath = GetExecutionLocation();
		if (!exeFullPath.empty())
		{
			wstring exeName = exeFullPath.substr(exeFullPath.find_last_of(L"\\/") + 1);
			exeName = exeName.substr(0, exeName.find_last_of(L"."));

			wstring appSpecificPath = wstring(appDataPath) + L"\\" + exeName;
			CreateDirectoryW(appSpecificPath.c_str(), NULL);
			logFilePath = appSpecificPath + L"\\" + logFileName + L".txt";
		}
		CoTaskMemFree(appDataPath);
	}

	return logFilePath;
}

void Util::Log(const wstring& Message, const wstring& logFileName, int TTL_Seconds)
{
	wstring logFilePath = Util::GetLogFilePath(logFileName);
	if (!logFilePath.empty())
	{
		wofstream logFile(logFilePath, ios::app);
		if (!logFile)
		{
			MessageBoxW(GetForegroundWindow(), L"Could not open log file.", L"File Error", MB_ICONERROR | MB_OK);
			TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
			return;
		}
		auto now = chrono::system_clock::now();
		time_t now_c = chrono::system_clock::to_time_t(now);
		struct tm local_time;
		localtime_s(&local_time, &now_c);
		logFile.close();
		wifstream readFile(logFilePath);
		vector<wstring> newLines;
		wstring line;
		bool TimeThresholdMet = false;

		while (getline(readFile, line))
		{
			struct tm logTime = {};
			wistringstream iss(line);
			if (!TimeThresholdMet)
			{
				if (iss >> get_time(&logTime, L"%Y-%m-%d %H:%M:%S") && difftime(mktime(&local_time), mktime(&logTime)) <= TTL_Seconds)
				{
					TimeThresholdMet = true;
					newLines.push_back(line);
				}
			}
			else { newLines.push_back(line); }
		}
		// Overwrite log file with new content
		readFile.close();
		DeleteFileW(logFilePath.c_str()); // Delete the file
		if (GetLastError() != 0)
		{
			MessageBoxW(GetForegroundWindow(), L"Could not delete old log file.", L"File Error", MB_ICONERROR | MB_OK);
			TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
			return;
		}
		// Re-open log file in append mode
		logFile.open(logFilePath, ios::app);
		for (const auto& nl : newLines)
		{
			logFile << nl << L"\n";
		}
		wchar_t buff[20];
		wcsftime(buff, sizeof(buff) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &local_time);
		// Log the date/time, function name, line number, and error message
		logFile << buff << L" - " << Util::wstring_to_utf8(Message).c_str() << L"\n\n";
	}
}

void Util::RECTF::checkRectIntegrity() const
{
	Util_Assert(Bottom >= Top, L"Bottom cannot be above the top.");
	Util_Assert(Right >= Left, L"Right cannot be left to the left.");
}

bool Util::AnyProcessFullScreen()
{
	QUERY_USER_NOTIFICATION_STATE pquns;
	SHQueryUserNotificationState(&pquns);

	switch (pquns) {
	case QUNS_NOT_PRESENT:
	case QUNS_ACCEPTS_NOTIFICATIONS:
	case QUNS_QUIET_TIME:
		return false; // Not fullscreen
	case QUNS_BUSY:
	case QUNS_RUNNING_D3D_FULL_SCREEN:
	case QUNS_PRESENTATION_MODE:
	case QUNS_APP:
		return true; // Fullscreen
	default:
		return false; // Default to not fullscreen
	}
}

void Util::ShiftCursorPos(Util::Vector2 Distance)
{
	if (Distance.X == 0.0f && Distance.Y == 0.0f) { return; }
	Vector2 pos = GetCursorPos();
	SetCursorPos(static_cast<int>(pos.X + Distance.X), static_cast<int>(pos.Y + Distance.Y));
}

Util::Vector2 Util::GetCursorPos()
{
	POINT pt;
	GetCursorPos(&pt);
	return Util::Vector2(pt);
}

Util::RECTF Util::GetClientRect(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	return Util::RECTF(rc);
}

Util::Vector2 Util::GetClientAreaPos(HWND hwnd, Util::Vector2 pt)
{
	POINT point = { static_cast<LONG>(pt.X), static_cast<LONG>(pt.Y) };
	if (hwnd) { ScreenToClient(hwnd, &point); }
	return Util::Vector2(point);
}

Util::Vector2 Util::PosDistanceClientArea(HWND hwnd, Util::Vector2 pt)
{
	Util::Vector2 clientAreaPoint = GetClientAreaPos(hwnd, pt);
	Util::RECTF rc = GetClientRect(hwnd);
	if (rc.Contains(clientAreaPoint)) { return Util::Vector2(0, 0); }

	Util::Vector2 distance(0, 0);
	if (clientAreaPoint.X < rc.Left) { distance.X = clientAreaPoint.X - rc.Left; }
	else if (clientAreaPoint.X > rc.Right) { distance.X = clientAreaPoint.X - rc.Right; }
	if (clientAreaPoint.Y < rc.Top) { distance.Y = clientAreaPoint.Y - rc.Top; }
	else if (clientAreaPoint.Y > rc.Bottom) { distance.Y = clientAreaPoint.Y - rc.Bottom; }
	return distance;
}

Util::Vector2 Util::GetClientAreaCursorPos(HWND hwnd) {
	POINT pt;
	GetCursorPos(&pt);
	return GetClientAreaPos(hwnd, Util::Vector2(pt));
}

Util::Vector2 Util::CursorDistanceClientArea(HWND hwnd) {
	POINT pt;
	GetCursorPos(&pt);
	return PosDistanceClientArea(hwnd, Util::Vector2(pt));
}

Util::Vector2 Util::GetCenterOfClientArea(HWND hwnd)
{
	RECT clientRect;
	if (GetClientRect(hwnd, &clientRect)) {
		POINT center = {
			clientRect.left + (clientRect.right),
			clientRect.top + (clientRect.bottom)
		};
		return Util::Vector2(center);
	}
	return { 0, 0 };
}

std::function<bool(wchar_t)> Util::Or(const std::vector<std::function<bool(wchar_t)>>& funcs)
{
	return [funcs](wchar_t ch) {
		for (const auto& func : funcs)
		{
			if (func(ch)) { return true; }
		}
		return false;
		};
}

std::function<bool(wchar_t)> Util::And(const std::vector<std::function<bool(wchar_t)>>& funcs)
{
	return [funcs](wchar_t ch) {
		for (const auto& func : funcs)
		{
			if (!func(ch)) { return false; }
		}
		return true;
		};
}

vector<wstring> Util::Split(wstring input, wchar_t delimiter, bool Trim, bool RemoveEmpty)
{
	vector<wstring> returncollection;
	wstring ns;
	returncollection.push_back(ns);
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == delimiter)
		{
			wstring ns;
			returncollection.push_back(ns);
			continue;
		}
		returncollection.back() += input[i];
	}
	if (Trim)
	{
		for (int i = 0; i < returncollection.size(); i++)
		{
			returncollection[i] = Util::Trim(returncollection[i]);
		}
	}
	if (RemoveEmpty)
	{
		vector<wstring> temp;
		for (int i = 0; i < returncollection.size(); i++)
		{
			if (!returncollection[i].empty()) { temp.push_back(returncollection[i]); }
		}
		returncollection = temp;
	}
	return returncollection;
}

wstring Util::Longest(vector<wstring> strings) //get longest string in vector
{
	wstring word;
	int max = 0;
	for (int i = 0; i < size(strings); i++)
	{
		if (max < (strings[i]).length()) { word = strings[i]; }
	}
	return word;
}

bool Util::StringContains(wstring str, wstring substr)
{
	return str.find(substr) != wstring::npos;
}

wstring Util::to_percentage(double input)
{
	wstringstream stream;
	stream << fixed << setprecision(2) << (input * 100.0) << L"%";
	return stream.str();
}

wstring Util::utf8_to_wstring(const string& utf8_string)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, nullptr, 0);
	wstring wstring(length, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wstring[0], length);
	return wstring;
}


string Util::wstring_to_utf8(const wstring& str)
{
	wstring_convert<codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

bool Util::iswhitespace(const wstring& str)
{
	if (str.empty() || (str.length() == 1 && str[0] == L'\0')) { return true; }
	else {
		return all_of(str.begin(), str.end(), [](wchar_t c) {
			return iswspace(c);
			});
	}
}


void Util::MuteOtherApplications(bool mute)
{
	static mutex muteMutex;
	static bool muted = false;
	std::lock_guard<std::mutex> lock(muteMutex);
	if ((mute && muted) || (!mute && !muted)) { return; }
	else { muted = mute; }
	Util_Assert(SUCCEEDED(CoInitialize(NULL)), L"CoInitialize failed");
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	IMMDevice* defaultDevice = NULL;
	IAudioSessionManager2* sessionManager = NULL;

	Util_Assert(SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator)), L"Error creating device enumerator");
	Util_Assert(SUCCEEDED(deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice)), L"Error getting default audio endpoint");
	deviceEnumerator->Release();
	Util_Assert(SUCCEEDED(defaultDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_INPROC_SERVER, NULL, (void**)&sessionManager)), L"Error activating audio session manager");
	defaultDevice->Release();

	IAudioSessionEnumerator* sessionEnumerator = NULL;
	Util_Assert(SUCCEEDED(sessionManager->GetSessionEnumerator(&sessionEnumerator)), L"Error getting session enumerator");

	int sessionCount = 0;
	sessionEnumerator->GetCount(&sessionCount);

	for (int i = 0; i < sessionCount; i++) {
		IAudioSessionControl* ctrl = NULL;
		IAudioSessionControl2* ctrl2 = NULL;
		ISimpleAudioVolume* volume = NULL;

		Util_Assert(SUCCEEDED(sessionEnumerator->GetSession(i, &ctrl)), L"Error getting session");

		if (ctrl)
		{
			Util_Assert(SUCCEEDED(ctrl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&ctrl2)), L"Error querying IAudioSessionControl2");
			Util_Assert(SUCCEEDED(ctrl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&volume)), L"Error querying ISimpleAudioVolume");
			DWORD processID = 0;
			ctrl2->GetProcessId(&processID);

			if (mute && processID != GetCurrentProcessId()) { volume->SetMute(true, NULL); }
			else if (!mute) { volume->SetMute(false, NULL); }
			ctrl->Release();
			if (ctrl2) ctrl2->Release();
			if (volume) volume->Release();
		}
	}
	sessionEnumerator->Release();
	sessionManager->Release();
	CoUninitialize();
	return;
}




std::string Util::wideStringToBytes(const std::wstring& ws)
{
	std::string result;
	for (std::wstring::size_type i = 0; i < ws.size(); )
	{
		std::string charBits;
		wchar_t wc = ws[i];
		if (wc >= 0xD800 && wc <= 0xDBFF) {  // High surrogate
			// Check if the high surrogate is followed by a low surrogate
			if (i + 1 == ws.size() || ws[i + 1] < 0xDC00 || ws[i + 1] > 0xDFFF)
			{
				Util_LogErrorTerminate(L"Invalid surrogate pair");
				// Skip this character and continue with the next one
				++i;
				continue;
			}

			std::bitset<16> highBits(wc);
			std::bitset<16> lowBits(ws[++i]);
			charBits = highBits.to_string() + "," + lowBits.to_string();
		}
		else {
			if (wc >= 0xDC00 && wc <= 0xDFFF) {
				// This is a low surrogate without a preceding high surrogate
				Util_LogErrorTerminate(L"Low surrogate without preceding high surrogate");
			}

			std::bitset<16> bits(wc);
			charBits = bits.to_string();
		}
		if (!result.empty()) {
			result += "\n";
		}
		result += "[" + charBits + "]";
		++i;
	}
	return result;
}

std::wstring Util::bytesToWideString(const std::string& binaryString)
{
	std::wstring result;
	std::istringstream stream(binaryString);

	std::string line;
	while (std::getline(stream, line)) {
		if (line.size() < 3 || line[0] != '[' || line[line.size() - 1] != ']') {
			Util_LogErrorTerminate(L"Line does not start and end with brackets");
			continue;
		}

		line = line.substr(1, line.size() - 2);
		std::stringstream ss(line);
		std::string token;

		std::vector<std::bitset<16>> bitsets;
		while (std::getline(ss, token, ',')) {
			try {
				bitsets.push_back(std::bitset<16>(token));
			}
			catch (...) {
				Util_LogErrorTerminate(L"Invalid binary number");
				// Skip this token and continue with the next one
				continue;
			}
		}

		if (bitsets.size() == 2) {
			wchar_t highSurrogate = static_cast<wchar_t>(bitsets[0].to_ulong());
			wchar_t lowSurrogate = static_cast<wchar_t>(bitsets[1].to_ulong());

			// Check if the high surrogate and low surrogate form a valid pair
			if (highSurrogate < 0xD800 || highSurrogate > 0xDBFF || lowSurrogate < 0xDC00 || lowSurrogate > 0xDFFF) {
				Util_LogErrorTerminate(L"Invalid surrogate pair");
				continue;
			}

			result += highSurrogate;
			result += lowSurrogate;
		}
		else if (bitsets.size() == 1) {
			wchar_t wc = static_cast<wchar_t>(bitsets[0].to_ulong());

			// Check if the character is a standalone surrogate
			if ((wc >= 0xD800 && wc <= 0xDBFF) || (wc >= 0xDC00 && wc <= 0xDFFF)) {
				Util_LogErrorTerminate(L"Standalone surrogate");
			}

			result += wc;
		}
		else {
			Util_LogErrorTerminate(L"Line contains more than two binary numbers");
		}
	}

	return result;
}


int Util::LevenshteinDistance(wstring s1, wstring s2)
{
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
	unsigned int s1len, s2len, x, y, lastdiag, olddiag;
	s1len = static_cast<int>(s1.size());
	s2len = static_cast<int>(s2.size());
	unsigned int* column = new unsigned int[s1len + 1];

	for (y = 1; y <= s1len; ++y) { column[y] = y; }

	for (x = 1; x <= s2len; ++x)
	{
		column[0] = x;

		for (y = 1, lastdiag = x - 1; y <= s1len; ++y)
		{
			olddiag = column[y];
			column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
			lastdiag = olddiag;
		}
	}
	return(column[s1len]);
}

int64_t Util::GetTimeStamp()
{
	int64_t timestamp = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	return timestamp;
}


wstring Util::Trim(wstring str)
{
	str.erase(str.find_last_not_of(L" \f\n\r\t\v") + 1);
	str.erase(0, str.find_first_not_of(L" \f\n\r\t\v"));
	return str;
}

wstring Util::ToLower(wstring str)
{
	transform(str.begin(), str.end(), str.begin(), towlower);
	return str;
}

wstring Util::EraseAllSubStr(wstring Str, const wstring& toErase)
{
	size_t pos = wstring::npos;
	while ((pos = Str.find(toErase)) != wstring::npos)
	{
		Str.erase(pos, toErase.length());
	}
	return Str;
}

wstring Util::ReplaceSubStrAt(wstring Str, int StartIndex, int EndIndex, const wstring& ReplaceWith)
{
	if (StartIndex >= 0 && EndIndex >= StartIndex && EndIndex <= Str.size())
	{
		size_t count = static_cast<size_t>(EndIndex) - static_cast<size_t>(StartIndex);
		Str.replace(StartIndex, count, ReplaceWith);
	}
	else { Util_LogErrorTerminate(L"Error bad replace index"); }
	return Str;
}


wstring Util::FloatToWstring(float number, int roundTo)
{
	return std::to_wstring(std::round(number * std::pow(10, roundTo)) / std::pow(10, roundTo)).substr(0, std::to_wstring(std::round(number * std::pow(10, roundTo)) / std::pow(10, roundTo)).find(L".") + roundTo + 1);
}

wstring Util::ReplaceAllSubStr(wstring Str, const wstring& toErase, const wstring& ReplaceWith)
{
	size_t pos = wstring::npos;
	while ((pos = Str.find(toErase)) != wstring::npos)
	{
		Str.replace(pos, toErase.length(), ReplaceWith);
	}
	return Str;
}

wstring Util::ReplaceAllSubStrWhere(wstring Str, std::function<bool(wchar_t)> func, const wstring& ReplaceWith) {
	for (size_t i = 0; i < Str.length(); ++i) {
		if (func(Str[i])) {
			Str.replace(i, 1, ReplaceWith);
			i += ReplaceWith.length() - 1;
		}
	}
	return Str;
}

wstring Util::EraseAllSubStrWhere(wstring Str, std::function<bool(wchar_t)> func) {
	wstring result;
	for (wchar_t ch : Str) {
		if (!func(ch)) {
			result += ch;
		}
	}
	return result;
}


bool Util::StringsEqual(const wstring& s1, const wstring& s2)
{
	if (s1.length() != s2.length()) { return false; }
	for (size_t i = 0; i < s1.length(); ++i)
	{
		if (s1[i] != s2[i]) { return false; }
	}
	return true;
}

wstring Util::Join(vector<wstring> arr, wstring delimiter)
{
	if (arr.empty()) { return L""; }
	wstring str;
	for (auto& i : arr)
	{
		str += i + delimiter;
	}
	str = str.substr(0, str.size() - delimiter.size());
	return str;
}

