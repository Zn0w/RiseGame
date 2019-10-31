#include <windows.h>

LRESULT CALLBACK MainWindowCallback(
	HWND window_handle,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT result = 0;
	
	switch (message)
	{
		case WM_SIZE:
		{
			OutputDebugStringA("WM_SIZE\n");
		} break;

		case WM_DESTROY:
		{
			OutputDebugStringA("WM_DESTROY\n");
		} break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			OutputDebugStringA("WM_CLOSE\n");
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;

		case WM_PAINT:
		{
			static DWORD color = WHITENESS;
			if (color == WHITENESS)
				color = BLACKNESS;
			else
				color = WHITENESS;
			
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window_handle, &paint);
			PatBlt(device_context, paint.rcPaint.left, paint.rcPaint.top, paint.rcPaint.right, paint.rcPaint.bottom, color);
			EndPaint(window_handle, &paint);
		} break;

		default:
		{
			//OutputDebugStringA("default\n");
			result = DefWindowProc(window_handle, message, wParam, lParam);
		} break;
	}

	return result;
}

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) // NOTE: MSDN
{
	WNDCLASSA window_class = {};	// init every member to 0
	window_class.style = CS_CLASSDC | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = MainWindowCallback;
	window_class.hInstance = hInstance;
	//window_class.hIcon;
	window_class.lpszClassName = "RiseGame_WindowClass";

	if (!RegisterClass(&window_class))
	{
		OutputDebugStringA("Failed to create window (class did not register)\n");
		return -1;	
	}

	HWND window_handle = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Rise Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0
	);

	if (window_handle)
	{
		while (1)
		{
			MSG message;
			if (GetMessage(&message, 0, 0, 0))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);		// Send message to the WindowProc
			}
			else
			{
				break;
			}
		}
	}
	
	return 0;
}