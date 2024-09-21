Compile using

```
g++ vec3.cpp boid.cpp swarm.cpp predator.cpp functions.cpp boids.cpp -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -D_GLIBCXX_ASSERTIONS -fsanitize=address -o boids -lsfml-graphics -lsfml-window -lsfml-system
```
