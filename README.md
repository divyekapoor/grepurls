GrepURLs
========

This is a simple C++ program that reads its input and filters out URLs from it
and prints it to stdout. All parsed URLs should conform to the RFC 3986 subject
to a few minor caveats.

Useful as a filter to long command chains since finding a regex that does a
decent job of URL parsing and then making use of it is hard.
