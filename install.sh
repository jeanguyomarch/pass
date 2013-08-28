#!/bin/bash -e
# Copyright (c) 2013 Jean Guyomarc'h
# See license and stuff at the hosting page https://github.com/jeanguyomarch/pass

function err()
{
   echo >&2
   echo "*** [install:error$?] $@" >&2
   echo >&2
   exit 1
}

function usage()
{
   echo "Usage: ./install.sh [prefix]. The default prefix is /usr/local"
}

function check_os()
{
   os="`uname`"
   echo "Checking for OS... $os"
   if [ "$os" != "Darwin" ]; then
      err "Mac OS X 'Darwin' required"
   fi
}

function check_program()
{
   prog="`which "$1"`"
   echo "Checking for $1... $prog"
   if [ $? -ne 0 ]; then
      err "'$1' not found"
   fi
}

prefix="$1"

if [ x"$1" == x"--help" ] || [ x"$1" == x"-help" ] || [ x"$1" == x"-h" ]; then
   usage
   exit 0
fi

check_os
check_program "eet"
check_program "openssl"

if [ $# -eq 0 ]; then
   prefix="/usr/local"
fi

echo "Installing with prefix [$prefix]"

sudo install -m 0755 pass "$prefix/bin"
echo "(bin) $prefix/bin"
manpath="/usr/share/man/man1"
sudo install -g 0 -o 0 -m 0644 pass.1 "$manpath"
sudo gzip "$manpath/pass.1"
echo "(man) $manpath/pass.1.gz"


