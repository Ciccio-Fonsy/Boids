Compile using

```
g++ boid.cpp functions.cpp predator.cpp prey.cpp swarm.cpp variables.cpp vec3.cpp boids.cpp -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -D_GLIBCXX_ASSERTIONS -fsanitize=address -o boids -lsfml-graphics -lsfml-window -lsfml-system

cmake -S . -B build -G"Ninja Multi-Config"
cmake --build build --config Debug
cmake --build build --config Debug --target test
cmake --build build --config Release
cmake --build build --config Release --target test
```

