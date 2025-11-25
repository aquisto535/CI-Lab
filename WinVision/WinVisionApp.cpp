#include "App.h"

#include "../ImageCore/ImageFilter.h"
#include "../ImageCore/ImageIO.h"
#include "../ImageCore/ImageStats.h"

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace WinVision {

	namespace {

		void PrintUsage(const char* exe) {
			std::cout << "Usage: " << exe << " [--action stats|blur|sharpen|normalize]"
				<< " [--input input.pgm] [--output output.pgm] [--generate-demo]\n";
		}

		std::optional<std::string> ReadValue(const std::vector<std::string>& args, std::size_t& index) {
			if (index + 1 >= args.size()) {
				return std::nullopt;
			}
			++index;
			return args[index];
		}

		void PrintHistogram(const ImageCore::Histogram& histogram) {
			std::cout << "Histogram (0, 64, 128, 192, 255 bins preview): ";
			std::array<int, 5> probes{ 0, 64, 128, 192, 255 };
			for (auto probe : probes) {
				std::cout << probe << ":" << histogram.bins[probe] << " ";
			}
			std::cout << "\n";
		}

	}  // namespace

	CommandLineOptions ParseCommandLine(int argc, char** argv) {
		CommandLineOptions options;
		std::vector<std::string> args(argv + 1, argv + argc);

		for (std::size_t i = 0; i < args.size(); ++i) {
			const auto& arg = args[i];
			if (arg == "--action") {
				auto value = ReadValue(args, i);
				if (!value) {
					throw std::runtime_error("--action requires a value");
				}
				options.action = *value;
			}
			else if (arg == "--input") {
				auto value = ReadValue(args, i);
				if (!value) {
					throw std::runtime_error("--input requires a value");
				}
				options.input = *value;
			}
			else if (arg == "--output") {
				auto value = ReadValue(args, i);
				if (!value) {
					throw std::runtime_error("--output requires a value");
				}
				options.output = *value;
			}
			else if (arg == "--generate-demo") {
				options.generateDemo = true;
			}
			else if (arg == "--help" || arg == "-h") {
				PrintUsage(argv[0]);
				std::exit(0);
			}
		}
		return options;
	}

	void RunWorkflow(const CommandLineOptions& options) {
		using namespace ImageCore;

		Image image;
		if (options.generateDemo) {
			image = CreateDemoGradient(256, 128);
			SavePGM(image, options.output.empty() ? std::filesystem::path{ "gradient_demo.pgm" } : options.output);
			std::cout << "Generated demo gradient to " << options.output << "\n";
			return;
		}

		if (options.input.empty()) {
			throw std::runtime_error("Input file is required unless --generate-demo is used.");
		}

		image = LoadPGM(options.input);

		auto action = options.action;
		std::transform(action.begin(), action.end(), action.begin(), ::tolower);

		if (action == "stats") {
			auto histogram = ComputeHistogram(image);
			auto mean = ComputeMean(image);
			auto stddev = ComputeStdDev(image);
			PrintHistogram(histogram);
			std::cout << "Mean: " << mean << " StdDev: " << stddev << "\n";
		}
		else if (action == "blur") {
			auto blurred = ApplyBoxBlur(image, 2);
			SavePGM(blurred, options.output);
			std::cout << "Blurred image written to " << options.output << "\n";
		}
		else if (action == "sharpen") {
			auto sharpened = ApplyUnsharpMask(image, 1.2f);
			SavePGM(sharpened, options.output);
			std::cout << "Sharpened image written to " << options.output << "\n";
		}
		else if (action == "normalize") {
			auto normalized = NormalizeContrast(image);
			SavePGM(normalized, options.output);
			std::cout << "Contrast-normalized image written to " << options.output << "\n";
		}
		else {
			throw std::runtime_error("Unknown action: " + options.action);
		}
	}

}  // namespace WinVision

int main(int argc, char** argv) {
	try {
		auto options = WinVision::ParseCommandLine(argc, argv);
		WinVision::RunWorkflow(options);
	}
	catch (const std::exception & ex) {
		std::cerr << "[WinVision] Error: " << ex.what() << "\n";
		std::cerr << "Run with --help to see command options.\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}