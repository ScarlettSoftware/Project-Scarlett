#include "cmenu.hpp"
#include <unordered_map>
#include "..\fonts.h"

void renderer::setup_fonts()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontDefault();
    
    c_menu.set_font("logo", io.Fonts->AddFontFromMemoryTTF(scarlett_icons, sizeof(scarlett_icons), 44));
    c_menu.set_font("selectionicons", io.Fonts->AddFontFromMemoryTTF(scarlett_icons, sizeof(scarlett_icons), 25));
    c_menu.set_font("subtabselection", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 17));
    c_menu.set_font("childtitle", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 17));
    c_menu.set_font("childicon", io.Fonts->AddFontFromMemoryTTF(scarlett_icons, sizeof(scarlett_icons), 17));
    c_menu.set_font("itemlabel", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 17));
    c_menu.set_font("slidervaluelabel", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 12));
    c_menu.set_font("comboitemlabel", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 15));
    c_menu.set_font("comboinlabel", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 15));
    c_menu.set_font("comboitemarrow", io.Fonts->AddFontFromMemoryTTF(scarlett_icons, sizeof(scarlett_icons), 12));
    c_menu.set_font("biglogo", io.Fonts->AddFontFromMemoryTTF(scarlett_icons, sizeof(scarlett_icons), 88));
    c_menu.set_font("keybindselection", io.Fonts->AddFontFromMemoryTTF(inter_font, sizeof(inter_font), 15));

    io.Fonts->Build();
}

void renderer::setup()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    c_cfg.save("monkey.cfg");

    style->WindowPadding = ImVec2(0, 0);
    style->WindowRounding = 15.0f;
    style->FramePadding = ImVec2(0, 0);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(0, 0);
    style->ItemInnerSpacing = ImVec2(0, 0);
    style->IndentSpacing = 0.0f;
    style->ScrollbarSize = 0.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImColor(204, 204, 211, 255);
    style->Colors[ImGuiCol_TextDisabled] = ImColor(61, 58, 74, 255);
    style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 22, 255);
    style->Colors[ImGuiCol_ChildBg] = ImColor(23, 23, 23, 255);
    style->Colors[ImGuiCol_PopupBg] = ImColor(23, 23, 23, 255);
    style->Colors[ImGuiCol_Border] = ImColor(33, 33, 36, 225);
    style->Colors[ImGuiCol_BorderShadow] = ImColor(235, 232, 224, 0);
    style->Colors[ImGuiCol_FrameBg] = ImColor(20, 20, 22, 255);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(61, 58, 74, 255);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(143, 143, 147, 255);
    style->Colors[ImGuiCol_TitleBg] = ImColor(25, 23, 31, 255);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(255, 250, 242, 191);
    style->Colors[ImGuiCol_TitleBgActive] = ImColor(23, 23, 23, 255);
    style->Colors[ImGuiCol_MenuBarBg] = ImColor(25, 23, 31, 255);
    style->Colors[ImGuiCol_ScrollbarBg] = ImColor(25, 23, 31, 255);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(204, 204, 211, 79);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(143, 143, 147, 255);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(15, 13, 18, 255);
    style->Colors[ImGuiCol_CheckMark] = ImColor(204, 204, 211, 79);
    style->Colors[ImGuiCol_SliderGrab] = ImColor(204, 204, 211, 79);
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(15, 13, 18, 255);
    style->Colors[ImGuiCol_Button] = ImColor(25, 23, 31, 255);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(61, 58, 74, 255);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(143, 143, 147, 255);
    style->Colors[ImGuiCol_Header] = ImColor(25, 23, 31, 255);
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(143, 143, 147, 255);
    style->Colors[ImGuiCol_HeaderActive] = ImColor(15, 13, 18, 255);
    style->Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(143, 143, 147, 255);
    style->Colors[ImGuiCol_ResizeGripActive] = ImColor(15, 13, 18, 255);
    style->Colors[ImGuiCol_PlotLines] = ImColor(102, 100, 97, 161);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImColor(64, 255, 0, 255);
    style->Colors[ImGuiCol_PlotHistogram] = ImColor(102, 100, 97, 161);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImColor(64, 255, 0, 255);
    style->Colors[ImGuiCol_TextSelectedBg] = ImColor(64, 255, 0, 110);
}

void renderer::render()
{

    c_menu.begin("Scarlett");

    switch (c_menu.MAIN_SELECTION)
    {
        // Aimbot
    case 0: {
        c_menu.selection_begin(c_menu.MAIN_SELECTION);

        static int SUBTAB_SELECTION = 0;
        c_menu.add_subtab({ "Legitbot", "Ragebot" }, &SUBTAB_SELECTION);

        switch (SUBTAB_SELECTION)
        {
        case 0: // Legitbot
            c_menu.begin_child("Global", "G", { LEFT, HALF });
            {
                c_menu.add_keybind("Enabled", & c_cfg.get_keybind<std::vector<int>>("legitbot_global_enabled"), ImGuiKey_E);
                c_menu.add_slider("Field of view", &c_cfg.get<int>("legitbot_fov"), 1, 180, " ");
                c_menu.add_checkbox("Silent aim", &c_cfg.get<bool>("legitbot_silent_aim"));
                c_menu.add_checkbox("Automatic fire", &c_cfg.get<bool>("legitbot_automatic_fire"));
                c_menu.add_checkbox("Automatic scope", &c_cfg.get<bool>("legitbot_automatic_scope"));
            }
            c_menu.end_child();

            c_menu.begin_child("Triggerbot", "H", { HALFLEFT, HALF });
            {
                c_menu.add_checkbox("Enabled", &c_cfg.get<bool>("legitbot_triggerbot_enabled"));
                c_menu.add_keybind("Triggerbot on key", &c_cfg.get_keybind<std::vector<int>>("legitbot_triggerbot_on_key"), ImGuiKey_E);
                c_menu.add_slider("Field of view ", &c_cfg.get<int>("legitbot_triggerbot_fov"), 1, 180, "");
                c_menu.add_slider("Delay", &c_cfg.get<int>("legitbot_triggerbot_delay"), 1, 1000, "ms");

            }
            c_menu.end_child();

            c_menu.begin_child("Selection", "V", { RIGHT, HALF });
            {
                c_menu.add_multicombo("Target hitbox", &c_cfg.get<std::vector<bool>>("legitbot_selection_target_hitbox"), { "Head", "Neck", "Chest", "Stomach", "Pelvis", "Legs", "Feet" });
                c_menu.add_multicombo("Target multi-point", &c_cfg.get<std::vector<bool>>("legitbot_selection_target_multi_point"), { "Head", "Neck", "Chest", "Stomach", "Pelvis", "Legs", "Feet" });
                c_menu.add_slider("Hitchance", &c_cfg.get<int>("legitbot_selection_hitchance"), 1, 100, "%");
                c_menu.add_slider("Minimum damage", &c_cfg.get<int>("legitbot_selection_minimum_damage"), 1, 120, "%");
                c_menu.add_checkbox("Auto stop", &c_cfg.get<bool>("legitbot_selection_autostop"));
            }
            c_menu.end_child();

            c_menu.begin_child("Extra", "P", { HALFRIGHT, HALF });
            {
                c_menu.add_checkbox("Blockbot", &c_cfg.get<bool>("legitbot_extra_blockbot"));
            }
            c_menu.end_child();
            break;

        case 1: // Ragebot
            c_menu.begin_child("Global", "G", { LEFT, FULL });
            {
                c_menu.add_keybind("Enabled", &c_cfg.get_keybind<std::vector<int>>("ragebot_global_enabled"), ImGuiKey_E);
                c_menu.add_slider("Field of view ", &c_cfg.get<int>("ragebot_fov"), 1, 180, "°");
                c_menu.add_checkbox("Silent aim ", &c_cfg.get<bool>("ragebot_silent_aim"));
                c_menu.add_checkbox("Automatic fire ", &c_cfg.get<bool>("ragebot_automatic_fire"));
                c_menu.add_checkbox("Automatic scope ", &c_cfg.get<bool>("ragebot_automatic_scope"));
                c_menu.add_keybind("Prefer body aim", &c_cfg.get_keybind<std::vector<int>>("ragebot_"), ImGuiKey_E);
            }
            c_menu.end_child();

            c_menu.begin_child("Selection", "V", { RIGHT, HALF });
            {
                c_menu.add_multicombo("Target hitbox ", &c_cfg.get<std::vector<bool>>("ragebot_selection_target_hitbox"), { "Head", "Neck", "Chest", "Stomach", "Pelvis", "Legs", "Feet" });
                c_menu.add_multicombo("Target multi-point ", &c_cfg.get<std::vector<bool>>("ragebot_selection_target_multi_point"), { "Head", "Neck", "Chest", "Stomach", "Pelvis", "Legs", "Feet" });
                c_menu.add_slider("Hitchance ", &c_cfg.get<int>("ragebot_selection_hitchance"), 1, 100, "%");
                c_menu.add_slider("Minimum damage ", &c_cfg.get<int>("ragebot_selection_minimum_damage"), 1, 120, "DMG");
                c_menu.add_combo("Auto stop ", &c_cfg.get<int>("ragebot_selection_autostop"), { "Force Accuracy", "In Air", "Stop Between Shots"});
            }
            c_menu.end_child();

            c_menu.begin_child("Exploits", "W", { HALFRIGHT, HALF });
            {
                c_menu.add_checkbox("Rapid fire", &c_cfg.get<bool>("ragebot_extra_rapid_fire"));
                c_menu.add_checkbox("Remove spread", &c_cfg.get<bool>("ragebot_extra_remove_spread"));
                c_menu.add_checkbox("Magic bullet", &c_cfg.get<bool>("ragebot_extra_magic_bullet"));
            }
            c_menu.end_child();
            break;
        }

        c_menu.selection_end();
        break;
    }

          // Anti-Aim
    case 1: {
        c_menu.selection_begin(c_menu.MAIN_SELECTION);

        static int SUBTAB_SELECTION = 0;
        c_menu.add_subtab({ "Anti-Aim" }, &SUBTAB_SELECTION);

        switch (SUBTAB_SELECTION)
        {
        case 0: // Anti-Aim -> AA
            c_menu.begin_child("Global", "G", { LEFT, FULL });
            {
                c_menu.add_checkbox("Enabled", &c_cfg.get<bool>("anti_aim_global_enabled"));

                c_menu.add_combo("Pitch", &c_cfg.get<int>("anti_aim_global_pitch"), { "None", "Down", "Up" });
                c_menu.add_combo("Yaw", &c_cfg.get<int>("anti_aim_global_yaw"), { "Disabled", "Backward" });   
                if (c_cfg.get<int>("anti_aim_global_yaw")) 
                {
                    c_menu.add_combo("Yaw modifier", &c_cfg.get<int>("anti_aim_global_yaw_modifier"), { "Disabled", "Center", "Offset", "Random" });
                }
                
                c_menu.add_checkbox("Freestanding", &c_cfg.get<bool>("anti_aim_global_freestand"));
                c_menu.add_keybind("Slow walk", &c_cfg.get_keybind<std::vector<int>>("anti_aim_misc_slow_walk"), ImGuiKey_E);
            }
            c_menu.end_child();

            c_menu.begin_child("Other", "O", { RIGHT, FULL });
            {
                c_menu.add_colorpicker("Direction Indicator", &c_cfg.get_color<ImU32>("anti_aim_other_indicator_colorpicker"));
                c_menu.add_slider("Indicator size", &c_cfg.get<int>("anti_aim_other_indicator_size"), 1, 120, " ");
                c_menu.add_keybind("Anti-Aim back", &c_cfg.get_keybind<std::vector<int>>("anti_aim_other_back"), ImGuiKey_E);
                c_menu.add_keybind("Anti-Aim left", &c_cfg.get_keybind<std::vector<int>>("anti_aim_other_left"), ImGuiKey_E);
                c_menu.add_keybind("Anti-Aim right", &c_cfg.get_keybind<std::vector<int>>("anti_aim_other_right"), ImGuiKey_E);
                c_menu.add_keybind("Anti-Aim forward", &c_cfg.get_keybind<std::vector<int>>("anti_aim_other_forward"), ImGuiKey_E);
            }
            c_menu.end_child();
            break;

        }

        c_menu.selection_end();
        break;
    }
          // Visuals
    case 2: {
        c_menu.selection_begin(c_menu.MAIN_SELECTION);

        static int SUBTAB_SELECTION = 0;
        c_menu.add_subtab({ "Players", "Self", "World" }, &SUBTAB_SELECTION);

        switch (SUBTAB_SELECTION)
        {
        case 0: // Visuals -> Players
            c_menu.begin_child("Global", "G", { LEFT, FULL });
            {
                c_menu.add_checkbox("Box", &c_cfg.get<bool>("visuals_players_box"));
                c_menu.add_checkbox("Glow", &c_cfg.get<bool>("visuals_players_glow"));
                c_menu.add_checkbox("Name", &c_cfg.get<bool>("visuals_players_name"));
                c_menu.add_checkbox("Health bar", &c_cfg.get<bool>("visuals_players_healthbar"));              
                c_menu.add_checkbox("Ammo bar", &c_cfg.get<bool>("visuals_players_ammo_bar"));
                c_menu.add_checkbox("Skeleton", &c_cfg.get<bool>("visuals_players_skeleton"));

            }
            c_menu.end_child();

            c_menu.begin_child("Chams", "Z", { RIGHT, FULL });
            {
                c_menu.add_checkbox("Enabled", &c_cfg.get<bool>("visuals_players_chams_enabled"));
                c_menu.add_multicombo("Configure", &c_cfg.get<std::vector<bool>>("visuals_players_chams_configure"), { "Visible", "Hidden" });
                c_menu.add_checkbox("Override material", &c_cfg.get<bool>("visuals_players_chams_override_material"));
                if (c_cfg.get<bool>("visuals_chams_override_material"))
                {
                    c_menu.add_multicombo("Material", &c_cfg.get<std::vector<bool>>("visuals_players_chams_material"), { "Default", "Flat", "Glow" });
                }
            }
            c_menu.end_child();
            break;

        case 1: // Visuals -> Self
            c_menu.begin_child("Global", "G", { LEFT, FULL });
            {
                c_menu.add_checkbox("Box ", &c_cfg.get<bool>("visuals_self_box"));
                c_menu.add_checkbox("Glow ", &c_cfg.get<bool>("visuals_self_glow"));
                c_menu.add_checkbox("Name ", &c_cfg.get<bool>("visuals_self_name"));
                c_menu.add_checkbox("Health bar ", &c_cfg.get<bool>("visuals_self_healthbar"));            
                c_menu.add_checkbox("Ammo bar ", &c_cfg.get<bool>("visuals_self_ammo_bar"));
                c_menu.add_checkbox("Skeleton ", &c_cfg.get<bool>("visuals_self_skeleton"));

            }
            c_menu.end_child();

            c_menu.begin_child("Chams", "Z", { RIGHT, FULL });
            {
                c_menu.add_checkbox("Enabled ", &c_cfg.get<bool>("visuals_self_chams_enabled"));
                c_menu.add_multicombo("Configure ", &c_cfg.get<std::vector<bool>>("visuals_self_chams_configure"), { "Visible", "Hidden" });
                c_menu.add_checkbox("Override material ", &c_cfg.get<bool>("visuals_self_chams_override_material"));
                if (c_cfg.get<bool>("visuals_chams_override_material "))
                {
                    c_menu.add_multicombo("Material ", &c_cfg.get<std::vector<bool>>("visuals_self_chams_material"), { "Default", "Flat", "Glow" });
                }
            }
            c_menu.end_child();
            break;

        case 2: // Visuals -> World
            c_menu.begin_child("Global", "G", { LEFT, HALF });
            {
                c_menu.add_slider("Field of view", &c_cfg.get<int>("visuals_world_fov"), 1, 120, " ");
                c_menu.add_slider("Override zoom", &c_cfg.get<int>("visuals_world_override_zoom"), 1, 100, " ");
                c_menu.add_keybind("Thirdperson", &c_cfg.get_keybind<std::vector<int>>("visuals_world_thirdperson"), ImGuiKey_E);
                if (c_cfg.get<bool>("visuals_world_thirdperson"))
                {
                    c_menu.add_slider("Thirdperson Distance", &c_cfg.get<int>("visuals_world_thirdperson_distance"), 1, 100, " ");
                }
            }
            c_menu.end_child();

            c_menu.begin_child("World", "N", { HALFLEFT, HALF });
            {
                c_menu.add_slider("Nightmode", &c_cfg.get<int>("visuals_world_nightmode"), 1, 100, " ");
                c_menu.add_slider("Asus walls", &c_cfg.get<int>("visuals_world_asus_walls"), 1, 100, " ");
                c_menu.add_slider("Asus props", &c_cfg.get<int>("visuals_world_asus_props"), 1, 100, " ");
            }
            c_menu.end_child();

            c_menu.begin_child("World ESP", "P", { RIGHT, HALF });
            {

                c_menu.add_checkbox("Bomb", &c_cfg.get<bool>("visuals_worldesp_bomb"));
                c_menu.add_checkbox("Grenades", &c_cfg.get<bool>("visuals_worldesp_grenades"));
                c_menu.add_checkbox("Weapons", &c_cfg.get<bool>("visuals_worldesp_weapons"));
                c_menu.add_colorpicker("Grenade warning", &c_cfg.get_color<ImU32>("visuals_worldesp_grenade_warning"));
                c_menu.add_colorpicker("Grenade prediction", &c_cfg.get_color<ImU32>("visuals_worldesp_grenade_prediction"));
            }
            c_menu.end_child();

            c_menu.begin_child("Other", "X", { HALFRIGHT, HALF });
            {
                c_menu.add_multicombo("Removals", &c_cfg.get<std::vector<bool>>("visuals_other_removals"), { "Flashbang", "Smoke", "Scope" });
                c_menu.add_multicombo("Crosshair", &c_cfg.get<std::vector<bool>>("visuals_other_crosshair"), { "Penetration", "Sniper" });
                c_menu.add_multicombo("Visual recoil", &c_cfg.get<std::vector<bool>>("visuals_other_visual_recoil"), { "Remove shake", "Remove all" });
                c_menu.add_checkbox("Hit marker", &c_cfg.get<bool>("visuals_other_hitmarker"));
            }
            c_menu.end_child();
            break;
        }
        c_menu.selection_end();
        break;
    }
          // Misc
    case 3: {
        c_menu.selection_begin(c_menu.MAIN_SELECTION);

        static int SUBTAB_SELECTION = 0;
        c_menu.add_subtab({ "General", "Skins" }, &SUBTAB_SELECTION);

        static bool checkbox_value = false;
        static int selectedcombo = 0;

        switch (SUBTAB_SELECTION)
        {
        case 0: // Misc -> General
            c_menu.begin_child("Configuration", "S", { LEFT, HALF });
            {

                c_menu.add_combo("Slot", &c_cfg.get<int>("misc_configuration_slot"), { "0", "1", "2", "3", "4", "5" });
               
                if (c_menu.add_button("Save Slot " + std::to_string(c_cfg.get<int>("misc_configuration_slot"))))
                {
                    c_cfg.save(("slot_" + std::to_string(c_cfg.get<int>("misc_configuration_slot"))));
                }

                if (c_menu.add_button("Load Slot " + std::to_string(c_cfg.get<int>("misc_configuration_slot"))))
                {
                    c_cfg.load(("slot_" + std::to_string(c_cfg.get<int>("misc_configuration_slot"))));
                }

                if (c_menu.add_button("Delete Slot " + std::to_string(c_cfg.get<int>("misc_configuration_slot")) + " ?"))
                {
                    c_cfg.load(("slot_" + std::to_string(c_cfg.get<int>("misc_configuration_slot"))));
                }
               

            }
            c_menu.end_child();

            c_menu.begin_child("General", "G", { HALFLEFT, HALF });
            {
                c_menu.add_checkbox("Anti-untrusted", &c_cfg.get<bool>("misc_anti_untrusted"));
                c_menu.add_checkbox("Auto Accept", &c_cfg.get<bool>("misc_autoaccept"));
                c_menu.add_checkbox("Unlock Inventory", &c_cfg.get<bool>("misc_unlock_inventory"));
                c_menu.add_multicombo("Window List", &c_cfg.get<std::vector<bool>>("misc_general_window_list"), { "Watermark","Keybind list","Spectator list" });
                c_menu.add_checkbox("Auto Buy", &c_cfg.get<bool>("misc_auto_buy"));
                if (c_cfg.get<bool>("misc_auto_buy"))
                {
                    c_menu.add_multicombo("Primary", &c_cfg.get<std::vector<bool>>("misc_auto_buy_primary"), { "Ak-47","M4A4", "AWP", "GALIL/FAMAS", "G3SG1/SCAR20", "Scout"});
                    c_menu.add_multicombo("Secondary", &c_cfg.get<std::vector<bool>>("misc_auto_buy_secondary"), { "CZ-75/TEC9/Five Seven", "Dual Berettas", "Desert Eagle/Revolver", "Glock/USP" });
                    c_menu.add_multicombo("Utilities", &c_cfg.get<std::vector<bool>>("misc_auto_buy_utilities"), { "Helmet", "Kevlar", "Smoke Grenade", "He Grenade", "Molotov", "Defuser", "Taser" });
                }

            }
            c_menu.end_child();

            c_menu.begin_child("Menu", "T", { RIGHT, HALF });
            {               
                c_menu.add_colorpicker("Accent Color", &c_cfg.get_color<ImU32>("misc_menu_accent_color"));
                c_menu.set_accentcolor(c_cfg.get_color<ImU32>("misc_menu_accent_color"));
                c_menu.add_keybind("Menu Key", &c_cfg.get_keybind<std::vector<int>>("misc_menu_menu_key"), ImGuiKey_E);
                c_menu.add_checkbox("Menu Glow", &c_cfg.get<bool>("misc_menu_glow"));
                if (c_cfg.get<bool>("misc_menu_glow")) 
                {
                    c_menu.add_colorpicker("Menu Glow Color", &c_cfg.get_color<ImU32>("misc_menu_glow_color"));
                }
            }
            c_menu.end_child();

            c_menu.begin_child("Movement", "I", { HALFRIGHT, HALF });
            {
                c_menu.add_checkbox("Bunny Hop", &c_cfg.get<bool>("misc_bunny_hop"));
                c_menu.add_checkbox("Auto Strafe", &c_cfg.get<bool>("misc_auto_strafe"));
                c_menu.add_combo("Strafe Mode", &c_cfg.get<int>("misc_movement_strafe_mode"), { "Legit","Rage","Directional" });
                c_menu.add_keybind("Quick-Peek Assist", &c_cfg.get_keybind<std::vector<int>>("misc_quick_peek_assist"), ImGuiKey_E);
                c_menu.add_keybind("Edge Jump", &c_cfg.get_keybind<std::vector<int>>("misc_movement_edgejump"), ImGuiKey_E);
                c_menu.add_checkbox("Scarlett-Bot", &c_cfg.get<bool>("misc_scarlett_ai_bot"));
            }
            c_menu.end_child();
            break;
        case 1:
            c_menu.begin_child("General", "O", { FULL, FULLX2 });
            {

            }
            c_menu.end_child();
            break;
        }

        c_menu.selection_end();
        break;
    }
          // Scripting
    case 4: {
        c_menu.selection_begin(c_menu.MAIN_SELECTION);

        static int SUBTAB_SELECTION = 0;
        c_menu.add_subtab({ "General", "Options" }, &SUBTAB_SELECTION);

        switch (SUBTAB_SELECTION)
        {
        case 0: //

            break;

        case 1: //

            break;
        }

        c_menu.selection_end();
        break;
    }


    }

    c_menu.draw_frame();

    c_menu.end();
}
