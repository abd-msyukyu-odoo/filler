cd minilibx_macos;
make;
cd ..;
gcc -I minilibx_macos main.c -L minilibx_macos -lmlx -framework OpenGL -framework AppKit;
cd minilibx_macos;
make clean;
cd ..;
./a.out;
rm a.out;
