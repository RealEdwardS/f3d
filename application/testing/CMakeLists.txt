include(f3dTest)

include(tests.native.cmake)
include(tests.features.cmake)
include(tests.backends.cmake)
include(tests.custom.cmake)
include(tests.home.cmake)
include(tests.screenshot.cmake)
include(tests.gaussian.cmake)
include(tests.interaction.cmake)
include(tests.plugins.cmake)

if(F3D_PLUGIN_BUILD_ALEMBIC)
  include(tests.alembic.cmake)
endif()

if(F3D_PLUGIN_BUILD_ASSIMP)
  include(tests.assimp.cmake)
endif()

if(F3D_PLUGIN_BUILD_DRACO)
  include(tests.draco.cmake)
endif()

if(F3D_PLUGIN_BUILD_HDF)
  include(tests.hdf.cmake)
endif()

if(F3D_PLUGIN_BUILD_WEBIFC)
  include(tests.webifc.cmake)
endif()

if(F3D_PLUGIN_BUILD_OCCT)
  include(tests.occt.cmake)
endif()

if(F3D_PLUGIN_BUILD_USD)
  include(tests.usd.cmake)
endif()

if(F3D_PLUGIN_BUILD_VDB)
  include(tests.vdb.cmake)
endif()

if(F3D_PLUGIN_BUILD_PDAL)
  include(tests.pdal.cmake)
endif()

f3d_test(NAME TestFinalShaderNegative DATA cow.vtp ARGS --final-shader "vec4 pixel(vec2 uv){return vec4(vec3(1.0) - texture(source, uv).rgb, 1.0)\\\\\\\;}")
f3d_test(NAME TestFinalShaderNegativeFileName DATA cow.vtp ARGS --filename --final-shader "vec4 pixel(vec2 uv){return vec4(vec3(1.0) - texture(source, uv).rgb, 1.0)\\\\\\\;}" UI)
f3d_test(NAME TestFinalShaderUndefined DATA cow.vtp ARGS --final-shader "undefined" REGEXP "Final shader must define a function" NO_BASELINE)
f3d_test(NAME TestFinalShaderCompilationFailure DATA cow.vtp ARGS --final-shader "vec4 pixel(vec2 uv){}" --verbose REGEXP " build the shader program" NO_BASELINE)
f3d_test(NAME TestFinalShaderTimeUniform DATA cow.vtp ARGS --final-shader "vec4 pixel(vec2 uv){return vec4(texture(source, uv).rgb * (0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4))), 1.0)\\\\\\\;}")

## Interaction Tests
# Test hotkeys
f3d_test(NAME TestInteractionPostFX DATA cow.vtp INTERACTION TONE_MAPPING LONG_TIMEOUT) #PQAAAAT
# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  f3d_test(NAME TestInteractionActors DATA cow.vtp INTERACTION UI) #EXGMN
endif()
f3d_test(NAME TestInteractionTimer DATA cow.vtp NO_BASELINE INTERACTION UI LONG_TIMEOUT) #Z
f3d_test(NAME TestInteractionTrackball DATA cow.vtp ARGS --interaction-trackball INTERACTION LONG_TIMEOUT) #Mouse movements;
f3d_test(NAME TestInteractionTrackballBind DATA cow.vtp INTERACTION LONG_TIMEOUT) #K#Mouse movements;
f3d_test(NAME TestInteractionCycleCell DATA waveletArrays.vti INTERACTION LONG_TIMEOUT) #VCCC
f3d_test(NAME TestInteractionCycleComp DATA dragon.vtu INTERACTION) #SYYYY
f3d_test(NAME TestInteractionCycleScalars DATA dragon.vtu INTERACTION) #BSSSS
f3d_test(NAME TestInteractionCycleCellInvalidIndex DATA waveletArrays.vti INTERACTION) #SSC
f3d_test(NAME TestInteractionCycleBlending DATA suzanne.ply ARGS --opacity=0.8 INTERACTION LONG_TIMEOUT) #PPPPP # Cycle to ddp
f3d_test(NAME TestInteractionVolumeCycle DATA waveletArrays.vti ARGS INTERACTION) #VSS
f3d_test(NAME TestInteractionVolumeAfterColoring DATA waveletArrays.vti ARGS INTERACTION) #SSSV
f3d_test(NAME TestInteractionVolumeInverse DATA HeadMRVolume.mhd ARGS --camera-position=127.5,-400,127.5 --camera-view-up=0,0,1 INTERACTION THRESHOLD 0.05)#VI #Small rendering differences due to volume rendering
f3d_test(NAME TestInteractionMultiFileVolume DATA multi ARGS --multi-file-mode=all INTERACTION) #SSVB
f3d_test(NAME TestInteractionPointCloud DATA pointsCloud.vtp ARGS --point-sprites-size=20 INTERACTION) #O
f3d_test(NAME TestInteractionDirectory DATA mb INTERACTION ARGS --scalar-coloring) #Right;Right;Right;Left;Up;
f3d_test(NAME TestInteractionDirectoryLoop DATA mb/recursive INTERACTION ARGS --scalar-coloring --filename UI) #Left;Left;Left;Left;Left;
f3d_test(NAME TestInteractionDirectoryEmpty DATA mb INTERACTION NO_DATA_FORCE_RENDER UI) #Right;Right;Right;
f3d_test(NAME TestInteractionDirectoryEmptyVerbose DATA mb ARGS --verbose NO_BASELINE INTERACTION REGEXP "is not a file of a supported file format") #Right;Right;Right;HMCSY
f3d_test(NAME TestInteractionAnimationNotStopped DATA InterpolationTest.glb NO_BASELINE INTERACTION)#Space;
f3d_test(NAME TestInteractionResetCamera DATA dragon.vtu INTERACTION LONG_TIMEOUT)#MouseMovements;Return;
f3d_test(NAME TestInteractionResetCameraWithCameraIndex DATA CameraAnimated.glb ARGS --camera-index=0 INTERACTION)#MouseMovements;Return;
f3d_test(NAME TestInteractionTensorsCycleComp DATA tensors.vti ARGS --scalar-coloring --coloring-component=-2 INTERACTION) #SYYYYYYYYYY
f3d_test(NAME TestInteractionCycleScalarsCompCheck DATA dragon.vtu ARGS -b --scalar-coloring --coloring-component=2 INTERACTION) #S
f3d_test(NAME TestInteractionConfigFileMulti DATA multi CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json INTERACTION UI) #SY;Right;XG;Right;N;Right;Right
# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  f3d_test(NAME TestInteractionConfigFileNoColorBar DATA multi CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json INTERACTION UI) #Right;Right;Left
  f3d_test(NAME TestInteractionConfigFileBindings DATA dragon.vtu CONFIG ${F3D_SOURCE_DIR}/testing/configs/bindings.json INTERACTION UI) #Ctrl+Shift+O;Ctrl+O;Shift+O;O;3
  f3d_test(NAME TestInteractionConfigFileImperative DATA dragon.vtu suzanne.stl ARGS --edges CONFIG ${F3D_SOURCE_DIR}/testing/configs/imperative.json INTERACTION UI) #E;Right
  f3d_test(NAME TestInteractionConfigFileImperativeNoData CONFIG ${F3D_SOURCE_DIR}/testing/configs/imperative.json INTERACTION NO_DATA_FORCE_RENDER UI) #X;Up
  f3d_test(NAME TestInteractionConfigFileOptional DATA zombie.mdl f3d.glb CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json INTERACTION UI) #Right
endif()
f3d_test(NAME TestInteractionCycleVerbose DATA dragon.vtu ARGS --verbose -s NO_BASELINE INTERACTION REGEXP "Not coloring")#SSSSYC
f3d_test(NAME TestInteractionCycleVerboseLevelsUsingBinding DATA dragon.vtu ARGS --verbose=info NO_BASELINE INTERACTION REGEXP "Verbose level changed to: Debug")#Shift+V;Shift+V;Shift+V;Shift+V;Shift+V
f3d_test(NAME TestInteractionVerboseLevelPreservedOnReload DATA dragon.vtu NO_BASELINE INTERACTION REGEXP "Not coloring")#Shift+V;Shift+V;Shift+V;Shift+V;Up
f3d_test(NAME TestInteractionEmptyDrop INTERACTION REGEXP "Drop event without any provided files.")#DropEvent Empty;
f3d_test(NAME TestInteractionCameraUpdate DATA dragon.vtu INTERACTION) #MouseWheel;MouseWheel;MouseWheel;S
f3d_test(NAME TestInteractionFocalPointPickingDefault DATA dragon.vtu INTERACTION LONG_TIMEOUT)
f3d_test(NAME TestInteractionFocalPointPickingShift DATA dragon.vtu INTERACTION LONG_TIMEOUT)
f3d_test(NAME TestInteractionFocalPointPickingPoints DATA pointsCloud.vtp INTERACTION THRESHOLD 0.05) # Threshold needed because sometime a point does not appear
f3d_test(NAME TestInteractionLightIntensity DATA dragon.vtu INTERACTION LONG_TIMEOUT)
f3d_test(NAME TestInteractionMultiFileColoring DATA mb/recursive ARGS --multi-file-mode=all INTERACTION) #SSSB
f3d_test(NAME TestInteractionOpacity DATA dragon.vtu INTERACTION) # Shift+P * 21; Ctrl+P * 5
f3d_test(NAME TestInteractionInitUpVectorOption DATA dragon.vtu ARGS --up=-Y INTERACTION LONG_TIMEOUT) #Small drag left, camera should maintain orientation
# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  f3d_test(NAME TestInteractionDragRotateVertical DATA offset-flat-box.glb ARGS -g -x --up=y INTERACTION UI) # Drag down, should look straight down and avoid gimbal lock
endif()
f3d_test(NAME TestInteractionVerticalDragRotate DATA dragon.vtu ARGS --up=y INTERACTION LONG_TIMEOUT) # 7 (top-view); left click and drag
f3d_test(NAME TestInteractionDynamicUpDirection DATA dragon.vtu INTERACTION) # Ctrl+Z
f3d_test(NAME TestInteractionReload DATA dragon.vtu ARGS -e INTERACTION) #Up;
f3d_test(NAME TestInteractionLoadParentDirectory DATA multi/dragon.vtu ARGS --filename INTERACTION UI) #Down;
f3d_test(NAME TestInteractionEmptyLoadParentDirectory INTERACTION NO_BASELINE REGEXP "No files loaded, no rendering performed") #Down;
f3d_test(NAME TestInteractionMultiFileLoadParentDirectory DATA mb/mb_0_0.vtu ARGS --multi-file-mode=all --filename INTERACTION UI) #Down;
f3d_test(NAME TestInteractionInvertZoom DATA suzanne.ply ARGS --invert-zoom INTERACTION)
f3d_test(NAME TestInteractionCameraHotkeys DATA cow.vtp INTERACTION)
f3d_test(NAME TestInteractionZoomToMouse DATA cow.vtp INTERACTION)
f3d_test(NAME TestInteractionOrthographicProjection DATA cow.vtp INTERACTION) #5;5
f3d_test(NAME TestInteractionZoomToggleOrthographicProjection DATA cow.vtp INTERACTION) #MouseWheel;5;Mousewheelx6;5
f3d_test(NAME TestInteractionRotateCameraMinus90 DATA f3d.glb INTERACTION)
f3d_test(NAME TestInteractionRotateCamera90 DATA f3d.glb INTERACTION)
f3d_test(NAME TestInteractionRollCameraRotation DATA f3d.glb ARGS -g INTERACTION) #1;4;LeftMouse;MouseMovements
f3d_test(NAME TestInteractionElevationCameraRotation DATA f3d.glb ARGS -g INTERACTION THRESHOLD 0.06) #8;8;2 # Threshold is needed because camera seems to move slightly differently sometimes
f3d_test(NAME TestInteractionPanWithShift DATA f3d.glb INTERACTION) #Shift;LeftMouse;MouseMovements
f3d_test(NAME TestInteractionConsoleOpen DATA f3d.glb INTERACTION UI) #Escape
f3d_test(NAME TestInteractionConsoleOpenExit DATA f3d.glb REGEXP "Interactor has been stopped" INTERACTION NO_BASELINE UI) #Escape;exit;Return
f3d_test(NAME TestInteractionConsoleOpenWarningKeyboard DATA f3d.glb ARGS --position=0 INTERACTION UI) #Escape
f3d_test(NAME TestInteractionConsoleOpenWarningClick DATA f3d.glb ARGS --position=0 INTERACTION UI) #Click badge
f3d_test(NAME TestInteractionConsoleToggleGrid DATA f3d.glb INTERACTION UI) #Escape;toggle render.grid.enable;Return;Escape
f3d_test(NAME TestInteractionConsoleInvalidCommand DATA f3d.glb INTERACTION UI) #Escape;foo;Return
f3d_test(NAME TestInteractionConsoleInvalidOption DATA f3d.glb INTERACTION UI) #Escape;toggle foo;Return
f3d_test(NAME TestInteractionConsoleClear DATA dragon.vtu INTERACTION UI) #Escape;e;Escape;printt;BackSpace;_scee;Left;n;Right;_info;Return;clear;Return
f3d_test(NAME TestInteractionConsoleTypingSceneInfo RESOLUTION 400,300 DATA f3d.glb INTERACTION UI) #Escape;e;Escape;printt;BackSpace;_scee;Left;Right;_info;Return
f3d_test(NAME TestInteractionConsoleReload DATA f3d.glb INTERACTION UI) #Escape;reload_current_file_group;Return
f3d_test(NAME TestInteractionConsoleLoadNext DATA f3d.glb cow.vtp INTERACTION UI) #Escape;load_next_file_group;Return
f3d_test(NAME TestInteractionConsoleRender ARGS --coloring-by-cells DATA waveletMaterial.vti INTERACTION UI) #Escape;toggle_volume_rendering;Return
f3d_test(NAME TestInteractionConsoleCamera DATA f3d.glb INTERACTION UI) #Escape;set_camera top;Return
f3d_test(NAME TestInteractionConsoleScrollbar DATA f3d.glb INTERACTION UI) #Escape;a;Enter;Grab scrollbar
f3d_test(NAME TestInteractionConsoleEmptyCommand DATA f3d.glb INTERACTION UI) #Escape;Enter
f3d_test(NAME TestInteractionConsoleOverCheatSheet DATA f3d.glb INTERACTION UI) #h;Escape;Enter
f3d_test(NAME TestInteractionCorrectCameraForVolumeSwitch ARGS --no-config -v DATA dragon.vtu INTERACTION UI) #v

# Cycling through gaussian splatting require > 9.3 to not warn
if(VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240203)
  f3d_test(NAME TestInteractionConfigFileAndCommand DATA multi ARGS -o CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json INTERACTION UI LONG_TIMEOUT) #OOX;Right;N;Right;Right;Right
endif()

# Need SSIM comparison for some reason
if(VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240729)
  f3d_test(NAME TestInteractionConsoleAutoComplete DATA BoxAnimated.gltf INTERACTION UI) #Escape;Tab;to;Tab;Tab;_a;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteNoMatch DATA BoxAnimated.gltf INTERACTION UI) #Escape;zz;Tab
  f3d_test(NAME TestInteractionConsoleAutoCompleteCandidates DATA BoxAnimated.gltf INTERACTION UI) #Escape;Tab;to;Tab
  f3d_test(NAME TestInteractionConsoleAutoCompleteHide DATA BoxAnimated.gltf INTERACTION UI) #Escape;Tab;to;Tab;Esc;Esc;Esc
  f3d_test(NAME TestInteractionConsoleAutoCompleteToggleOptions DATA cow.vtp INTERACTION UI) #Escape;to;Tab;space;m;Tab;p;Tab;e;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteSetOptions ARGS --point-sprites=gaussian DATA cow.vtp INTERACTION UI RESOLUTION 400,300) #Escape;se;Tab;space;m;Tab;p;Tab;t;Tab;Tab;s;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteSetDisplayCompl DATA cow.vtp INTERACTION UI RESOLUTION 400,300) #Escape;se;Tab;space;m;Tab;p;Tab;t;Tab;Tab;Tab
  f3d_test(NAME TestInteractionConsoleAutoCompleteReaderOptions DATA BoxAnimated.gltf INTERACTION UI) #Escape;set_r;Tab;Q;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteColoring DATA BoxAnimated.gltf INTERACTION UI) #Escape;cy;Tab;;c;Tab;a;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteCamera DATA BoxAnimated.gltf INTERACTION UI) #Escape;se;Tab;_c;Tab;i;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteHelp DATA BoxAnimated.gltf INTERACTION UI) #Escape;h;Tab;h;Tab;Enter
  f3d_test(NAME TestInteractionConsoleAutoCompleteTokenizeError DATA BoxAnimated.gltf INTERACTION UI NO_BASELINE REGEXP "unable to tokenize") #Escape;";Tab;

  f3d_test(NAME TestInteractionConsoleHistory DATA BoxAnimated.gltf INTERACTION UI) #Escape;ro;Tab;45;Enter;set_c;Tab;top;Enter;Up;Up;Enter;Up;Up;Up;Up;Down;Down;Down;Down
  f3d_test(NAME TestInteractionConsoleHistoryEmpty DATA BoxAnimated.gltf INTERACTION UI)#Escape;Up;Down
  f3d_test(NAME TestInteractionConsoleHistoryHide DATA BoxAnimated.gltf INTERACTION UI)#Escape;a;Enter;b;Enter;Up;Escape;Escape;Escape;Up

  file(COPY "${F3D_SOURCE_DIR}/testing/data/cow.vtp" DESTINATION "${CMAKE_BINARY_DIR}/Testing/data/") # Capital T, to avoid confusion on Windows and in-sources build
  # First file (`.d`) doesn't exist and is there to test multi args
  f3d_test(NAME TestInteractionConsoleAutoCompleteFilesystem INTERACTION UI) #Escape;ad;Tab;_f;Tab; ;.;Tab;d ..;Tab;T;Tab;i;Tab;d;Tab;Tab;Enter;Esc
  if(WIN32 OR APPLE)
    f3d_test(NAME TestInteractionConsoleAutoCompleteDirCaseInsensitiveUppercaseF INTERACTION UI NO_DATA_FORCE_RENDER WORKING_DIR ${F3D_SOURCE_DIR}/testing/data/AutoCompletion) #Escape;add_files F;Tab
    f3d_test(NAME TestInteractionConsoleAutoCompleteDirCaseInsensitiveLowercaseF INTERACTION UI NO_DATA_FORCE_RENDER WORKING_DIR ${F3D_SOURCE_DIR}/testing/data/AutoCompletion) #Escape;add_files f;Tab
  else()
    f3d_test(NAME TestInteractionConsoleAutoCompleteDirCaseSensitive INTERACTION UI NO_DATA_FORCE_RENDER WORKING_DIR ${F3D_SOURCE_DIR}/testing/data/AutoCompletion) #Escape;add_files F;Tab
  endif()

  if(NOT WIN32)
    # Does not work on windows as it uses hard coded slash
    f3d_test(NAME TestInteractionConsoleAutoCompleteFilesystemError INTERACTION UI NO_BASELINE LONG_TIMEOUT REGEXP "File name too long") #Escape;ad;Tab;_f;Tab; ;Tab;300*a/a;Tab;Esc
  endif()
endif()

if(VTK_VERSION VERSION_GREATER_EQUAL 9.4.20250513)
  f3d_test(NAME TestInteractionAxesGridToggle INTERACTION DATA suzanne.ply THRESHOLD 0.08)#Shift+x
endif()

f3d_test(NAME TestInteractionMinimalConsoleOpen DATA f3d.glb INTERACTION UI) #Colon
f3d_test(NAME TestInteractionMinimalConsoleEscape DATA f3d.glb INTERACTION UI) #Colon;Escape
f3d_test(NAME TestInteractionMinimalConsoleReturn DATA f3d.glb INTERACTION UI) #Colon;Return
f3d_test(NAME TestInteractionMinimalConsoleValidCommand DATA f3d.glb INTERACTION UI) #Colon;set_camera top;Return
f3d_test(NAME TestInteractionMinimalConsoleInvalidCommand DATA f3d.glb INTERACTION UI) #Colon;foo;Return
f3d_test(NAME TestInteractionMinimalConsoleResize DATA f3d.glb INTERACTION UI) #Colon;foo;Return;Colon
f3d_test(NAME TestInteractionMinimalConsoleBadgeRemains DATA f3d.glb INTERACTION UI) #Colon;foo;Return;Colon;Escape
f3d_test(NAME TestInteractionMinimalConsoleOverCheatSheet DATA f3d.glb INTERACTION UI) #h;:
f3d_test(NAME TestInteractionMinimalConsoleOverCheatSheetAndFilename DATA f3d.glb INTERACTION UI) #h;n;:

if (F3D_MODULE_TINYFILEDIALOGS)
  f3d_test(NAME TestInteractionFileOpen INTERACTION NO_BASELINE ENV CTEST_OPEN_DIALOG_FILE=${F3D_SOURCE_DIR}/testing/data/cow.vtp)
endif ()

if(F3D_MODULE_DMON)
  f3d_test(NAME TestInteractionVerboseWatchUnwatch ARGS --watch --verbose DATA cow.vtp multi/dragon.vtu NO_BASELINE INTERACTION REGEXP "Stopped watching") #Right
endif()

# Test camera preserving/resetting when switching files in interaction mode
f3d_test(NAME TestInteractionSwitchFileNoCameraKeeping DATA cow.vtp cowlow.vtp INTERACTION)
f3d_test(NAME TestInteractionSwitchFilePrevCameraKeeping DATA cow.vtp cowlow.vtp INTERACTION)
f3d_test(NAME TestInteractionSwitchFileNextCameraKeeping DATA cow.vtp cowlow.vtp INTERACTION)

# Progress test
f3d_test(NAME TestProgress DATA cow.vtp ARGS --loading-progress NO_BASELINE)
f3d_test(NAME TestProgressScene DATA WaterBottle.glb ARGS --loading-progress NO_BASELINE)
f3d_test(NAME TestInteractionProgressReload DATA cow.vtp ARGS --loading-progress NO_BASELINE INTERACTION) #Up;Up;Up;Up

f3d_test(NAME TestInteractionAnimationCycleAnimation DATA InterpolationTest.glb INTERACTION) #WWWWWWWWWWWW;Space;Space;
f3d_test(NAME TestInteractionAnimationIndexDeprecatedCycleAnimation DATA InterpolationTest.glb ARGS --animation-index=2 INTERACTION) #WWWWWWWWWWWW;Space;Space;
f3d_test(NAME TestInteractionCycleAnimationNoAnimation DATA cow.vtp INTERACTION NO_BASELINE) #W
f3d_test(NAME TestInteractionCycleAnimationOneAnimation DATA f3d.glb ARGS --verbose INTERACTION NO_BASELINE REGEXP "Current animation is: No animation") #W
if(VTK_VERSION VERSION_GREATER_EQUAL 9.4.20250507)
  f3d_test(NAME TestInteractionAnimationCycleAnimationSingle DATA soldier_animations.mdl INTERACTION) #WWWWWWWWWWWW;Space;Space;
endif()
# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  f3d_test(NAME TestInteractionDropFiles ARGS -n INTERACTION_CONFIGURE UI)#X;DropEvent cow.vtp;DropEvent dragon.vtu suzanne.stl;
  f3d_test(NAME TestInteractionDropSameFiles ARGS -x INTERACTION_CONFIGURE UI) #DropEvent cow.vtp;#DropEvent dragon.vtu;#DropEvent cow.vtp#DropEvent cow.vtp;
endif()
f3d_test(NAME TestInteractionMultiFileDrop ARGS --multi-file-mode=all -e INTERACTION_CONFIGURE) #DropEvent mb_1_0.vtp mb_2_0.vtp

# A proper test for this is not possible because of the double quotes
f3d_test(NAME TestInteractionDropFileWithQuotes ARGS -n INTERACTION REGEXP "\"'`Quotes\"'`.stl does not exist" NO_BASELINE)#X;DropEvent "'`Quotes"'`.stl";

if(WIN32)
  # Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
  if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
    # Windows specific drop test, using backslashes
    f3d_test(NAME TestInteractionDropFileBackslashes ARGS -n INTERACTION_CONFIGURE)#X;DropEvent path\to\cow.vtp;
  endif()
endif()

# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  # Interaction HDRI tests
  f3d_test(NAME TestInteractionDropHDRI INTERACTION_CONFIGURE LONG_TIMEOUT UI)#X;DropEvent dragon.vtu;DropEvent palermo.hdr;
  f3d_test(NAME TestInteractionDropHDRIInvert INTERACTION_CONFIGURE LONG_TIMEOUT UI)#X;DropEvent palermo.hdr;DropEvent dragon.vtu;
  f3d_test(NAME TestInteractionDropHDRIMulti INTERACTION_CONFIGURE LONG_TIMEOUT UI)#X;DropEvent dragon.vtu palermo.hdr;
endif()
# Test modified drops, this test rendering is impacted by https://github.com/f3d-app/f3d/issues/1558
# Empty drop is for coverage
f3d_test(NAME TestInteractionDropHDRIModifiers INTERACTION_CONFIGURE LONG_TIMEOUT)#CTRL+DropEvent f3d.tif;SHIFT+DropEvent;SHIFT+DropEvent palermo.tif;SYYYY

# Test interactive animation and dropping HDRI
f3d_test(NAME TestInteractionAnimationDropHDRI DATA InterpolationTest.glb ARGS --animation-indices=-1 --animation-progress INTERACTION_CONFIGURE LONG_TIMEOUT)#Space;DropEvent palermo.hdr;Space;

if(F3D_MODULE_EXR)
  # Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
  if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
    f3d_test(NAME TestInteractionDropHDRIExr INTERACTION_CONFIGURE LONG_TIMEOUT)#X;DropEvent kloofendal.exr;DropEvent dragon.vtu;
  endif()
endif()

## Tests to increase coverage
# Output option test
f3d_test(NAME TestOutput DATA cow.vtp NO_BASELINE)
f3d_test(NAME TestOutputOutput DATA cow.vtp ARGS --reference=${CMAKE_BINARY_DIR}/Testing/Temporary/TestOutput.png DEPENDS TestOutput NO_BASELINE)
f3d_test(NAME TestUnsupportedInputOutput DATA unsupportedFile.dummy REGEXP "No files loaded, no rendering performed" NO_BASELINE)
f3d_test(NAME TestOutputNoBackground DATA cow.vtp ARGS --no-background NO_BASELINE)

# Basic record and play test
f3d_test(NAME TestInteractionRecord DATA cow.vtp ARGS --interaction-test-record=${CMAKE_BINARY_DIR}/Testing/Temporary/TestInteractionRecord.log NO_BASELINE)
f3d_test(NAME TestInteractionPlay DATA cow.vtp ARGS --interaction-test-play=${CMAKE_BINARY_DIR}/Testing/Temporary/TestInteractionRecord.log DEPENDS TestInteractionRecord NO_BASELINE)

# Command Script Test
f3d_test(NAME TestCommandScriptBasic DATA dragon.vtu SCRIPT TestCommandScriptBasic.txt) # roll_camera 90;toggle ui.scalar_bar;print_scene_info;increase_light_intensity
f3d_test(NAME TestCommandScriptElevation DATA dragon.vtu SCRIPT TestCommandScriptElevation.txt) # elevation_camera 90;toggle ui.scalar_bar;print_scene_info;increase_light_intensity
f3d_test(NAME TestCommandScriptAzimuth DATA dragon.vtu SCRIPT TestCommandScriptAzimuth.txt) # azimuth_camera 90;toggle ui.scalar_bar;print_scene_info;increase_light_intensity
f3d_test(NAME TestCommandScriptInvalidCommand DATA dragon.vtu SCRIPT TestCommandScriptInvalid.txt REGEXP "Command: \"INVALID_COMMAND_1\" is not recognized, ignoring" NO_BASELINE) # INVALID_COMMAND_1
f3d_test(NAME TestCommandScriptMissingFile DATA dragon.vtu SCRIPT TestCommandScriptMissingFile.txt REGEXP "Unable to open command script file" NO_BASELINE)
f3d_test(NAME TestCommandScriptPrintScene DATA dragon.vtu SCRIPT TestCommandScriptPrintScene.txt REGEXP "Camera position: 2.23745, 3.83305, 507.598" NO_BASELINE) # print_scene_info
f3d_test(NAME TestCommandScriptPrintColoring DATA dragon.vtu SCRIPT TestCommandScriptPrintColoring.txt REGEXP "Not coloring" NO_BASELINE) # print_coloring_info
f3d_test(NAME TestCommandScriptPrintMesh DATA dragon.vtu SCRIPT TestCommandScriptPrintMesh.txt REGEXP "Number of points: 13268" NO_BASELINE) # print_mesh_info
f3d_test(NAME TestCommandScriptPrintOptions DATA dragon.vtu SCRIPT TestCommandScriptPrintOptions.txt REGEXP "interactor.invert_zoom: false" NO_BASELINE) # print_options_info
f3d_test(NAME TestCommandScriptAlias DATA dragon.vtu SCRIPT TestCommandScriptAlias.txt --reference=${F3D_SOURCE_DIR}/testing/baselines/TestCommandScriptAlias.png) # alias myrotate roll_camera 90;myrotate
f3d_test(NAME TestCommandScriptReset DATA dragon.vtu suzanne.stl ARGS --edges SCRIPT TestCommandScriptReset.txt) # reset render.show_edges; load_next_file_group;
f3d_test(NAME TestCommandScriptParseOptionalBoolExtraArg DATA dragon.vtu SCRIPT TestCommandScriptParseOptionalBoolExtraArg.txt REGEXP "Command: load_previous_file_group takes at most 1 argument, got 2 arguments instead." NO_BASELINE) # load_previous_file_group true extra
f3d_test(NAME TestCommandScriptRemoveCurrentFileGroup DATA cow.vtp dragon.vtu SCRIPT TestCommandScriptRemoveCurrentFileGroup.txt) # remove_current_file_group
f3d_test(NAME TestCommandScriptRemoveFileGroups DATA dragon.vtu SCRIPT TestCommandScriptRemoveFileGroups.txt NO_DATA_FORCE_RENDER UI) # remove_file_groups
f3d_test(NAME TestCommandScriptInvalidReaderOptions DATA dragon.vtu SCRIPT TestCommandScriptInvalidReaderOptions.txt REGEXP "point to an inexistent option, ignoring" NO_BASELINE) # set_reader_option invalid value
f3d_test(NAME TestCommandScriptHelp DATA dragon.vtu SCRIPT TestCommandScriptHelp.txt REGEX "set a libf3d option" NO_BASELINE) # help set
f3d_test(NAME TestCommandScriptHelpInvalid DATA dragon.vtu SCRIPT TestCommandScriptHelpInvalid.txt REGEX "is not a recognized command" NO_BASELINE) # help invalid

# Window position test
f3d_test(NAME TestPosition DATA dragon.vtu ARGS --position=100,100 NO_BASELINE)
f3d_test(NAME TestInvalidPosition DATA dragon.vtu ARGS --position=100,100,300 REGEXP "Provided position could not be applied" NO_BASELINE)
f3d_test(NAME TestInvalidResolution DATA dragon.vtu RESOLUTION 800 REGEXP "Provided resolution could not be applied" NO_BASELINE)

# Simple verbosity test
f3d_test(NAME TestVerbose DATA dragon.vtu ARGS --verbose REGEXP "Number of points: 13268\nNumber of cells: 26532" NO_BASELINE)

# Test verbose quiet
f3d_test(NAME TestVerboseQuiet DATA mb.vtm ARGS --verbose=quiet REGEXP_FAIL "A non data set block was ignored while reading a multiblock." NO_BASELINE)

# Test verbose error
f3d_test(NAME TestVerboseError DATA mb.vtm ARGS --verbose=error REGEXP_FAIL "A non data set block was ignored while reading a multiblock." NO_BASELINE)

# Test verbose warning, rely on the log::info about image comparison
f3d_test(NAME TestVerboseWarning DATA cow.vtp ARGS --verbose=warning REGEXP_FAIL "Image comparison success with an error difference of")

# Test verbose debug
f3d_test(NAME TestVerboseDebug DATA dragon.vtu ARGS --verbose REGEXP "Number of points: 13268\nNumber of cells: 26532" NO_BASELINE)

# Test verbose invalid verbose value
f3d_test(NAME TestVerboseInvalid DATA dragon.vtu ARGS --verbose=invalid REGEXP "Unrecognized verbose level" NO_BASELINE)

# Unknown scalar array verbosity test
f3d_test(NAME TestVerboseWrongArray DATA dragon.vtu ARGS -s --coloring-array=dummy --verbose REGEXP "Unknown scalar array: \"dummy\"" NO_BASELINE)

# Default scalar array verbosity test
f3d_test(NAME TestVerboseDefaultScalar DATA HeadMRVolume.mhd ARGS -s --verbose REGEXP "Coloring using point array named MetaImage, Magnitude" NO_BASELINE)

# Volume array verbosity test
f3d_test(NAME TestVerboseVolume DATA HeadMRVolume.mhd ARGS -v --verbose REGEXP "Coloring using point array named MetaImage .forced., Magnitude" NO_BASELINE)

# Incorrect component test
f3d_test(NAME TestIncorrectComponent DATA dragon.vtu ARGS -s --coloring-component=4 REGEXP "Invalid component index: 4" NO_BASELINE)

# Incorrect volume coloring with multi file
f3d_test(NAME TestIncorrectMultiFileVolume DATA multi ARGS -sv --coloring-array=Normals --multi-file-mode=all REGEXP "Cannot find the array \"Normals\" to display volume with" NO_BASELINE)

# Incorrect color map
f3d_test(NAME TestIncorrectColormap DATA IM-0001-1983.dcm ARGS --scalar-coloring --roughness=1 --colormap=0,1,0,0,1,0,1 REGEXP "Incorrect number of tokens in provided colormap" NO_BASELINE)

# Test opening a directory
f3d_test(NAME TestVerboseDirectory DATA mb REGEXP "mb_0_0.vtu" NO_RENDER)

# Test opening multiple file and rendering only one
f3d_test(NAME TestVerboseMultiFileRender DATA mb REGEXP "An output image was saved using a single 3D file, other provided 3D files were ignored." NO_BASELINE)
f3d_test(NAME TestVerboseMultiFileCompare DATA mb REGEXP "Image comparison was performed using a single 3D file, other provided 3D files were ignored.")

# Test Animation invalid code paths
f3d_test(NAME TestAnimationIndicesWarningRange DATA InterpolationTest.glb ARGS --animation-indices=48 NO_BASELINE REGEXP "Specified animation index: 48 is not in range")
f3d_test(NAME TestAnimationIndicesWarningNone DATA cow.vtp ARGS --animation-indices=1 REGEXP "Animation indices have been specified but there are no animation available" NO_BASELINE)
f3d_test(NAME TestAnimationIndicesWarningAllAnimations DATA InterpolationTest.glb ARGS --animation-indices=-1,2,3 REGEXP "Multiple animation indices have been specified include a negative one, all animations will be selected" NO_BASELINE)
f3d_test(NAME TestVerboseAnimationNoAnimationTime DATA cow.vtp ARGS --animation-time=2 --verbose REGEXP "No animation available, cannot load a specific animation time" NO_BASELINE)

if(VTK_VERSION VERSION_GREATER_EQUAL 9.4.20250507)
  f3d_test(NAME TestAnimationIndicesWarningSingle DATA soldier_animations.mdl ARGS --animation-indices=1,2 REGEXP "Multiple animation indices have been specified but currently loaded files may not support enabling multiple animations" NO_BASELINE)
  if(F3D_PLUGIN_BUILD_HDF)
    f3d_test(NAME TestAnimationIndicesWarningUniqueNonZero DATA blob.vtkhdf ARGS --animation-indices=1 --load-plugins=hdf REGEXP "Non-zero or multiple animation indices have been specified but currently loaded file does not support it." NO_BASELINE)
    f3d_test(NAME TestAnimationIndicesWarningUniqueMulti DATA blob.vtkhdf ARGS --animation-indices=0,2 --load-plugins=hdf REGEXP "Non-zero or multiple animation indices have been specified but currently loaded file does not support it." NO_BASELINE)
  endif()
endif()

# Test Grid verbose output
f3d_test(NAME TestVerboseGrid DATA suzanne.ply ARGS -g --verbose REGEXP "Grid origin set to" NO_BASELINE)

# Test bounding box no render output
f3d_test(NAME TestNoRenderBBox DATA suzanne.ply NO_RENDER REGEXP "Scene bounding box: -1.32819 ≤ x ≤ 1.32819, -0.971822 ≤ y ≤ 0.939236, -0.778266 ≤ z ≤ 0.822441")

# Test Scalars coloring verbose output
f3d_test(NAME TestVerboseScalars DATA suzanne.ply ARGS -s --verbose REGEXP "Coloring using point array named Normals, Magnitude." NO_BASELINE)

# Test direct scalars surface rendering with a 9 comp array
f3d_test(NAME TestTensorsDirect DATA tensors.vti ARGS -s --coloring-array=tensors1 --coloring-component=-2 REGEXP "Direct scalars rendering not supported by array with more than 4 components" NO_BASELINE)

# Test direct scalars volume rendering with a 9 comp array
f3d_test(NAME TestTensorsVolumeDirect DATA tensors.vti ARGS -v -s --coloring-array=tensors1 --coloring-component=-2 REGEXP "Direct scalars rendering not supported by array with more than 4 components" NO_BASELINE)

# Test volume rendering without any array
f3d_test(NAME TestVerboseVolumeNoArray DATA cow.vtp ARGS -v REGEXP "Cannot use volume with this data" NO_BASELINE)

# Test scalar rendering without any array
f3d_test(NAME TestVerboseNoArray DATA cow.vtp ARGS -s --verbose=debug REGEXP "No array to color with" NO_BASELINE)

# Test invalid scalar range
f3d_test(NAME TestInvalidScalarsRange DATA suzanne.ply ARGS -s --coloring-array=Normals --coloring-component=1 --coloring-range=0,1,2 REGEXP "Invalid scalar range provided, using automatic range" NO_BASELINE)

# Test invalid backface type
f3d_test(NAME TestInvalidBackface DATA backface.vtp ARGS --backface-type=invalid REGEXP "is not a valid backface type, assuming it is not set" NO_BASELINE)

# Test non existent file, do not create nonExistentFile.vtp
f3d_test(NAME TestVerboseNonExistentFile DATA nonExistentFile.vtp REGEXP ".*nonExistentFile.vtp does not exist" NO_RENDER)

# Test non existent font file, do not create nonExistentFile.ttf
f3d_test(NAME TestVerboseNonExistentFont DATA suzanne.ply ARGS -n --font-file=${F3D_SOURCE_DIR}/testing/data/nonExistentFile.ttf REGEXP "Cannot find \".*nonExistentFile.ttf\" font file" NO_BASELINE)

# Test scalar rendering without any array
f3d_test(NAME TestVerboseOptionsCLI ARGS -x --verbose=debug REGEXP "'axis' = '1' from CLI options" NO_BASELINE)
f3d_test(NAME TestVerboseOptionsConfig ARGS --verbose=debug CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json REGEXP "'ui.axis' = 'true'" NO_BASELINE)

# Test quiet with a non existent file
f3d_test(NAME TestQuietNonExistentFile DATA nonExistentFile.vtp ARGS --verbose=quiet --no-render REGEXP_FAIL "File .*nonExistentFile.vtp does not exist")

# Test non supported file, do not add support for .dummy file.
f3d_test(NAME TestUnsupportedFileText DATA unsupportedFile.dummy ARGS --filename REGEXP ".*unsupportedFile.dummy is not a file of a supported file format" NO_RENDER)

# Test non existent texture, do not add a dummy.png
f3d_test(NAME TestNonExistentTexture DATA cow.vtp ARGS --texture-material=${F3D_SOURCE_DIR}/testing/data/dummy.png REGEXP "Texture file does not exist" NO_BASELINE)

if(VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240707)
  # Test invalid file
  f3d_test(NAME TestInvalidFile DATA duck_invalid.gltf REGEXP "failed to load scene" NO_BASELINE)

  # Test invalid animation
  f3d_test(NAME TestAnimationInvalid DATA BoxAnimated_invalid_animation.gltf ARGS --animation-time 1 REGEXP "Could not load time value: 1" NO_BASELINE)
endif ()

# Test invalid texture
f3d_test(NAME TestInvalidTexture DATA cow.vtp ARGS --texture-material=${F3D_SOURCE_DIR}/testing/data/invalid.png REGEXP "Cannot open texture file" NO_BASELINE)

# Test invalid color
f3d_test(NAME TestInvalidColor DATA cow.vtp ARGS --color=0,0,0,1 REGEXP "Provided vector does not have the right size" NO_BASELINE)

# Test non existent interaction file, do not add a TestNonExistentInteraction
f3d_test(NAME TestNonExistentInteraction DATA cow.vtp INTERACTION REGEXP "Interaction record file to play does not exist" NO_BASELINE)

# Test unknown options, do not add a --colour option
f3d_test(NAME TestUnknownOptionVerbose ARGS --colour=a=b REGEXP "Did you mean '--color=a=b'?")
f3d_test(NAME TestUnknownOptionExitCode ARGS --colour=1,0,0 WILL_FAIL)
f3d_test(NAME TestUnknownAppOptionVerbose ARGS --revolution=17,32 REGEXP "Did you mean '--resolution=17,32'?")
f3d_test(NAME TestUnknownBooleanOptionVerbose ARGS --helpo REGEXP "Did you mean '--help'?")
f3d_test(NAME TestUnknownInputOptionVerbose ARGS --inprut=file REGEXP "Did you mean '--input=file'?")

# Test app option error code path
f3d_test(NAME TestAppOptionError ARGS --animation-time=invalid REGEXP "Could not parse 'invalid' into 'animation-time' option")
f3d_test(NAME TestAppOptionNoConfigError ARGS --no-config=invalid CONFIG invalid REGEXP "Could not parse 'invalid' into 'no-config' option, assuming false")

# Test non-existent config filepath, do not add a dummy.json
f3d_test(NAME TestNonExistentConfigFilePath DATA cow.vtp CONFIG "${F3D_SOURCE_DIR}/testing/configs/dummy.json" REGEXP "Configuration file does not exist" NO_BASELINE)

# Test non-existent config filename, do not add a dummy.json or dummy.d
f3d_test(NAME TestNonExistentConfigFileStem DATA cow.vtp CONFIG "dummy" REGEXP "Configuration file for \"dummy\" could not be found" NO_BASELINE)

# Test invalid config file
f3d_test(NAME TestInvalidConfigFile DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/invalid.json REGEXP "Unable to parse the configuration file" NO_BASELINE)

# Test invalid reset key in config file
f3d_test(NAME TestInvalidResetOptions DATA cow.vtp ARGS --reset= REGEXP "Invalid option: 'reset' must be followed by a valid option name, ignoring entry" NO_BASELINE)

# Test invalid multifile mode
f3d_test(NAME TestInvalidMultiFileMode DATA mb/recursive ARGS --multi-file-mode=add REGEXP "Unrecognized multi-file-mode: add. Assuming \"single\" mode." NO_BASELINE)

# Test unnamed cameras/animation
f3d_test(NAME TestVerboseUnnamedCamera DATA Cameras.gltf ARGS --verbose REGEXP "1: unnamed_1" NO_BASELINE)
f3d_test(NAME TestVerboseUnnamedAnimation DATA BoxAnimated.gltf ARGS --verbose REGEXP "0: unnamed_0" NO_BASELINE)

# Test invalid value in config file
f3d_test(NAME TestConfigFileInvalidOptions DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/invalid_options.json REGEXP "Error processing config file" NO_BASELINE)

# Test invalid value in config file
f3d_test(NAME TestConfigFileNoOptions DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/no_options.json REGEXP "does not contains options" NO_BASELINE)

# Test update interaction verbose
f3d_test(NAME TestConfigFileBindingsVerbose DATA dragon.vtu ARGS --verbose CONFIG ${F3D_SOURCE_DIR}/testing/configs/bindings.json REGEXP "'Shift.O' : '`toggle model.point_sprites.enable` '" NO_BASELINE)

# Test list-bindings display with config file
f3d_test(NAME TestConfigFileBindingsList ARGS --list-bindings CONFIG ${F3D_SOURCE_DIR}/testing/configs/bindings.json REGEXP "Ctrl.Shift.O `toggle ui.filename`" NO_BASELINE)
f3d_test(NAME TestConfigFileBindingsListData DATA dragon.vtu ARGS --list-bindings CONFIG ${F3D_SOURCE_DIR}/testing/configs/bindings.json REGEXP "Any.3        `roll_camera 90`" NO_BASELINE)

# Test invalid value in config file
f3d_test(NAME TestConfigFileInvalidValue DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/invalid_value.json REGEXP "must be a string, a boolean or a number" NO_BASELINE)

# Test non-parsable value in config file
f3d_test(NAME TestConfigFileNonParsableValue DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/nonparsable_value.json REGEXP "Cannot parse nonparsable into an int" NO_BASELINE)

# Test inexistent key in config file
f3d_test(NAME TestConfigFileInexistentKey DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/inexistent_key.json REGEXP "does not exists , did you mean 'scene.animation.indices" NO_BASELINE)

# Test quiet in config file
f3d_test(NAME TestConfigFileQuiet DATA nonExistentFile.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/quiet.json REGEXP_FAIL "File .*/testing/data/nonExistentFile.vtp does not exist" NO_BASELINE)

# Test no file with config file
f3d_test(NAME TestNoFileConfigFile CONFIG ${F3D_SOURCE_DIR}/testing/configs/verbose.json ARGS --verbose REGEXP "No files to load provided" NO_BASELINE)

# Test that --no-config overrides --config
f3d_test(NAME TestNoConfigWithConfig DATA f3d.glb ARGS --no-config CONFIG ${F3D_SOURCE_DIR}/testing/configs/complex.json)

# Test that config file search display logs
f3d_test(NAME TestConfigFileLocationVerbose ARGS --verbose CONFIG inexistent.json REGEXP "Candidate config file not found: .*inexistent.json" NO_BASELINE)

# Test invalid match type in config file
f3d_test(NAME TestConfigInvalidMatchType DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/invalid_match_type.json REGEXP "There was an error in the config .*invalid_match_type.json for regex pattern.*" NO_BASELINE)

# Test glob match type but no match expression in config file
f3d_test(NAME TestConfigMatchTypeNoMatch DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/match_type_no_match.json REGEXP "A config block in config file .*match_type_no_match.json has match-type glob but no match expression, using a catch-all regex" NO_BASELINE)

# Test invalid glob expression in config file
f3d_test(NAME TestConfigInvalidGlobExp DATA cow.vtp CONFIG ${F3D_SOURCE_DIR}/testing/configs/invalid_glob_exp.json REGEXP "There was an error in the config .*invalid_glob_exp.json for glob pattern.*" NO_BASELINE)

# Test glob matching
if(WIN32)
  set(GLOBSTAR_EXPR [=[**\\\\*stl]=])
else()
  set(GLOBSTAR_EXPR "**/*stl")
endif()
configure_file("${F3D_SOURCE_DIR}/testing/configs/glob.json.in" "${CMAKE_BINARY_DIR}/glob.json")
# Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
  f3d_test(NAME TestMatchFirstConfigGlobOptionBlock DATA dragon.vtu CONFIG ${CMAKE_BINARY_DIR}/glob.json UI)
endif()
f3d_test(NAME TestMatchSecondConfigGlobOptionBlock DATA suzanne.stl CONFIG ${CMAKE_BINARY_DIR}/glob.json)

# Test exact matching
cmake_path(SET FIRST_EXACT_PATH "${F3D_SOURCE_DIR}/testing/data/suzanne.obj")
cmake_path(NATIVE_PATH FIRST_EXACT_PATH NATIVE_FIRST_EXACT_PATH)
cmake_path(SET SECOND_EXACT_PATH "${F3D_SOURCE_DIR}/testing/data/suzanne.ply")
cmake_path(NATIVE_PATH SECOND_EXACT_PATH NATIVE_SECOND_EXACT_PATH)
if(WIN32)
  string(REPLACE "\\" "\\\\" NATIVE_FIRST_EXACT_PATH "${NATIVE_FIRST_EXACT_PATH}")
  string(REPLACE "\\" "\\\\" NATIVE_SECOND_EXACT_PATH "${NATIVE_SECOND_EXACT_PATH}")
endif()
configure_file("${F3D_SOURCE_DIR}/testing/configs/exact.json.in" "${CMAKE_BINARY_DIR}/exact.json")
f3d_test(NAME TestExactMatch DATA suzanne.obj CONFIG ${CMAKE_BINARY_DIR}/exact.json)

# Test help display
f3d_test(NAME TestHelp ARGS --help REGEXP "Usage:")
f3d_test(NAME TestHelpPositional ARGS --help REGEXP "file1 file2 \.\.\.")

# Test version display
f3d_test(NAME TestVersion ARGS --version REGEXP "Version:")

# Test list-readers display
f3d_test(NAME TestReadersList ARGS --list-readers REGEXP_FAIL "No registered reader found")

# Test invalid component string coverage
f3d_test(NAME TestInteractionInvalidComponent INTERACTION UI DATA cow.vtp ARGS --coloring-component=1 NO_BASELINE) #H

# Test opening invalid file then switching to another file
f3d_test(NAME TestInteractionInvalidFile INTERACTION DATA invalid.vtp cow.vtp ARGS --loading-progress) #Right

# Test multi plugin list-readers
if(F3D_PLUGIN_BUILD_ALEMBIC AND F3D_PLUGIN_BUILD_ASSIMP)
  f3d_test(NAME TestReadersListMultiplePlugins ARGS --list-readers --load-plugins=assimp,alembic NO_BASELINE REGEXP_FAIL "Plugin failed to load")
endif()

if(VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240707)
  f3d_test(NAME TestForceReaderFail DATA suzanne.stl ARGS --force-reader=GLTF NO_BASELINE REGEXP "Some of these files could not be loaded: failed to load scene")
  f3d_test(NAME TestForceReaderPass DATA suzanne.not_supported ARGS --force-reader=STL)
  f3d_test(NAME TestForceReaderInvalid DATA suzanne.stl ARGS --force-reader=INVALID NO_BASELINE REGEXP "Forced reader .* doesn't exist")

  if(F3D_PLUGIN_BUILD_DRACO)
    f3d_test(NAME TestForceReaderGLTFDraco DATA BoxAnimated.gltf ARGS --load-plugins=draco --force-reader=GLTFDraco)
    f3d_test(NAME TestForceReaderGLTFDracoIntoGLTF DATA Box_draco.gltf ARGS --load-plugins=draco --force-reader=GLTF NO_BASELINE REGEXP "failed to load scene")
  endif()
endif()

# Test bindings-list display
f3d_test(NAME TestBindingsList ARGS --list-bindings REGEXP "Any.5        Orthographic Projection")

# Test rendering backends
# For some reason the sanitizer detects leaks because of EGL and OSMesa
f3d_test(NAME TestRenderingBackendAuto DATA cow.vtp RENDERING_BACKEND auto)
if(UNIX AND NOT APPLE AND VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240914 AND F3D_SANITIZER STREQUAL "none")
  if(F3D_TESTING_ENABLE_EGL_TESTS)
    f3d_test(NAME TestRenderingBackendEGL DATA cow.vtp RENDERING_BACKEND egl)
    f3d_test(NAME TestRenderingBackendEGLCheckClass DATA cow.vtp RENDERING_BACKEND egl ARGS --verbose REGEXP "vtkF3DEGLRenderWindow" NO_BASELINE)
    f3d_test(NAME TestRenderingBackendEGLNoInteractiveVerbose RENDERING_BACKEND egl REGEXP "You are using an offscreen configuration, interactive rendering is disabled" NO_BASELINE NO_OUTPUT)
  endif()

  if(F3D_TESTING_ENABLE_OSMESA_TESTS)
    f3d_test(NAME TestRenderingBackendOSMesa DATA cow.vtp RENDERING_BACKEND osmesa)
    f3d_test(NAME TestRenderingBackendOSMesaCheckClass DATA cow.vtp RENDERING_BACKEND osmesa ARGS --verbose REGEXP "vtkOSOpenGLRenderWindow" NO_BASELINE)
    f3d_test(NAME TestRenderingBackendOSMesaNoInteractiveVerbose RENDERING_BACKEND osmesa REGEXP "You are using an offscreen configuration, interactive rendering is disabled" NO_BASELINE NO_OUTPUT)
  endif()

  if(F3D_TESTING_ENABLE_GLX_TESTS)
    f3d_test(NAME TestRenderingBackendGLX DATA cow.vtp RENDERING_BACKEND glx)
    f3d_test(NAME TestRenderingBackendGLXNoDisplay DATA cow.vtp RENDERING_BACKEND glx ENV "DISPLAY=:7" REGEXP "Cannot create a window" NO_BASELINE)
    f3d_test(NAME TestRenderingBackendGLXCheckClass DATA cow.vtp RENDERING_BACKEND glx ARGS --verbose REGEXP "vtkF3DGLXRenderWindow" NO_BASELINE)
    f3d_test(NAME TestRenderingBackendLinuxAutoCheckClass DATA cow.vtp ARGS --verbose REGEXP "vtkF3DGLXRenderWindow" NO_BASELINE)
  endif()

  f3d_test(NAME TestRenderingBackendWGLFailure DATA cow.vtp RENDERING_BACKEND wgl REGEXP "Cannot use a WGL context on this platform" NO_BASELINE)
endif()
if(WIN32)
  f3d_test(NAME TestRenderingBackendWGL DATA cow.vtp RENDERING_BACKEND wgl)
  f3d_test(NAME TestRenderingBackendWGLCheckClass DATA cow.vtp RENDERING_BACKEND wgl ARGS --verbose REGEXP "vtkF3DWGLRenderWindow" NO_BASELINE)
  f3d_test(NAME TestRenderingBackendWindowsAutoCheckClass DATA cow.vtp ARGS --verbose REGEXP "vtkF3DWGLRenderWindow" NO_BASELINE)
  f3d_test(NAME TestRenderingBackendGLXFailure DATA cow.vtp RENDERING_BACKEND glx REGEXP "Cannot use a GLX context on this platform" NO_BASELINE)
endif()
f3d_test(NAME TestRenderingBackendInvalid DATA cow.vtp RENDERING_BACKEND invalid ARGS --verbose REGEXP "rendering-backend value is invalid, falling back to" NO_BASELINE)

# Test that f3d can try to read a system config file
add_test(NAME f3d::TestNoNoConfig COMMAND $<TARGET_FILE:f3d> --no-render)
set_tests_properties(f3d::TestNoNoConfig PROPERTIES TIMEOUT 4)

# Test that we can try loading a plugin without F3D_PLUGINS_PATH being defined
add_test(NAME f3d::TestNoEnvInvalidPlugin COMMAND $<TARGET_FILE:f3d> --load-plugins=invalid --no-render)
set_tests_properties(f3d::TestNoEnvInvalidPlugin PROPERTIES PASS_REGULAR_EXPRESSION "Cannot open the library")

# Test invalid CLI args
add_test(NAME f3d::TestInvalidCLIArgs COMMAND $<TARGET_FILE:f3d> --up)
set_tests_properties(f3d::TestInvalidCLIArgs PROPERTIES PASS_REGULAR_EXPRESSION "Error parsing command line arguments")

# Test that f3d resolution can be controlled from config file
add_test(NAME f3d::TestConfigResolution COMMAND $<TARGET_FILE:f3d> --config=${F3D_SOURCE_DIR}/testing/configs/resolution.json ${F3D_SOURCE_DIR}/testing/data/suzanne.stl --output=${CMAKE_BINARY_DIR}/Testing/Temporary/TestConfigResolution.png --reference=${F3D_SOURCE_DIR}/testing/baselines/TestConfigResolution.png)

# Test filename template with multiple files
add_test(NAME f3d::TestMultiFileFileNameTemplate COMMAND $<TARGET_FILE:f3d> ${F3D_SOURCE_DIR}/testing/data/suzanne.stl ${F3D_SOURCE_DIR}/testing/data/dragon.vtu --output=${CMAKE_BINARY_DIR}/Testing/Temporary/{model.ext}.png --multi-file-mode=all --verbose)
set_tests_properties(f3d::TestMultiFileFileNameTemplate PROPERTIES PASS_REGULAR_EXPRESSION "multi_file.png")

# Test filename template with no files
add_test(NAME f3d::TestNoFileFileNameTemplate COMMAND $<TARGET_FILE:f3d> --output=${CMAKE_BINARY_DIR}/Testing/Temporary/{model.ext}.png --verbose)
set_tests_properties(f3d::TestNoFileFileNameTemplate PROPERTIES PASS_REGULAR_EXPRESSION "no_file.png")
set_tests_properties(f3d::TestNoFileFileNameTemplate PROPERTIES ENVIRONMENT "CTEST_F3D_NO_DATA_FORCE_RENDER=1")

if(NOT WIN32)
  f3d_test(NAME TestHDRINoCachePath ENV "HOME=" DATA dragon.vtu ARGS -f --hdri-file=${F3D_SOURCE_DIR}/testing/data/palermo_park_1k.hdr REGEXP "Cannot cache .* as no cache path has been set." NO_BASELINE LONG_TIMEOUT)

  add_test(NAME f3d::TestHOMEOutput COMMAND $<TARGET_FILE:f3d> ${F3D_SOURCE_DIR}/testing/data/suzanne.stl --output=~/Testing/Temporary/TestHOMEOutput.png --resolution=300,300 --no-config)
  set_tests_properties(f3d::TestHOMEOutput PROPERTIES ENVIRONMENT "HOME=${CMAKE_BINARY_DIR}")
  set_tests_properties(f3d::TestHOMEOutput PROPERTIES FIXTURES_SETUP f3d::TestHOMEOutput_FIXTURE)

  add_test(NAME f3d::TestHOMEReference COMMAND $<TARGET_FILE:f3d> ${F3D_SOURCE_DIR}/testing/data/suzanne.stl --output=${CMAKE_BINARY_DIR}/Testing/Temporary/TestHOMEReference.png --reference=~/Testing/Temporary/TestHOMEOutput.png --resolution=300,300 --no-config)
  set_tests_properties(f3d::TestHOMEReference PROPERTIES ENVIRONMENT "HOME=${CMAKE_BINARY_DIR}")
  set_tests_properties(f3d::TestHOMEReference PROPERTIES FIXTURES_REQUIRED f3d::TestHOMEOutput_FIXTURE)

  add_test(NAME f3d::TestHOMEOutputTemplate COMMAND $<TARGET_FILE:f3d> ${F3D_SOURCE_DIR}/testing/data/suzanne.stl --output=~/Testing/Temporary/{model}{n}.png --resolution=300,300 --verbose)
  set_tests_properties(f3d::TestHOMEOutputTemplate PROPERTIES ENVIRONMENT "HOME=${CMAKE_BINARY_DIR}")

  f3d_test(NAME TestHOMEInput ARGS --input=~/testing/data/suzanne.stl ENV "HOME=${F3D_SOURCE_DIR}")
  # Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
  if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
    f3d_test(NAME TestHOMEConfig DATA suzanne.stl CONFIG ~/testing/configs/complex.json ENV "HOME=${F3D_SOURCE_DIR}" UI)
  endif()
  f3d_test(NAME TestHOMEPlugin ARGS --load-plugins=~/testing/data/invalid.so REGEXP "Cannot open the library" NO_BASELINE ENV "HOME=${F3D_SOURCE_DIR}")
  f3d_test(NAME TestHOMEInteractionRecord DATA cow.vtp ARGS --interaction-test-record=~/Testing/Temporary/TestHOMEInteractionRecord.log NO_BASELINE ENV "HOME=${CMAKE_BINARY_DIR}")
  f3d_test(NAME TestHOMEInteractionPlay DATA cow.vtp ARGS --interaction-test-play=~/Testing/Temporary/TestHOMEInteractionRecord.log DEPENDS TestHOMEInteractionRecord NO_BASELINE ENV "HOME=${CMAKE_BINARY_DIR}")
  f3d_test(NAME TestHOMEColorMapFile DATA dragon.vtu ARGS --colormap-file=~/testing/data/viridis8.png --scalar-coloring --coloring-component=1 ENV "HOME=${F3D_SOURCE_DIR}")
  f3d_test(NAME TestHOMEScreenshot DATA suzanne.ply ARGS --screenshot-filename=~/Testing/Temporary/TestHOMEScreenshot.png --interaction-test-play=${F3D_SOURCE_DIR}/testing/recordings/TestScreenshot.log REGEXP "saving screenshot to ${CMAKE_BINARY_DIR}/Testing/Temporary/TestHOMEScreenshot.png" NO_BASELINE ENV "HOME=${CMAKE_BINARY_DIR}")
  f3d_test(NAME TestHOMECommandScript DATA suzanne.ply ARGS --command-script=~/testing/scripts/TestHOMECommandScript.txt REGEXP "Camera focal point" NO_BASELINE ENV "HOME=${F3D_SOURCE_DIR}")
  f3d_test(NAME TestHOMEFontFile DATA suzanne.stl ARGS -n --font-file=~/testing/data/Crosterian.ttf ENV "HOME=${F3D_SOURCE_DIR}" UI)
  f3d_test(NAME TestHOMETexture DATA suzanne.ply ARGS --texture-matcap=~/testing/data/skin.png ENV "HOME=${F3D_SOURCE_DIR}")
  f3d_test(NAME TestHOMEHDRI DATA suzanne.stl ARGS --hdri-file=~/testing/data/palermo_park_1k.hdr --hdri-ambient --hdri-skybox ENV "HOME=${F3D_SOURCE_DIR}" LONG_TIMEOUT)
  # Needs https://gitlab.kitware.com/vtk/vtk/-/merge_requests/12489
  if(VTK_VERSION VERSION_GREATER_EQUAL 9.5.20251001)
    f3d_test(NAME TestHOMEInteractionDropHDRICollapse INTERACTION_CONFIGURE ENV "HOME=${F3D_SOURCE_DIR}" LONG_TIMEOUT UI)#X;DropEvent dragon.vtu;DropEvent palermo.hdr;
  endif()

  STRING(RANDOM LENGTH 257 ALPHABET "x" _f3d_test_invalid_folder)
  f3d_test(NAME TestColorMapTooLong DATA dragon.vtu ARGS --colormap-file=${_f3d_test_invalid_folder}/file.ext --scalar-coloring REGEXP "File name too long" NO_BASELINE)
  f3d_test(NAME TestScreenshotTooLong DATA suzanne.ply ARGS --screenshot-filename=${_f3d_test_invalid_folder}/file.ext --interaction-test-play=${F3D_SOURCE_DIR}/testing/recordings/TestScreenshot.log REGEXP "File name too long" NO_BASELINE)
  f3d_test(NAME TestInputTooLong ARGS --input=${_f3d_test_invalid_folder}/file.ext REGEXP "File name too long" NO_BASELINE)
  f3d_test(NAME TestReferenceTooLong DATA suzanne.ply ARGS --output=file.png --reference=${_f3d_test_invalid_folder}/file.ext REGEXP "File name too long" NO_BASELINE NO_OUTPUT)
  f3d_test(NAME TestOutputTooLong DATA suzanne.ply ARGS --output=${_f3d_test_invalid_folder}/file.ext REGEXP "File name too long" NO_BASELINE NO_OUTPUT)
  f3d_test(NAME TestOutputWithReferenceTooLong DATA suzanne.ply ARGS --reference=file.png --output=${_f3d_test_invalid_folder}/file.ext REGEXP "File name too long" NO_BASELINE NO_OUTPUT)
  f3d_test(NAME TestOutputWithExistingReferenceTooLong DATA suzanne.ply ARGS --reference=${F3D_SOURCE_DIR}/testing/data/world.png --output=${_f3d_test_invalid_folder}/file.ext REGEXP "File name too long" NO_BASELINE NO_OUTPUT)

  if(NOT APPLE)

    # This tests the XDG_CONFIG_HOME is taken into account AND that config order between locations is respected
    file(COPY "${F3D_SOURCE_DIR}/testing/configs/02_home.json" DESTINATION "${CMAKE_BINARY_DIR}/share/a_home_config/f3d/config_build.d")
    f3d_test(NAME TestXDG_CONFIG_HOME DATA suzanne.ply CONFIG config_build ENV "XDG_CONFIG_HOME=${CMAKE_BINARY_DIR}/share/a_home_config" UI TONE_MAPPING)

    f3d_test(NAME TestXDG_CACHE_HOMECoverage DATA suzanne.ply NO_RENDER NO_BASELINE ENV "XDG_CACHE_HOME=${CMAKE_BINARY_DIR}")

    # Setting XDG_CACHE_HOME is needed for cache to work when HOME is not set
    f3d_test(NAME TestNoHOMEScreenshot DATA suzanne.ply ARGS --screenshot-filename=TestNoHOMEScreenshot.png --interaction-test-play=${F3D_SOURCE_DIR}/testing/recordings/TestScreenshot.log  REGEXP "saving screenshot to ${CMAKE_BINARY_DIR}/application/testing/TestNoHOMEScreenshot.png" NO_BASELINE ENV "XDG_CACHE_HOME=${CMAKE_BINARY_DIR};HOME=")
    f3d_test(NAME TestNoHOMEConfig DATA suzanne.ply CONFIG config_build REGEXP "Using config directory ${CMAKE_BINARY_DIR}/share/f3d/configs/config_build.d" NO_RENDER NO_BASELINE ENV "XDG_CACHE_HOME=${CMAKE_BINARY_DIR};HOME=")
  endif()

else()
  cmake_path(SET _user_profile_path ${CMAKE_BINARY_DIR}/Testing/Temporary/ùser/🥷)
  cmake_path(NATIVE_PATH _user_profile_path _user_profile_path)
  add_test(NAME f3d::TestSetupWinUserProfile COMMAND ${CMAKE_COMMAND} -E make_directory "${_user_profile_path}/AppData/Local")
  f3d_test(NAME TestWinUserProfileNonStandard DATA dragon.vtu ENV "USERPROFILE=${_user_profile_path}" DEPENDS TestSetupWinUserProfile)
endif()

# Test failure without a reference, please do not create a TestNoRef.png file
f3d_test(NAME TestNoRef DATA cow.vtp WILL_FAIL)

# Test failure without a reference and without an output, please do not create a TestNoRef.png file
f3d_test(NAME TestNoRefNoOutput DATA cow.vtp ARGS --reference=${F3D_SOURCE_DIR}/testing/baselines/TestNoRef.png REGEXP "use the output option to output current rendering into an image file." NO_BASELINE NO_OUTPUT)

# Test failure with a bad reference, please do not create a good TestBadRef.png file
f3d_test(NAME TestBadRef DATA cow.vtp WILL_FAIL)

# Test failure with a bad reference without an output, please do not create a good TestBadRef.png file
f3d_test(NAME TestBadRefNoOutput DATA cow.vtp ARGS --reference=${F3D_SOURCE_DIR}/testing/baselines/TestBadRef.png REGEXP "Use the --output option to be able to output current rendering and diff images into files." NO_BASELINE NO_OUTPUT)

# Test failure with a bad interaction play file, please do not create a dummy.log
f3d_test(NAME TestPlayNoFile DATA cow.vtp ARGS --interaction-test-play=${CMAKE_BINARY_DIR}/Testing/Temporary/dummy.log WILL_FAIL)

# Test that --help is displayed even when there is an unknown option
f3d_test(NAME TestHelpPrecedenceWithUnknownOption ARGS --help --unknown REGEXP "Usage:"  NO_BASELINE)

# Test that --version is displayed even when there is an unknown option
f3d_test(NAME TestVersionPrecedenceWithUnknownOption ARGS --version --unknown REGEXP "Version:" NO_BASELINE)

# Test rendering backend list
if(WIN32)
  f3d_test(NAME TestRenderingBackenListWGL ARGS --list-rendering-backends NO_RENDER NO_BASELINE REGEXP "wgl: available")
elseif(APPLE)
  f3d_test(NAME TestRenderingBackenListCOCOA ARGS --list-rendering-backends NO_RENDER NO_BASELINE REGEXP "cocoa: available")
endif()

if(F3D_TESTING_ENABLE_GLX_TESTS AND VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240914)
  f3d_test(NAME TestRenderingBackenListGLX ARGS --list-rendering-backends NO_RENDER NO_BASELINE REGEXP "glx: available")
endif()

if(F3D_TESTING_ENABLE_EGL_TESTS AND VTK_VERSION VERSION_GREATER_EQUAL 9.3.20240914)
  f3d_test(NAME TestRenderingBackenListEGL ARGS --list-rendering-backends NO_RENDER NO_BASELINE REGEXP "egl: available")
endif()

if(F3D_TESTING_ENABLE_OSMESA_TESTS)
  f3d_test(NAME TestRenderingBackenListOSMesa ARGS --list-rendering-backends NO_RENDER NO_BASELINE REGEXP "osmesa: available")
endif()

# Test scan plugins
if(NOT F3D_MACOS_BUNDLE)
  f3d_test(NAME TestScanPluginsCheckNative ARGS --scan-plugins NO_RENDER NO_BASELINE REGEXP " - native")

  if(F3D_PLUGIN_BUILD_HDF)
    f3d_test(NAME TestScanPluginsCheckHDF ARGS --scan-plugins NO_RENDER NO_BASELINE REGEXP " - hdf")
  endif()
endif()

# Watch testing require GLX rendering on Linux
if(F3D_MODULE_DMON AND (WIN32 OR APPLE OR F3D_TESTING_ENABLE_GLX_TESTS))
  if(UNIX)
    set(_f3d_os_script_ext "sh")
    set(_f3d_os_script_exec "")
  elseif(WIN32)
    set(_f3d_os_script_ext "ps1")
    find_program(_f3d_os_script_exec NAMES pwsh powershell)
  endif()
  # Custom bash/pwsh test for testing watch option
  add_test (NAME f3d::TestWatch COMMAND ${_f3d_os_script_exec} ${CMAKE_CURRENT_SOURCE_DIR}/test_watch.${_f3d_os_script_ext} $<TARGET_FILE:f3d> ${F3D_SOURCE_DIR}/testing/data ${CMAKE_BINARY_DIR}/Testing/Temporary)
  set_tests_properties(f3d::TestWatch PROPERTIES RUN_SERIAL TRUE TIMEOUT 90)
endif()
