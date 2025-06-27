# Set source and output directories
set(MAN_SRC_DIR ${PROJECT_SOURCE_DIR}/man)
set(MAN_OUT_DIR ${PROJECT_BINARY_DIR}/man)

# Create output directory if needed
file(MAKE_DIRECTORY ${MAN_OUT_DIR})

# Find all Markdown manpage sources (*.md)
file(GLOB MAN_MD_FILES "${MAN_SRC_DIR}/*.md")

# Prepare list of generated manpage files
set(GENERATED_MANPAGES)

foreach(MD_FILE ${MAN_MD_FILES})
    get_filename_component(FILENAME ${MD_FILE} NAME)
    string(REPLACE ".md" "" MANPAGE_NAME ${FILENAME})
    set(OUT_FILE ${MAN_OUT_DIR}/${MANPAGE_NAME})

    add_custom_command(
            OUTPUT ${OUT_FILE}
            DEPENDS ${MD_FILE}
            COMMAND pandoc --standalone --to=man ${MD_FILE} -o ${OUT_FILE}
            COMMENT "Generating manpage: ${MANPAGE_NAME}"
    )

    list(APPEND GENERATED_MANPAGES ${OUT_FILE})
endforeach()

add_custom_target(generate_manpage ALL
        DEPENDS ${GENERATED_MANPAGES}
)
