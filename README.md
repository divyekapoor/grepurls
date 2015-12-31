GrepURLs
========

This is a simple C++ program that reads its input extracts URLs from it
and prints those URLs to stdout. URLs conforming to the [RFC 3986][rfc] should be
parsed successfully.  URLs that are delimited by various characters such as (),
<>, {}, [], "", '' are also supported.


Compilation
-----------

Just fetch and run Make:

```bash
git clone --recursive https://github.com/divyekapoor/grepurls.git
make
sudo make install
```

Execution
---------

```bash
grepurls < input.txt > urls.txt
```

OR

```bash
cat /tmp/input.txt | grepurls
```

if you prefer using it as a pipeline. The URLs will be output 1 per line.

[rfc]: https://www.ietf.org/rfc/rfc3986.txt
