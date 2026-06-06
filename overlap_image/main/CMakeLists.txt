set(srcs "main.c" "keyboard.c" "http_server.c" "cv_overlay_image.cpp")
idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
