#!/bin/bash

aclocal
libtoolize --force --copy
autoheader
automake --add-missing --copy
autoconf
./configure
