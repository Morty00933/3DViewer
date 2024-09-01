#!/bin/bash

cd ..
docker rmi -f valgrind_image
docker build . -t valgrind_image -f src/docker/valgrind/Dockerfile
docker run --rm valgrind_image
