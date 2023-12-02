project(
    'colorize',
    'cpp',
    default_options: {
        'cpp_std': 'c++17',
        'warning_level': '3',
        'werror': true,
    },
    version: '1.0.0',
    meson_version: '>=1.2.0',
)


colorize_dep = declare_dependency(
    include_directories: include_directories('.'),
    version: meson.project_version(),
)

if get_option('build_example')
    executable(
        'color_test',
        files('color_test.cc'),
        dependencies: colorize_dep,
    )
endif

