#include "pch.h"
#include "DebugGUI.h"

DebugGUI::DebugGUI()
{
	SetName(STR_GUI_Debug);
}

DebugGUI::~DebugGUI()
{
}

void DebugGUI::Update()
{
    _ShowDebug(&m_bGUIOpen);
}

void DebugGUI::Debug(const char* fmt, ...)
{
    log.AddLog(fmt);
    log.AddLog("\n");
}

void DebugGUI::_ShowDebug(bool* p_open)
{
    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    //ImGui::Begin("Example: Log", p_open);
    //if (ImGui::SmallButton("[Debug] Add 5 entries"))
    //{
        /*const char* category = categories[counter % IM_ARRAYSIZE(categories)];
        const char* word = words[counter % IM_ARRAYSIZE(words)]; static int counter = 0;
        const char* categories[3] = { "info", "warn", "error" };
        const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
        counter++;*/

    //}
    //for (UINT n = 0; n < m_vecLog.size(); n++)
    //{
   
    //    log.AddLog(m_vecLog[n]);
    //    //log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
    //        //ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
    //}
    //ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    log.Draw("Debug", p_open);
}
