add_executable(PixeonApp
    main.cpp
    PXMainWindow.cpp
    PXMainWindow.h
    PXRenderThread.cpp
    PXRenderThread.h
    )

target_link_libraries(PixeonApp Qt${QT_VERSION_MAJOR}::Widgets)
