macro(format_astyle)
    if (MSVC)
		set(ASTYLE_EXECUTABLE ${CMAKE_SOURCE_DIR}/scripts/astyle/AStyle.exe)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		  COMMAND "${ASTYLE_EXECUTABLE}" 
		  --mode=c 
		  --indent=spaces=4 
		  --style=allman --max-code-length=120 --keep-one-line-blocks 
		  --recursive "\"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp,*.h,*.c\""
		)
    endif(MSVC)
endmacro(format_astyle)