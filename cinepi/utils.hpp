#include <iostream>
#include "raw_options.hpp"
#include <filesystem>
namespace fs = std::filesystem;

enum CompressionType { NONE = 1, LOSSLESS = 7 };

static bool disk_mounted(RawOptions const *options){
	return fs::exists(fs::path(options->mediaDest));
}

static void generate_filename(RawOptions *options, unsigned int clip_number = 0)
{
	char filename[128];
	std::time_t raw_time;
	std::time(&raw_time);
	char time_string[32];
	std::tm *time_info = std::localtime(&raw_time);
	std::strftime(time_string, sizeof(time_string), "%y-%m-%d_%H%M", time_info);
	snprintf(filename, sizeof(filename), "%s_%s_C%05d", "CINEPI", time_string, clip_number);
	options->folder = std::string(filename);
}

static bool create_clip_folder(RawOptions *options, unsigned int clip_number = 0)
{
	if(!disk_mounted(options))
		return false;
	generate_filename(options, clip_number);
	return fs::create_directories(options->mediaDest + std::string("/") + options->folder);
}


static bool create_stills_folder(RawOptions *options, unsigned int still_number = 0)
{
	if(!disk_mounted(options))
		return false;
	std::string stillsPath = options->mediaDest + std::string("/stills");
	bool exists = fs::exists(fs::path(stillsPath));
	generate_filename(options, still_number);
	if(!exists){
		return fs::create_directories(options->mediaDest + std::string("/stills"));
	}
	return exists;
}