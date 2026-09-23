add_executable(CoreScriptConverter cscmain.cpp)

target_link_libraries(CoreScriptConverter
    $<TARGET_OBJECTS:Base>
    $<TARGET_OBJECTS:Log>
    $<TARGET_OBJECTS:App>
    $<TARGET_OBJECTS:Network>
    $<TARGET_OBJECTS:G3D>
    $<TARGET_OBJECTS:AppDraw>
    $<TARGET_OBJECTS:RbxG3D>
    $<TARGET_OBJECTS:GfxBase>
    $<TARGET_OBJECTS:GfxCore>
    $<TARGET_OBJECTS:GfxRender>
    $<TARGET_OBJECTS:App.BulletPhysics>
)

target_include_directories(CoreScriptConverter PRIVATE src resources)
set_target_properties(CoreScriptConverter PROPERTIES OUTPUT_NAME "ARL.CoreScriptConverter")

add_custom_target(CompileCoreScripts 
  $<TARGET_FILE:CoreScriptConverter> --source "${CMAKE_SOURCE_DIR}/content/scripts" --output "$/App/script/LuaGenCS.inl" 
  DEPENDS CoreScriptConverter
  COMMENT "Compiling CoreScript bytecode"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET CompileCoreScripts POST_BUILD
  COMMENT "Copying runtime files to build directory"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${RUNTIME_FILES}"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)