pass
====

Password manager
----------------

A file `~/.pass_db` is created. It is an EET file which uses key-value couples. A key is associated to an encrypted password.

Usage
-----

* `pass -l`: lists all keys
* `pass -a MyKey`: adds __MyKey__ to the keys
* `pass -x MyKey`: extracts the password for the key __MyKey__
* `pass -d MyKey`: deletes the couple __MyKey__/associated password
* `pass -r MyKey`: replaces the contents of __MyKey__

Documentation
-------------

`man pass` (after installation).


Requirements
------------

* `EFL` must be installed (http://www.enlightenment.org)


Installation
------------

* `make`
* `sudo make install` (you can override the `PREFIX` variable to change the installation directory).


License
-------

The MIT License (MIT)

Copyright (c) 2013 - 2015 Jean Guyomarc'h

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

