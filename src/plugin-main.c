#include <obs-module.h>
#include <obs-frontend-api.h>
#include <windows.h>

#define WINDOW_CLASS_NAME L"OBS_RedDot_Indicator"

static HWND indicator_hwnd = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
            HGDIOBJ oldBrush = SelectObject(hdc, brush);
            HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(NULL_PEN));
            
            // Draw a circle that fills the 15x15 window
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
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Use black background for the class

    if (!RegisterClassExW(&wc)) {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            blog(LOG_ERROR, "Failed to register window class for RedDot");
            return;
        }
    }

    // Create a 15x15 window at top right
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
        // Create a circular region so the window corners are transparent
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
        UnregisterClassW(WINDOW_CLASS_NAME, GetModuleHandle(NULL));
        blog(LOG_INFO, "RedDot indicator destroyed");
    }
}

static void frontend_event_callback(enum obs_frontend_event event, void *private_data) {
    if (event == OBS_FRONTEND_EVENT_RECORDING_STARTED) {
        create_indicator();
    } else if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED) {
        destroy_indicator();
    }
    UNUSED_PARAMETER(private_data);
}

OBS_DECLARE_MODULE()

bool obs_module_load(void) {
    obs_frontend_add_event_callback(frontend_event_callback, NULL);
    
    if (obs_frontend_recording_active()) {
        create_indicator();
    }

    blog(LOG_INFO, "RedDot plugin loaded");
    return true;
}

void obs_module_unload(void) {
    destroy_indicator();
    blog(LOG_INFO, "RedDot plugin unloaded");
}
