GrepURLs / UGrep
================

This is a simple C++ program that reads its input extracts URLs from it
and prints those URLs to stdout. URLs conforming to the [RFC 3986][rfc] should be
parsed successfully.  URLs that are delimited by various characters such as (),
<>, {}, [], "", '' are also supported.

[![Build Status](https://travis-ci.org/divyekapoor/grepurls.svg?branch=master)](https://travis-ci.org/divyekapoor/grepurls)


Installation
------------

Just fetch and run Make:

```bash
git clone --recursive https://github.com/divyekapoor/grepurls.git
cd grepurls
./configure && make && sudo make install
```

by default, we install to /usr/local/bin/. If you'd like to install to another
path, just edit the Makefile.

Note: The binary is installed as "grepurls" and "ugrep". You may use either one
as per your convenience.

Usage
-----

```bash
ugrep input.html input1.txt input2.txt
cat /tmp/input.html /tmp/input1.txt /tmp/input2.txt | ugrep
```

OR

```bash
grepurls input.html input1.txt input2.txt
cat /tmp/input.html /tmp/input1.txt /tmp/input2.txt | grepurls
```

You can use standard shell expansion to run `ugrep` over entire directories:

```sh
ugrep mirrored_website/*.html
```

The URLs will be output 1 per line.

Here's an example:

```sh
$ ugrep testdata/contrib_uri.txt
http://de.wikipedia.org/wiki/Uniform_Resource_Identifier
ftp://ftp.is.co.za/rfc/rfc1808.txt
file:///C:/Users/Benutzer/Desktop/Uniform%20Resource%20Identifier.html
file:///etc/fstab
geo:48.33,14.122;u=22.5
ldap://[2001:db8::7]/c=GB?objectClass?one
gopher://gopher.floodgap.com
mailto:John.Doe@example.com
sip:911@pbx.mycompany.com
news:comp.infosystems.www.servers.unix
data:text/plain;charset=iso-8859-7,%be%fa%be
tel:+1-816-555-1212
telnet://192.0.2.16:80/
urn:oasis:names:specification:docbook:dtd:xml:4.1.2
git://github.com/rails/rails.git
crid://broadcaster.com/movies/BestActionMovieEver
http://nobody:password@example.org:8080/cgi-bin/script.php?action=submit&pageid=86392001#section_2
```


Uninstall
---------

```bash
sudo make uninstall
```

[rfc]: https://www.ietf.org/rfc/rfc3986.txt
