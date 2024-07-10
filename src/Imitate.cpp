#include "Imitate.h"

namespace Alastor {

    Imitate::Imitate()
    {

    }



    void Imitate::mouseMessage(HWND hWnd, int x, int y, int mouseButton, int mouseAction, bool wait) {
        // 检查窗口句柄是否有效  
        if (hWnd == NULL) {
            hWnd = GetDesktopWindow();
        }

        // 确保坐标有效  
        if (x < 0 || y < 0) {
            return;
        }

        // 计算坐标值  
        LPARAM lParam = MAKELPARAM(x, y);

        // 根据鼠标按钮和动作发送消息  
        switch (mouseButton) {
        case 1: // 左键  
            switch (mouseAction) {
            case 1: // 单击  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                break;
            case 2: // 双击  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                    SendMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                    // 注意：异步发送双击消息可能不会按预期工作，因为两次点击可能不会在很短的时间内到达  
                    PostMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, lParam);
                }
                break;
            case 3: // 按下  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
                }
                break;
            case 4: // 放开  
                if (wait) {
                    SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_LBUTTONUP, 0, lParam);
                }
                break;
            }
            break;
        case 2: // 右键  
            switch (mouseAction) {
            case 1: // 单击  
                if (wait) {
                    SendMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
                    SendMessage(hWnd, WM_RBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
                    PostMessage(hWnd, WM_RBUTTONUP, 0, lParam);
                }
                break;
                // 右键的双击、按下和放开可以根据需要添加  
            }
            break;
        case 3: // 中键  
            switch (mouseAction) {
            case 1: // 单击  
                if (wait) {
                    SendMessage(hWnd, WM_MBUTTONDOWN, MK_MBUTTON, lParam);
                    SendMessage(hWnd, WM_MBUTTONUP, 0, lParam);
                }
                else {
                    PostMessage(hWnd, WM_MBUTTONDOWN, MK_MBUTTON, lParam);
                    PostMessage(hWnd, WM_MBUTTONUP, 0, lParam);
                }
                break;
                // 中键的双击、按下和放开可以根据需要添加  
            }
            break;
            // 可以添加更多的鼠标按钮类型  
        default:
            // 如果鼠标按钮不在1-3的范围内，则不发送任何消息  
            break;
        }
    }
}
