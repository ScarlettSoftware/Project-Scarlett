#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>

#include <imgui.h>
#include <imgui_impl_win32.h>

#include <nlohmann/json.hpp>

enum CHILDPOSITIONS
{
    LEFT,
    RIGHT,
    HALFLEFT,
    HALFRIGHT
};

enum CHILDSIZES
{
    FULL,
    HALF,
    FULLX2
};

enum SLIDERDEFINES
{
    SLIDER_INT,
    SLIDER_FLOAT
};

class menu
{
public:
    menu();

    int childId;
    int mainChildId;

    bool MENU_OPEN;
    bool ISBOOTING;
    bool BOOTLOOP;
    int MAIN_ROUNDING; // main round for the whole window ( sleepy this is only for the edges of the window and the rects it dosent do nothing else)
    int MAIN_SELECTION; // main selection index
    std::vector<int> MAIN_SELECTIONALPHA; // alpha value for main selection

    std::vector<std::string> MAIN_BUTTONSLABELS;
    std::vector<std::string> MAIN_BUTTONSICONS;

    std::vector<int> MAIN_BUTTONSYPOS; // position of the main button when selected or not !DONT TOUCH!

    std::vector<int> MAIN_SELECTIONYPOS; // y position of every tab ( needed for animation ) !DONT TOUCH!
    float MAIN_SELECTIONTABALPHA; // alpha of every tab 0.0f <-> 1.0f

    ImDrawList* draw_list;
    ImDrawList* current_child_dl;

    void set_font(std::string label, ImFont* font);
    void set_accentcolor(ImColor clr);

    void boot();

    void begin(std::string label);
    void end();

    void draw_frame();

    void selection_begin(static int selected);
    void selection_end();

    void add_subtab(std::vector<std::string> labels, static int* selected);
    void begin_child(std::string Label, std::string Icon, ImVec2 pos);
    void end_child();

    bool add_button(std::string Label);
    void add_label(std::string Label, bool centered = false);

    void add_checkbox(std::string Label, static bool* value); // checkbox

    void add_slider(std::string Label, int* value, int min, int max, std::string format = "");
    void add_slider(std::string Label, float* value, float min, float max, std::string format = "");

    void add_combo(std::string label, int* selected, std::vector<std::string> items);
    void add_multicombo(std::string label, std::vector<bool> *selected, std::vector<std::string> items);

    void add_colorpicker(std::string label, ImColor* color, bool alpha = true);
    void add_keybind(std::string Label, static bool* value, static ImGuiKey default_value);
};

class renderer
{
public:
    void render();
    void setup_fonts();
    void setup();
};

using SettingValue = std::variant<int, float, bool, std::vector<bool>>;
using json = nlohmann::json;
class config_manager
{
public:
    std::unordered_map<std::string, SettingValue> CONFIG_VARIABLES;

    void set(std::string name, SettingValue value) {
        CONFIG_VARIABLES[name] = value;
    }

    template <typename T>
    T& get(const std::string& name) {
        auto it = CONFIG_VARIABLES.find(name);
        if (it == CONFIG_VARIABLES.end()) {
            // If the setting does not exist, create it with a default value
            CONFIG_VARIABLES[name] = createDefault<T>();
            it = CONFIG_VARIABLES.find(name);
        }

        if (T* val = std::get_if<T>(&it->second)) {
            return *val;
        }
    }

    void save(const std::string& filename) {
        json j;
        for (const auto& item : CONFIG_VARIABLES) {
            const auto& key = item.first;
            const auto& value = item.second;
            std::visit([&j, &key](auto&& val) {
                j[key] = val;
                }, value);
        }
        std::ofstream file(filename);
        file << j.dump(4);  // Pretty print with 4 spaces
        file.close();
    }

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }

        json j;
        file >> j;

        for (auto it = j.begin(); it != j.end(); ++it) {
            const std::string& key = it.key();
            const json& value = it.value();

            if (value.is_number_integer()) {
                CONFIG_VARIABLES[key] = value.get<int>();
            }
            else if (value.is_number_float()) {
                CONFIG_VARIABLES[key] = value.get<float>();
            }
            else if (value.is_boolean()) {
                CONFIG_VARIABLES[key] = value.get<bool>();
            }
        }
    }

    void set_location(std::string path);

private:
    template <typename T>
    SettingValue createDefault() {
        if constexpr (std::is_same_v<T, int>) {
            return 0;
        }
        else if constexpr (std::is_same_v<T, float>) {
            return 0.0f;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return false;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return std::string();
        }
        else if constexpr (std::is_same_v<T, std::vector<bool>>) {
            return std::vector<bool>({false,false,false,false,false,false,false,false,false,false,false,false });
        }
        else {
            static_assert(always_false<T>::value, "Unsupported type");
        }
    }

    // Helper struct for static_assert to always fail in createDefault
    template <typename>
    struct always_false : std::false_type {};
};

inline menu c_menu;
inline renderer c_renderer;
inline config_manager c_cfg;
