/*
	add_compile_definitions				Added in version 3.12.
	target_sources						Added in version 3.1.	
===================================================================================================================================
cmake-commands:
	Scripting Commands:
		cmake_minimum_required

	Project Commands:
		project
		enable_language
		add_executable
		add_library
		set_target_properties				get_target_property
		set_source_files_properties			get_source_file_property
		add_definitions						remove_definitions
		add_compile_definitions				target_compile_definitions			#define	
		add_compile_options					target_compile_options				/utf-8,					# MSVC utf-8	
																				/W4						# MSVC 的警告级别
																				-input-charset=utf-8	# GCC/Clang utf-8		
																				-Wall					# GCC/Clang 的警告选项
																				-O3  
		include_directories					target_include_directories
		link_directories					target_link_directories
		link_libraries						target_link_libraries			
		add_link_options					target_link_options					
===================================================================================================================================
cmake-variables:
	Variables that Provide Information:
		CMAKE_GENERATOR
		CMAKE_GENERATOR_INSTANCE
		CMAKE_GENERATOR_PLATFORM
		CMAKE_GENERATOR_TOOLSET
		CMAKE_PROJECT_NAME
		CMAKE_SOURCE_DIR
		PROJECT_NAME
		PROJECT_SOURCE_DIR
	Variables that Describe the System:
		CMAKE_HOST_SYSTEM
		CMAKE_HOST_SYSTEM_NAME
	Variables that Control the Build:
		CMAKE_AUTOMOC					Whether to handle moc automatically for Qt targets.
		CMAKE_AUTORCC					Whether to handle rcc automatically for Qt targets.
		CMAKE_AUTOUIC					Whether to handle uic automatically for Qt targets.
	Variables for Languages:
		CMAKE_C_EXTENSIONS
		CMAKE_C_STANDARD
		CMAKE_C_STANDARD_REQUIRED
		CMAKE_CXX_EXTENSIONS
		CMAKE_CXX_STANDARD
		CMAKE_CXX_STANDARD_REQUIRED
cmake-env-variables:
	Environment Variables that Change Behavior:
	Environment Variables that Control the Build:
		CMAKE_GENERATOR
		CMAKE_GENERATOR_INSTANCE
		CMAKE_GENERATOR_PLATFORM
		CMAKE_GENERATOR_TOOLSET
	Environment Variables for Languages:
		CC
		CFLAGS
		CXX
		CXXFLAGS
		OBJC
		OBJCFLAGS
		OBJCXX
		OBJCXXFLAGS
		CUDACXX
		CUDAFLAGS
CMake Generators:
	Makefile Generators:	
		Borland Makefiles
		MSYS Makefiles
		MinGW Makefiles
		NMake Makefiles
		NMake Makefiles JOM
		Unix Makefiles						windows上调用MinGW
		Watcom WMake	
	Visual Studio Generators:
		Visual Studio 6
		Visual Studio 7
		Visual Studio 7 .NET 2003
		Visual Studio 8 2005
		Visual Studio 9 2008
		Visual Studio 10 2010
		Visual Studio 11 2012
		Visual Studio 12 2013
		Visual Studio 14 2015
		Visual Studio 15 2017
		Visual Studio 16 2019
		Visual Studio 17 2022
	Ninja Generators:
		Ninja
		Ninja Multi-Config
===================================================================================================================================
cmake指定Generator:
	cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
===================================================================================================================================
cmake command tool:
	Generate a Project Buildsystem
		cmake [<options>] -B <path-to-build> [-S <path-to-source>]
		cmake [<options>] <path-to-source | path-to-existing-build>

	Build a Project
		cmake --build <dir> [<options>] [-- <build-tool-options>]

	Install a Project
		cmake --install <dir> [<options>]

	Open a Project
		cmake --open <dir>

	Run a Script
		cmake [-D <var>=<value>]... -P <cmake-script-file>

	Run a Command-Line Tool
		cmake -E <command> [<options>]

	Run the Find-Package Tool
		cmake --find-package [<options>]

	Run a Workflow Preset
		cmake --workflow <options>

	View Help
		cmake --help[-<topic>]
*/