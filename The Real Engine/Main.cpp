#include <windows.h>
#include <thread>
#include <mutex>
#include <vector>
//#include "GameObject.h"
#include "Renderer.h"
#include "ModelMenager.h"
#include "Camera.h"
#include "Game.h"
#include "../resource.h"
#include <time.h>
#include <utility>

#define TIMER_ID 1
#define TIMER_TIME 1

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::vector<std::pair<Rendering, Transformations>> PaintObj;
mat ProjectonMatrix(4);
mat ViewMatrix(4);
std::mutex ObjMutex;
bool ChangeToObj = false;
std::mutex BoolObjMutex;


bool EndProgram = false;
std::mutex EndProgramMutex;

void Drawing(HWND& hwnd, int width, int height);
void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd) {

	const wchar_t Name[] = L"KF5H";

	srand(time(NULL));

	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	WNDCLASS Window = {};
	Window.lpfnWndProc = WindowProc;
	Window.hInstance = hInstance;
	Window.hIcon = hIcon;
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
	}else
		ShowWindow(hwnd, nShowCmd);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	ModelMenager::LoadModels(L".\\textFiles");
	std::thread Draw(Drawing, std::ref(hwnd), std::ref(screenWidth), std::ref(screenHeight));

	MSG msg = {};

	Game::ChangeState(Game_Menu);

	SetCursor(NULL);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		switch (msg.message)
		{
		case WM_KEYDOWN:
			Game::KeysPresed[msg.wParam] = true;
			break;
		case WM_KEYUP:
			Game::KeysPresed[msg.wParam] = false;
			break;
		default:
			break;
		}
	}

	{
		std::lock_guard<std::mutex> lock(EndProgramMutex);

		EndProgram = true;
	}

	Draw.join();

	DestroyIcon(hIcon);

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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &ps.rcPaint, white);
		EndPaint(hwnd, &ps);
		return 0;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	static bool running = false;
	if (!running) {

		running = true;
		static long long lastTime = GetTickCount64();
		long long newTime = GetTickCount64();
		double dt = newTime - lastTime;
		lastTime = newTime;

		dt *= 0.001;

		Game::Update(dt);

		{
			std::lock_guard<std::mutex> lock(BoolObjMutex);
			{
				std::lock_guard<std::mutex> lock(ObjMutex);

				PaintObj.clear();
				for (int i = 0; i < Game::Objects.size(); i++) {
					Rendering now(Game::Objects[i]->Stage[Game::Objects[i]->activeStage]);
					Transformations now2 = Game::Objects[i]->Transform;
					PaintObj.push_back({now, now2});
				}
				ViewMatrix = Game::camera->GetViewMatrix();
				ProjectonMatrix = Game::camera->GetProjectionMatrix();
			}

			ChangeToObj = true;
		}

		running = false;
	}
}

void Drawing(HWND& hwnd, int width, int height) {
	long long lastTime = GetTickCount64();

	double dts[10];
	bool CanCopy;
	std::vector<std::pair<Rendering, Transformations>> Objects;
	mat Projection(4);
	mat View(4);
	HDC hdc, hdcBuffer;
	HBITMAP hBitmap, hOldBitmap;
	int slep = 0;
	while (true)
	{
		slep++;
		{
			std::lock_guard<std::mutex> lock(BoolObjMutex);

			CanCopy = ChangeToObj;

			if (CanCopy)
				ChangeToObj = false;
		}

		if (CanCopy) {
			long long e = GetTickCount64();
			{
				std::lock_guard<std::mutex> lock(ObjMutex);

				Objects.clear();
				for (int i = 0; i < PaintObj.size(); i++) {
					Rendering now(PaintObj[i].first);
					Transformations now2 = PaintObj[i].second;
					Objects.push_back({ now, now2 });
				}

				Projection = ProjectonMatrix;
				View = ViewMatrix;
			}
			//InvalidateRect(hwnd, NULL, true);
			hdc = GetDC(hwnd);
			hdcBuffer = CreateCompatibleDC(hdc);					//
			hBitmap = CreateCompatibleBitmap(hdc, width, height);  //Emergency!!!
			hOldBitmap = (HBITMAP)SelectObject(hdcBuffer, hBitmap);//

			RECT rect;
			GetClientRect(hwnd, &rect);
			HBRUSH white = CreateSolidBrush(RGB(0, 0, 0));
			FillRect(hdcBuffer, &rect, white);

			long long eee = GetTickCount64();
			Renderer::DrawGame(Objects, hdcBuffer, Projection, View, width / 2.0f, height / 2.0f);
			long long ee = GetTickCount64();

			long long newTime = GetTickCount64();
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
			HFONT hOldFont = (HFONT)SelectObject(hdcBuffer, hFont);

			// Set the text color
			SetTextColor(hdcBuffer, RGB(255, 0, 0));

			// Set the background color
			SetBkColor(hdcBuffer, RGB(255, 255, 0));

			// Draw the text
			std::wstring dtString = std::to_wstring(slep) + L", " + std::to_wstring(allDt) + L", " + std::to_wstring(eee - e) + L", " + std::to_wstring(ee - eee);
			TextOut(hdcBuffer, 10, 10, dtString.c_str()/*std::to_wstring(Objects.size()).c_str()*/, static_cast<int>(dtString.length()));

			BitBlt(hdc, 0, 0, width, height, hdcBuffer, 0, 0, SRCCOPY);

			SelectObject(hdcBuffer, hOldBitmap);//
			DeleteObject(hBitmap);//
			DeleteDC(hdcBuffer);//
			ReleaseDC(hwnd, hdc);//
			slep = 0;
		}

		{
			std::lock_guard<std::mutex> lock(EndProgramMutex);

			if (EndProgram)
				break;
		}
	}
}