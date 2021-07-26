project(
  'libcgif',
  'c',
  license : 'MIT',
  default_options : ['c_std=c99'],
)

gio = dependency(
  'gio-2.0',
  required : true,
)

lib = library(
  'cgif',
  'cgif.c',
  install : true,
  dependencies: [gio],
)

install_headers('cgif.h')

import('pkgconfig').generate(
  lib,
  name : 'cgif',
  description : 'A fast and lightweight GIF encoder',
)

if get_option('tests') and not meson.is_cross_build()
  libcgif_dep = declare_dependency(link_with : lib)
  subdir('tests')
endif
