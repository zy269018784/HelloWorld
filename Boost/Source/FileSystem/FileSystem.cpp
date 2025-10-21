#include <iostream>
#include <filesystem>

#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/process.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

int TestCXXFileSystem()
{
	std::filesystem::path path = "1";
	std::filesystem::exists(path);
	std::filesystem::create_directory(path);
	std::filesystem::create_directories(path);
	std::filesystem::create_directory_symlink(path, "2");
	std::filesystem::create_hard_link(path, "2");
	std::filesystem::create_symlink(path, "2");
	std::filesystem::create_hard_link(path, "2");
	std::filesystem::file_size(path);
	std::filesystem::copy(path, "2");
	std::filesystem::copy_file(path, "2");
	std::filesystem::copy_symlink(path, "2");

	return 0;
}

int TestBoostFileSystem()
{
	boost::filesystem::path path = "1";

	boost::filesystem::exists(path);
	boost::filesystem::create_directory(path);
	boost::filesystem::create_directories(path);
	boost::filesystem::create_directory_symlink(path, "2");
	boost::filesystem::create_hard_link(path, "2");
	boost::filesystem::create_symlink(path, "2");
	boost::filesystem::create_hard_link(path, "2");
	boost::filesystem::file_size(path);
	boost::filesystem::copy(path, "2");
	boost::filesystem::copy_file(path, "2");
	boost::filesystem::copy_symlink(path, "2");
	return 0;
}