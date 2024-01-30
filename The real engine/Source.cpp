/*#include <Windows.h>

class SimpleGraphicsLibrary {
public:
    SimpleGraphicsLibrary(int width, int height) : width(width), height(height), bgColor(RGB(0, 0, 0)) {
        // Initialize the window
        CreateMainWindow();
    }

    // Function to draw a colored rectangle
    void DrawRectangle(int x, int y, int width, int height, COLORREF color) {
        RECT rect = { x, y, x + width, y + height };
        HBRUSH brush = CreateSolidBrush(color);
        
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }

    // Function to clear the window with a specified color
    void ClearWindow() {
        HBRUSH brush = CreateSolidBrush(bgColor);
        RECT rect = { 0, 0, width, height };
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }

    // Function to update the window
    void UpdateWindow() {
        InvalidateRect(hwnd, nullptr, TRUE);
        UpdateWindow();
    }

private:
    int width;
    int height;
    HWND hwnd;
    HDC hdc;
    COLORREF bgColor;

    // Function to create the main window
    void CreateMainWindow() {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
        wc.lpszClassName = L"SimpleGraphicsLibrary";

        RegisterClass(&wc);

        hwnd = CreateWindow(
            L"SimpleGraphicsLibrary", L"Simple Graphics Library Window",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            width, height, nullptr, nullptr, GetModuleHandle(nullptr), this);

        if (hwnd) {
            ShowWindow(hwnd, SW_SHOWNORMAL);
            hdc = GetDC(hwnd);
        }
    }

    // Static window procedure to forward messages to the class member function
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        SimpleGraphicsLibrary* pThis;
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<SimpleGraphicsLibrary*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            pThis->hwnd = hwnd;
        }
        else {
            pThis = reinterpret_cast<SimpleGraphicsLibrary*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis) {
            return pThis->RealWindowProc(hwnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    // Member window procedure
    LRESULT RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            ClearWindow();
            // Draw a new colored rectangle in each frame
            DrawRectangle(100, 100, 200, 150, RGB(255, 0, 0));  // Red
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        // Let the default window procedure handle WM_PAINT
        ValidateRect(hwnd, nullptr);

        return 0;
    }
};

int main() {
    SimpleGraphicsLibrary graphics(800, 600);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}*/