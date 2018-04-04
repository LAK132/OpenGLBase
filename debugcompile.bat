@echo off

REM ImGui
cl /Fo:bin\imgui.obj        /c R:\dear-imgui\imgui.cpp      /nologo -EHsc -DNDEBUG /MD /IR:\dear-imgui
cl /Fo:bin\imgui_draw.obj   /c R:\dear-imgui\imgui_draw.cpp /nologo -EHsc -DNDEBUG /MD /IR:\dear-imgui
cl /Fo:bin\imgui_demo.obj   /c R:\dear-imgui\imgui_demo.cpp /nologo -EHsc -DNDEBUG /MD /IR:\dear-imgui
cl /Fo:bin\imgui_opegl.obj  /c lib\imgui_impl_glfw_gl3.cpp  /nologo -EHsc -DNDEBUG /MD /IR:\dear-imgui /Iinclude

REM gl3w
cl /Fo:bin\gl3w.obj     /c lib\gl3w.c   /nologo -EHsc -DNDEBUG /MD /Iinclude

REM main
cl /Fo:bin\defines.obj  /c src\defines.cpp  /nologo -EHsc -DNDEBUG /MD /Isrc
cl /Fo:bin\glak.obj     /c src\glak.cpp     /nologo -EHsc -Zi /MD /Isrc /Iinclude /IR:\dear-imgui
cl /Fo:bin\main.obj     /c src\main.cpp     /nologo -EHsc -Zi /MD /Isrc /Iinclude /IR:\dear-imgui

REM link
link /DEBUG /nologo /out:out\app.exe ^
bin\main.obj bin\glak.obj ^
bin\imgui_opegl.obj bin\imgui.obj bin\imgui_demo.obj bin\imgui_draw.obj ^
bin\gl3w.obj lib\glfw3dll.lib lib\glfw3.lib 