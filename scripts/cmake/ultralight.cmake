macro(add_ultralight)
    target_include_directories(${PROJECT_NAME} PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ultralight/include>
		$<INSTALL_INTERFACE:/>
	)
	
	if(WIN32)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/windows/AppCore.dll"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/windows/Ultralight.dll"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/windows/UltralightCore.dll"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/windows/WebCore.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
		target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Ultralight/lib)
	endif()
	
	if(UNIX)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/linux/libAppCore.so"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/linux/libUltralight.so"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/linux/libUltralightCore.so"
			"${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/linux/libWebCore.so"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
		target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Ultralight/bin/linux)
	endif()
endmacro(add_ultralight)