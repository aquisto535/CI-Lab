#pragma once

#include <filesystem>
#include <string>

namespace WinVision {

	struct CommandLineOptions {
		std::string action{ "stats" };
		std::filesystem::path input;
		std::filesystem::path output{ "winvision_output.pgm" };
		bool generateDemo{ false };
	};

	CommandLineOptions ParseCommandLine(int argc, char** argv);
	void RunWorkflow(const CommandLineOptions& options);

}  // namespace WinVision
