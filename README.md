Compile using

```
g++ boid.cpp functions.cpp predator.cpp statistics.cpp swarm.cpp variables.cpp vec3.cpp boids.cpp -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -D_GLIBCXX_ASSERTIONS -fsanitize=address -o boids -lsfml-graphics -lsfml-window -lsfml-system
```
