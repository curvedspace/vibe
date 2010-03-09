project(QubeGuiHeaders)

set(QubeGuiHeaderFiles
        QClickableLabel
        QNavBar
        QNavBarGroup
        QNavBarItem
        QSelectableWidget
)

install(FILES ${QubeGuiHeaderFiles} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeGui COMPONENT Devel)
