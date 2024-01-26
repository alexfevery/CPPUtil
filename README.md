# Windows Utility Library for C++
This is a library with many unrelated helper functions and structs for common windows programming tasks. Includes input gathering tools, Sound player, Debugging tools like precise timing of code execution, string manipulation, vector manipulation, tools for uploading and downloading files. The key theme of this library is doing things simply without needing a lot of code.


# Namespace: `Util::`

## Struct: `Util::Vector2`

Represents a 2D vector with basic vector operations.

### Properties
- `float X`: The horizontal component of the vector.
- `float Y`: The vertical component of the vector.

### Constructors
- `Vector2()`: Default constructor initializes X and Y to 0.
- `Vector2(float i)`: Initializes both X and Y to the given value `i`.
- `Vector2(float x, float y)`: Initializes X to `x` and Y to `y`.
- `Vector2(POINT pos)`: Initializes from a `POINT` structure.
- `Vector2(D2D1_POINT_2F pos)`: Initializes from a `D2D1_POINT_2F` structure.

### Operator Overloads
- `Vector2 operator+(const Vector2& rhs) const`: Vector addition.
- `Vector2 operator-(const Vector2& rhs) const`: Vector subtraction.
- `Vector2 operator+(float rhs) const`: Adds scalar to both components.
- `Vector2 operator-(float rhs) const`: Subtracts scalar from both components.
- `Vector2 operator-() const`: Unary minus, negates both X and Y.
- `Vector2 operator*(const Vector2& rhs) const`: Element-wise multiplication.
- `Vector2 operator/(const Vector2& rhs) const`: Element-wise division. Throws an assertion if `rhs.X` or `rhs.Y` is 0.
- `Vector2 operator*(float scalar) const`: Scalar multiplication.
- `Vector2 operator/(float scalar) const`: Scalar division. Throws an assertion if `scalar` is 0.
- `friend Vector2 operator-(float scalar, const Vector2& vec)`: Scalar subtraction from vector.
- `friend Vector2 operator+(float scalar, const Vector2& vec)`: Scalar addition to vector.
- `friend Vector2 operator*(float scalar, const Vector2& vec)`: Scalar multiplication with vector.
- `friend Vector2 operator/(float scalar, const Vector2& vec)`: Scalar division by vector components. Throws an assertion if `vec.X` or `vec.Y` is 0.
- `friend Vector2& operator+=(Vector2& lhs, const Vector2& rhs)`: Compound assignment for vector addition.
- `friend Vector2& operator-=(Vector2& lhs, const Vector2& rhs)`: Compound assignment for vector subtraction.
- `friend Vector2& operator*=(Vector2& lhs, const Vector2& rhs)`: Compound assignment for element-wise multiplication.
- `friend Vector2& operator/=(Vector2& lhs, const Vector2& rhs)`: Compound assignment for element-wise division. Throws an assertion if `rhs.X` or `rhs.Y` is 0.
- `friend Vector2& operator+=(Vector2& lhs, float rhs)`: Compound assignment for scalar addition.
- `friend Vector2& operator-=(Vector2& lhs, float rhs)`: Compound assignment for scalar subtraction.
- `friend Vector2& operator*=(Vector2& lhs, float rhs)`: Compound assignment for scalar multiplication.
- `friend Vector2& operator/=(Vector2& lhs, float rhs)`: Compound assignment for scalar division. Throws an assertion if `rhs` is 0.
- `bool operator==(const Vector2& rhs) const`: Returns true if both X and Y are equal to `rhs`'s X and Y.
- `bool operator!=(const Vector2& rhs) const`: Returns true if either X or Y is not equal to `rhs`'s X and Y.

### Methods
- `float Min() const`: Returns the smaller of X and Y.
- `float Max() const`: Returns the larger of X and Y.
- `float Length() const`: Returns the length of the vector.
- `static float Distance(const Vector2& a, const Vector2& b)`: Calculates the distance between two vectors.
- `Vector2 Normalized() const`: Returns a normalized version of the vector.
- `Vector2 Rounded() const`: Returns a vector with X and Y rounded to the nearest whole number.
- `static float Dot(const Vector2& a, const Vector2& b)`: Calculates the dot product of two vectors.
- `static float Angle(const Vector2& a, const Vector2& b)`: Calculates the angle in degrees between two vectors.
- `operator D2D1_POINT_2F() const`: Converts the vector to a `D2D1_POINT_2F` structure.



## Struct: `Util::RECTF`
Represents a rectangle in 2D space. This struct provides a set of methods for manipulation, querying, and basic geometric operations.

### Properties
- `float Left`: The left side of the rectangle.
- `float Top`: The top side of the rectangle.
- `float Right`: The right side of the rectangle.
- `float Bottom`: The bottom side of the rectangle.
- `float Width`: The width of the rectangle.
- `float Height`: The height of the rectangle.

### Constructors
- `RECTF()`: Initializes a rectangle with all sides set to 0.
- `RECTF(const Vector2& position)`: Initializes a rectangle at a specified position with zero size.
- `RECTF(const Vector2& position, float Width, float Height)`: Initializes a rectangle at a specified position with given width and height.
- `RECTF(const Vector2& position, const Vector2& size)`: Initializes a rectangle at a specified position with a size defined by a `Vector2`.
- `RECTF(float left, float top, float right, float bottom)`: Initializes a rectangle with given left, top, right, and bottom values.
- `RECTF(RECT rc)`: Initializes a rectangle from a `RECT` structure.
- `RECTF(const D2D1_RECT_F& d2dRect)`: Initializes a rectangle from a `D2D1_RECT_F` structure.

### Methods
- `Vector2 GetCenter() const`: Returns the center point of the rectangle.
- `bool Contains(const Vector2& point) const`: Checks if a point is inside the rectangle.
- `bool Contains(const RECTF& otherRECTF) const`: Checks if another `RECTF` is entirely within this rectangle.
- `bool Overlaps(const RECTF& otherRECTF) const`: Checks if another `RECTF` overlaps with this rectangle.
- `RECTF Union(const RECTF& otherRECTF) const`: Creates a new `RECTF` that represents the union of this rectangle and another.
- `RECTF Resize(float scaleFactor, bool keepCenter) const`: Resizes the rectangle by a scale factor. If `keepCenter` is true, the center point remains the same.
- `Vector2 GetSize() const`: Returns the size of the rectangle as a `Vector2`.
- `Vector2 GetPosition() const`: Returns the position of the rectangle's top-left corner as a `Vector2`.
- `void SetPosition(const Vector2& newPosition)`: Sets a new position for the rectangle, keeping its size constant.
- `operator D2D1_RECT_F() const`: Converts the `RECTF` to a `D2D1_RECT_F` structure.

  
- `Util::Timer`: Provides functionality to measure execution time of code blocks.
- `Util::Property`: Generic property class for creating properties with getter and setter methods.
- `Util::SoundPlayer`: Handles playing, stopping, and querying audio files.
- `Util::InputManager`: Manages keyboard and mouse input states.

## Struct: `Util::VariableMonitor`

Provides a system for monitoring and debugging various types of variables during runtime. Supports monitoring of basic data types and offers a mechanism to display their values as strings.

### Methods
- `static VariableMonitor& GetInstance()`: Returns a reference to the singleton instance of `VariableMonitor`.
  
- `template<typename T>`
  `static void AddValueMonitor(const std::wstring& name, T* var, const std::wstring& units = L"")`: Adds a variable to the monitor list. 
  - `const std::wstring& name`: The name identifier for the variable.
  - `T* var`: Pointer to the variable to be monitored.
  - `const std::wstring& units`: Optional units description for the variable value.

- `static void PrintMonitorValues(HWND hwnd)`: Prints the monitored values to a window.
  - `HWND hwnd`: Handle to the window where values will be displayed.

- `static void ClearMonitors()`: Clears all monitored variables from the list.

### Usage Notes
- `VariableMonitor` is designed as a singleton to ensure a single, centralized monitoring system.
- Monitored variables are displayed as strings, with optional units for clarity.
- It's useful for real-time tracking of variables in a graphical application, particularly for debugging or performance monitoring.

### Supported Variable Types
- Integer (`int`)
- Double-precision floating point (`double`)
- Boolean (`bool`)
- Single-precision floating point (`float`)
- Long integer (`long`)
- Wide string (`std::wstring`)

## Class: `Util::Timer`

This class is used for precise timing of code execution. It provides static methods to start and stop timers and calculate execution times over intervals. The `Timer` class allows for simultaneous timing of two separate code sections with separate timers, Timer1 and Timer2.

### Public Static Members
- `std::chrono::time_point<std::chrono::high_resolution_clock> startTime1, startTime2`: Stores the start time points for Timer1 and Timer2.
- `std::wstring name1, name2`: Names assigned to Timer1 and Timer2 for identification.
- `std::vector<float> intervals1, intervals2`: Stores intervals of time measurements for Timer1 and Timer2.
- `int intervalCount1, intervalCount2`: The number of intervals to measure before calculating the average time.
- `float averageTime1, averageTime2`: The average execution time calculated for Timer1 and Timer2.

### Public Static Methods
- `void StartTimer1(const std::wstring& name, int intervalCount)`: Starts Timer1 with a specified name and number of intervals.
- `void EndTimer1()`: Stops Timer1 and calculates the average execution time if the specified number of intervals is reached.
- `void StartTimer2(const std::wstring& name, int intervalCount)`: Starts Timer2 with a specified name and number of intervals.
- `void EndTimer2()`: Stops Timer2 and calculates the average execution time if the specified number of intervals is reached.

### Usage
The `Timer` class is used to measure the execution time of code blocks. To use it, call `StartTimer1` or `StartTimer2` before the code block and `EndTimer1` or `EndTimer2` after the code block. The timer will calculate the average execution time. If the code runs continuosly you can use the interval parameter to specify how many cycles before it updates the average. 

## Class: `Util::SoundPlayer`

Provides functionality to play, stop, and query the duration of audio files. This class utilizes the Media Control Interface (MCI) to handle audio playback.

### Public Methods
- `static void Play(const std::wstring& fileName)`: Plays an audio file. If the file is already playing, it stops and replays it. The `fileName` parameter is the path to the audio file.
- `static unsigned long GetDuration(const std::wstring& fileName)`: Returns the duration of the specified audio file in milliseconds. The `fileName` parameter is the path to the audio file.
- `static void Stop(const std::wstring& fileName)`: Stops the playback of the specified audio file. The `fileName` parameter is the path to the audio file being played.
- `static void StopLast()`: Stops the last played audio file.

## Standalone Functions:

- `GetApplicationDataPath`: Retrieves the application data path for a given application and folder.
- `execCommand`: Executes a command in the windows terminal and returns the output.
- `downloadFile`: Downloads a file from a specified URL to a destination path using the windows terminal.
- `wstring_to_urlencoded`: Converts a wide string to a URL-encoded string.
- `getDeviceIdentifier`: Retrieves a unique identifier for the computer running the code.
- `uploadFile`: Uploads a file to a server using the windows terminal.
- `UploadMessage`: Uploads a message to a server using the windows terminal.
- `GetLogFilePath`: Retrieves the path that can be used for saving a log file.
- `ConvertFunctionNameToWide`: Converts a function name to a wide string.
- `GetFileName`: Extracts the filename from a path.
- `Max`, `Min`, `Clamp`: Functions for finding the maximum, minimum, and clamping values.
- `Log`: Logs a message to a specified log file.
- `OpenLog`: Opens a log file in the default text editor.
- `Split`: Splits a string based on a delimiter.
- `Longest`: Finds the longest string in a vector of strings.
- `StringContains`: Checks if a string contains a substring.
- `to_percentage`: Converts a number to a percentage string.
- `utf8_to_wstring`, `wstring_to_utf8`: Converts between UTF-8 and wide strings.
- `iswhitespace`: Checks if a string consists only of whitespace.
- `MuteOtherApplications`: Mutes or unmutes all applications except running one.
- `wideStringToBytes`, `bytesToWideString`: Converts between wide strings and byte strings.
- `LevenshteinDistance`: Calculates the Levenshtein distance between two strings.
- `Trim`: Trims whitespace from a string.
- `ToLower`: Converts a string to lowercase.
- `EraseAllSubStr`, `ReplaceSubStrAt`, `ReplaceAllSubStr`, `ReplaceAllSubStrWhere`, `EraseAllSubStrWhere`: String manipulation functions for erasing and replacing substrings.
- `StringsEqual`: Checks if two strings are equal.
- `AnyProcessFullScreen`: Checks if any windows process is running in full-screen mode.
- `Join`: Joins elements of a vector into a string with a delimiter.
- `PreventDuplicateAppInstance`: Prevents running duplicate instances of the current application.
- `GetExecutionLocation`: Retrieves the execution location of the application.
- `CreateShortcut`: Creates a shortcut to a specified path.
- `Or`, `And`: Functions to combine multiple predicates using logical OR and AND.
- `IsAll`, `IsAny`: Checks if all or any elements in a string satisfy a given condition.
- `StringWhere`, `StringExcept`: Selects characters from a string based on a condition.
- `lerp`: Linearly interpolates between two values.
- `FloatToWstring`: Converts a float to a formatted wide string.
- `GetTimeStamp`: Retrieves the current timestamp.
- `VectorContains`, `VectorAddRange`: Functions for vector manipulation.
- `GetCursorPos`: Retrieves the current cursor position.
- `GetClientRect`: Retrieves the client rectangle of a window.
- `GetClientAreaPos`, `PosDistanceClientArea`, `GetClientAreaCursorPos`, `CursorDistanceClientArea`, `GetCenterOfClientArea`, `ShiftCursorPos`: Functions related to window client area and cursor position.

## Macros: Error Handling and Debugging

The `Windows Utility Library for C++` includes a set of macros for error handling and debugging. These macros behave differently depending on whether a debugger is attached or not (`NDEBUG` not defined) or release mode (`NDEBUG` defined).

### `bool GlobalError`
- `inline bool GlobalError = false;`
- A global flag indicating if an error has occurred. This is activated when LogErrorTerminate is called. This can be queried in any threads so they know that the program has encountered a fatal error and is shutting down before the user clicks a button in the Fatal Error MessageBox. 

### `Util_LogErrorTerminate(wstring ErrorMessage)`
- When debugger attached: Triggers a breakpoint (`__debugbreak()`).
- When debugger NOT attached: Logs the error message to a log file, sets GlobalError to true, displays a message box, and terminates the process.
- `ErrorMessage`: The error message to log.

### `Util_Assert(bool condition, wstring ErrorMessage)`
- When debugger attached: Asserts a condition and triggers a breakpoint if the assertion fails (`__debugbreak()`).
- When debugger NOT attached: If the condition is false, logs the error message and terminates the process.
- `condition`: The condition to assert.
- `ErrorMessage`: The error message to log if the assertion fails.

### `Util_NotImplemented()`
- When debugger attached: Triggers a breakpoint.
- When debugger NOT attached: Logs a "Not implemented" error message and terminates the process.
- Used as a placeholder for functionality that is planned but not yet implemented.

### `Util_D2DCall(HRESULT operationResult)`
- When debugger attached: Checks a D2D operation result and triggers a breakpoint if the operation fails.
- When debugger NOT attached: If the operation fails, logs an error message and terminates the process.
- `operationResult`: The result of the D2D operation to check.

### Logging Mechanism in Non-Debug Mode
- The logging function (`Util::Log`) writes error messages to a log file located in the roaming app data directory of the application.
- It uses `Util::GetLogFilePath` to determine the log file path based on the application's executable name and the specified log file name.
- Logs are maintained with a Time-To-Live (TTL) mechanism, where old log entries are purged based on the specified TTL in seconds.
- In case of a logging failure (e.g., unable to open the log file), the program displays a message box indicating the failure and then terminates.


These macros facilitate consistent error handling across different build configurations and debugging scenarios, ensuring robust error management in the library.
