#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>

#include <imgui.h>
#include <imgui_impl_win32.h>

#include <nlohmann/json.hpp>

#include <iostream>

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
    float alpha;
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
    void set_accentcolor(float color[4]);

    float* convertImColortoFloat(ImColor cl);

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
    void add_multicombo(std::string label, std::vector<bool>* selected, std::vector<std::string> items);

    void add_colorpicker(std::string label, ImU32* color, bool alpha = true);
    void add_keybind(std::string Label, static std::vector<int>* value, static ImGuiKey default_value);
};

class renderer
{
public:
    void render();
    void setup_fonts();
    void setup();
};

using SettingValue = std::variant<int, float, bool, std::vector<bool>, std::vector<int>, std::vector<float>>;
using json = nlohmann::json;

class config_manager
{
public:
    std::unordered_map<std::string, SettingValue> CONFIG_VARIABLES;
    std::unordered_map<std::string, ImU32> CONFIG_COLOR;
    std::unordered_map <std::string, std::vector<int>> CONFIG_KEYBIND;

    template<typename T>
    T createdefaultmonkey();

    template<typename T>
    T createdefaultkeybind();

    template<>
    ImColor createdefaultmonkey<ImColor>() {
        return ImColor(1.0f, 1.0f, 1.0f, 1.0f);  // Default white color
    }

    template<>
    std::vector<int> createdefaultkeybind<std::vector<int>>() {
        //    key        type 
        return { ImGuiKey_None, 0 }; // 0 = Always On, 1 = Hold, 2 = Toggle
    }

    bool isUInt32(int64_t value) {
        return value >= 0 && value <= 0xFFFFFFFF;  // 0xFFFFFFFF is 4,294,967,295
    }

    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string& in) {
        std::string out;
        int val = 0, valb = -6;
        for (unsigned char c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() % 4) out.push_back('=');
        return out;
    }

    std::string base64_decode(const std::string& in) {
        std::string out;
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

        int val = 0, valb = -8;
        for (unsigned char c : in) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
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

    template<typename T>
    T& get_color(const std::string& name) {
        auto it = CONFIG_COLOR.find(name);
        if (it == CONFIG_COLOR.end()) {
            // If the setting does not exist, create it with a default value
            ImColor defaultColor = createdefaultmonkey<ImColor>();
            CONFIG_COLOR[name] = ImGui::ColorConvertFloat4ToU32(defaultColor);
            it = CONFIG_COLOR.find(name);
        }
        return it->second;
    }

    template<typename T>
    T& get_keybind(const std::string& name) {
        auto it = CONFIG_KEYBIND.find(name);
        if (it == CONFIG_KEYBIND.end()) {
            // If the setting does not exist, create it with a default value
            std::vector<int> defaultKeybind = createdefaultkeybind<std::vector<int>>();
            CONFIG_KEYBIND[name] = defaultKeybind;
            it = CONFIG_KEYBIND.find(name);
        }
        return it->second;
    }

    // Helper to convert ImColor to ImU32
    ImU32 toImU32(const ImColor& color) {
        return ImGui::ColorConvertFloat4ToU32(color.Value);
    }

    void save(const std::string& filename) {
        std::vector<std::string> arrayofnames = { "vars","cols","keys" };

        //json j;
        /*
        for (const auto& item : CONFIG_VARIABLES
            ) {
            const auto& key = item.first;
            const auto& value = item.second;
            std::visit([&j, &key](auto&& val) {
                j[key] = val;
                }, value);
        }

        for (const auto& item : c_cfg_colorpicker->CONFIG_COLOR
            ) {
            const auto& key = item.first;
            const auto& value = item.second;
            std::visit([&j, &key](auto&& val) {
                j[key] = val;
                }, value);
        }


        for (const auto& item : c_cfg_keybind->CONFIG_KEYBIND
            ) {
            const auto& key = item.first;
            const auto& value = item.second;
            std::visit([&j, &key](auto&& val) {
                j[key] = val;
                }, value);
        }


        for (const auto& item : CONFIG_COLOR
            ) {
            const auto& key = item.first;
            const auto& value = item.second;
            [&j, &key](auto&& val) {
                j[key] = val;
                }, value;
        }
        */

        nlohmann::json j_color = CONFIG_COLOR;
        nlohmann::json j_keybind = CONFIG_KEYBIND;

        std::cout << j_color.dump(4) << std::endl;
        std::cout << j_keybind.dump(4) << std::endl;
        //std::ofstream file(filename + ".cfg");
        //file << j_color.dump(4); //base64_encode(j.dump(4));  // Pretty print with 4 spaces
        //file.close();
    }

    void load(const std::string& filename) {
        std::ifstream file(filename + ".cfg");
        if (!file) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();

        json j;
        //std::istringstream(base64_decode(buffer.str())) >> j;
        file >> j;

        for (auto it = j.begin(); it != j.end(); ++it) {
            const std::string& key = it.key();
            const json& value = it.value();

            if (value.is_number_integer()) {
                //int val = value.get<int>();

                //if(!isUInt32(val) )
                CONFIG_VARIABLES[key] = value.get<int>();
                //else
                //    CONFIG_VARIABLES[key] = ImU32(val);
            }
            else if (value.is_number_float()) {
                CONFIG_VARIABLES[key] = value.get<float>();
            }
            else if (value.is_boolean()) {
                CONFIG_VARIABLES[key] = value.get<bool>();
            }
            else if (value.is_array())
            {
                CONFIG_VARIABLES[key] = value.get<std::vector<bool>>();
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
            return std::vector<bool>({ false,false,false,false,false,false,false,false,false,false,false,false });
        }
        else if constexpr (std::is_same_v<T, ImColor>) {
            return ImColor(1.0f, 1.0f, 1.0f, 1.0f);
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

class color_class
{
public:
    ImColor get(ImColor v)
    {
        // if its 1 just use the alpha inputted
        if (c_menu.alpha == 1)
        {
            return ImColor(v.Value.x, v.Value.y, v.Value.z, v.Value.w);
        }
        else // the alpha is changing so start to decrease
        {
            if (c_menu.alpha <= v.Value.w)
                return ImColor(v.Value.x, v.Value.y, v.Value.z, c_menu.alpha);
        }
    }

    ImColor get(int r, int g, int b, int a = 255)
    {
        float alphaFraction = static_cast<float>(a) / 255.0f;
        int finalAlpha = (c_menu.alpha <= alphaFraction) ?
            static_cast<int>(c_menu.alpha * 255.0f) : a;
        return ImColor(r, g, b, finalAlpha);
    }

    ImColor get(float r, float g, float b, float a = 255)
    {
        // if its 1 just use the alpha inputted
        if (c_menu.alpha == 1)
        {
            return ImColor(r, g, b, a);
        }
        else // the alpha is changing so start to decrease
        {
            if (c_menu.alpha <= a)
                return ImColor(r, g, b, c_menu.alpha);
        }
    }
};

inline color_class* c_color;
