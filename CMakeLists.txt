cmake_minimum_required(VERSION 3.16)
project(spl_karel C)
include(data/targets.cmake)

# ---------------------------------------------------------------------------
# Targets
# Formats:
# add_spl_target(name src1.c src2.c ...) for projects using the SPL interface
# add_karel_target(name src1.c src2.c ...) for Karel, the Robot projects
# ---------------------------------------------------------------------------

add_spl_target(spl Pacman.c)
add_karel_target(karel LivingRoomKarel.c)

# For the PG1 exercises
include(exercises/exercises.cmake)