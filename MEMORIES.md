# Project Memories: OBS RedDot Indicator

## Objective
Create a 64-bit OBS Studio plugin for Windows that displays a red circle overlay on the screen during recording sessions to provide status feedback in fullscreen games.

## Technical Details
- **Language**: C / Win32 API / OBS Frontend API.
- **Compiler**: MSVC (Visual Studio 17 2022).
- **Build System**: CMake (via `obs-plugintemplate`).
- **Target**: Windows x64.
- **Current Path**: `C:\Users\IOSYS\obs-plugin-reddot`.

## Current Progress
- **Fullscreen Detection**: Added logic to only show the indicator when a fullscreen game is in the foreground.
- **Polling System**: Implemented a 200ms timer (`SetTimer`) to periodically check the system state.
- **Auto-Cleanup**: The indicator now automatically disappears if the game is closed (Alt+F4), minimized, or switched (Alt+Tab) while recording.
- **Shape Update**: Changed the indicator from a red square to a **15x15 red circle**.
- **Implementation**: 
    - Used `CreateEllipticRgn` and `SetWindowRgn` in `src/plugin-main.c` to create a truly circular window.
    - Used `Ellipse` GDI function for smooth drawing.
- **v1.1.0 Release**: 
    - Published v1.1.0 on GitHub with the updated DLL.
    - Added fullscreen detection logic and 200ms polling for high responsiveness.
- **Documentation & Assets**:
    - Added `reddot-sample.jpg` to the repository.
    - Updated `README.md` to include the sample image overlay preview.
- **GitHub Migration**: 
    - Moved the project to a new GitHub account: **guitarmuggle**.
    - Repository: [https://github.com/guitarmuggle/obs-plugin-reddot](https://github.com/guitarmuggle/obs-plugin-reddot).
    - Initial Release: **v1.0.0** published with `obs-plugin-reddot.dll` attached.

## GitHub Info
- **Account**: guitarmuggle
- **Repository URL**: `https://github.com/guitarmuggle/obs-plugin-reddot.git`
- **Identity**: Configured local git user `guitarmuggle` with email `iosys.secret@gmail.com`.

## Future Considerations
- Add options to change dot position or size via OBS settings.
- Support for different colors or flashing effects.
- Multi-monitor support adjustment (currently targets primary screen metrics).
