# Utility Library for C++

## Namespace: `Util::`

### Structs
- `Util::Vector2`: Represents a 2D vector with basic vector operations (addition, subtraction, etc.).
- `Util::RECTF`: Represents a rectangle in 2D space with methods for manipulation and querying (union, resize, etc.).
- `Util::VariableMonitor`: Manages variables for monitoring and debugging purposes.
- `Util::Timer`: Provides functionality to measure execution time of code blocks.
- `Util::Property`: Generic property class for creating properties with getter and setter methods.
- `Util::SoundPlayer`: Handles playing, stopping, and querying audio files.
- `Util::InputManager`: Manages keyboard and mouse input states.

### Functions
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

### Macros
- Error handling and assertion macros like `Util_LogErrorTerminate`, `Util_Assert`, `Util_NotImplemented`, etc.
- `Util_D2DCall` and `Util_WinAPICall`: Macros for calling Direct2D and Windows API functions with error handling.
