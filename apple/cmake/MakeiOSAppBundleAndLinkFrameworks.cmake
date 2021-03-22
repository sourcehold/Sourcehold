set(DATA_DIR_PATH ${CMAKE_SOURCE_DIR}/data)
set(BUNDLE_DIR_PATH $<TARGET_FILE_DIR:${PROJECT_NAME}>)
set(RESOURCES_DIR_PATH ${CMAKE_SOURCE_DIR}/apple/iOS/Resources)
set(TEMPLATE_INFO_PLIST_PATH "${CMAKE_SOURCE_DIR}/apple/iOS/Supporting Files/Info.plist")
set(INFO_PLIST_BUNDLE_DIR_PATH ${BUNDLE_DIR_PATH}/Info.plist)
set(ACTOOL_GENERATED_PLIST_PATH ${CMAKE_BINARY_DIR}/assetcatalog_generated_info.plist)
set(BUNDLE_IDENTIFIER "com.sourcehold.\${PRODUCT_NAME:identifier}")
  
set(APP_RESOURCES
  ${RESOURCES_DIR_PATH}/Base.lproj/LaunchScreen.storyboard
  ${RESOURCES_DIR_PATH}/Assets.xcassets
)
 
if(EXISTS ${DATA_DIR_PATH})
  set(APP_RESOURCES ${APP_RESOURCES} ${DATA_DIR_PATH})
  source_group("Resources" FILES ${DATA_DIR_PATH})
else()
  message(WARNING "'data' directroy does not exist. Skipping adding to resources.")
endif() # If data dir path exists
 
target_sources(${PROJECT_NAME}
  PRIVATE ${APP_RESOURCES}
)
  
set_target_properties(${PROJECT_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_INFO_PLIST ${TEMPLATE_INFO_PLIST_PATH}
  XCODE_ATTRIBUTE_PRODUCT_NAME ${PROJECT_NAME}
  XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
  XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${BUNDLE_IDENTIFIER}
  # Specify that app is universal and can be run on iPhone (1) and iPad (2). By default cmake set this value to 1
  # and thus we get scaled iPhone app on iPad.
  XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
  XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon
  RESOURCE "${APP_RESOURCES}"
)
  
# Update app Info.plist with project name and version which are set using cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND plutil -replace CFBundleExecutable -string ${PROJECT_NAME} ${INFO_PLIST_BUNDLE_DIR_PATH}
  COMMAND plutil -replace CFBundleIdentifier -string ${BUNDLE_IDENTIFIER} ${INFO_PLIST_BUNDLE_DIR_PATH}
  COMMAND plutil -replace CFBundleLongVersionString -string ${PROJECT_VERSION} ${INFO_PLIST_BUNDLE_DIR_PATH}
  COMMAND plutil -replace CFBundleShortVersionString -string ${PROJECT_VERSION} ${INFO_PLIST_BUNDLE_DIR_PATH}
)
  
target_link_libraries(${PROJECT_NAME}
  PRIVATE iconv
  PRIVATE bz2
  PRIVATE z
  PRIVATE "-framework Foundation"
  PRIVATE "-framework AVFoundation"
  PRIVATE "-framework AudioToolbox"
  PRIVATE "-framework CoreAudio"
  PRIVATE "-framework CoreGraphics"
  PRIVATE "-framework CoreHaptics"
  PRIVATE "-framework CoreMedia"
  PRIVATE "-framework CoreMotion"
  PRIVATE "-framework CoreVideo"
  PRIVATE "-framework GameController"
  PRIVATE "-framework Metal"
  PRIVATE "-framework OpenGLES"
  PRIVATE "-framework Security"
  PRIVATE "-framework QuartzCore"
  PRIVATE "-framework UIKit"
  PRIVATE "-framework VideoToolbox"
)
