#include <imgui_internal.h>
#include <imgui.h>

#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <variant>

#include <d3d11.h>
#include <unordered_map>
#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

static int ACCENT_R = 255;
static int ACCENT_G = 0;
static int ACCENT_B = 0;

#define getArraySize(array) (sizeof(array) / sizeof((array)[0]))

static int ordershit = 1;
ImGuiID GenRandomID() {
    static std::mt19937 rng(std::random_device{}()); // Random number generator initialized with std::random_device for non-deterministic seeds
    std::uniform_int_distribution<unsigned int> dist; // Default range is the full range of unsigned int

    return dist(rng); // Generate and return a random ImGuiID
}


void SetAlphaForDrawList(ImDrawList* draw_list, float alpha) {
    if (!draw_list) return;

    // Clamp the alpha value between 0.0f and 1.0f
    alpha = ImClamp(alpha, 0.0f, 1.0f);
    ImU32 alpha_u32 = (ImU32)(alpha * 255.0f);

    for (int i = 0; i < draw_list->VtxBuffer.Size; i++) {
        ImDrawVert& vertex = draw_list->VtxBuffer[i];

        // Extract current color components
        ImU32 col = vertex.col;
        ImU32 r = (col >> IM_COL32_R_SHIFT) & 0xFF;
        ImU32 g = (col >> IM_COL32_G_SHIFT) & 0xFF;
        ImU32 b = (col >> IM_COL32_B_SHIFT) & 0xFF;
        ImU32 a = (col >> IM_COL32_A_SHIFT) & 0xFF;

        // Modify the alpha component
        a = alpha_u32;

        // Pack the color back together
        vertex.col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
    }
}

class HighResTimer {
public:
    HighResTimer() {
        QueryPerformanceFrequency(&frequency);
        startCount.QuadPart = 0;
        startCount.QuadPart = 0;
    }

    void Start() {
        QueryPerformanceCounter(&startCount);
    }

    float GetElapsedSeconds() {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        return static_cast<float>((current.QuadPart - startCount.QuadPart) / double(frequency.QuadPart));
    }

private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER startCount;
};

HighResTimer timer;

template<typename T>
struct AnimationControl {
    steady_clock::time_point startTime;
    T startValue;
    bool isActive;
};

template<typename T>
void animrender(T from, T to, T* v, float duration) {
    static unordered_map<T*, AnimationControl<T>> animations;

    // Retrieve or create the control block for this variable
    auto& ctrl = animations[v];

    // Initialize only if not active and the current value is not the target
    if (!ctrl.isActive && *v != to) {
        ctrl.startTime = steady_clock::now();
        ctrl.startValue = *v;  // Set to current value to start from
        *v = from;
        ctrl.isActive = true;
    }

    // Process animation if it's active
    if (ctrl.isActive) {
        float elapsedMillis = duration_cast<milliseconds>(steady_clock::now() - ctrl.startTime).count();
        float totalMillis = duration * 1000;

        if (elapsedMillis < totalMillis) {
            float fraction = elapsedMillis / totalMillis;
            *v = ctrl.startValue + (to - ctrl.startValue) * fraction;  // Linear interpolation
        }
        else {
            *v = to;  // Ensure it ends exactly at the target
            ctrl.isActive = false;  // Deactivate animation
        }
    }
}

template<typename T>
void Animate(T from, T to, T* v, float duration = 1.0f) {
    animrender(from, to, v, duration);
}

std::string ImGuiKeyToString(int key)
{
    switch (key)
    {
    case ImGuiKey_None: return "None";
    case ImGuiKey_Tab: return "Tab";
    case ImGuiKey_LeftArrow: return "LeftArrow";
    case ImGuiKey_RightArrow: return "RightArrow";
    case ImGuiKey_UpArrow: return "UpArrow";
    case ImGuiKey_DownArrow: return "DownArrow";
    case ImGuiKey_PageUp: return "PageUp";
    case ImGuiKey_PageDown: return "PageDown";
    case ImGuiKey_Home: return "Home";
    case ImGuiKey_End: return "End";
    case ImGuiKey_Insert: return "Ins";
    case ImGuiKey_Delete: return "Del";
    case ImGuiKey_Backspace: return "Backspace";
    case ImGuiKey_Space: return "Space";
    case ImGuiKey_Enter: return "Ent";
    case ImGuiKey_Escape: return "Esc";
    case ImGuiKey_LeftCtrl: return "Ctrl";
    case ImGuiKey_LeftShift: return "Shift";
    case ImGuiKey_LeftAlt: return "Alt";
    case ImGuiKey_LeftSuper: return "Super";
    case ImGuiKey_RightCtrl: return "Ctrl";
    case ImGuiKey_RightShift: return "Shift";
    case ImGuiKey_RightAlt: return "Alt";
    case ImGuiKey_RightSuper: return "Super";
    case ImGuiKey_Menu: return "Menu";
    case ImGuiKey_0: return "0";
    case ImGuiKey_1: return "1";
    case ImGuiKey_2: return "2";
    case ImGuiKey_3: return "3";
    case ImGuiKey_4: return "4";
    case ImGuiKey_5: return "5";
    case ImGuiKey_6: return "6";
    case ImGuiKey_7: return "7";
    case ImGuiKey_8: return "8";
    case ImGuiKey_9: return "9";
    case ImGuiKey_A: return "A";
    case ImGuiKey_B: return "B";
    case ImGuiKey_C: return "C";
    case ImGuiKey_D: return "D";
    case ImGuiKey_E: return "E";
    case ImGuiKey_F: return "F";
    case ImGuiKey_G: return "G";
    case ImGuiKey_H: return "H";
    case ImGuiKey_I: return "I";
    case ImGuiKey_J: return "J";
    case ImGuiKey_K: return "K";
    case ImGuiKey_L: return "L";
    case ImGuiKey_M: return "M";
    case ImGuiKey_N: return "N";
    case ImGuiKey_O: return "O";
    case ImGuiKey_P: return "P";
    case ImGuiKey_Q: return "Q";
    case ImGuiKey_R: return "R";
    case ImGuiKey_S: return "S";
    case ImGuiKey_T: return "T";
    case ImGuiKey_U: return "U";
    case ImGuiKey_V: return "V";
    case ImGuiKey_W: return "W";
    case ImGuiKey_X: return "X";
    case ImGuiKey_Y: return "Y";
    case ImGuiKey_Z: return "Z";
    case ImGuiKey_F1: return "F1";
    case ImGuiKey_F2: return "F2";
    case ImGuiKey_F3: return "F3";
    case ImGuiKey_F4: return "F4";
    case ImGuiKey_F5: return "F5";
    case ImGuiKey_F6: return "F6";
    case ImGuiKey_F7: return "F7";
    case ImGuiKey_F8: return "F8";
    case ImGuiKey_F9: return "F9";
    case ImGuiKey_F10: return "F10";
    case ImGuiKey_F11: return "F11";
    case ImGuiKey_F12: return "F12";
    case ImGuiKey_F13: return "F13";
    case ImGuiKey_F14: return "F14";
    case ImGuiKey_F15: return "F15";
    case ImGuiKey_F16: return "F16";
    case ImGuiKey_F17: return "F17";
    case ImGuiKey_F18: return "F18";
    case ImGuiKey_F19: return "F19";
    case ImGuiKey_F20: return "F20";
    case ImGuiKey_F21: return "F21";
    case ImGuiKey_F22: return "F22";
    case ImGuiKey_F23: return "F23";
    case ImGuiKey_F24: return "F24";
    case ImGuiKey_Apostrophe: return "Apostrophe";
    case ImGuiKey_Comma: return "Comma";
    case ImGuiKey_Minus: return "Minus";
    case ImGuiKey_Period: return "Period";
    case ImGuiKey_Slash: return "Slash";
    case ImGuiKey_Semicolon: return "Semicolon";
    case ImGuiKey_Equal: return "Equal";
    case ImGuiKey_LeftBracket: return "LeftBracket";
    case ImGuiKey_Backslash: return "Backslash";
    case ImGuiKey_RightBracket: return "RightBracket";
    case ImGuiKey_GraveAccent: return "GraveAccent";
    case ImGuiKey_CapsLock: return "CapsLock";
    case ImGuiKey_ScrollLock: return "ScrollLock";
    case ImGuiKey_NumLock: return "NumLock";
    case ImGuiKey_PrintScreen: return "PrintScreen";
    case ImGuiKey_Pause: return "Pause";
    case ImGuiKey_Keypad0: return "Keypad0";
    case ImGuiKey_Keypad1: return "Keypad1";
    case ImGuiKey_Keypad2: return "Keypad2";
    case ImGuiKey_Keypad3: return "Keypad3";
    case ImGuiKey_Keypad4: return "Keypad4";
    case ImGuiKey_Keypad5: return "Keypad5";
    case ImGuiKey_Keypad6: return "Keypad6";
    case ImGuiKey_Keypad7: return "Keypad7";
    case ImGuiKey_Keypad8: return "Keypad8";
    case ImGuiKey_Keypad9: return "Keypad9";
    case ImGuiKey_KeypadDecimal: return "KeypadDecimal";
    case ImGuiKey_KeypadDivide: return "KeypadDivide";
    case ImGuiKey_KeypadMultiply: return "KeypadMultiply";
    case ImGuiKey_KeypadSubtract: return "KeypadSubtract";
    case ImGuiKey_KeypadAdd: return "KeypadAdd";
    case ImGuiKey_KeypadEnter: return "KeypadEnter";
    case ImGuiKey_KeypadEqual: return "KeypadEqual";
    case ImGuiKey_AppBack: return "AppBack";
    case ImGuiKey_AppForward: return "AppForward";
    case ImGuiKey_GamepadStart: return "GamepadStart";
    case ImGuiKey_GamepadBack: return "GamepadBack";
    case ImGuiKey_GamepadFaceLeft: return "GamepadFaceLeft";
    case ImGuiKey_GamepadFaceRight: return "GamepadFaceRight";
    case ImGuiKey_GamepadFaceUp: return "GamepadFaceUp";
    case ImGuiKey_GamepadFaceDown: return "GamepadFaceDown";
    case ImGuiKey_GamepadDpadLeft: return "GamepadDpadLeft";
    case ImGuiKey_GamepadDpadRight: return "GamepadDpadRight";
    case ImGuiKey_GamepadDpadUp: return "GamepadDpadUp";
    case ImGuiKey_GamepadDpadDown: return "GamepadDpadDown";
    case ImGuiKey_GamepadL1: return "GamepadL1";
    case ImGuiKey_GamepadR1: return "GamepadR1";
    case ImGuiKey_GamepadL2: return "GamepadL2";
    case ImGuiKey_GamepadR2: return "GamepadR2";
    case ImGuiKey_GamepadL3: return "GamepadL3";
    case ImGuiKey_GamepadR3: return "GamepadR3";
    case ImGuiKey_GamepadLStickLeft: return "GamepadLStickLeft";
    case ImGuiKey_GamepadLStickRight: return "GamepadLStickRight";
    case ImGuiKey_GamepadLStickUp: return "GamepadLStickUp";
    case ImGuiKey_GamepadLStickDown: return "GamepadLStickDown";
    case ImGuiKey_GamepadRStickLeft: return "GamepadRStickLeft";
    case ImGuiKey_GamepadRStickRight: return "GamepadRStickRight";
    case ImGuiKey_GamepadRStickUp: return "GamepadRStickUp";
    case ImGuiKey_GamepadRStickDown: return "GamepadRStickDown";
    case ImGuiKey_MouseLeft: return "MouseLeft";
    case ImGuiKey_MouseRight: return "MouseRight";
    case ImGuiKey_MouseMiddle: return "MouseMiddle";
    case ImGuiKey_MouseX1: return "MouseX1";
    case ImGuiKey_MouseX2: return "MouseX2";
    case ImGuiKey_MouseWheelX: return "MouseWheelX";
    case ImGuiKey_MouseWheelY: return "MouseWheelY";
    case ImGuiKey_ReservedForModCtrl: return "ReservedForModCtrl";
    case ImGuiKey_ReservedForModShift: return "ReservedForModShift";
    case ImGuiKey_ReservedForModAlt: return "ReservedForModAlt";
    case ImGuiKey_ReservedForModSuper: return "ReservedForModSuper";
    default: return "Unknown";
    };
}
