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
ID3D11RenderTargetView * backBuffer;
ID3D11VertexShader * pVs;
ID3D11PixelShader * pPs;

void InitD3D(HWND inhWnd);
void InitPipeline();
void CleanD3D(void);
void RenderFrame(void);

//hInstance is our handle for the first application and hPrevinstance is our handle to the previous instance
//windows provides application with individual memory space so prevInstance is for backwards compatibility and is usually NULL
//nCmdshow  This parameter indicates how the window is to appear when created.
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR LpCmdLine, int nCmdShow);
LRESULT CALLBACK ourWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

void InitD3D(HWND inhWnd)
{
	//Create swap chain struct and clear and reserve it
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1; //one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //dictates swap chain usage
	scd.OutputWindow = inhWnd; //window to be used
	scd.SampleDesc.Count = 4; //multisamples used
	scd.Windowed = TRUE; 
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &devcon);

	//Gets the address of the back buffer, we are only using 1 back buffer on the chain so it's address is going to be 0
	ID3D11Texture2D * pBackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//use the bb address to create a render target
	device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	//set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backBuffer, NULL);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;

	//Activates viewport structs, param1 = number of viewports being used, param2 address of list of points to vp structs
	devcon->RSSetViewports(1, &vp);

}

void InitPipeline()
{
	ID3D10Blob * vs, *ps;
	D3DX11CompileFromFile(L"Shaders/Shaders.shader", 0, 0, "vert", "vs_4_0", 0, 0, 0, &vs, 0, 0);
	D3DX11CompileFromFile(L"Shaders/Shaders.shader", 0, 0, "frag", "ps_4_0", 0, 0, 0, &ps, 0, 0);
	device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &pVs);
	device->CreatePixelShader(ps->GetBufferPointer(), vs->GetBufferSize(), NULL, &pPs);

	devcon->VSSetShader(pVs, 0, 0);
	devcon->PSSetShader(pPs, 0, 0);
}

void CleanD3D(void)
{
	swapChain->SetFullscreenState(false, NULL);

	pVs->Release();
	pPs->Release();

	swapChain->Release();
	backBuffer->Release();
	device->Release();
	devcon->Release();
}

void RenderFrame(void)
{
	// clears bb to grey
	devcon->ClearRenderTargetView(backBuffer, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	//3d Rendering would be done here

	//Swap the back buffer to the front
	swapChain->Present(0, 0);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR LpCmdLine, int nCmdShow)
{
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
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&wc);
	//0 top and left position,
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	//Create a window, class name and title of window
	//dword styles would be where we keep alot of options for window creating
	hWnd = CreateWindowEx(NULL, wc.lpszClassName, L"Moses Playboy Mansion", WS_OVERLAPPEDWINDOW,
		//x, y position and width and height of the window, no parent or menues
		300, 300, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hinstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	
	InitD3D(hWnd);

	InitPipeline();

	//Holds window event messages
	MSG msg;

	//MainLoop
	//Wait for the next msg in queue and store it 
	while (TRUE)
	{	// exits loop if program is closed
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
			//gamecode
			RenderFrame();
	}
	CleanD3D();
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


