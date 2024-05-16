emcc -std=c++17 "main.cpp" "Application.cpp" "Renderer/Shader.cpp" -s USE_GLFW=3 -s FULL_ES3=1 -DWITH_EMSCRIPTEN=1 --memory-init-file 0 --embed-file res -O3 -o Web\index.html
