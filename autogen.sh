#!/bin/sh

echo "Running libtool ..."
libtoolize --copy --force --automake 

echo "Running aclocal ..."
aclocal --force 

echo "Running automake ..."
automake -i --add-missing --copy --foreign 

echo "Running autoconf ..."
autoconf --force 

echo "Done setting up project"
