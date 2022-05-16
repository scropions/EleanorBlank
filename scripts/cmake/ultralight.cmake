macro(add_ultralight)
    target_include_directories(${PROJECT_NAME} PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ultralight/include>
		$<INSTALL_INTERFACE:/>
	)
	target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Ultralight/lib)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/AppCore.dll"
		"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/Ultralight.dll"
		"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/UltralightCore.dll"
		"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/WebCore.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endmacro(add_ultralight)