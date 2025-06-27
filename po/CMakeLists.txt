# Define locales
set(LOCALES de)

# Directory for generated .mo-Dateien
set(LOCALE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/locale)

# Template-Target
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/s7cal.pot
        COMMAND xgettext --from-code=UTF-8 -k_ -o s7cal.pot ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h
        DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating translation template (.pot)"
)

add_custom_target(potfile DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/s7cal.pot)

# For every .po -> .mo
foreach(locale ${LOCALES})
    set(PO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${locale}.po)
    set(MO_FILE ${LOCALE_OUTPUT_DIR}/${locale}/LC_MESSAGES/s7cal.mo)

    add_custom_command(
            OUTPUT ${MO_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${LOCALE_OUTPUT_DIR}/${locale}/LC_MESSAGES
            COMMAND msgfmt -o ${MO_FILE} ${PO_FILE}
            DEPENDS ${PO_FILE}
            COMMENT "Compiling ${locale}.po -> ${MO_FILE}"
    )

    add_custom_target(${locale}.mo ALL DEPENDS ${MO_FILE})
endforeach()
