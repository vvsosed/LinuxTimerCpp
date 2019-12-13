set(MAIN_SRC
    main.cpp  )

add_executable(main ${MAIN_SRC})

target_link_libraries( main tmrwrap)
