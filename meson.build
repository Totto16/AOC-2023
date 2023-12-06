project(
    'AoC-2023',
    'cpp',
    default_options: {
        'cpp_std': 'c++23',
        'warning_level': '3',
        'werror': true,
    },
    version: '1.0.0',
    meson_version: '>=1.2.0',
)


deps = []

deps += dependency(
    'argparse',
)

deps += dependency(
    'lexy',
)

deps += dependency(
    'ppk_assert',
)

src_files = []
header_files = []
inc_dirs = []


subdir('src')

compile_flags = [
    '-DPROGRAM_NAME=' + meson.project_name(),
    '-DVERSION_STRING=' + meson.project_version(),
]

executable(
    'aoc',
    src_files,
    header_files,
    dependencies: deps,
    cpp_args: compile_flags,
    include_directories: inc_dirs,
)
