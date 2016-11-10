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

Pass in under the MIT License. See the `LICENSE` file for details.
