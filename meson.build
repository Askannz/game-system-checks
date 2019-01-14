project('game-system-checks', 'cpp', default_options : ['cpp_std=c++17'])

cpp = meson.get_compiler('cpp')
gl_dep = cpp.find_library('GL', required : true)
x11_dep = cpp.find_library('X11', required : true)

executable('game-system-checks', 'src/main.cpp', 'src/pci.cpp', 'src/gl_info.cpp', dependencies : [gl_dep, x11_dep])
 
