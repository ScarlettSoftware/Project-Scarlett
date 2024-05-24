#include "cmenu.hpp"
#include "functions.hpp"

// oyster perpetual

static std::unordered_map<std::string, ImFont*> menufonts;

using namespace ImGui;

ImDrawList* draw_list;

ImVec2 p, s;

long mapval(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int mapval(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapval(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int binaryStringToInt(const std::string& binaryString) {
    int result = 0;
    for (char digit : binaryString) {
        result = (result << 1) + (digit - '0');
    }
    return result;
}

menu::menu()
{
    MENU_OPEN = true;
    ISBOOTING = false;
    MAIN_ROUNDING = 15.f; // main round for the whole window ( sleepy this is only for the edges of the window and the rects it dosent do nothing else)
    MAIN_SELECTION = 0; // main selection index
    MAIN_SELECTIONALPHA = { 125,125,125,125,125,125 }; // alpha value for main selection

    MAIN_BUTTONSLABELS = { "Aimbot","Anti-Aim","Visuals","Misc","Scripts" };
    MAIN_BUTTONSICONS = { "B", "C", "D", "E", "F" };

    MAIN_BUTTONSYPOS = { 0,0,0,0,0,0,0 }; // position of the main button when selected or not !DONT TOUCH!

    MAIN_SELECTIONYPOS = { 120,120,120,120,120,120 }; // y position of every tab ( needed for animation ) !DONT TOUCH!
    MAIN_SELECTIONTABALPHA = 0; // alpha of every tab 0.0f <-> 1.0f

    timer.Start();
}

void menu::set_font(string label, ImFont* font)
{
    menufonts[label] = font;
}

void U32ToImColor(uint32_t u32_color, float out_color[4]) {
    // Extract the individual color components
    uint8_t a = (u32_color >> 24) & 0xFF;
    uint8_t r = (u32_color >> 16) & 0xFF;
    uint8_t g = (u32_color >> 8) & 0xFF;
    uint8_t b = u32_color & 0xFF;

    // Convert the components to the float range [0.0, 1.0]
    out_color[0] = r / 255.0f;
    out_color[1] = g / 255.0f;
    out_color[2] = b / 255.0f;
    out_color[3] = a / 255.0f;
}

void menu::set_accentcolor(ImColor clr)
{
    float col[4];
    U32ToImColor(clr, col);
    ACCENT_R = col[0] * 255;
    ACCENT_G = col[1] * 255;
    ACCENT_B = col[2] * 255;
}

void menu::set_accentcolor(float color[4])
{
    ACCENT_R = color[0];
    ACCENT_G = color[1];
    ACCENT_B = color[2];
}

void menu::begin(std::string label)
{
    ImGui::SetNextWindowSize(ImVec2(620, 580)); //Window Size
    Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

    p = ImGui::GetWindowPos();
    s = ImGui::GetWindowSize();
    draw_list = ImGui::GetWindowDrawList();
    childId = 1;
    mainChildId = 1;

    ImGui::GetBackgroundDrawList()->AddShadowRect({ p.x + 1, p.y + 1 }, { p.x + s.x - 1, p.y + s.y - 1 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 55), 50, { 0,0 }, 0, MAIN_ROUNDING);
    draw_list->AddRectFilled({ p.x + 1, p.y + 1 }, { p.x + s.x - 1, p.y + s.y - 1 }, c_color->get(20, 20, 20, 255), MAIN_ROUNDING);
}

void menu::end()
{
    childId = 1;
    mainChildId = 1;
    End();
}

void menu::draw_frame()
{
    ImGui::SetCursorPos({ 0, 0 });
    // Top
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
    ImGui::BeginChild(childId, { ImGui::GetWindowSize().x, 70 });
    childId++;

    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + s.x, p.y + 60), c_color->get(26, 26, 26, 255), 15, ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);

    ImGui::PushFont(menufonts["logo"]);
    ImGui::GetWindowDrawList()->AddText({ p.x + (ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("J").x / 2), p.y + 5 + (60 / 2 - ImGui::CalcTextSize("J").y / 2) }, c_color->get(165, 165, 165, 255), "J");
    ImGui::PopFont();

    ImGui::GetWindowDrawList()->AddShadowRect({ p.x,p.y + 60 }, ImVec2(p.x + ImGui::GetWindowSize().x, p.y + 60), c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 255), 5, { 0,0 });

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetCursorPos({ 0, ImGui::GetWindowSize().y - 73 });

    // Bottom
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
    ImGui::BeginChild(childId, { ImGui::GetWindowSize().x, 73 }, 0, ImGuiWindowFlags_NoDecoration);
    childId++;

    ImGui::GetWindowDrawList()->AddShadowRect({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y - 1 }, { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x,ImGui::GetWindowPos().y - 1 }, c_color->get(25, 25, 25, 75), 500, { 0,0 });
    ImGui::GetWindowDrawList()->AddRectFilled({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 3 }, { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x,ImGui::GetWindowPos().y + 70 }, c_color->get(26, 26, 26, 255), MAIN_ROUNDING, ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight);

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));

    ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2 - (((40 * 5) + 20 * 4) / 2) - 5, ImGui::GetWindowSize().y - 60 });

    ImGui::GetWindowDrawList()->ChannelsSplit(2);

    static int MAIN_ORDER = 0;

    for (int i = 0; i < MAIN_BUTTONSICONS.size(); i++)
    {
        ImGui::GetWindowDrawList()->ChannelsSetCurrent(0);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
        if (ImGui::Button(MAIN_BUTTONSICONS[i].c_str(), { 40,40 }))
            MAIN_SELECTION = MAIN_ORDER;
        ImGui::PopStyleColor();

        ImGui::GetWindowDrawList()->ChannelsSetCurrent(1);

        //if (selected == order)
        {
            if (MAIN_SELECTION == MAIN_ORDER)
            {
                Animate(0, 5, &MAIN_BUTTONSYPOS[i], 0.2);
                Animate(125, 255, &MAIN_SELECTIONALPHA[MAIN_ORDER], 0.5);
            }
            else
            {
                Animate(5, 0, &MAIN_BUTTONSYPOS[i], 0.2);
            }

            ImGui::PushFont(menufonts["selectionicons"]);

            ImGui::GetWindowDrawList()->AddText({ ImGui::GetItemRectMin().x + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).x / 2),ImGui::GetItemRectMin().y + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).y / 2) - MAIN_BUTTONSYPOS[i] }, c_color->get(168, 168, 169), MAIN_BUTTONSICONS[i].c_str());

            ImGui::GetWindowDrawList()->AddRectFilled({ ImGui::GetItemRectMin().x + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).x / 2) - 10,ImGui::GetItemRectMin().y + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).y / 2) - MAIN_BUTTONSYPOS[i] - 10 }, { ImGui::GetItemRectMin().x + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).x / 2) + 30,ImGui::GetItemRectMin().y + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).y / 2) - MAIN_BUTTONSYPOS[i] + 30 }, c_color->get(26, 26, 26, MAIN_SELECTIONALPHA[MAIN_ORDER]));
            ImGui::GetWindowDrawList()->AddText({ ImGui::GetItemRectMin().x + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).x / 2),ImGui::GetItemRectMin().y + (20 - ImGui::CalcTextSize(MAIN_BUTTONSICONS[i].c_str()).y / 2) - MAIN_BUTTONSYPOS[i] }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, MAIN_SELECTIONALPHA[MAIN_ORDER]), MAIN_BUTTONSICONS[i].c_str());

            ImGui::PopFont();

            if (MAIN_SELECTION != MAIN_ORDER)
            {
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetItemRectMin().x + 15,ImGui::GetItemRectMax().y + 3 }, { ImGui::GetItemRectMax().x - 15,ImGui::GetItemRectMax().y + 3 }, c_color->get(175, 175, 175, 255), 2);
                ImGui::GetWindowDrawList()->AddShadowRect({ ImGui::GetItemRectMin().x + 15,ImGui::GetItemRectMax().y + 3 }, { ImGui::GetItemRectMax().x - 15,ImGui::GetItemRectMax().y + 3 }, c_color->get(175, 175, 175, 255), 25, { 0,0 });
            }

            if (MAIN_SELECTIONALPHA[MAIN_ORDER] > 0)
                ImGui::GetWindowDrawList()->AddRectFilled({ ImGui::GetItemRectMin().x + 5,ImGui::GetItemRectMax().y }, { ImGui::GetItemRectMax().x - 5,ImGui::GetItemRectMax().y + 8 }, c_color->get(26, 26, 26, 255));
            ImGui::GetWindowDrawList()->AddLine({ ImGui::GetItemRectMin().x + 10,ImGui::GetItemRectMax().y + 3 }, { ImGui::GetItemRectMax().x - 10,ImGui::GetItemRectMax().y + 3 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, MAIN_SELECTIONALPHA[MAIN_ORDER]), 2);
            ImGui::GetWindowDrawList()->AddShadowRect({ ImGui::GetItemRectMin().x + 10,ImGui::GetItemRectMax().y + 3 }, { ImGui::GetItemRectMax().x - 10,ImGui::GetItemRectMax().y + 3 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, MAIN_SELECTIONALPHA[MAIN_ORDER]), 25, { 0,0 });
        }

        for (int i = 0; i < MAIN_SELECTIONALPHA.size(); i++)
        {
            if (i != MAIN_SELECTION)
                Animate(255, 0, &MAIN_SELECTIONALPHA[i], 0.5);
        }

        ImGui::SameLine();
        ImGui::Dummy({ 20,0 });
        ImGui::SameLine();

        MAIN_ORDER++;
    }
    MAIN_ORDER = 0;

    ImGui::PopStyleColor(3);

    ImGui::GetWindowDrawList()->ChannelsMerge();

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void menu::add_subtab(std::vector<std::string> labels, static int* selected)
{
    // ImGui::SetCursorPos({0,0});
     // begin
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(24, 24, 24, 255));
    ImGui::BeginChild(childId, { ImGui::GetWindowSize().x, 28 }, 0, ImGuiWindowFlags_NoDecoration);
    childId++;
    ImGui::SetCursorPos({ 1,1 });

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));

    static int order = 0;

    ImDrawList* draw_listsubtab = ImGui::GetWindowDrawList();

    draw_listsubtab->ChannelsSplit(2);

    static int alphas[10] = { 0,0,0,0,0,0,0,0,0 };

    //order = 0;

    for (int i = 0; i < labels.size(); i++)
    {
        draw_listsubtab->ChannelsSetCurrent(1);

        if (selected[0] != order)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(110, 110, 110, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(245, 245, 245, 255));

        ImGui::PushFont(menufonts["subtabselection"]);
        if (ImGui::Button(labels.at(i).c_str(), { (ImGui::GetWindowSize().x) / labels.size() , 26 }))
            selected[0] = order;
        ImGui::PopFont();

        ImGui::PopStyleColor();

        ImGui::SameLine();

        draw_listsubtab->ChannelsSetCurrent(0);

        if (selected[0] == order)
            Animate(0, 255, &alphas[order], 0.2);

        draw_listsubtab->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, alphas[order]), 4);
        draw_listsubtab->AddShadowRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, alphas[order]), 25, { 0,0 });


        for (int j = 0; j < 5; j++)
        {
            if (j != selected[0])
                Animate(255, 0, &alphas[order], 0.5);
        }

        order++;
    }

    order = 0;

    draw_listsubtab->ChannelsMerge();

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    // end
    ImGui::GetWindowDrawList()->AddRect({ ImGui::GetWindowPos().x,ImGui::GetWindowPos().y }, { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x,ImGui::GetWindowPos().y + 28 }, IM_COL32(44, 44, 48, 255), 4);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void menu::selection_begin(static int selected)
{
    for (int i = 0; i < MAIN_SELECTIONYPOS.size(); i++)
    {
        if (selected != i)
            MAIN_SELECTIONYPOS[i] = 100;
    };
    ImGui::SetCursorPos({ 14, 70 });

    Animate(0.f, 1.f, &MAIN_SELECTIONTABALPHA, 0.35);
    Animate(100, 70, &MAIN_SELECTIONYPOS[selected], 0.35);
    ImGui::SetCursorPosY(MAIN_SELECTIONYPOS[selected]);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(20, 20, 20, 255));
    ImGui::BeginChildFrame(childId, { ImGui::GetWindowSize().x - 28,ImGui::GetWindowSize().y - 140 }, ImGuiWindowFlags_NoDecoration);
    childId++;

}

void menu::boot() // start boot animation
{
    if (ISBOOTING) {
        static int alpha = 0;
        static int alpha1 = 0;
        static int alpha2 = 0;

        Animate(0, 165, &alpha, 0.5);

        ImGui::GetForegroundDrawList()->AddRectFilled({ 0,0 }, { ImGui::GetIO().DisplaySize }, c_color->get(35, 35, 35, alpha));

        if (alpha == 165)
        {
            Animate(0, 200, &alpha1, 0.5);
            Animate(0, 255, &alpha2, 0.5);
        }


        ImGui::GetForegroundDrawList()->AddShadowRect({ ImGui::GetIO().DisplaySize.x / 2 - 100,ImGui::GetIO().DisplaySize.y / 2 - 100 }, { ImGui::GetIO().DisplaySize.x / 2 + 100, ImGui::GetIO().DisplaySize.y / 2 + 100 }, c_color->get(20, 20, 20, alpha1 + 25), 50, { 1,1 }, 0, 5);
        ImGui::GetForegroundDrawList()->AddRectFilled({ ImGui::GetIO().DisplaySize.x / 2 - 100,ImGui::GetIO().DisplaySize.y / 2 - 100 }, { ImGui::GetIO().DisplaySize.x / 2 + 100, ImGui::GetIO().DisplaySize.y / 2 + 100 }, c_color->get(35, 35, 35, alpha1));
        ImGui::PushFont(menufonts["biglogo"]);
        ImGui::GetForegroundDrawList()->AddText({ ImGui::GetIO().DisplaySize.x / 2 - ImGui::CalcTextSize("J").x / 2, ImGui::GetIO().DisplaySize.y / 5 }, c_color->get(255, 255, 255, alpha1), "J");
        ImGui::PopFont();

        ImGui::SetNextWindowPos({ 100000,10000 });
        ImGui::Begin("fnse");
        ImGui::Loading("##loader", 50, 10, alpha2, c_color->get(23, 23, 23, alpha2), GetForegroundDrawList(), CircleAnimBase_Default);
        ImGui::End();

        //ImGui::GetForegroundDrawList()->AddRectFilled({ ImGui::GetIO().DisplaySize.x / 2 - 100,ImGui::GetIO().DisplaySize.y / 2 - 100 }, { ImGui::GetIO().DisplaySize.x / 2 + 100, ImGui::GetIO().DisplaySize.y / 2 + 100 }, c_color->get(35, 35, 35, alpha2));
    }
    else
    {
        static int alpha = 165;
        static int alpha1 = 200;
        static int alpha2 = 255;

        Animate(200, 0, &alpha1, 0.5);
        Animate(255, 0, &alpha2, 0.5);

        ImGui::GetForegroundDrawList()->AddRectFilled({ 0,0 }, { ImGui::GetIO().DisplaySize }, c_color->get(35, 35, 35, alpha));

        if (alpha1 == 0)
        {
            Animate(165, 0, &alpha, 0.5);
        }


        ImGui::GetForegroundDrawList()->AddShadowRect({ ImGui::GetIO().DisplaySize.x / 2 - 100,ImGui::GetIO().DisplaySize.y / 2 - 100 }, { ImGui::GetIO().DisplaySize.x / 2 + 100, ImGui::GetIO().DisplaySize.y / 2 + 100 }, c_color->get(20, 20, 20, alpha1 + 25), 50, { 1,1 }, 0, 5);
        ImGui::GetForegroundDrawList()->AddRectFilled({ ImGui::GetIO().DisplaySize.x / 2 - 100,ImGui::GetIO().DisplaySize.y / 2 - 100 }, { ImGui::GetIO().DisplaySize.x / 2 + 100, ImGui::GetIO().DisplaySize.y / 2 + 100 }, c_color->get(35, 35, 35, alpha1));
        ImGui::PushFont(menufonts["biglogo"]);
        ImGui::GetForegroundDrawList()->AddText({ ImGui::GetIO().DisplaySize.x / 2 - ImGui::CalcTextSize("J").x / 2, ImGui::GetIO().DisplaySize.y / 5 }, c_color->get(255, 255, 255, alpha1), "J");
        ImGui::PopFont();

        ImGui::SetNextWindowPos({ 100000,10000 });
        ImGui::Begin("fnse");
        ImGui::Loading("##loader", 50, 10, alpha2, c_color->get(23, 23, 23, alpha2), GetForegroundDrawList(), CircleAnimBase_Default);
        ImGui::End();
    }


}

void menu::selection_end()
{
    ImGui::EndChildFrame();
    ImGui::PopStyleColor();
}

void menu::begin_child(std::string Label, std::string Icon, ImVec2 cfg)
{
    ImGui::SetCursorPos({ 0,41 });

    switch ((int)cfg.x)
    {
    case LEFT:
        ImGui::SetCursorPosX(0);
        break;

    case RIGHT:
        ImGui::SetCursorPosX(301);
        break;

    case HALFLEFT:
        ImGui::SetCursorPosY(41 + (390 / 2 + 5));
        break;

    case HALFRIGHT:
        ImGui::SetCursorPosX(301);
        ImGui::SetCursorPosY(41 + (390 / 2 + 5));
        break;
    }

    ImVec2 childsize = { 291,390 };
    switch ((int)cfg.y)
    {
    case FULL:
        childsize.y = 390;
        break;

    case HALF:
        childsize.y = 390 / 2 - 5;
        break;

    case FULLX2:
        childsize.x = 291 + 10;
        childsize.y = 390;
        break;
    }

    // ImGui::SetCursorPos({ 0,41 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
    ImGui::BeginChild(binaryStringToInt(Label), childsize, 0, ImGuiWindowFlags_NoDecoration);

    ImVec2 pos = { ImGui::GetWindowPos() };//{p.x + ImGui::GetCursorPosX() ,p.y + ImGui::GetCursorPosY()};
    ImVec2 size = { pos.x + ImGui::GetWindowSize().x, pos.y + ImGui::GetWindowSize().y };

    ImDrawList* cdl = ImGui::GetWindowDrawList();
    current_child_dl = ImGui::GetWindowDrawList();
    cdl->AddRectFilled(pos, size, c_color->get(23, 23, 23), 10);
    cdl->AddRect(pos, size, c_color->get(33, 33, 33), 10, 0, 2);
    cdl->AddLine({ pos.x, pos.y + 30 }, { size.x, pos.y + 30 }, c_color->get(33, 33, 33), 1.5);

    current_child_dl->ChannelsSplit(2);

    current_child_dl->ChannelsSetCurrent(1);

    cdl->AddRectFilled(pos, { size.x, pos.y + 30 }, c_color->get(33, 33, 33), 10, ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);

    ImGui::PushFont(menufonts["childtitle"]);

    ImGui::PushFont(menufonts["childicon"]);
    cdl->AddText({ pos.x + 10,pos.y + (31 / 2 - ImGui::CalcTextSize(Icon.c_str()).y / 2) }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B), Icon.c_str());
    ImGui::PopFont();

    cdl->AddText({ pos.x + 35, pos.y + (31 / 2 - ImGui::CalcTextSize(Label.c_str()).y / 2) }, c_color->get(193, 193, 193), Label.c_str());
    ImGui::PopFont();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 35);
    //ImGui::Text("Position: (%.1f, %.1f)", pos.x, pos.y);
    //ImGui::Text("Size: (%.1f, %.1f)", size.x, size.y);

    current_child_dl->ChannelsSetCurrent(0);
}

float* menu::convertImColortoFloat(ImColor cl)
{
    float col[4] = { cl.Value.x, cl.Value.y, cl.Value.z, cl.Value.w };
    return col;
}

void menu::end_child()
{
    current_child_dl->ChannelsMerge();

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::SetCursorPos({ 0,41 });
}

void spacingelement()
{
    ImGui::Dummy({ 0,7 }); // spacing
}

std::unordered_map<string, int> checkbox_animationX;

void menu::add_checkbox(std::string Label, static bool* value)
{
    ImGui::Dummy({ 291, 25 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // checkbox
    ImGui::SameLine();
    ImGui::SetCursorPosX(291 - 35);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    if (ImGui::Button(Label.c_str(), { 28,18 }))
        value[0] = !value[0];

    ImGui::PopStyleColor(4);

    ImVec2 ch_min = ImGui::GetItemRectMin();
    ImVec2 ch_max = ImGui::GetItemRectMax();

    current_child_dl->AddShadowRect(ch_min, ch_max, c_color->get(255, 255, 255, 25), 20, { 0,0 }, 0, 0);

    current_child_dl->AddRectFilled(ch_min, ch_max, c_color->get(40, 40, 40), 25);

    if (checkbox_animationX[Label] > 0)
    {
        current_child_dl->AddShadowRect({ ch_min.x + 2,ch_min.y + 2 }, { ch_min.x + checkbox_animationX[Label] + 15 , ch_max.y - 2 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 10), 50, { 0,0 }, 0, 25);
        current_child_dl->AddRectFilled({ ch_min.x + 2,ch_min.y + 2 }, { ch_min.x + checkbox_animationX[Label] + 15 , ch_max.y - 2 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 255), 25);
    }
    //current_child_dl->AddShadowRect({ ch_min.x + 2,ch_min.y + 2 }, { ch_min.x + checkbox_animationX[Label] + 15 , ch_max.y - 2 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 50), 100, { 0,0 }, 0, 25);
    current_child_dl->AddRect(ch_min, ch_max, c_color->get(55, 55, 55), 25);

    static ImVec2 cursor_pos = { ch_min.x + 8 ,ch_min.y };

    cursor_pos = { ch_min.x + 8 + checkbox_animationX[Label] ,ch_min.y + 9 };

    if (value[0])
    {
        Animate(checkbox_animationX[Label], 11, &checkbox_animationX[Label], 0.15);
    }
    else
    {
        Animate(checkbox_animationX[Label], 0, &checkbox_animationX[Label], 0.15);
    }

    //cursor_pos = { ch_min.x + cx,ch_min.y + 9 };
    if (!value[0])
        current_child_dl->AddShadowCircle(cursor_pos, 6, c_color->get(255, 255, 255, 5), 50, { 0,0 });
    current_child_dl->AddCircleFilled(cursor_pos, 6, c_color->get(255, 255, 255));

    spacingelement();
}


std::unordered_map<string, float> slider_cursorx;
std::unordered_map<string, bool> slider_cursorHold;
std::unordered_map<string, int> slider_cursorSize;

std::unordered_map<string, bool> slider_startup;
std::unordered_map<string, float> slider_mappedvalue;
std::unordered_map<string, char[4]> slider_textvalue;

std::unordered_map<string, int> slider_savedpos;

std::string trim_to_one_decimal(float value) {
    std::string str = std::to_string(value);
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    size_t dot_pos = str.find('.');
    if (dot_pos != std::string::npos && dot_pos + 2 < str.size()) {
        str = str.substr(0, dot_pos + 2);
    }
    return str;
}

float truncateThreeDecimals(float value) {
    return static_cast<int>(value * 100) / 100.0;
}

void menu::add_slider(std::string Label, int* value, int min, int max, std::string format)
{
    if (slider_startup[Label] == false)
    {
        slider_cursorx[Label] = slider_mappedvalue[Label];
        slider_mappedvalue[Label] = mapval(value[0], min, max, 0, 225);
        Animate(0.f, slider_mappedvalue[Label], &slider_cursorx[Label], 0.3);
        //slider_startup[Label] = true;
        if (slider_cursorx[Label] == slider_mappedvalue[Label])
            slider_startup[Label] = true;
    }

    ImGui::Dummy({ 291, 35 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(dummy_min, dummy_max, c_color->get(255, 255, 255, 15));

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 - 3) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // slider ModifyValue
    ImGui::SetCursorPosX(271 - 35);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
    ImGui::Dummy({ 50,20 });
    ImVec2 sliderch_min = ImGui::GetItemRectMin();
    ImVec2 sliderch_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get255, 0, 0));
    current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(sliderch_min, sliderch_max, c_color->get(55, 55, 55), 5);

    std::string textvalue = to_string(value[0]) + format;
    ImGui::PushFont(menufonts["slidervaluelabel"]);
    current_child_dl->AddText({ sliderch_min.x + 50 / 2 - ImGui::CalcTextSize(textvalue.c_str()).x / 2, sliderch_min.y + 20 / 2 - ImGui::CalcTextSize(to_string(value[0]).c_str()).y / 2 }, c_color->get(255, 255, 255), textvalue.c_str());
    ImGui::PopFont();

    // slider item
    ImGui::SetCursorPosX(5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
    ImGui::Dummy({ 225, 10 });
    ImVec2 slider_min = ImGui::GetItemRectMin();
    ImVec2 slider_max = ImGui::GetItemRectMax();

    current_child_dl->AddRectFilled(slider_min, slider_max, c_color->get(40, 40, 40), 5);

    if (*value > min)
        current_child_dl->AddRectFilled({ slider_min.x + 1, slider_min.y }, { slider_min.x + slider_cursorx[Label] + 8,slider_min.y + 10 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B), 5);
    current_child_dl->AddRect(slider_min, slider_max, c_color->get(55, 55, 55), 5);

    current_child_dl->AddCircleFilled({ slider_min.x + slider_cursorx[Label] + 5,slider_min.y + 5 }, slider_cursorSize[Label], c_color->get(255, 255, 255, 255));

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = true;
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = false;

    ImGui::SetCursorPosX(slider_cursorx[Label] + 5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));

    ImGui::Button(Label.c_str(), { 10,10 });

    ImGui::PopStyleColor(4);

    if (ImGui::IsItemHovered() && ImGui::IsItemFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = true;
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = false;

    int mappedshit = (ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8);
    mappedshit = ImClamp({ (float)mappedshit, 0 }, { 0,0 }, { 225, 0 }).x;

    if (slider_cursorHold[Label])
    {
        for (auto& pair : slider_cursorHold) {
            if (pair.first != Label)
                pair.second = false;
        }

        Animate(4, 6, &slider_cursorSize[Label], 0.15);

        if (!(mappedshit < slider_cursorx[Label] + 10 && mappedshit > slider_cursorx[Label] - 10))
            Animate(slider_cursorx[Label], (ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8), &slider_cursorx[Label], 0.15);
        else
            slider_cursorx[Label] = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8;

        // slider_cursorx[Label] = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8;
        slider_cursorx[Label] = ImClamp({ (float)slider_cursorx[Label], 0 }, { 0,0 }, { 215, 0 }).x;

        value[0] = mapval((int)slider_cursorx[Label], 0, 215, min, max);
        value[0] = ImClamp({ (float)*value, 0 }, { (float)min,0 }, { (float)max, 0 }).x;
    }
    else
    {
        Animate(6, 4, &slider_cursorSize[Label], 0.15);
    }

    // Animate(6, 4, &slider_cursorSize[Label]);

    spacingelement();
}

void menu::add_slider(std::string Label, float* value, float min, float max, std::string format)
{
    if (slider_startup[Label] == false)
    {
        slider_cursorx[Label] = slider_mappedvalue[Label];
        slider_mappedvalue[Label] = mapval(value[0], min, max, 0.f, 215.f);
        Animate(0.f, slider_mappedvalue[Label], &slider_cursorx[Label], 0.3);
        //slider_startup[Label] = true;
        if (slider_cursorx[Label] == slider_mappedvalue[Label])
            slider_startup[Label] = true;
    }

    ImGui::Dummy({ 291, 35 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 - 3) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // slider ModifyValue
    ImGui::SetCursorPosX(281 - 35);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
    ImGui::Dummy({ 40,20 });
    ImVec2 sliderch_min = ImGui::GetItemRectMin();
    ImVec2 sliderch_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get(255, 0, 0));
    current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(sliderch_min, sliderch_max, c_color->get(55, 55, 55), 5);
    std::string textvalue = trim_to_one_decimal(value[0]) + format;
    ImGui::PushFont(menufonts["slidervaluelabel"]);
    current_child_dl->AddText({ sliderch_min.x + 40 / 2 - ImGui::CalcTextSize(textvalue.c_str()).x / 2, sliderch_min.y + 20 / 2 - ImGui::CalcTextSize(to_string(value[0]).c_str()).y / 2 }, c_color->get(255, 255, 255), textvalue.c_str());
    ImGui::PopFont();
    //ImGui::Text(to_string((int)value[0]).c_str());
    //current_child_dl->AddText(sliderch_min, c_color->get(255, 0, 0), std::to_string((int)value[0]).c_str());

    // slider item
    ImGui::SetCursorPosX(5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 14);
    ImGui::Dummy({ 225, 10 });
    ImVec2 slider_min = ImGui::GetItemRectMin();
    ImVec2 slider_max = ImGui::GetItemRectMax();

    current_child_dl->AddRectFilled(slider_min, slider_max, c_color->get(40, 40, 40), 5);
    if (*value > min)
        current_child_dl->AddRectFilled({ slider_min.x + 1, slider_min.y }, { slider_min.x + slider_cursorx[Label] + 8,slider_min.y + 10 }, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B), 5);
    current_child_dl->AddRect(slider_min, slider_max, c_color->get(55, 55, 55), 5);

    current_child_dl->AddCircleFilled({ slider_min.x + slider_cursorx[Label] + 5,slider_min.y + 5 }, slider_cursorSize[Label], c_color->get(255, 255, 255, 255));

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = true;
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = false;

    ImGui::SetCursorPosX(slider_cursorx[Label] + 5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));

    ImGui::Button(Label.c_str(), { 10,10 });

    ImGui::PopStyleColor(4);

    if (ImGui::IsItemHovered() && ImGui::IsItemFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = true;
    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        slider_cursorHold[Label] = false;

    int mappedshit = (ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8);
    mappedshit = ImClamp({ (float)mappedshit, 0 }, { 0,0 }, { 215, 0 }).x;

    if (slider_cursorHold[Label])
    {
        for (auto& pair : slider_cursorHold) {
            if (pair.first != Label)
                pair.second = false;
        }

        Animate(4, 6, &slider_cursorSize[Label], 0.15);

        if (!(mappedshit < slider_cursorx[Label] + 10 && mappedshit > slider_cursorx[Label] - 10))
            Animate(slider_cursorx[Label], (ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8), &slider_cursorx[Label], 0.15);
        else
            slider_cursorx[Label] = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8;

        // slider_cursorx[Label] = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - 8;
        slider_cursorx[Label] = ImClamp({ (float)slider_cursorx[Label], 0 }, { 0,0 }, { 215, 0 }).x;

        value[0] = mapval(slider_cursorx[Label], 0.f, 215.f, min, max);
        value[0] = ImClamp({ (float)*value, 0 }, { (float)min,0 }, { (float)max, 0 }).x;
    }
    else
    {
        Animate(6, 4, &slider_cursorSize[Label], 0.15);
    }

    spacingelement();
}

bool in_bounds(int rectX, int rectY, int rectXX, int rectYY) {
    return (ImGui::GetMousePos().x >= rectX && ImGui::GetMousePos().x <= rectXX &&
        ImGui::GetMousePos().y >= rectY && ImGui::GetMousePos().y <= rectYY);
}

std::unordered_map<string, bool> combo_toggled;
std::unordered_map<string, float> combo_ininalpha;
std::unordered_map<string, int> combo_count;

void menu::add_combo(std::string Label, int* selected, std::vector<string> items)
{
    ImGui::Dummy({ 291, 30 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(dummy_min, dummy_max, c_color->get(255, 255, 255, 15));

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((30 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // combox
    ImGui::SetCursorPosX(291 - 155);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 25);
    ImGui::Dummy({ 150,20 });
    ImVec2 combo_min = ImGui::GetItemRectMin();
    ImVec2 combo_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get(255, 0, 0));
    current_child_dl->AddRectFilled(combo_min, combo_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(combo_min, combo_max, c_color->get(55, 55, 55), 5);

    ImGui::PushFont(menufonts["comboitemlabel"]);
    current_child_dl->AddText({ combo_min.x + 5, combo_min.y + ((20 / 2) - ImGui::CalcTextSize(items[*selected].c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), items[*selected].c_str());
    ImGui::PopFont();

    ImGui::PushFont(menufonts["comboitemarrow"]);
    if (combo_toggled[Label])
        current_child_dl->AddText({ combo_max.x - 25, combo_min.y + ((20 / 2) - ImGui::CalcTextSize("M").y / 2 + 1) }, c_color->get(225, 225, 225), "M");
    else
        current_child_dl->AddText({ combo_max.x - 25, combo_min.y + ((20 / 2) - ImGui::CalcTextSize("K").y / 2 + 1) }, c_color->get(225, 225, 225), "K");
    ImGui::PopFont();

    if (ImGui::IsItemClicked())
        combo_toggled[Label] = !combo_toggled[Label];

    if (combo_toggled[Label])
    {
        ImGui::SetNextWindowPos({ combo_min.x, combo_min.y + 25 });
        ImGui::PushFont(menufonts["itemlabel"]);
        ImGui::SetNextWindowSize({ combo_max.x - combo_min.x,10 + float(15 * items.size()) });
        ImGui::PopFont();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1);
        ImGui::Begin(Label.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
        ImGui::BringWindowToFocusFront(ImGui::GetCurrentWindow());

        ImVec4 inboundsposition(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y - 25, ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 16 * (float)items.size());
        if (!in_bounds(inboundsposition.x, inboundsposition.y, inboundsposition.z, inboundsposition.w) && ImGui::IsAnyMouseDown())
            combo_toggled[Label] = false;

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 comboin_min = { ImGui::GetWindowPos().x + 1,ImGui::GetWindowPos().y + 1 };
        ImVec2 comboin_max = { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 1,ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 1 };

        dl->AddRectFilled(comboin_min, comboin_max, c_color->get(40, 40, 40, 255), 5);
        dl->AddRect(comboin_min, comboin_max, c_color->get(55, 55, 55, 255), 5, 0, 1);
        Dummy({ 5, 5 });

        int summ = 0;
        for (int i = 0; i < items.size(); i++) {
            ImGui::SetCursorPosX(5);

            if (selected[0] == i)
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, 255));
            else
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::PushFont(menufonts["itemlabel"]);
            dl->AddText({ comboin_min.x + 5, comboin_min.y + summ + 5 }, c_color->get(ImGui::GetColorU32(ImGuiCol_Text)), items[i].c_str());
            ImGui::PopFont();
            ImGui::PopStyleColor();

            ImGui::Dummy({ 75, 15 });
            if (ImGui::IsItemClicked())
            {
                selected[0] = i;
                combo_toggled[Label] = false;
            }

            //ImGui::Dummy({ 0,1 });
            summ += 15;
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    spacingelement();
}

std::unordered_map<string, bool> multicombo_toggled;
std::unordered_map<string, int> multicombo_ininalpha;
std::unordered_map<string, std::string> multicombo_stritems;
std::unordered_map<string, int> multicombo_selectedcount;

void menu::add_multicombo(std::string Label, std::vector<bool>* selected, std::vector<std::string> items)
{
    ImGui::Dummy({ 291, 30 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(dummy_min, dummy_max, c_color->get(255, 255, 255, 15));

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((30 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // combox
    ImGui::SetCursorPosX(291 - 155);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 25);
    ImGui::Dummy({ 150,20 });
    ImVec2 combo_min = ImGui::GetItemRectMin();
    ImVec2 combo_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(sliderch_min, sliderch_max, c_color->get(255, 0, 0));
    current_child_dl->AddRectFilled(combo_min, combo_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(combo_min, combo_max, c_color->get(55, 55, 55), 5);

    multicombo_selectedcount[Label] = 0;
    multicombo_stritems[Label] = "";
    for (int i = 0; i < items.size(); i++)
    {
        if (selected[0][i])
        {
            multicombo_stritems[Label] += items[i] + ",";
            multicombo_selectedcount[Label]++;

        }
        if (multicombo_stritems[Label].length() > 6)
        {
            multicombo_stritems[Label].pop_back();
            multicombo_stritems[Label] += "....";
            break;
        }

    }

    if (multicombo_selectedcount[Label] == 0)
        multicombo_stritems[Label] = "...";
    else if (multicombo_selectedcount[Label] >= 1)
        multicombo_stritems[Label].pop_back();

    ImGui::PushFont(menufonts["comboitemlabel"]);
    current_child_dl->AddText({ combo_min.x + 5, combo_min.y + ((20 / 2) - ImGui::CalcTextSize(multicombo_stritems[Label].c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), multicombo_stritems[Label].c_str());
    ImGui::PopFont();

    ImGui::PushFont(menufonts["comboitemarrow"]);
    if (multicombo_toggled[Label])
        current_child_dl->AddText({ combo_max.x - 25, combo_min.y + ((20 / 2) - ImGui::CalcTextSize("M").y / 2 + 1) }, c_color->get(225, 225, 225), "K");
    else
        current_child_dl->AddText({ combo_max.x - 25, combo_min.y + ((20 / 2) - ImGui::CalcTextSize("M").y / 2 + 1) }, c_color->get(225, 225, 225), "K");
    ImGui::PopFont();

    if (ImGui::IsItemClicked())
        multicombo_toggled[Label] = !multicombo_toggled[Label];

    if (multicombo_toggled[Label])
    {
        for (auto& pair : multicombo_toggled) {
            if (pair.first != Label)
                pair.second = false;
        }

        ImGui::SetNextWindowPos({ combo_min.x, combo_min.y + 25 });
        ImGui::PushFont(menufonts["itemlabel"]);
        ImGui::SetNextWindowSize({ combo_max.x - combo_min.x,10 + float(15 * items.size()) });
        ImGui::PopFont();

        ImGui::Begin(Label.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        ImGui::BringWindowToFocusFront(ImGui::GetCurrentWindow());
        ImDrawList* dl = ImGui::GetWindowDrawList();


        ImVec4 inboundsposition(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y - 25, ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 16 * (float)items.size());
        if (!in_bounds(inboundsposition.x, inboundsposition.y, inboundsposition.z, inboundsposition.w) && ImGui::IsAnyMouseDown())
            multicombo_toggled[Label] = false;

        ImVec2 comboin_min = { ImGui::GetWindowPos().x + 1,ImGui::GetWindowPos().y + 1 };
        ImVec2 comboin_max = { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 1,ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 1 };

        dl->AddRectFilled(comboin_min, comboin_max, c_color->get(40, 40, 40, 255), 5);
        dl->AddRect(comboin_min, comboin_max, c_color->get(55, 55, 55, 255), 5, 0, 1);
        Dummy({ 5, 5 });

        int summ = 0;
        for (int i = 0; i < items.size(); i++) {
            ImGui::SetCursorPosX(5);

            if (selected[0][i] == true)
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, 255));
            else
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::PushFont(menufonts["itemlabel"]);
            dl->AddText({ comboin_min.x + 5, comboin_min.y + summ + 5 }, c_color->get(ImGui::GetColorU32(ImGuiCol_Text)), items[i].c_str());
            ImGui::PopFont();
            ImGui::PopStyleColor();

            ImGui::Dummy({ 75, 15 });
            if (ImGui::IsItemClicked())
            {
                selected[0][i] = !selected[0][i];
            }

            //ImGui::Dummy({ 0,1 });
            summ += 15;
        }
        ImGui::End();
    }

    spacingelement();

}

std::unordered_map<string, int> button_shadowalpha;

bool menu::add_button(std::string Label)
{
    ImGui::Dummy({ 291, 25 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    //current_child_dl->AddRectFilled(dummy_min, dummy_max, c_color->get(255, 255, 255, 15));

    bool result = false;
    ImGui::SetCursorPosX(10);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
    if (InvisibleButton(Label.c_str(), { 271, 20 }))
        result = true;

    ImVec2 button_min = ImGui::GetItemRectMin();
    ImVec2 button_max = ImGui::GetItemRectMax();

    current_child_dl->AddShadowRect(button_min, button_max, c_color->get(ACCENT_R, ACCENT_G, ACCENT_B, 75), button_shadowalpha[Label], { 0,0 }, 0, 5);
    current_child_dl->AddRectFilled(button_min, button_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(button_min, button_max, c_color->get(55, 55, 55), 5);

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        current_child_dl->AddRectFilled(button_min, button_max, c_color->get(57, 57, 57), 5);
        current_child_dl->AddRect(button_min, button_max, c_color->get(67, 67, 67), 5);
    }
    else if (ImGui::IsItemHovered())
    {
        current_child_dl->AddRectFilled(button_min, button_max, c_color->get(47, 47, 47), 5);
        current_child_dl->AddRect(button_min, button_max, c_color->get(65, 65, 65), 5);
        Animate(button_shadowalpha[Label], 15, &button_shadowalpha[Label], 0.3);
    }
    else
    {
        Animate(button_shadowalpha[Label], 0, &button_shadowalpha[Label], 0.3);
    }

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + (291 / 2 - ImGui::CalcTextSize(Label.c_str()).x / 2), dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    spacingelement();
    return result;
}

void menu::add_label(std::string Label, bool centered)
{
    ImGui::Dummy({ 291, 25 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    spacingelement();
}

std::unordered_map<string, bool> colorpicker_toggle;
//std::unordered_map<string, bool> colorpicker_toggle;
std::unordered_map<string, float[4]> colorpicker_floatclr;

uint32_t ImGuiColorToU32(const float color[4]) {
    // Clamp and convert the color components from float [0, 1] to int [0, 255]
    uint8_t r = static_cast<uint8_t>(std::clamp(color[0], 0.0f, 1.0f) * 255.0f);
    uint8_t g = static_cast<uint8_t>(std::clamp(color[1], 0.0f, 1.0f) * 255.0f);
    uint8_t b = static_cast<uint8_t>(std::clamp(color[2], 0.0f, 1.0f) * 255.0f);
    uint8_t a = static_cast<uint8_t>(std::clamp(color[3], 0.0f, 1.0f) * 255.0f);

    // Combine the components into a single U32 value in ARGB format
    uint32_t u32_color = (a << 24) | (r << 16) | (g << 8) | b;
    return u32_color;
}

void menu::add_colorpicker(std::string Label, ImU32* color, bool alpha)
{
    ImGui::Dummy({ 291, 25 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // color
    ImGui::SetCursorPosX(291 - 35);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
    ImGui::Dummy({ 30,20 });
    ImVec2 clr_min = ImGui::GetItemRectMin();
    ImVec2 clr_max = ImGui::GetItemRectMax();

    current_child_dl->AddRectFilled(clr_min, clr_max, c_color->get(colorpicker_floatclr[Label][0], colorpicker_floatclr[Label][1], colorpicker_floatclr[Label][2], 1.0f), 5);
    current_child_dl->AddRect(clr_min, clr_max, c_color->get(55, 55, 55, 255), 5, 0, 2);

    if (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGuiKey_MouseLeft, false))
        colorpicker_toggle[Label] = !colorpicker_toggle[Label];

    float temp[4];
    U32ToImColor(*color, temp);
    colorpicker_floatclr[Label][0] = temp[0];
    colorpicker_floatclr[Label][1] = temp[1];
    colorpicker_floatclr[Label][2] = temp[2];
    colorpicker_floatclr[Label][3] = temp[3];

    if (colorpicker_toggle[Label])
    {

        ImGui::SetNextWindowPos({ clr_min.x - 135, clr_min.y + 25 });
        ImGui::PushFont(menufonts["itemlabel"]);
        ImGui::SetNextWindowSize({ clr_max.x - clr_min.x + 135, 130 });
        ImGui::PopFont();

        ImGui::Begin(Label.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        ImGui::BringWindowToFocusFront(ImGui::GetCurrentWindow());
        ImDrawList* dl = ImGui::GetWindowDrawList();


        if (!ImGui::IsWindowFocused())
            colorpicker_toggle[Label] = false;


        ImVec4 inboundsposition(ImGui::GetWindowPos().x - 135, ImGui::GetWindowPos().y - 25, ImGui::GetWindowPos().x + ImGui::GetWindowSize().x + 155, ImGui::GetWindowPos().y + 155);
        if (!in_bounds(inboundsposition.x, inboundsposition.y, inboundsposition.z, inboundsposition.w) && ImGui::IsKeyDown(ImGuiKey_MouseLeft))
            multicombo_toggled[Label] = false;

        //dl->AddRect({ inboundsposition.x, inboundsposition.y }, { inboundsposition.z, inboundsposition.w }, c_color->get(255, 0, 0));

        ImVec2 comboin_min = { ImGui::GetWindowPos().x + 1,ImGui::GetWindowPos().y + 1 };
        ImVec2 comboin_max = { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 1,ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 1 };

        dl->AddRectFilled(comboin_min, comboin_max, c_color->get(40, 40, 40, 255), 5);
        dl->AddRect(comboin_min, comboin_max, c_color->get(55, 55, 55, 255), 5, 0, 1);

        ImGui::SetCursorPos({ 10,9 });
        int cfgalpha = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

        ImGui::ColorPicker4(Label.c_str(), colorpicker_floatclr[Label], (cfgalpha | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview));
        //std::cout << *color << endl;

        // TO DO COMPLETE AND MODIFY COLOR PICKER DINGUS LESS GO

        ImGui::End();
    }
    *color = ImGuiColorToU32(colorpicker_floatclr[Label]);

    spacingelement();
}

std::unordered_map<string, bool> keybind_toggle;
std::unordered_map<string, bool> keybind_toggleright;
std::unordered_map<string, string> keybind_strkey;
std::unordered_map<string, ImGuiKey> keybind_key;
std::unordered_map<string, int> keybind_keycounter;
std::unordered_map<string, int> keybind_type; // 0 = Always on, 1 = Hold, 2 = Toggle
std::unordered_map<string, float> keybind_sizex;

void menu::add_keybind(std::string Label, std::vector<int>* value, static ImGuiKey default_value)
{
    ImGui::Dummy({ 291, 25 });
    ImVec2 dummy_min = ImGui::GetItemRectMin();
    ImVec2 dummy_max = ImGui::GetItemRectMax();

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ dummy_min.x + 10, dummy_min.y + ((25 / 2) - ImGui::CalcTextSize(Label.c_str()).y / 2 + 1) }, c_color->get(225, 225, 225), Label.c_str());
    ImGui::PopFont();

    // keybind
    ImGui::PushFont(menufonts["itemlabel"]);
    float calculatedkeybindsizex = ImGui::CalcTextSize(keybind_strkey[Label].c_str()).x + 10;
    Animate(calculatedkeybindsizex, calculatedkeybindsizex, &keybind_sizex[Label], 0.3);
    ImGui::PopFont();

    ImGui::SetCursorPosX(291 - (keybind_sizex[Label] + 5));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 22);
    ImGui::Dummy({ keybind_sizex[Label],20 });
    ImVec2 bind_min = ImGui::GetItemRectMin();
    ImVec2 bind_max = ImGui::GetItemRectMax();

    if (ImGui::IsItemClicked() || keybind_toggle[Label] && !keybind_toggleright[Label]) // set the keybind to the first key
    {
        keybind_toggle[Label] = true; // wait for toggle
        keybind_toggleright[Label] = false;

        keybind_strkey[Label] = "[..]";

        for (int key = 0; key < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); key++)
        {
            if (ImGui::IsKeyPressed((ImGuiKey)key))
            {
                keybind_keycounter[Label]++;

                // Ignore the first 5 key presses
                if (keybind_keycounter[Label] <= 2)
                {
                    continue;
                }

                keybind_key[Label] = (ImGuiKey)key;
                keybind_toggle[Label] = false; // get out
                break;
            }
        }
    }
    else if (ImGui::IsItemClicked(ImGuiMouseButton_Right) || keybind_toggleright[Label] && !keybind_toggle[Label])
    {
        keybind_toggleright[Label] = true;
        keybind_toggle[Label] = false;

        ImGui::SetNextWindowPos({ dummy_max.x - 80, dummy_min.y + 25 });
        ImGui::PushFont(menufonts["itemlabel"]);
        ImGui::SetNextWindowSize({ 75, 57 });
        ImGui::PopFont();

        ImGui::Begin(Label.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        ImGui::BringWindowToFocusFront(ImGui::GetCurrentWindow());
        ImDrawList* dl = ImGui::GetWindowDrawList();

        ImVec2 rightin_min = { ImGui::GetWindowPos().x + 1,ImGui::GetWindowPos().y + 1 };
        ImVec2 rightin_max = { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 1,ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 1 };

        dl->AddRectFilled(rightin_min, rightin_max, c_color->get(40, 40, 40), 5);
        dl->AddRect(rightin_min, rightin_max, c_color->get(55, 55, 55), 5);

        if (!ImGui::IsWindowFocused())
            keybind_toggleright[Label] = false;

        //ImVec4 inboundsposition(ImGui::GetWindowPos().x - 135, ImGui::GetWindowPos().y - 25, ImGui::GetWindowPos().x + ImGui::GetWindowSize().x + 155, ImGui::GetWindowPos().y + 155);
        //if (!in_bounds(inboundsposition.x, inboundsposition.y, inboundsposition.z, inboundsposition.w) && ImGui::IsKeyDown(ImGuiKey_MouseLeft))
        //    keybind_toggleright[Label] = false;

        ImGui::SetCursorPos({ ImGui::GetCursorPos().x + 4, ImGui::GetCursorPos().y + 3 });

        ImGui::PushFont(menufonts["keybindselection"]);

        if (keybind_type[Label] == 0)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

        ImGui::Text("Always on");
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            keybind_type[Label] = 0;
            keybind_toggleright[Label] = false;
        }

        ImGui::SetCursorPos({ ImGui::GetCursorPos().x + 4, ImGui::GetCursorPos().y + 3 });

        if (keybind_type[Label] == 1)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

        ImGui::Text("Hold");
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            keybind_type[Label] = 1;
            keybind_toggleright[Label] = false;
        }

        ImGui::SetCursorPos({ ImGui::GetCursorPos().x + 4, ImGui::GetCursorPos().y + 3 });

        if (keybind_type[Label] == 2)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(ACCENT_R, ACCENT_G, ACCENT_B, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));

        ImGui::Text("Toggle");
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked())
        {
            keybind_type[Label] = 2;
            keybind_toggleright[Label] = false;
        }

        ImGui::PopFont();

        ImGui::End();

    }

    if (keybind_toggle[Label] == false)
    {
        std::string lbl = ImGuiKeyToString(keybind_key[Label]);
        if (lbl == "Esc")
            keybind_strkey[Label] = "[None]";
        else
            keybind_strkey[Label] = "[" + lbl + "]";

        keybind_keycounter[Label] = 0;
    }

    current_child_dl->AddRectFilled(bind_min, bind_max, c_color->get(40, 40, 40), 5);
    current_child_dl->AddRect(bind_min, bind_max, c_color->get(55, 55, 55), 5);

    ImGui::PushFont(menufonts["itemlabel"]);
    current_child_dl->AddText({ bind_min.x + keybind_sizex[Label] / 2 - ImGui::CalcTextSize(keybind_strkey[Label].c_str()).x / 2, bind_min.y + 20 / 2 - ImGui::CalcTextSize(keybind_strkey[Label].c_str()).y / 2 }, c_color->get(255, 255, 255), keybind_strkey[Label].c_str());
    ImGui::PopFont();

    *value = std::vector<int>{ keybind_key[Label], keybind_type[Label] };

    // keybind behavior not do it here

    spacingelement();
}
