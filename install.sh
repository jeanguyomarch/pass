#!/bin/bash -e
# Copyright (c) 2013 Jean Guyomarc'h
# See license and stuff at the hosting page https://github.com/jeanguyomarch/pass

function err()
{
   echo >&2
   echo "*** [install:error$?] $@" >&2
   echo >&2
   exit 2
}

function usage()
{
   echo "Usage: ./install.sh [prefix]. The default prefix is /usr/local/bin"
}

check_eet="`which eet`"
check_os="`uname`"
prefix="$1"

if [ x"$1" == x"--help" ] || [ x"$1" == x"-help" ] || [ x"$1" == x"-h" ]; then
   usage
   exit 0
fi

echo "Checking for eet... $check_eet"
if [ $? -ne 0 ]; then
   err "'eet' not found"
fi

echo "Checking for OS... $check_os"
if [ "$check_os" != "Darwin" ]; then
   err "Mac OS X 'Darwin' required"
fi

if [ $# -eq 0 ]; then
   prefix="/usr/local/bin"
fi

echo "Installing at path [$prefix]"

sudo install -m 0755 pass "$prefix"


