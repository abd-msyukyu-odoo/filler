cd minilibx_macos;
make;
cd ..;
# gcc -I minilibx_macos main.c -L minilibx_macos -lmlx -framework OpenGL -framework AppKit;
gcc -g -fsanitize=address main.c read.c;
cd minilibx_macos;
make clean;
cd ..;
./a.out
# rm a.out;
rm -rf a.out.dSYM
