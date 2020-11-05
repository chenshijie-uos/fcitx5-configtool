set(configtool_SRCS
  main.cpp
  mainwindow.cpp
  )

add_executable(fcitx5-config-qt ${configtool_SRCS})
set_target_properties(fcitx5-config-qt
    PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
    AUTOUIC_OPTIONS "-tr=fcitx::tr2fcitx;--include=fcitxqti18nhelper.h")

target_include_directories(fcitx5-config-qt PUBLIC
    ${DtkWidget_INCLUDE_DIRS}
    ${DtkCore_INCLUDE_DIRS}
)

target_link_libraries(fcitx5-config-qt
    ${DtkWidget_LIBRARIES}
    ${DtkCore_LIBRARIES}
    Qt5::Widgets configwidgetslib
)

install(TARGETS fcitx5-config-qt DESTINATION ${CMAKE_INSTALL_BINDIR})
