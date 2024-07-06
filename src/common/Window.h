#pragma once
#include "Alastor.h"

class ALASTOR_API Window {
public:
	explicit Window();
public:
	static bool hide(HWND hWnd, bool wait = false);
	static bool show(HWND hWnd, bool wait = false);
	static bool minimize(HWND hWnd, bool wait = false);
	static bool maximize(HWND hWnd, bool wait = false);
	static bool restore(HWND hWnd, bool wait = false);
	static bool visible(HWND hWnd);
	static bool minimum(HWND hWnd);
	static bool maximum(HWND hWnd);
	static void close(HWND hWnd, bool wait = false);
	static HWND ancestor(HWND hWnd);
	static HWND parent(HWND hWnd);
	static std::string title(HWND hWnd);
	static std::string className(HWND hWnd);
	static std::vector<HWND> subWindows(HWND hWnd = NULL);
	static HWND find(const char* title,const char* className = nullptr);
	static COLORREF color(HWND hWnd, int x, int y);
};