set(srcs "main.c" "keyboard.c" "http_server.c" "cv_insert_text.cpp")
idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
