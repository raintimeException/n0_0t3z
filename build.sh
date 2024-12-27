#!/bin/sh

set -xe

cc -Wall -Wextra -pedantic -o app main.c

./app
