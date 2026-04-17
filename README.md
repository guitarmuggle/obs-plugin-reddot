# OBS RedDot Indicator Plugin

A simple OBS plugin for Windows that displays a small red circle overlay in the top-right corner of your screen whenever you are recording. This provides visual status feedback even while playing games in fullscreen mode without a second monitor.

## Features
- **Visual Feedback**: Displays a 15x15 red circle at the top-right of your primary screen.
- **Automatic**: Appears when recording starts and disappears when it stops.
- **Topmost Overlay**: Stays on top of most windows.
- **Transparent & Click-Through**: Won't interfere with mouse clicks or game interactions.

## Installation
1.  Go to the **[Releases](https://github.com/guitarmuggle/obs-plugin-reddot/releases)** section of this repository.
2.  Download the latest **`obs-plugin-reddot.dll`**.
3.  Copy the `.dll` file to your OBS Studio plugin directory:
    `C:\Program Files\obs-studio\obs-plugins\64bit\`
4.  Restart **OBS Studio**.

## Usage Note
- This plugin uses the Win32 API to create an overlay. It works best in **Borderless Fullscreen** or **Windowed** modes.
- In **Exclusive Fullscreen**, some games may block third-party overlays.

## For Developers (Build Instructions)
If you wish to modify the code or compile the plugin yourself:
1.  Open PowerShell in the project root.
2.  Run the following commands:
    ```powershell
    $cmake = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
    & $cmake --preset windows-x64
    & $cmake --build --preset windows-x64
    ```
3.  The compiled DLL will be found in `build_x64\RelWithDebInfo\`.

## Technical Information
- **Language**: C / Win32 API / OBS Frontend API.
- **Platform**: Windows x64.
- **Compiler**: MSVC (Visual Studio 17 2022).
