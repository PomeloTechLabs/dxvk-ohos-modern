dxgi_res = wrc_generator.process('version.rc')

dxgi_src = [
  'dxgi_adapter.cpp',
  'dxgi_enums.cpp',
  'dxgi_factory.cpp',
  'dxgi_format.cpp',
  'dxgi_main.cpp',
  'dxgi_monitor.cpp',
  'dxgi_options.cpp',
  'dxgi_output.cpp',
  'dxgi_surface.cpp',
  'dxgi_swapchain.cpp',
]

if dxvk_native_ohos
  dxgi_src += ['dxgi_ohos_window.cpp', 'dxgi_ohos_perf.cpp']
endif

dxgi_ld_args      = []
dxgi_link_depends = []
dxgi_platform_deps = []

if dxvk_native_ohos
  dxgi_platform_deps += [cpp.find_library('native_window')]
endif

if platform != 'windows'
  dxgi_ld_args      += [ '-Wl,--version-script', join_paths(meson.current_source_dir(), 'dxgi.sym') ]
  dxgi_link_depends += files('dxgi.sym')
endif

dxgi_dll = shared_library(dxvk_name_prefix+'dxgi', dxgi_src, dxgi_res,
  dependencies        : [ dxvk_dep ] + dxgi_platform_deps,
  include_directories : dxvk_include_path,
  install             : true,
  vs_module_defs      : 'dxgi'+def_spec_ext,
  link_args           : dxgi_ld_args,
  link_depends        : [ dxgi_link_depends ],
  kwargs              : dxvk_so_version,
)

dxgi_dep = declare_dependency(
  link_with           : [ dxgi_dll ],
  include_directories : [ dxvk_include_path ],
)

if platform != 'windows'
  pkg.generate(dxgi_dll,
    filebase: dxvk_pkg_prefix + 'dxgi',
    subdirs:  'dxvk',
  )
endif
