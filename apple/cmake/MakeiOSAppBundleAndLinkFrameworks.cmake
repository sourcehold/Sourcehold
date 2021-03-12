set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
set(DATA_DIR_PATH ${CMAKE_SOURCE_DIR}/data)
  
set(BUNDLE_DIR_PATH $<TARGET_FILE_DIR:${PROJECT_NAME}>)
set(RESOURCES_DIR_PATH ${CMAKE_SOURCE_DIR}/apple/iOS/Resources)
set(INFO_PLIST_BUNDLE_DIR_PATH ${BUNDLE_DIR_PATH}/Info.plist)
set(ACTOOL_GENERATED_PLIST_PATH ${CMAKE_BINARY_DIR}/assetcatalog_generated_info.plist)
  
set_target_properties(${PROJECT_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.sourcehold.\${PRODUCT_NAME:identifier}"
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
)
  
if(EXISTS ${DATA_DIR_PATH})
  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_DIR_PATH} ${BUNDLE_DIR_PATH}/data
  )
else()
  message("'data' directroy does not exist. Skipping copy to app bundle.")
endif()
  
# All storyboards in the Base.lproj directory
set(STORYBOARDS ${RESOURCES_DIR_PATH}/Base.lproj/*.storyboard)

# The output folder for compiled storyboards
set(STORYBOARD_OUT_DIR_PATH ${BUNDLE_DIR_PATH}/Base.lproj)

# Compile all storyboards and add them to app bundle
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND mkdir -p ${STORYBOARD_OUT_DIR_PATH}
  COMMAND for storyboard_path in ${STORYBOARDS}\; do ibtool $storyboard_path --compilation-directory ${STORYBOARD_OUT_DIR_PATH}\; done
)

# Compile main assets catalog and app icon
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND xcrun actool Assets.xcassets --compile ${BUNDLE_DIR_PATH} --compress-pngs --app-icon AppIcon --output-partial-info-plist ${ACTOOL_GENERATED_PLIST_PATH} --platform iphoneos --minimum-deployment-target ${CMAKE_OSX_DEPLOYMENT_TARGET}
  WORKING_DIRECTORY ${RESOURCES_DIR_PATH}
)
  
# Update app Info.plist with necessary values
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  # Specify that app is universal and can be run on iPhone (1) and iPad (2). By default cmake set this value to 1
  # and thus we get scaled iPhone app on iPad.
  COMMAND plutil -replace UIDeviceFamily -json '[1, 2]' ${INFO_PLIST_BUNDLE_DIR_PATH}
  # We do not want to participate in Slide Over and Split View, thus we have to set this flag to YES.
  COMMAND plutil -replace UIRequiresFullScreen -bool YES ${INFO_PLIST_BUNDLE_DIR_PATH}
  COMMAND plutil -replace UILaunchStoryboardName -string LaunchScreen ${INFO_PLIST_BUNDLE_DIR_PATH}
  # Set arm64 device capability in order to filter ancient devices and avoid 32-bit architectures support.
  COMMAND plutil -replace UIRequiredDeviceCapabilities -json '[\"arm64\"]' ${INFO_PLIST_BUNDLE_DIR_PATH}
  # This values are necessary to show app icon
  COMMAND plutil -replace CFBundleIcons -json `plutil -extract CFBundleIcons json -o - -- ${ACTOOL_GENERATED_PLIST_PATH}` ${INFO_PLIST_BUNDLE_DIR_PATH}
  COMMAND plutil -replace CFBundleIcons~ipad -json `plutil -extract CFBundleIcons~ipad json -o - -- ${ACTOOL_GENERATED_PLIST_PATH}` ${INFO_PLIST_BUNDLE_DIR_PATH}
)
  
# Remove unnecessary temporary files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND rm ${ACTOOL_GENERATED_PLIST_PATH}
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
