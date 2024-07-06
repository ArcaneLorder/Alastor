#include "Window.h"

using namespace std;
using namespace Alastor;
Window::Window()
{
}

bool Window::hide(HWND hWnd, bool wait)
{
	if (wait)
	{
		return ShowWindow(hWnd, SW_HIDE) == TRUE;
	}
	else
	{
		return ShowWindowAsync(hWnd, SW_HIDE) == TRUE;
	}

	return false;
}

bool Window::show(HWND hWnd, bool wait)
{
	if (wait)
	{
		return ShowWindow(hWnd, SW_SHOWNORMAL) == TRUE;
	}
	else
	{
		return ShowWindowAsync(hWnd, SW_SHOWNORMAL) == TRUE;
	}
	return false;
}

bool Window::minimize(HWND hWnd, bool wait)
{
	if (wait)
	{
		return ShowWindow(hWnd, SW_SHOWMINIMIZED) == TRUE;
	}
	else
	{
		return ShowWindowAsync(hWnd, SW_SHOWMINIMIZED) == TRUE;
	}
	return false;
}

bool Window::maximize(HWND hWnd, bool wait)
{
	if (wait)
	{
		return ShowWindow(hWnd, SW_SHOWMAXIMIZED) == TRUE;
	}
	else
	{
		return ShowWindowAsync(hWnd, SW_SHOWMAXIMIZED) == TRUE;
	}
	return false;
}

bool Window::restore(HWND hWnd, bool wait)
{
	if (wait)
	{
		return ShowWindow(hWnd, SW_RESTORE) == TRUE;
	}
	else
	{
		return ShowWindowAsync(hWnd, SW_RESTORE) == TRUE;
	}
	return false;
}

bool Window::visible(HWND hWnd)
{
	return IsWindowVisible(hWnd) == TRUE;
}

bool Window::minimum(HWND hWnd)
{
	return IsIconic(hWnd) == TRUE;
}

bool Window::maximum(HWND hWnd)
{
	return IsZoomed(hWnd) == TRUE;
}

void Window::close(HWND hWnd, bool wait)
{
	if (wait)
	{
		SendMessage(hWnd,WM_CLOSE,NULL,NULL);
	}
	else
	{
		PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}

HWND Window::ancestor(HWND hWnd)
{
	return GetAncestor(hWnd, GA_ROOTOWNER);
}

HWND Window::parent(HWND hWnd)
{
	return GetParent(hWnd);
}

std::string Window::title(HWND hWnd)
{
	std::string title;
	int length = GetWindowTextLengthA(hWnd);
	if (!length) 
	{
		return title;
	}
	title.resize(length);
	GetWindowTextA(hWnd, const_cast<char*>(title.c_str()),length);
	return title;
}

std::string Window::className(HWND hWnd)
{
	char buffer[MAX_PATH] = { 0 };
	GetClassNameA(hWnd, buffer, MAX_PATH);
	return buffer;
}

std::vector<HWND> Window::subWindows(HWND hWnd)
{
	std::vector<HWND> list;
	if (!hWnd)
	{
		hWnd = GetDesktopWindow();
	}

	HWND childHandle = GetWindow(hWnd, GW_CHILD);
	while (childHandle)
	{
		list.push_back(childHandle);
		childHandle = GetWindow(childHandle, GW_HWNDNEXT);
	}

	return list;
}

HWND Window::find(const char* title,const char* className)
{
	if (!title && !className)
	{
		return NULL;
	}

	HWND desktop = GetDesktopWindow();
	HWND child = GetWindow(desktop, GW_CHILD);
	while(child)
	{
		size_t index;
		if (!title && className)
		{
			if (String(Window::className(child)).toLower().indexOf(String(className).toLower(), index))
			{
				return child;
			}
		}

		if (title && !className)
		{
			if (String(Window::title(child)).toLower().indexOf(String(title).toLower(), index))
			{
				return child;
			}
		}

		if (title && className)
		{
			if (String(Window::className(child)).toLower().indexOf(String(className).toLower(), index) &&
				String(Window::title(child)).toLower().indexOf(String(title).toLower(), index))
			{
				return child;
			}
		}

		child = GetWindow(child, GW_HWNDNEXT);
	}
	return NULL;
}

COLORREF Window::color(HWND hWnd, int x, int y)
{
	HDC dc = GetDC(hWnd);
	COLORREF color = GetPixel(dc,x,y);
	ReleaseDC(hWnd,dc);
	return color;
}
