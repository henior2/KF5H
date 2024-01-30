#include <windows.h>
#include <thread>
#include <mutex>
#include <vector>
//#include "GameObject.h"
#include "Renderer.h"
#include "ModelMenager.h"
#include "Camera.h"

#define TIMER_ID 1
#define TIMER_TIME 1

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::vector<GameObject*> PaintObj;
mat ProjectonMatrix(4);
mat ViewMatrix(4);
std::mutex ObjMutex;
bool ChangeToObj = false;
std::mutex BoolObjMutex;


bool EndProgram = false;
std::mutex EndProgramMutex;

void Drawing(HWND& hwnd, int width, int height);
void Frame(double dt, std::vector<GameObject*>& OBJS, Camera* camera);
void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd) {

	const wchar_t Name[] = L"KF5H";

	//todo: load icon;

	WNDCLASS Window = {};
	Window.lpfnWndProc = WindowProc;
	Window.hInstance = hInstance;
	//Window.hIcon = hIcon;
	Window.lpszClassName = Name;

	RegisterClass(&Window);

	// Get the screen width
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);

	// Get the screen height
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd = CreateWindowEx(
		0,
		Name,
		L"Gry wektorowe",
		WS_POPUP,

		0, 0, screenWidth, screenHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == 0) {
		MessageBoxW(NULL, L"Nast¹pi³ nieoczekiwany b³¹d!", L"B³¹d", MB_OK);
	}

	ShowWindow(hwnd, nShowCmd);

	//todo: connect it somewhat
	ModelMenager::LoadModels(L"textFiles");
	std::thread Draw(Drawing, std::ref(hwnd), std::ref(screenWidth), std::ref(screenHeight));

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	{
		std::lock_guard<std::mutex> lock(EndProgramMutex);

		EndProgram = true;
	}

	Draw.join();

	//DestroyIcon(hIcon);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hwnd, TIMER_ID, TIMER_TIME, TimerCallback);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		//my code here
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &ps.rcPaint, white);
		EndPaint(hwnd, &ps);
		//InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	static bool running = false;
	static Camera* camera = new Camera;
	//camera->RotateCamera(0.0f, 90.0f);
	camera->perspective = true;
	if (!running) {
		//InvalidateRect(hwnd, NULL, TRUE);

		running = true;
		//todo: w osobnym threadzie program, a w tym rysowanie, po³¹czyæ je po jednym zadzia³aniu + zrobiæ flagêprzed dt i w niej ca³y kod.
		static long long lastTime = GetTickCount();
		long long newTime = GetTickCount();
		double dt = newTime - lastTime;
		lastTime = newTime;

		dt *= 0.001;

		std::vector<GameObject*> RenderObj;

		Frame(dt, RenderObj, camera);

		{
			std::lock_guard<std::mutex> lock(BoolObjMutex);
			{
				std::lock_guard<std::mutex> lock(ObjMutex);

				PaintObj = RenderObj;
				ViewMatrix = camera->GetViewMatrix();
				ProjectonMatrix = camera->GetProjectionMatrix();
			}

			ChangeToObj = true;
		}

		running = false;
	}
}

void Drawing(HWND& hwnd, int width, int height) {
	long long lastTime = GetTickCount();

	double dts[10];
	bool CanCopy;
	std::vector<GameObject*> Objects;
	mat Projection(4);
	mat View(4);
	HDC hdc, hdcBuffer;
	HBITMAP hBitmap, hOldBitmap;
	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(BoolObjMutex);

			CanCopy = ChangeToObj;

			if (CanCopy)
				ChangeToObj = false;
		}

		if (CanCopy) {
			{
				std::lock_guard<std::mutex> lock(ObjMutex);

				Objects = PaintObj;

				Projection = ProjectonMatrix;
				View = ViewMatrix;
			}
			//InvalidateRect(hwnd, NULL, true);
			hdc = GetDC(hwnd);
			///hdcBuffer = CreateCompatibleDC(hdc);
			//hBitmap = CreateCompatibleBitmap(hdc, width, height);  //Emergency!!!
			//hOldBitmap = (HBITMAP)SelectObject(hdcBuffer, hBitmap);

			RECT rect;
			GetClientRect(hwnd, &rect);
			HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(hdc, &rect, white);

			Renderer::DrawGame(Objects, hdc, Projection, View, width, height);

			long long newTime = GetTickCount();
			double dt = newTime - lastTime;
			double allDt = dt;
			for (int i = 1; i < 10; i++) {
				dts[i - 1] = dts[i];
				allDt += dts[i];
			}

			allDt /= 10.0;
			dts[9] = dt;
			lastTime = newTime;

			allDt = 1000.0 / allDt;

			HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
				OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Arial");
			HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

			// Set the text color
			SetTextColor(hdc, RGB(255, 0, 0));

			// Set the background color
			SetBkColor(hdc, RGB(255, 255, 255));

			// Draw the text
			std::wstring dtString = std::to_wstring(allDt);
			TextOut(hdc, 10, 10, dtString.c_str(), static_cast<int>(dtString.length()));

			//BitBlt(hdc, 0, 0, width, height, hdcBuffer, 0, 0, SRCCOPY);

			//SelectObject(hdcBuffer, hOldBitmap);
			//DeleteObject(hBitmap);
			//DeleteDC(hdcBuffer);
			ReleaseDC(hwnd, hdc);
		}

		{
			std::lock_guard<std::mutex> lock(EndProgramMutex);

			if (EndProgram)
				break;
		}
	}
}

void Frame(double dt, std::vector<GameObject*>& OBJS, Camera* camera) {
	GameObject* N = new GameObject(vec(0, 0, -10), vec(0, 0, 0), vec(0.1, 3), L"MenuCube", 0);
	//camera->MoveCamera(RIGHT, dt * 100);
	//camera->RotateCamera(dt * 1, 0);
	//N->Move(vec(dt, 3));
	std::vector<GameObject*> a;
	a.push_back(N);
	OBJS = a;
}