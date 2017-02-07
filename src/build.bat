@echo off

if not exist ..\build mkdir ..\build
pushd ..\build

set LibsDirectory=../libs
set Defines=/DWIN32 /DGLEW_STATIC /D_CRT_SECURE_NO_WARNINGS

set IncludeDirectories= ^
	-I%LibsDirectory%/AntTweakBar/include ^
	-I%LibsDirectory%/glew/include ^
	-I%LibsDirectory%/glutil/include ^
	-I%LibsDirectory%/boost ^
	-I%LibsDirectory%/glm ^
	-I%LibsDirectory%/freeglut/include

set LibraryDirectories= ^
-LIBPATH:%LibsDirectory%/AntTweakBar/lib ^
-LIBPATH:%LibsDirectory%/freeglut/lib ^
-LIBPATH:%LibsDirectory%/glew/lib/Release/Win32 ^
-LIBPATH:%LibsDirectory%/glutil/lib

set Libs= ^
    freeglut.lib ^
    glew32s.lib ^
    glutil.lib

set FilesToCompile= ^
../src/main.cpp ^
../src/Functions.cpp ^
../src/EditorWindow.cpp ^
../src/Balloon.cpp ^
../src/Basket.cpp ^
../src/Camera.cpp ^
../src/Command.cpp ^
../src/Ground.cpp ^
../src/ImageData.cpp ^
../src/InputManager.cpp ^
../src/SkyBox.cpp ^
../src/TerrainGenerator.cpp ^
../src/TestObject.cpp ^
../src/Vector2f.cpp ^
../src/Window.cpp ^
../src/Game.cpp

set CompilerOptions=%Defines% /FC /Zi /EHsc /MD /MP /wd4503 /wd4312 /nologo /FeBalloon.exe %IncludeDirectories%
set LinkerOptions=/link %LibraryDirectories%

cl /O2x %CompilerOptions% %FilesToCompile% %Libs% %LinkerOptions%

popd
