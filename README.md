pass
====


Requirements
------------

* `eet` must be installed http://docs.enlightenment.org/stable/eet/index.html
* `openssl`
* For now, the script is only Mac OS X compatible, feel free to make it portable.


Password manager
----------------

* A file `~/.cryptofile` is an EET file that contains a password for a given key. It is possible to encrypt it using the eet encryption.
* `pass -h` : query help
* `pass -v` : query version


Installation
------------

* Query help: `./install.sh --help` or `./install.sh -h` `./install.sh -help`
* To install it at `/usr/local/bin` just run `./install.sh`. Requirements checks are made, and your password will be asked if all the requirements are fulfilled.
* To install it at a custom path: `./install.sh /your/custom/path`


Copying
-------

Copyright notice for Eet:

Copyright (C) 2002-2011 Carsten Haitzler and various contributors (see AUTHORS)

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


The pass license is under MIT license (see LICENSE.md)
