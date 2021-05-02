FROM emscripten/emsdk AS compiler

WORKDIR /usr/compile

# Copy all
COPY . .

# Download the ROMs
RUN rm public/rom && \
    mkdir public/rom && \
    cd public/rom && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/programs/IBM%20Logo.ch8" -O IBM.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/games/Lunar%20Lander%20(Udo%20Pernisz%2C%201979).ch8" -O Lunar.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/games/Tetris%20%5BFran%20Dachille%2C%201991%5D.ch8" -O Tetris.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/programs/Fishie%20%5BHap%2C%202005%5D.ch8" -O Fish.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/games/Brick%20(Brix%20hack%2C%201990).ch8" -O Brick.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/games/Pong%202%20(Pong%20hack)%20%5BDavid%20Winter%2C%201997%5D.ch8" -O Pong.ch8 && \
    wget "https://github.com/dmatlack/chip8/raw/master/roms/games/Space%20Invaders%20%5BDavid%20Winter%5D%20(alt).ch8" -O SI.ch8

# Make the build directory and enter it
RUN mkdir build && cd build && emcmake cmake .. && make

FROM nginx:alpine
COPY --from=compiler /usr/compile/public/ /usr/share/nginx/html/