# ----------------------------
# Makefile Options
# ----------------------------

NAME = CEYBOARD
ICON = icon.png
DESCRIPTION = "CEyboard"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
