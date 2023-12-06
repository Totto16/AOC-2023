project(
    'ppk_assert',
    'cpp',
    version: '1.0.0',
    meson_version: '>=1.2.0',
)

inc_dirs = []
override_options = []


subdir('src')


ppk_assert_dep = declare_dependency(
    include_directories: inc_dirs,
    version: meson.project_version(),
    link_with: ppk_assert_lib,
)
