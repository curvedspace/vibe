project(QubeDataFonts)

set(QubeFonts
    DroidSans.ttf
    DroidSans-Bold.ttf
    DroidSerif-Regular.ttf
    DroidSerif-Bold.ttf
    DroidSerif-Italic.ttf
    DroidSerif-BoldItalic.ttf
    DroidSansMono.ttf
    Clockopia.ttf
)

install(FILES ${QubeFonts}
    DESTINATION ${QUBE_DATA_FONTS_DIR} COMPONENT Data)
