FROM emscripten/emsdk AS compiler

WORKDIR /usr/compile

# Copy all
COPY . .

# Make the build directory and enter it
RUN mkdir build && cd build && emcmake cmake .. && make

FROM nginx:alpine
COPY --from=compiler /usr/compile/public/* /usr/share/nginx/html/