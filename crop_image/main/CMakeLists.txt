set(srcs "main.c" "keyboard.c" "http_server.c" "cv_crop_image.cpp")
idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
