// D3DD11Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>

#include <d3d11.h>
#include <d3dX11.h>
#include <D3DX10.h>

//Direct3D lib files
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


IDXGISwapChain * swapChain; //swapchain interface
ID3D11Device * device; // device interface
ID3D11DeviceContext * devcon; //device content

void InitD3D(HWND inhWnd);
void CleanD3D(void);

//hInstance is our handle for the first application and hPrevinstance is our handle to the previous instance
//windows provides application with individual memory space so prevInstance is for backwards compatibility and is usually NULL
//nCmdshow  This parameter indicates how the window is to appear when created.
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR LpCmdLine, int nCmdShow);

LRESULT CALLBACK ourWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//bool AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, bool bMenu);

void InitD3D(HWND inhWnd)
{
	//Create swap chain struct and clear and reserve it
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1; //one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //dictates swap chain usage
	scd.OutputWindow = inhWnd; //window to be used
	scd.SampleDesc.Count = 4; //multisamples used
	scd.Windowed = TRUE; 

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &devcon);



}

void CleanD3D(void)
{
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR LpCmdLine, int nCmdShow)
{
	//Havent created a window, so this message comes from the desktop not a window hence NULL 
	// We Give it an exclamation icon aswell as a single ok to continue box
	//MessageBox(NULL, L"Wot", L"GoingToIgnoreThat", MB_ICONEXCLAMATION | MB_OK);


	//window Handle
	HWND hWnd;

	//holds window information
	WNDCLASSEX wc;

	//Clears and reserves
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; // will reset this value when we get into fullscreen games
	wc.lpfnWndProc = ourWndProc;
	wc.hInstance = hinstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&wc);
	//0 top and left position,
	RECT wr = { 0, 0, 500, 400 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	//Create a window, class name and title of window
	//dword styles would be where we keep alot of options for window creating
	hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Moses Playboy Mansion", WS_OVERLAPPEDWINDOW,
		//x, y position and width and height of the window, no parent or menues
		300, 300, 500, 400, NULL, NULL, hinstance, NULL);


	ShowWindow(hWnd, nCmdShow);
	
	//Holds window event messages
	MSG msg;

	//MainLoop
	//Wait for the next msg in queue and store it 
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Translates keystrokes to the right format
			TranslateMessage(&msg);

			//sends msgs to our windows proc function
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			//gamecode
		}
	}

	return msg.wParam;
}
LRESULT CALLBACK ourWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY://We return back that the window was closed as getmessage returns false if the program is about to quit
		PostQuitMessage(0);
		//Tells windows that we handled the message
		return 0;
		break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


