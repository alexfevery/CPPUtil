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
#include <cassert>
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
#include <optional>

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
	Util::LogErrorTerminate_Debug(ErrorMessage);

#define Util_Assert(condition, ErrorMessage) \
    if (!(condition)) { __debugbreak();} \
	assert(condition);

	//A function for terminating the program if the code is reached with a not implemented message
#define Util_NotImplemented() \
	__debugbreak(); \
	assert(false); 

#define Util_D2DCall(operationResult) \
	if (FAILED(operationResult)) \
	{ \
		std::wstring errorMsg = L"Error: " + std::wstring(_com_error(operationResult).ErrorMessage()) + L" AFTER D2D CALL\n"; \
		OutputDebugString(errorMsg.c_str()); \
		__debugbreak(); \
		assert(false); \
	}

#else
#define Util_LogErrorTerminate(ErrorMessage) \
	Util::LogErrorTerminate_Release(ErrorMessage);

#define Util_Assert(condition, ErrorMessage) \
	if(!(condition)) { Util_LogErrorTerminate(ErrorMessage);} \
	assert(condition); //Just to get rid of compiler warnings

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
	[[noreturn]] void LogErrorTerminate_Debug(std::wstring ErrorMessage);
	[[noreturn]] void LogErrorTerminate_Release(std::wstring ErrorMessage);
	std::wstring GetApplicationDataPath(const std::wstring& appname, const std::wstring& folder,bool StorageModeLocal = false);
	std::string execCommand(const std::string& command);
	bool SetKeyboardLanguage(const std::wstring name);
	void DisableIme();
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
	void MuteOtherApplications(bool mute);
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
	bool usesIME(const std::wstring& languageName);
	std::vector<std::tuple<std::wstring, HKL>> GetInstalledKeyboardLayouts();
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

	template<typename Container, typename Func>
	auto Accumulate(const Container & elements, Func func) {
		using ReturnType = decltype(func(*elements.begin()));
		static_assert(std::is_same_v<ReturnType, std::vector<std::wstring*>>, "Func must return std::vector<std::wstring*>");

		std::vector<std::wstring*> result;
		for (const auto& element : elements) {
			auto partialResult = func(element);
			result.insert(result.end(), partialResult.begin(), partialResult.end());
		}
		return result;
	}

	template<typename T, typename Func>
	std::wstring JoinAll(const std::vector<T>& elements, Func func, std::optional<std::wstring> separator = std::nullopt) {
		std::wstringstream ss;
		for (auto it = elements.begin(); it != elements.end(); ++it) {
			if (it != elements.begin() && separator.has_value()) {
				ss << *separator;
			}
			ss << func(*it);
		}
		return ss.str();
	}


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
		template<typename T>
		Vector2(T i) : X(static_cast<float>(i)), Y(static_cast<float>(i)) {}
		template<typename T, typename U>
		Vector2(T x, U y) : X(static_cast<float>(x)), Y(static_cast<float>(y)) {}
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

		static Vector2 RotatePoint(const Vector2& point, const Vector2& pivot, float angle) {
			float radAngle = angle * static_cast<float>(M_PI) / 180.0f;
			float cosTheta = std::cos(radAngle);
			float sinTheta = std::sin(radAngle);

			return Vector2(
				cosTheta * (point.X - pivot.X) - sinTheta * (point.Y - pivot.Y) + pivot.X,
				sinTheta * (point.X - pivot.X) + cosTheta * (point.Y - pivot.Y) + pivot.Y
			);
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
		Vector2 GetTopLeft() const { return Vector2(Left, Top); }
		Vector2 GetTopRight() const { return Vector2(Right, Top); }
		Vector2 GetBottomLeft() const { return Vector2(Left, Bottom); }
		Vector2 GetBottomRight() const { return Vector2(Right, Bottom); }
		void SetPositionTopLeft(const Vector2& newTopLeft) {
			float width = Right - Left;
			float height = Bottom - Top;
			Left = newTopLeft.X;
			Top = newTopLeft.Y;
			Right = Left + width;
			Bottom = Top + height;
			checkRectIntegrity();
		}

		operator D2D1_RECT_F() const
		{
			return D2D1_RECT_F{ Left, Top, Right, Bottom };
		}



	private:
		void checkRectIntegrity() const;

	};

	class RotatedRECTF {
	public:
		RotatedRECTF(RECTF rect, float rotation) : rect(rect), rotation(rotation) {}

		Vector2 GetTopLeft() const {
			return Vector2::RotatePoint(rect.GetTopLeft(), rect.GetCenter(), rotation);
		}

		Vector2 GetTopRight() const {
			Vector2 topRight = Vector2(rect.Right, rect.Top);
			return Vector2::RotatePoint(topRight, rect.GetCenter(), rotation);
		}

		Vector2 GetBottomLeft() const {
			Vector2 bottomLeft = Vector2(rect.Left, rect.Bottom);
			return Vector2::RotatePoint(bottomLeft, rect.GetCenter(), rotation);
		}

		Vector2 GetBottomRight() const {
			Vector2 bottomRight = Vector2(rect.Right, rect.Bottom);
			return Vector2::RotatePoint(bottomRight, rect.GetCenter(), rotation);
		}

		float GetWidth() const {
			return rect.Width;
		}

		float GetHeight() const {
			return rect.Height;
		}

		Vector2 GetCenter() const {
			return rect.GetCenter();
		}

		bool Contains(const Vector2& point) const {
			Vector2 rotatedPoint = Vector2::RotatePoint(point, rect.GetCenter(), -rotation);
			return rect.Contains(rotatedPoint);
		}

		bool Contains(const RECTF& otherRECTF) const {
			return
				Contains(Vector2(otherRECTF.Left, otherRECTF.Top)) &&
				Contains(Vector2(otherRECTF.Right, otherRECTF.Top)) &&
				Contains(Vector2(otherRECTF.Left, otherRECTF.Bottom)) &&
				Contains(Vector2(otherRECTF.Right, otherRECTF.Bottom));
		}

		bool Overlaps(const RECTF& otherRECTF) const {
			return
				Contains(Vector2(otherRECTF.Left, otherRECTF.Top)) ||
				Contains(Vector2(otherRECTF.Right, otherRECTF.Top)) ||
				Contains(Vector2(otherRECTF.Left, otherRECTF.Bottom)) ||
				Contains(Vector2(otherRECTF.Right, otherRECTF.Bottom)) ||
				otherRECTF.Contains(GetTopLeft()) ||
				otherRECTF.Contains(GetTopRight()) ||
				otherRECTF.Contains(GetBottomLeft()) ||
				otherRECTF.Contains(GetBottomRight());
		}


	private:
		RECTF rect;
		float rotation; // Rotation in degrees
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
		Property(std::function<T()> get, std::function<void(const T&)> set) : getter(get), setter(set) {}

		Property(std::function<T()> get) : getter(get), setter([](const T&) { throw std::logic_error("Property is read-only."); }) {}

		operator T() const { return getter(); }

		Property<T>& operator =(const T& value) {
			setter(value);
			return *this;
		}
	};

	class SoundPlayer
	{
	public:
		enum AudioType { Voice, SoundEffect, BGM, NONE };
	private:

		struct Device
		{
			AudioType type;
			MCIDEVICEID ID;
			int64_t Start;
			int64_t RemainingPlayTime;
			long Duration;
			bool StopSignal;
		};

	private:
		static inline std::map<std::wstring, Device*> openSounds;

	public:
		static void Play(std::wstring fileName, AudioType audioType = AudioType::SoundEffect, bool AllowDuplicateSimultaneousPlay = false)
		{
			if (!PathFileExistsW(fileName.c_str())) { Util_LogErrorTerminate(L"File " + fileName + L" does not exist"); }
			if (audioType == AudioType::BGM)
			{
				if (IsPlaying(fileName)) { return; }
				StopAll(AudioType::BGM);
			}
			Device* d1 = new Device();
			openSounds[fileName] = d1;
			std::thread([fileName,audioType, d1]() {
			PlayStart:
				
				MCI_OPEN_PARMS mciOpen = {};
				mciOpen.lpstrElementName = fileName.c_str();
				if (MCIERROR err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen)) {
					Util_LogErrorTerminate(L"Error sending MCI audio command. Error code: " + std::to_wstring(err));
				}

				
				d1->type = audioType;
				d1->ID = mciOpen.wDeviceID;

				MCI_STATUS_PARMS mciStatus = {};
				mciStatus.dwItem = MCI_STATUS_LENGTH;
				if (MCIERROR err = mciSendCommand(d1->ID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus)) {
					mciSendCommand(d1->ID, MCI_CLOSE, 0, 0);
					Util_LogErrorTerminate(L"Error sending MCI audio command. Error code: " + std::to_wstring(err));
				}

				d1->Duration = static_cast<long>(mciStatus.dwReturn);
				MCI_PLAY_PARMS mciPlay = {};
				mciPlay.dwFrom = 0;
				if (MCIERROR err = mciSendCommand(d1->ID, MCI_PLAY, 0, (DWORD_PTR)&mciPlay)) {
					mciSendCommand(d1->ID, MCI_CLOSE, 0, 0);
					Util_LogErrorTerminate(L"Error sending MCI audio command. Error code: " + std::to_wstring(err));
				}
				d1->Start = GetTimeStamp();
				while (true)
				{
					if (d1->StopSignal || (GetTimeStamp() > (d1->Start + d1->Duration))) { break; }
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				mciSendCommand(d1->ID, MCI_CLOSE, 0, 0);
				if (!d1->StopSignal && d1->type == AudioType::BGM) { goto PlayStart; }
				delete d1;
				openSounds[fileName] = nullptr;
				}).detach();
		}

		static long GetDuration(std::wstring fileName)
		{
			if (!PathFileExistsW(fileName.c_str())) { Util_LogErrorTerminate(L"File " + fileName + L" does not exist"); }
			if (openSounds.find(fileName) == openSounds.end() || openSounds[fileName] == nullptr)
			{
				MCI_STATUS_PARMS mciStatus = {};
				mciStatus.dwItem = MCI_STATUS_LENGTH;
				MCI_OPEN_PARMS mciOpen = {};
				mciOpen.lpstrElementName = fileName.c_str();
				if (MCIERROR err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen)) {
					Util_LogErrorTerminate(L"Error sending MCI audio command. Error code: " + std::to_wstring(err));
				}
				if (MCIERROR err = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus)) {
					mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, 0);
					Util_LogErrorTerminate(L"Error sending MCI audio command. Error code: " + std::to_wstring(err));
				}
				mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, 0, 0);
				return static_cast<long>(mciStatus.dwReturn);
			}
			else { return openSounds[fileName]->Duration; }
		}

		static bool Stop(std::wstring fileName)
		{
			Device* it = openSounds[fileName];
			if (it != nullptr)
			{
				it->StopSignal = true;
				for (int i = 0; i < 10; i++)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					if (openSounds[fileName] == nullptr) { return true; }
				}
				return false;
			}
			return true;
		}

		static bool StopLast()
		{
			for (auto it = openSounds.rbegin(); it != openSounds.rend(); ++it)
			{
				if (it->second != nullptr)
				{
					return Stop(it->first);
				}
			}
			return true;
		}

		static bool StopAll(AudioType type = AudioType::NONE)
		{
			if (type == AudioType::NONE)
			{
				for (int i = 0; i < openSounds.size(); i++)
				{
					if (!StopLast()) { return false; }
				}
			}
			else
			{
				for (auto it = openSounds.rbegin(); it != openSounds.rend(); ++it)
				{
					if (it->second != nullptr && it->second->type == type)
					{
						return Stop(it->first);
					}
				}
			}
			return true;
		}

		static bool IsPlaying(std::wstring filePath)
		{
			if (!PathFileExistsW(filePath.c_str())) { Util_LogErrorTerminate(L"File " + filePath + L" does not exist"); }
			return openSounds[filePath] != nullptr;
		}
	};


	Vector2 GetCursorPos();
	RECTF GetClientRect(HWND hwnd);
	Vector2 GetClientAreaPos(HWND hwnd, Vector2 pt);
	Vector2 PosDistanceClientArea(HWND hwnd, Vector2 pt);
	Vector2 GetClientAreaCursorPos(HWND hwnd);
	Vector2 CursorDistanceClientArea(HWND hwnd);
	RECTF GetWindowRect(HWND hwnd);
	Vector2 GetWindowAreaPos(HWND hwnd, Vector2 pt);
	Vector2 PosDistanceWindowArea(HWND hwnd, Vector2 pt);
	Vector2 GetWindowAreaCursorPos(HWND hwnd);
	Vector2 CursorDistanceWindowArea(HWND hwnd);
	bool IsWindowMinimized(HWND hwnd);
	Vector2 GetCenterOfClientArea(HWND hwnd);
	void ShiftCursorPos(Vector2 Distance);
	std::wstring GetClipboardText(HWND hwnd);
	void CreateFolderPath(std::wstring fullPath);
}


