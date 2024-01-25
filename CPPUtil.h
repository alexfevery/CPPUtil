#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <comdef.h>
#include <shlobj.h>
#include <fstream>
#include <codecvt>
#include <string>
#include <Shlwapi.h>
#include <sstream>
#include <vector>
#include <iomanip>
#include <intrin.h>
#include <bitset>
#include <d2d1.h>
#include <functional>
#include <variant>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <filesystem>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Imm32.lib")



namespace Util
{
	inline bool GlobalError = false;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef NDEBUG
#define Util_LogErrorTerminate(ErrorMessage) \
	__debugbreak();

#define Util_Assert(condition, ErrorMessage) \
    if (!(condition)) { __debugbreak();}

	//A function for terminating the program if the code is reached with a not implemented message
#define Util_NotImplemented() \
	__debugbreak();

#define Util_D2DCall(operationResult) \
	if (FAILED(operationResult)) \
	{ \
		std::wstring errorMsg = L"Error: " + std::wstring(_com_error(operationResult).ErrorMessage()) + L" AFTER D2D CALL\n"; \
		OutputDebugString(errorMsg.c_str()); \
		__debugbreak(); \
	}

#else
#define Util_LogErrorTerminate(ErrorMessage) \
	Util::GlobalError = true; \
	std::wstring logFilePath = Util::GetLogFilePath(L"ErrorLog"); \
	Util::Log(Util::ConvertFunctionNameToWide(__FUNCTION__) + L" (Line: " + std::to_wstring(__LINE__) + L")\n" + ErrorMessage, L"ErrorLog", 30 * 86400); \
	int msgboxID = MessageBoxW(GetForegroundWindow(), L"A fatal error occurred. Would you like to view the error log?", L"Fatal Error", MB_ICONERROR | MB_YESNO | MB_DEFBUTTON2); \
	if (msgboxID == IDYES) { ShellExecuteW(NULL, L"open", logFilePath.c_str(), NULL, NULL, SW_SHOWNORMAL); } \
	TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);

#define Util_Assert(condition, ErrorMessage) \
	if(!(condition)) { Util_LogErrorTerminate(ErrorMessage);}

#define Util_NotImplemented() \
	Util_LogErrorTerminate(L"Error: Not implemented");

#define Util_D2DCall(operationResult) \
	if (FAILED(operationResult)) \
	{ \
		_com_error err(operationResult); \
		LPCTSTR errMsg = err.ErrorMessage(); \
		Util_LogErrorTerminate(L"D2D Error: " + std::wstring(errMsg)); \
	}

#endif

	std::wstring GetApplicationDataPath(const std::wstring& appname, const std::wstring& folder);
	std::string execCommand(const std::string& command);
	bool downloadFile(const std::wstring& url, const std::wstring& destinationPath);
	std::string wstring_to_urlencoded(const std::wstring& wstring);
	std::string getDeviceIdentifier();
	std::wstring uploadFile(const std::wstring& wstr, const std::string serverUrl);
	std::wstring UploadMessage(std::wstring wstr, const std::string serverUrl);
	std::wstring GetLogFilePath(const std::wstring& logFileName);
	std::wstring ConvertFunctionNameToWide(const char* FunctionName);
	std::wstring GetFileName(const std::wstring& path, bool withExtension);
	int Max(int a, int b);
	int Min(int a, int b);
	float Max(float a, float b);
	float Min(float a, float b);
	int Clamp(int value, int minVal, int maxVal);
	float Clamp(float value, float minVal, float maxVal);
	void Log(const std::wstring& Message, const std::wstring& logFileName, int TTL_Seconds);
	void OpenLog(const std::wstring& logFileName);
	std::vector<std::wstring> Split(std::wstring input, wchar_t delimiter, bool Trim = false, bool RemoveEmpty = false);
	std::wstring Longest(std::vector<std::wstring> strings);
	bool StringContains(std::wstring str, std::wstring substr);
	std::wstring to_percentage(double input);
	std::wstring utf8_to_wstring(const std::string& utf8_string);
	std::string wstring_to_utf8(const std::wstring& str);
	bool iswhitespace(const std::wstring& str);
	bool MuteOtherApplications(bool mute);
	std::string wideStringToBytes(const std::wstring& ws);
	std::wstring bytesToWideString(const std::string& binaryString);
	int LevenshteinDistance(std::wstring s1, std::wstring s2);
	std::wstring Trim(std::wstring str);
	std::wstring ToLower(std::wstring str);
	std::wstring EraseAllSubStr(std::wstring Str, const std::wstring& toErase);
	std::wstring ReplaceSubStrAt(std::wstring Str, int StartIndex, int EndIndex, const std::wstring& ReplaceWith);
	std::wstring ReplaceAllSubStr(std::wstring Str, const std::wstring& toErase, const std::wstring& ReplaceWith);
	std::wstring ReplaceAllSubStrWhere(std::wstring Str, std::function<bool(wchar_t)> func, const std::wstring& ReplaceWith);
	std::wstring EraseAllSubStrWhere(std::wstring Str, std::function<bool(wchar_t)> func);
	bool StringsEqual(const std::wstring& s1, const std::wstring& s2);
	bool AnyProcessFullScreen();
	std::wstring Join(std::vector<std::wstring> arr, std::wstring delimiter);
	void PreventDuplicateAppInstance(std::wstring AppName);
	std::wstring GetExecutionLocation();
	void CreateShortcut(const std::wstring& targetPath, const std::wstring& shortcutPath);

	std::function<bool(wchar_t)> Or(const std::vector<std::function<bool(wchar_t)>>& funcs);
	std::function<bool(wchar_t)> And(const std::vector<std::function<bool(wchar_t)>>& funcs);

	bool IsAll(const std::wstring& wstr, std::function<bool(wchar_t)> func);
	bool IsAny(const std::wstring& wstr, std::function<bool(wchar_t)> func);
	std::wstring StringWhere(const std::wstring& wstr, std::function<bool(wchar_t)> func);
	std::wstring StringExcept(const std::wstring& wstr, std::function<bool(wchar_t)> func);
	float lerp(float a, float b, float t);
	std::wstring FloatToWstring(float number, int roundTo);
	int64_t GetTimeStamp();

	template<typename T>
	static bool VectorContains(const std::vector<T>& vec, const T& item) {
		return std::find(vec.begin(), vec.end(), item) != vec.end();
	}
	template<typename T>
	static void VectorAddRange(std::vector<T>& AddTo, const std::vector<T>& AddFrom)
	{
		AddTo.insert(AddTo.end(), AddFrom.begin(), AddFrom.end());
	}

	class Vector2 {
	public:
		float X, Y;

		Vector2() : X(0), Y(0) {}
		Vector2(float i) : X(i), Y(i) {}
		Vector2(float x, float y) : X(x), Y(y) {}
		Vector2(POINT pos) : X(static_cast<float>(pos.x)), Y(static_cast<float>(pos.y)) {}
		Vector2(D2D1_POINT_2F pos) : X(static_cast<float>(pos.x)), Y(static_cast<float>(pos.y)) {}

		Vector2 operator+(const Vector2& rhs) const {
			return Vector2(X + rhs.X, Y + rhs.Y);
		}

		Vector2 operator-(const Vector2& rhs) const {
			return Vector2(X - rhs.X, Y - rhs.Y);
		}

		Vector2 operator+(const float rhs) const {
			return Vector2(X + rhs, Y + rhs);
		}

		Vector2 operator-(const float rhs) const {
			return Vector2(X - rhs, Y - rhs);
		}

		Vector2 operator-() const {
			return Vector2(-X, -Y);
		}

		Vector2 operator*(const Vector2& rhs) const {
			return Vector2(X * rhs.X, Y * rhs.Y);
		}

		Vector2 operator/(const Vector2& rhs) const {
			Util_Assert(rhs.X != 0.0f && rhs.Y != 0.0f, L"Division by 0");
			return Vector2(X / rhs.X, Y / rhs.Y);
		}

		Vector2 operator*(float scalar) const {
			return Vector2(X * scalar, Y * scalar);
		}

		Vector2 operator/(float scalar) const {
			Util_Assert(scalar != 0.0f, L"Division by 0");
			return Vector2(X / scalar, Y / scalar);
		}

		friend Util::Vector2 operator-(float scalar, const Util::Vector2& vec) {
			return Util::Vector2(scalar - vec.X, scalar - vec.Y);
		}

		friend Util::Vector2 operator+(float scalar, const Util::Vector2& vec) {
			return Util::Vector2(scalar + vec.X, scalar + vec.Y);
		}

		friend Util::Vector2 operator*(float scalar, const Util::Vector2& vec) {
			return Util::Vector2(scalar * vec.X, scalar * vec.Y);
		}

		friend Util::Vector2 operator/(float scalar, const Util::Vector2& vec) {
			Util_Assert(vec.X != 0.0f && vec.Y != 0.0f, L"Division by 0");
			return Util::Vector2(scalar / vec.X, scalar / vec.Y);
		}

		friend Util::Vector2& operator+=(Util::Vector2& lhs, const Util::Vector2& rhs) {
			lhs.X += rhs.X;
			lhs.Y += rhs.Y;
			return lhs;
		}

		friend Util::Vector2& operator-=(Util::Vector2& lhs, const Util::Vector2& rhs) {
			lhs.X -= rhs.X;
			lhs.Y -= rhs.Y;
			return lhs;
		}

		friend Util::Vector2& operator*=(Util::Vector2& lhs, const Util::Vector2& rhs) {
			lhs.X *= rhs.X;
			lhs.Y *= rhs.Y;
			return lhs;
		}

		friend Util::Vector2& operator/=(Util::Vector2& lhs, const Util::Vector2& rhs) {
			Util_Assert(rhs.X != 0.0f && rhs.Y != 0.0f, L"Division by 0");
			lhs.X /= rhs.X;
			lhs.Y /= rhs.Y;
			return lhs;
		}

		friend Util::Vector2& operator+=(Util::Vector2& lhs, float rhs) {
			lhs.X += rhs;
			lhs.Y += rhs;
			return lhs;
		}

		friend Util::Vector2& operator-=(Util::Vector2& lhs, float rhs) {
			lhs.X -= rhs;
			lhs.Y -= rhs;
			return lhs;
		}

		friend Util::Vector2& operator*=(Util::Vector2& lhs, float rhs) {
			lhs.X *= rhs;
			lhs.Y *= rhs;
			return lhs;
		}

		friend Util::Vector2& operator/=(Util::Vector2& lhs, float rhs) {
			Util_Assert(rhs != 0.0f, L"Division by 0");
			lhs.X /= rhs;
			lhs.Y /= rhs;
			return lhs;
		}

		bool operator==(const Vector2& rhs) const {
			return X == rhs.X && Y == rhs.Y;
		}

		bool operator!=(const Vector2& rhs) const {
			return !(*this == rhs);
		}

		float Min() const {
			return  (std::min)(X, Y);
		}

		float Max() const {
			return  (std::max)(X, Y);
		}

		float Length() const {
			return std::sqrt(X * X + Y * Y);
		}

		static float Distance(const Vector2& a, const Vector2& b) {
			float dx = b.X - a.X;
			float dy = b.Y - a.Y;
			return std::sqrt(dx * dx + dy * dy);
		}

		Vector2 Normalized() const {
			float len = Length();
			return *this / len;
		}

		Vector2 Rounded() const {
			return Vector2(round(X), round(Y));
		}

		static float Dot(const Vector2& a, const Vector2& b) {
			return a.X * b.X + a.Y * b.Y;
		}

		static float Angle(const Vector2& a, const Vector2& b)
		{
			float dot = Dot(a, b);
			float angleInRadians = std::acos(dot / (a.Length() * b.Length()));
			float angleInDegrees = angleInRadians * 180.0f / static_cast<float>(M_PI);
			return angleInDegrees;
		}

		operator D2D1_POINT_2F() const
		{
			return D2D1_POINT_2F{ X,Y };
		}
	};


	class RECTF {
	public:
		float Left, Top, Right, Bottom, Width, Height;

		RECTF() : Left(0), Top(0), Right(0), Bottom(0), Width(0), Height(0) {}

		RECTF(const Vector2& position) : Left(position.X), Top(position.Y), Right(0), Bottom(0), Width(0), Height(0) {}

		RECTF(const Vector2& position, float Width, float Height) : Left(position.X), Top(position.Y), Right(position.X + Width), Bottom(position.Y + Height), Width(Width), Height(Height) {}

		RECTF(const Vector2& position, const Vector2& size) : Left(position.X), Top(position.Y), Right(position.X + size.X), Bottom(position.Y + size.Y), Width(size.X), Height(size.X) {}


		RECTF(float left, float top, float right, float bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {
			checkRectIntegrity();
			Width = std::abs(Right - Left);
			Height = std::abs(Bottom - Top);
		}

		RECTF(RECT rc) : Left(static_cast<float>(rc.left)), Top(static_cast<float>(rc.top)), Right(static_cast<float>(rc.right)), Bottom(static_cast<float>(rc.bottom))
		{
			checkRectIntegrity();
			Width = std::abs(Right - Left);
			Height = std::abs(Bottom - Top);
		}

		RECTF(const D2D1_RECT_F& d2dRect) : Left(d2dRect.top), Top(d2dRect.left), Right(d2dRect.right), Bottom(d2dRect.bottom) {
			checkRectIntegrity();
			Width = std::abs(Right - Left);
			Height = std::abs(Bottom - Top);
		}
		Vector2 GetCenter() const { return Vector2((Left + Right) / 2.0f, (Top + Bottom) / 2.0f); }
		bool Contains(const Vector2& point) const { return (point.X >= Left && point.X <= Right) && (point.Y <= Bottom && point.Y >= Top); }
		bool Contains(const RECTF& otherRECTF) const { return (otherRECTF.Left >= Left && otherRECTF.Right <= Right) && (otherRECTF.Top >= Top && otherRECTF.Bottom <= Bottom); }
		bool Overlaps(const RECTF& otherRECTF) const { return (otherRECTF.Left < Right && otherRECTF.Right > Left) && (otherRECTF.Top < Bottom && otherRECTF.Bottom > Top); }
		RECTF Union(const RECTF& otherRECTF) const {
			float newLeft = (std::min)(Left, otherRECTF.Left);
			float newTop = (std::min)(Top, otherRECTF.Top);
			float newRight = (std::max)(Right, otherRECTF.Right);
			float newBottom = (std::max)(Bottom, otherRECTF.Bottom);

			return RECTF(newLeft, newTop, newRight, newBottom);
		}

		RECTF Resize(float scaleFactor, bool keepCenter) const
		{
			float newWidth = Width * scaleFactor;
			float newHeight = Height * scaleFactor;
			float newLeft, newTop;

			if (keepCenter) {
				Vector2 center = GetCenter();
				newLeft = center.X - newWidth / 2.0f;
				newTop = center.Y - newHeight / 2.0f;
			}
			else {
				newLeft = Left;
				newTop = Top;
			}

			float newRight = newLeft + newWidth;
			float newBottom = newTop + newHeight;

			return RECTF(newLeft, newTop, newRight, newBottom);
		}


		Vector2 GetSize() const { return Vector2(Width, Height); }
		Vector2 GetPosition() const { return Vector2(Left, Top); }
		void SetPosition(const Vector2& newPosition) {
			float width = Right - Left;
			float height = Bottom - Top;

			Left = newPosition.X;
			Top = newPosition.Y;
			Right = Left + width;
			Bottom = Top + height;

			checkRectIntegrity();
		}

		operator D2D1_RECT_F() const
		{
			return D2D1_RECT_F{ Left, Top, Right, Bottom };
		}



	private:
		void checkRectIntegrity();

	};

	class VariableMonitor {
	private:

		class Monitor {
		public:
			std::variant<int*, double*, bool*, float*, long*, std::wstring*> var;
			std::wstring units;

			template<typename T>
			Monitor(T* var, const std::wstring& units = L"") : var(var), units(units) {}

			std::wstring GetValueAsString() const {
				return std::visit([this](auto&& arg) -> std::wstring {
					using T = std::decay_t<decltype(*arg)>;
					if constexpr (std::is_same_v<T, bool>) {
						return *arg ? L"true" : L"false";
					}
					else if constexpr (std::is_same_v<T, std::wstring>) {
						return *arg; // Directly return the std::wstring value
					}
					else {
						return std::to_wstring(*arg) + L" " + units;
					}
					}, var);
			}

		};

		std::unordered_map<std::wstring, Monitor> monitors;

		VariableMonitor() {} // private constructor

	public:
		static VariableMonitor& GetInstance() {
			static VariableMonitor instance;
			return instance;
		}

		template<typename T>
		static void AddValueMonitor(const std::wstring& name, T* var, const std::wstring& units = L"") {
			auto& instance = GetInstance();
			auto it = instance.monitors.find(name);
			if (it != instance.monitors.end()) {
				it->second.var = var;
				it->second.units = units;
			}
			else {
				instance.monitors.emplace(name, Monitor(var, units));
			}
		}


		static void PrintMonitorValues(HWND hwnd) {
			std::wstring title;
			for (const auto& [name, monitor] : GetInstance().monitors) {
				title += name + L": " + monitor.GetValueAsString() + L" ";
			}
			SetWindowTextW(hwnd, title.c_str());
		}

		static void ClearMonitors() {
			GetInstance().monitors.clear();
		}
	};


	class Timer
	{
	public:
		static inline std::chrono::time_point<std::chrono::high_resolution_clock> startTime1, startTime2;
		static inline std::wstring name1, name2;
		static inline std::vector<float> intervals1, intervals2;
		static inline int intervalCount1, intervalCount2;
		static inline float averageTime1, averageTime2;

		static void StartTimer1(const std::wstring& name, int intervalCount) {
			StartTimer(startTime1, name, intervalCount, name1, intervals1, intervalCount1);
		}
		static void EndTimer1() {
			EndTimer(startTime1, intervals1, intervalCount1, name1, averageTime1);
		}

		static void StartTimer2(const std::wstring& name, int intervalCount) {
			StartTimer(startTime2, name, intervalCount, name2, intervals2, intervalCount2);
		}
		static void EndTimer2() {
			EndTimer(startTime2, intervals2, intervalCount2, name2, averageTime2);
		}

	private:
		static void StartTimer(std::chrono::time_point<std::chrono::high_resolution_clock>& startTime, const std::wstring& name, int intervalCount, std::wstring& timerName, std::vector<float>& intervals, int& storedIntervalCount)
		{
			timerName = name;
			storedIntervalCount = intervalCount;
			startTime = std::chrono::high_resolution_clock::now();
		}

		static void EndTimer(std::chrono::time_point<std::chrono::high_resolution_clock>& startTime, std::vector<float>& intervals, int& intervalCount, const std::wstring& timerName, float& averageTime)
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
			long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
			intervals.push_back(static_cast<float>(elapsed));

			if (intervals.size() >= static_cast<size_t>(intervalCount))
			{
				float sum = 0;
				for (float interval : intervals) {
					sum += interval;
				}
				averageTime = sum / static_cast<float>(intervals.size());
				std::wstring units = L"us"; // microseconds
				if (averageTime > 1000)
				{
					averageTime /= 1000;
					units = L"ms"; // milliseconds
					if (averageTime > 1000)
					{
						averageTime /= 1000;
						units = L"s"; // seconds
					}
				}
				VariableMonitor::AddValueMonitor(timerName, &averageTime, units);
				intervals.clear();
			}
		}

	};

	template<typename T>
	class Property {
	private:
		std::function<T()> getter;
		std::function<void(const T&)> setter;

	public:
		// Constructor for read-write property
		Property(std::function<T()> get, std::function<void(const T&)> set) : getter(get), setter(set) {}

		// Constructor for read-only property
		Property(std::function<T()> get) : getter(get), setter([](const T&) { throw std::logic_error("Property is read-only."); }) {}

		// The "getter" part
		operator T() const { return getter(); }

		// The "setter" part
		Property<T>& operator =(const T& value) {
			setter(value);
			return *this;
		}
	};


	class SoundPlayer
	{
	private:
		// Map to store device IDs for currently playing sounds
		static inline std::map<std::wstring, MCIDEVICEID> playingSounds;

	public:
		static void Play(const std::wstring& fileName)
		{
			if (playingSounds.find(fileName) != playingSounds.end()) { Stop(fileName); }
			MCI_OPEN_PARMS mciOpen = {};
			mciOpen.lpstrElementName = fileName.c_str();
			if (MCIERROR err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen)) { Util_LogErrorTerminate(L"Error sending MCI audio command"); }
			MCI_PLAY_PARMS mciPlay = {};
			mciPlay.dwFrom = 0;
			if (MCIERROR err = mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&mciPlay))
			{
				mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, 0);
				Util_LogErrorTerminate(L"Error sending MCI audio command");
			}
			playingSounds[fileName] = mciOpen.wDeviceID;
		}

		static unsigned long GetDuration(const std::wstring& fileName) {
			MCI_OPEN_PARMS mciOpen = {};
			MCI_STATUS_PARMS mciStatus = {};

			mciOpen.lpstrElementName = fileName.c_str();

			if (MCIERROR err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen)) {
				Util_LogErrorTerminate(L"Error sending MCI audio command");
			}

			mciStatus.dwItem = MCI_STATUS_LENGTH;
			if (MCIERROR err = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus)) {
				mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, 0);
				Util_LogErrorTerminate(L"Error sending MCI audio command");
			}

			mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, 0);

			return static_cast<unsigned long>(mciStatus.dwReturn);
		}

		static void Stop(const std::wstring& fileName)
		{
			auto it = playingSounds.find(fileName);
			if (it != playingSounds.end()) {
				mciSendCommand(it->second, MCI_STOP, 0, 0);
				mciSendCommand(it->second, MCI_CLOSE, 0, 0);
				playingSounds.erase(it);
			}
		}
		static void StopLast() {
			if (playingSounds.empty()) {
				return;
			}
			auto lastSoundIt = playingSounds.rbegin();
			mciSendCommand(lastSoundIt->second, MCI_STOP, 0, 0);
			mciSendCommand(lastSoundIt->second, MCI_CLOSE, 0, 0);
			playingSounds.erase(lastSoundIt->first);
		}
	};

	Vector2 GetCursorPos();
	RECTF GetClientRect(HWND hwnd);
	Vector2 GetClientAreaPos(HWND hwnd, Vector2 pt);
	Vector2 PosDistanceClientArea(HWND hwnd, Vector2 pt);
	Vector2 GetClientAreaCursorPos(HWND hwnd);
	Vector2 CursorDistanceClientArea(HWND hwnd);
	Vector2 GetCenterOfClientArea(HWND hwnd);
	void ShiftCursorPos(Vector2 Distance);


	class InputManager
	{
	public:
		static void CaptureInput()
		{
			Util_Assert(firstKeyPressed == -1, L"InputManager Error. Must call Reset Input before capturing again.");
			for (int key = 0; key < 256; key++)
			{
				prevKeyStates[key] = keyStates[key];
				keyStates[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
				if (firstKeyDown == -1 && IsControlDown(key) && key > 7) { firstKeyDown = key; }
				if (firstKeyPressed == -1 && IsControlFreshlyPressed(key) && key > 7) { firstKeyPressed = key; }
				if (firstKeyReleased == -1 && IsControlReleased(key) && key > 7) { firstKeyReleased = key; }
			}
			if (IsControlDown(VK_SHIFT))
			{
				if (firstKeyDown != -1) { firstKeyDown = VK_SHIFT | firstKeyDown; }
				if (firstKeyPressed != -1) { firstKeyPressed = VK_SHIFT | firstKeyPressed; }
				if (firstKeyReleased != -1) { firstKeyReleased = VK_SHIFT | firstKeyReleased; }
			}
			if (IsControlDown(VK_CONTROL))
			{
				if (firstKeyDown != -1) { firstKeyDown = VK_CONTROL | firstKeyDown; }
				if (firstKeyPressed != -1) { firstKeyPressed = VK_CONTROL | firstKeyPressed; }
				if (firstKeyReleased != -1) { firstKeyReleased = VK_CONTROL | firstKeyReleased; }
			}
			if (IsControlDown(VK_MENU))
			{
				if (firstKeyDown != -1) { firstKeyDown = VK_MENU | firstKeyDown; }
				if (firstKeyPressed != -1) { firstKeyPressed = VK_MENU | firstKeyPressed; }
				if (firstKeyReleased != -1) { firstKeyReleased = VK_MENU | firstKeyReleased; }
			}
		}

		static bool IsControlFreshlyPressed(int VK_keycode)
		{
			return keyStates[VK_keycode] && !prevKeyStates[VK_keycode];
		}

		static bool IsControlReleased(int VK_keycode)
		{
			return !keyStates[VK_keycode] && prevKeyStates[VK_keycode];
		}

		static bool IsControlDown(int VK_keycode)
		{
			return keyStates[VK_keycode];
		}

		static int GetFirstKeyPressed()
		{
			return firstKeyPressed;
		}

		static int GetFirstKeyDown()
		{
			return firstKeyDown;
		}

		static int GetFirstKeyReleased()
		{
			return firstKeyReleased;
		}

		static void UpdateMouseWheel(int wheelDelta)
		{
			mouseWheelDelta += wheelDelta;
		}

		static void UpdateMouseMove(int x, int y)
		{
			Vector2 newMousePos(static_cast<float>(x), static_cast<float>(y));
			mouseDelta += (newMousePos - mousePos);
			mousePos = newMousePos;
		}

		static void ResetInput()
		{
			firstKeyPressed = -1;
			firstKeyDown = -1;
			firstKeyReleased = -1;
			mouseWheelDelta = 0;
			mouseDelta = Vector2(0);
		}

		static int GetLastMouseWheelDelta()
		{
			return mouseWheelDelta / WHEEL_DELTA;
		}

		static Vector2 GetMousePos()
		{
			return mousePos;
		}

		static Vector2 GetLastMouseDelta()
		{
			return mouseDelta;
		}

	private:
		static inline bool keyStates[256] = { false };
		static inline Vector2 mousePos = Vector2();
		static inline bool prevKeyStates[256] = { false };
		static inline Vector2 mouseDelta = Vector2();
		static inline int mouseWheelDelta = 0;
		static inline int firstKeyPressed = -1;   //First key that is down in the current frame but was up in the previous frame. Captures freshly pressed keys
		static inline int firstKeyReleased = -1;  //First key that was down in the previous frame but is now up.  Captures freshly released keys.
		static inline int firstKeyDown = -1;      //First Key that is down regardless of previous state.  Captures keys being held down. 
	};

}
