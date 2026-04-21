#include <obs-module.h>
#include <obs-frontend-api.h>
#include <windows.h>

#define WINDOW_CLASS_NAME L"OBS_RedDot_Indicator"
#define TIMER_ID 1
#define POLLING_INTERVAL 200

static HWND indicator_hwnd = NULL;
static UINT_PTR polling_timer_id = 0;

static bool is_fullscreen_active() {
    HWND foreground = GetForegroundWindow();
    if (!foreground) return false;

    // Skip common windows that might be fullscreen but aren't games
    wchar_t className[256];
    if (GetClassNameW(foreground, className, 256)) {
        if (wcscmp(className, L"WorkerW") == 0 || wcscmp(className, L"Progman") == 0 ||
            wcscmp(className, L"Shell_TrayWnd") == 0) {
            return false;
        }
    }

    RECT windowRect;
    if (GetWindowRect(foreground, &windowRect)) {
        int width = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;

        // Check if it matches screen resolution
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        if (width >= screenWidth && height >= screenHeight) {
            return true;
        }
    }
    return false;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
            HGDIOBJ oldBrush = SelectObject(hdc, brush);
            HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(NULL_PEN));
            
            Ellipse(hdc, 0, 0, 15, 15);
            
            SelectObject(hdc, oldBrush);
            SelectObject(hdc, oldPen);
            DeleteObject(brush);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void create_indicator() {
    if (indicator_hwnd) return;

    HINSTANCE hInst = GetModuleHandle(NULL);
    WNDCLASSEXW wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassExW(&wc)) {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            blog(LOG_ERROR, "Failed to register window class for RedDot");
            return;
        }
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    indicator_hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,
        WINDOW_CLASS_NAME,
        L"RedDot",
        WS_POPUP,
        screenWidth - 25, 10, 15, 15,
        NULL, NULL, hInst, NULL
    );

    if (indicator_hwnd) {
        HRGN hRgn = CreateEllipticRgn(0, 0, 15, 15);
        SetWindowRgn(indicator_hwnd, hRgn, TRUE);
        SetLayeredWindowAttributes(indicator_hwnd, 0, 255, LWA_ALPHA);
        ShowWindow(indicator_hwnd, SW_SHOW);
        blog(LOG_INFO, "RedDot indicator created (Circle)");
    } else {
        blog(LOG_ERROR, "Failed to create RedDot indicator window");
    }
}

static void destroy_indicator() {
    if (indicator_hwnd) {
        DestroyWindow(indicator_hwnd);
        indicator_hwnd = NULL;
        // Do not unregister class here to avoid issues with repeated creation
        blog(LOG_INFO, "RedDot indicator destroyed");
    }
}

static void CALLBACK timer_tick(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    UNUSED_PARAMETER(hwnd);
    UNUSED_PARAMETER(uMsg);
    UNUSED_PARAMETER(idEvent);
    UNUSED_PARAMETER(dwTime);

    if (obs_frontend_recording_active() && is_fullscreen_active()) {
        if (!indicator_hwnd) {
            create_indicator();
        }
    } else {
        if (indicator_hwnd) {
            destroy_indicator();
        }
    }
}

static void frontend_event_callback(enum obs_frontend_event event, void *private_data) {
    if (event == OBS_FRONTEND_EVENT_RECORDING_STARTED) {
        if (!polling_timer_id) {
            polling_timer_id = SetTimer(NULL, TIMER_ID, POLLING_INTERVAL, timer_tick);
        }
    } else if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED) {
        if (polling_timer_id) {
            KillTimer(NULL, polling_timer_id);
            polling_timer_id = 0;
        }
        destroy_indicator();
    }
    UNUSED_PARAMETER(private_data);
}

OBS_DECLARE_MODULE()

bool obs_module_load(void) {
    obs_frontend_add_event_callback(frontend_event_callback, NULL);
    
    if (obs_frontend_recording_active()) {
        polling_timer_id = SetTimer(NULL, TIMER_ID, POLLING_INTERVAL, timer_tick);
    }

    blog(LOG_INFO, "RedDot plugin loaded");
    return true;
}

void obs_module_unload(void) {
    if (polling_timer_id) {
        KillTimer(NULL, polling_timer_id);
        polling_timer_id = 0;
    }
    destroy_indicator();
    UnregisterClassW(WINDOW_CLASS_NAME, GetModuleHandle(NULL));
    blog(LOG_INFO, "RedDot plugin unloaded");
}
