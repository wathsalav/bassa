#!/bin/bash

libtoolize --force
aclocal
autoconf
touch config.h.in
automake --add-missing
./configure
