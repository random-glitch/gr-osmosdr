# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/gr-osmosdr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/gr-osmosdr/build

# Utility rule file for osmosdr_grc_xml_blocks.

# Include the progress variables for this target.
include grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/progress.make

grc/CMakeFiles/osmosdr_grc_xml_blocks: grc/rtlsdr_source.xml
grc/CMakeFiles/osmosdr_grc_xml_blocks: grc/osmosdr_source.xml
grc/CMakeFiles/osmosdr_grc_xml_blocks: grc/osmosdr_sink.xml


grc/rtlsdr_source.xml: ../grc/gen_osmosdr_blocks.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pi/gr-osmosdr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating rtlsdr_source.xml"
	cd /home/pi/gr-osmosdr/build/grc && /usr/bin/python2 /home/pi/gr-osmosdr/grc/gen_osmosdr_blocks.py /home/pi/gr-osmosdr/build/grc/rtlsdr_source.xml

grc/osmosdr_source.xml: ../grc/gen_osmosdr_blocks.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pi/gr-osmosdr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating osmosdr_source.xml"
	cd /home/pi/gr-osmosdr/build/grc && /usr/bin/python2 /home/pi/gr-osmosdr/grc/gen_osmosdr_blocks.py /home/pi/gr-osmosdr/build/grc/osmosdr_source.xml

grc/osmosdr_sink.xml: ../grc/gen_osmosdr_blocks.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pi/gr-osmosdr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating osmosdr_sink.xml"
	cd /home/pi/gr-osmosdr/build/grc && /usr/bin/python2 /home/pi/gr-osmosdr/grc/gen_osmosdr_blocks.py /home/pi/gr-osmosdr/build/grc/osmosdr_sink.xml

osmosdr_grc_xml_blocks: grc/CMakeFiles/osmosdr_grc_xml_blocks
osmosdr_grc_xml_blocks: grc/rtlsdr_source.xml
osmosdr_grc_xml_blocks: grc/osmosdr_source.xml
osmosdr_grc_xml_blocks: grc/osmosdr_sink.xml
osmosdr_grc_xml_blocks: grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/build.make

.PHONY : osmosdr_grc_xml_blocks

# Rule to build all files generated by this target.
grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/build: osmosdr_grc_xml_blocks

.PHONY : grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/build

grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/clean:
	cd /home/pi/gr-osmosdr/build/grc && $(CMAKE_COMMAND) -P CMakeFiles/osmosdr_grc_xml_blocks.dir/cmake_clean.cmake
.PHONY : grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/clean

grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/depend:
	cd /home/pi/gr-osmosdr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/gr-osmosdr /home/pi/gr-osmosdr/grc /home/pi/gr-osmosdr/build /home/pi/gr-osmosdr/build/grc /home/pi/gr-osmosdr/build/grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : grc/CMakeFiles/osmosdr_grc_xml_blocks.dir/depend

