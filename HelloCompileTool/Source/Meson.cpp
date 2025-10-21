/*
	project('tutorial', 'c')

	链接gtp+3.0
	gtkdep = dependency('gtk+-3.0')

	incdir = include_directories('include')
	
	链接myadd
	my_add_lib = shared_library( 'myadd', ['math/add.cpp','math/add.h'])

	executable('demo', 'main.c', dependencies : gtkdep, include_directories : incdir, link_with: my_add_lib)
*/