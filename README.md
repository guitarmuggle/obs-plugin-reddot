# OBS RedDot Indicator Plugin

A simple OBS plugin for Windows that displays a small red circle overlay in the top-right corner of your screen whenever you are recording. This provides visual status feedback even while playing games in fullscreen mode without a second monitor.

## Features
- **Visual Feedback**: Displays a 15x15 red circle at the top-right of your primary screen.
- **Automatic**: Appears when recording starts and disappears when it stops.
- **Topmost Overlay**: Stays on top of most windows.
- **Transparent & Click-Through**: Won't interfere with mouse clicks or game interactions.

## Installation
1.  Navigate to the project root: `C:\Users\IOSYS\obs-plugin-reddot`.
2.  Run the **`install.bat`** script (Right-click > Run as Administrator).
3.  The script will copy `obs-plugin-reddot.dll` to your OBS Studio directory (`C:\Program Files\obs-studio\obs-plugins\64bit\`).
4.  Restart **OBS Studio**.

## Distribution & Required Files
If you want to share this plugin or install it on another PC, you only need one file:

*   **`obs-plugin-reddot.dll`**

### How to install manually on another PC:
1.  Copy `obs-plugin-reddot.dll` to the destination PC.
2.  Paste it into the OBS Studio plugin folder:
    `C:\Program Files\obs-studio\obs-plugins\64bit\`
3.  Restart OBS Studio.

*Note: No other files (like `data` folders or `.txt` files) are required for this plugin to function.*

## Usage Note
- This plugin uses the Win32 API to create an overlay. It works best in **Borderless Fullscreen** or **Windowed** modes.
- In **Exclusive Fullscreen**, some games may block third-party overlays.

## Build Instructions
If you wish to re-compile the plugin:
1.  Open PowerShell in the project root.
2.  Run the following commands:
    ```powershell
    $cmake = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
    & $cmake --preset windows-x64
    & $cmake --build --preset windows-x64
    ```
3.  The compiled DLL will be in the root and `build_x64\RelWithDebInfo\`.

## Technical Information
- **Language**: C / Win32 API / OBS Frontend API.
- **Platform**: Windows x64.
- **Compiler**: MSVC (Visual Studio 17 2022).
