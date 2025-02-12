#include "ProfilerPanel.h"

#include <implot.h>

namespace Laura
{

	void ProfilerPanel::OnImGuiRender(std::shared_ptr<const Profiler> profiler)
	{
        const char* const* labels = profiler->getLabels();
        const double* durations = profiler->getDurations();
        const uint16_t timersPerFrame = profiler->getCount(); // count of timers set per frame
        if (timersPerFrame == 0) return;

        static const int MAX_FRAMES_STORED = 250;

        static CircularBuffer buff = CircularBuffer();
        buff.setCapacity(MAX_FRAMES_STORED * timersPerFrame);

        for (int i = 0; i < timersPerFrame; i++) {
            buff.push_back(durations[i]);
        }

        const double* buffData = buff.data();

        const int framesStored = buff.size() / timersPerFrame;
        std::vector<double> data;

        for (int i = 0; i < timersPerFrame; i++){
            for (int j = i; j < buff.size(); j += timersPerFrame) {
                data.push_back(buffData[j]);
            }        
        }

		ImGui::Begin("Profiler");
        if (ImPlot::BeginPlot("##Frame Breakdown", ImVec2(-1, -1))) {
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, MAX_FRAMES_STORED, ImPlotCond_Always);

            ImPlot::SetupAxes("Frame", "elapsed_ms",
                ImPlotAxisFlags_AutoFit,  // disable X auto-fit
                ImPlotAxisFlags_AutoFit);      // keep Y auto-fit

            ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
            ImPlot::PlotBarGroups(labels, data.data(), timersPerFrame, framesStored, 1, 0, ImPlotBarGroupsFlags_Stacked);
            ImPlot::EndPlot();
        }
		ImGui::End();
	}
}