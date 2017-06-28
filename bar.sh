 #!/bin/bash
 echo "Generating makefile..."
  rm client_debug
 ./makeGen libs src -L -w -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU
 echo "Compiling..."
 make
 echo "Running..."
 #./client_debug