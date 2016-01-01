GrepURLs / UGrep
================

This is a simple C++ program that reads its input extracts URLs from it
and prints those URLs to stdout. URLs conforming to the [RFC 3986][rfc] should be
parsed successfully.  URLs that are delimited by various characters such as (),
<>, {}, [], "", '' are also supported.


Installation
------------

Just fetch and run Make:

```bash
git clone --recursive https://github.com/divyekapoor/grepurls.git
cd grepurls
./bootstrap.sh
make
sudo make install
```

by default, we install to /usr/local/bin/. If you'd like to install to another
path, just edit the Makefile.

Note: The binary is installed as "grepurls" and "ugrep". You may use either one
as per your convenience.

Usage
-----

```bash
ugrep < input.txt > urls.txt
cat /tmp/input.txt | ugrep
```

OR

```bash
grepurls < input.txt > urls.txt
cat /tmp/input.txt | grepurls
```

The URLs will be output 1 per line.

[rfc]: https://www.ietf.org/rfc/rfc3986.txt
