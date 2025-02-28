#include <IconsFontAwesome6.h>
#include <implot.h>
#include "ProfilerPanel.h"

namespace Laura {

    void ProfilerPanel::OnImGuiRender(std::shared_ptr<Profiler> profiler)
    {
        if (!m_EditorState->temp.ProfilerPanelOpen)
            return;

        auto t = profiler->timer("ProfilerPanel");

        ImPlot::PushColormap(ImPlotColormap_Deep);
        ImGui::Begin(ICON_FA_STOPWATCH " Profiler", &m_EditorState->temp.ProfilerPanelOpen);
        static ImPlotAxisFlags AutoFitFlagCheckboxBool = ImPlotAxisFlags_AutoFit;
        // Widgets - [Pause] [FPS & ms], [Clear Button], [Autofit]
        if (profiler->globalTimerSet) {
            const char* pauseLabel = (profiler->isPaused) ? ICON_FA_PLAY : ICON_FA_PAUSE;
            if (ImGui::Button(pauseLabel))
                profiler->isPaused = !profiler->isPaused;

            const ScrollingBuffer& buff = profiler->getGlobalBuffer();
            if (!buff.empty()) {
                ImGui::SameLine();
                double Global_ms = buff.data()[buff.size() - 1];
                int Global_FPS = static_cast<int>(1000.0 / Global_ms); // FPS = 1/ms
                ImGui::AlignTextToFramePadding();
                ImGui::Text("%d FPS", Global_FPS);
                ImGui::SameLine();
                ImGui::Text("[%.1f ms]", Global_ms);
                ImGui::SameLine();

                m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);
                if (ImGui::Button("Clear All " ICON_FA_REPEAT))
                    profiler->clear();
                ImGui::SameLine();

                ImGui::CheckboxFlags("Autofit", &AutoFitFlagCheckboxBool, ImPlotAxisFlags_AutoFit);
                m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
            }
        }

        // Subplots - 1 row, 2 columns.
        static float SubPlot_WidthRatios[] = { 5, 1 };
        const ImPlotSubplotFlags SubPlot_Flags = ImPlotSubplotFlags_ShareItems | ImPlotSubplotFlags_NoTitle;
        ImPlot::BeginSubplots("##SUBPLOTS", 1, 2, ImVec2(-1, -1), SubPlot_Flags, 0, SubPlot_WidthRatios);

        // Shared legend on left side
        ImPlot::SetupLegend(ImPlotLocation_West, ImPlotLegendFlags_Sort);

        if (ImPlot::BeginPlot("##LINE", ImVec2(-1, -1), ImPlotFlags_NoMouseText)) {
            if (profiler->globalTimerSet) {
                ImPlot::SetupAxes(nullptr, nullptr, AutoFitFlagCheckboxBool, AutoFitFlagCheckboxBool | ImPlotAxisFlags_Opposite);
                ImPlot::SetupAxisFormat(ImAxis_Y1, "%gms");
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.05f);

                for (const auto& p : profiler->data()) {
                    const std::string label = p.first;
                    const ScrollingBuffer buff = p.second;
                    ImPlot::PlotShaded<double>(label.c_str(), buff.data(), buff.size());
                    ImPlot::PlotLine<double>(label.c_str(), buff.data(), buff.size());
                }
                ImPlot::PopStyleVar();
            }
            ImPlot::EndPlot();
            ImGui::SameLine();
        }

        if (ImPlot::BeginPlot("##PIE", ImVec2(-1, -1), ImPlotFlags_Equal | ImPlotFlags_NoMouseText)) {
            std::vector<std::string> labels;
            std::vector<double> data;
            for (const auto& p : profiler->data()) {
                // Skip GLOBAL timer
                if (p.first == profiler->getGlobalLabel())
                    continue;
                labels.push_back(p.first);
                data.push_back(p.second.average(10)); // 10% -> percentage of the buffer to get the average of
            }

            // Convert std::vector<std::string> to const char**
            std::vector<const char*> clabels;
            for (const auto& str : labels) {
                clabels.push_back(str.c_str());
            }

            const ImPlotAxisFlags PieChart_AxisFlags = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit;
            ImPlot::SetupAxes(nullptr, nullptr, PieChart_AxisFlags, PieChart_AxisFlags);
            ImPlot::SetupAxesLimits(0, 1, 0, 1);
            ImPlot::PlotPieChart<double>(clabels.data(), data.data(), labels.size(), 0.5, 0.5, 0.40, "%.1f", 90, ImPlotPieChartFlags_Exploding);
            ImPlot::EndPlot();
        }
        ImPlot::EndSubplots();
        ImGui::End();
        ImPlot::PopColormap();
    }
}
