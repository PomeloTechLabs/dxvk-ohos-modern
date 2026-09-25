install_subdir(
  'directx',
  install_dir: get_option('includedir') / 'dxvk',
  strip_directory: true,
  exclude_files: '.git'
)

install_subdir(
  'windows',
  install_dir: get_option('includedir') / 'dxvk',
  strip_directory: true,
)

install_headers(
  'wsi/native_wsi.h',
  'wsi/native_sdl3.h',
  'wsi/native_sdl2.h',
  'wsi/native_glfw.h',
  'wsi/native_ohos.h',
  subdir: 'dxvk/wsi',
)

if dxvk_native_ohos
  install_headers('ohos/dxvk_native_ohos.h', subdir: 'dxvk/ohos')
endif
