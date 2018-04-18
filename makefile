app: bin/imgui.o bin/imgui_draw.o bin/imgui_impl_sdl_gl3.o bin/gl3w.o bin/defines.o bin/glak.o bin/main.o
	g++ -o out/app bin/imgui.o bin/imgui_draw.o bin/imgui_impl_sdl_gl3.o bin/gl3w.o bin/defines.o bin/glak.o bin/main.o -ldl -lSDL2

bin/imgui_draw.o: ../imgui/imgui_draw.cpp
	g++ -c -o bin/imgui_draw.o ../imgui/imgui_draw.cpp -I.

bin/imgui.o: ../imgui/imgui.cpp ../imgui/imgui_draw.cpp
	g++ -c -o bin/imgui.o ../imgui/imgui.cpp -I.

bin/imgui_impl_sdl_gl3.o: lib/imgui_impl_sdl_gl3.cpp
	g++ -c -o bin/imgui_impl_sdl_gl3.o lib/imgui_impl_sdl_gl3.cpp -I/usr/include/SDL2 -Iinclude -I../imgui

bin/gl3w.o: lib/gl3w.c
	g++ -c -o bin/gl3w.o lib/gl3w.c -Iinclude

bin/defines.o: src/defines.cpp
	g++ -c -o bin/defines.o src/defines.cpp -Iinclude

bin/glak.o: src/glak.cpp
	g++ -std=c++11 -c -o bin/glak.o src/glak.cpp -Iinclude -Iinclude/SDL -I../imgui

bin/main.o: src/main.cpp
	g++ -std=c++11 -c -o bin/main.o src/main.cpp -Iinclude -Iinclude/SDL -I../imgui