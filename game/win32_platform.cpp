
#include <windows.h>

boolean running = true;
void* buffer_memeory;
int buffer_width;
int buffer_height;
BITMAPINFO buffer_bitmap_info;
// Define the Window Procedure function
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
		case WM_CLOSE :
		case WM_DESTROY: {
			running = false;
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			buffer_width = rect.right - rect.left;
			buffer_height = rect.bottom - rect.top;
			int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

			if (buffer_memeory) VirtualFree(buffer_memeory, 0, MEM_RELEASE);

			buffer_memeory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
			buffer_bitmap_info.bmiHeader.biHeight = buffer_width;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
		}break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nShowCmd) {

	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"GameWindowClass";
	window_class.lpfnWndProc = window_callback;

	//Register Class
	RegisterClass(&window_class);

	//Create class
	HWND window = CreateWindow(window_class.lpszClassName, L"My First Game!",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720, 0, 0, hInstance,0);

	HDC hdc = GetDC(window);

		while (running) {
			MSG message;
			while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			};

			//simulate 
			unsigned int* pixel = (unsigned int*)buffer_memeory;
			for (int y = 0; y < buffer_height; y++) {
				for (int x = 0; x < buffer_width; x++) {
					*pixel++ = y*2;
				}

			}

			//render

			StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memeory,&buffer_bitmap_info, DIB_RGB_COLORS,SRCCOPY );
		};
};