#.rst:
# FindLibswresample
# -----------
#
# Find Libswresample
#
# Find Libswresample headers and libraries
#
# libswresample
#   LIBSWRESAMPLE_FOUND          - True if libswresample found.
#   LIBSWRESAMPLE_INCLUDE_DIRS   - Where to find libswresample.h.
#   LIBSWRESAMPLE_LIBRARIES      - List of libraries when using libswresample.
#   LIBSWRESAMPLE_VERSION_STRING - The version of libswresample found.
#
#

#=============================================================================
# Copyright 2018 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation;
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU GENERAL PUBLIC LICENSE for more details.
#
# You should have received a copy of the GNU General Public
# License along with this library.	If not, see <http://www.gnu.org/licenses/>.
#=============================================================================

find_package(PkgConfig QUIET)
pkg_check_modules(PC_LIBSWRESAMPLE QUIET Libswresample)

find_path(LIBSWRESAMPLE_INCLUDE_DIR
        NAMES libswresample/swresample.h
        PATH_SUFFIXES ffmpeg
        HINTS ${PC_LIBSWRESAMPLE_INCLUDEDIR} ${PC_LIBSWRESAMPLE_INCLUDE_DIRS})

find_library(LIBSWRESAMPLE_LIBRARY
        NAMES libswresample swresample
        HINTS ${PC_LIBSWRESAMPLE_LIBDIR} ${PC_LIBSWRESAMPLE_LIBRARY_DIRS})

set(LIBSWRESAMPLE_VERSION_MAJOR 0)
set(LIBSWRESAMPLE_VERSION_MINOR 0)
set(LIBSWRESAMPLE_VERSION_MICRO 0)
if (PC_LIBSWRESAMPLE_VERSION)
    set(LIBSWRESAMPLE_VERSION_STRING ${PC_LIBSWRESAMPLE_VERSION})
elseif (LIBSWRESAMPLE_INCLUDE_DIR AND EXISTS "${LIBSWRESAMPLE_INCLUDE_DIR}/libswresample/version.h")
    set(regex_libswresample_version_major "^#define[ \t]+LIBSWRESAMPLE_VERSION_MAJOR[ \t]+([^\"]+).*")
    file(STRINGS "${LIBSWRESAMPLE_INCLUDE_DIR}/libswresample/version.h" libswresample_version_major REGEX "${regex_libswresample_version_major}")
    string(REGEX REPLACE "${regex_libswresample_version_major}" "\\1" LIBSWRESAMPLE_VERSION_MAJOR "${libswresample_version_major}")
    unset(regex_libswresample_version_major)
    unset(libswresample_version_major)

    set(regex_libswresample_version_minor "^#define[ \t]+LIBSWRESAMPLE_VERSION_MINOR[ \t]+([^\"]+).*")
    file(STRINGS "${LIBSWRESAMPLE_INCLUDE_DIR}/libswresample/version.h" libswresample_version_minor REGEX "${regex_libswresample_version_minor}")
    string(REGEX REPLACE "${regex_libswresample_version_minor}" "\\1" LIBSWRESAMPLE_VERSION_MINOR "${libswresample_version_minor}")
    unset(regex_libswresample_version_minor)
    unset(libswresample_version_minor)

    set(regex_libswresample_version_micro "^#define[ \t]+LIBSWRESAMPLE_VERSION_MICRO[ \t]+([^\"]+).*")
    file(STRINGS "${LIBSWRESAMPLE_INCLUDE_DIR}/libswresample/version.h" libswresample_version_micro REGEX "${regex_libswresample_version_micro}")
    string(REGEX REPLACE "${regex_libswresample_version_micro}" "\\1" LIBSWRESAMPLE_VERSION_MICRO "${libswresample_version_micro}")
    unset(regex_libswresample_version_micro)
    unset(libswresample_version_micro)
		
		set (LIBSWRESAMPLE_VERSION_STRING "${LIBSWRESAMPLE_VERSION_MAJOR}.${LIBSWRESAMPLE_VERSION_MINOR}.${LIBSWRESAMPLE_VERSION_MICRO}")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libswresample
        REQUIRED_VARS LIBSWRESAMPLE_LIBRARY LIBSWRESAMPLE_INCLUDE_DIR
        VERSION_VAR LIBSWRESAMPLE_VERSION_STRING)

if (LIBSWRESAMPLE_FOUND)
    set(LIBSWRESAMPLE_LIBRARIES ${LIBSWRESAMPLE_LIBRARY})
    set(LIBSWRESAMPLE_INCLUDE_DIRS ${LIBSWRESAMPLE_INCLUDE_DIR})
endif ()

mark_as_advanced(LIBSWRESAMPLE_INCLUDE_DIR LIBSWRESAMPLE_LIBRARY)
