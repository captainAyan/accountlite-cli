@echo off

g++ *.cpp -o accountlite.exe -std=c++17 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

accountlite.exe
