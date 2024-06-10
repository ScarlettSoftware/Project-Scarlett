#include <unordered_map>
#include <chrono>
#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <variant>
#include <profileapi.h>

using namespace std;
using namespace chrono;

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
