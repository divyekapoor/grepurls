#!/bin/bash
#
# Author: Divye Kapoor (divyekapoor@gmail.com)
#
# Runs tests on the grepurls binary.

set -e

binary=./grepurls

for f in `ls testdata`; do
  filename=testdata/${f}
  echo -n Running ${filename}...

  # Use shell redirection to supply input and pipe it to wc -l.
  binary_result=$(${binary} < ${filename} | wc -l)
  expected_result=$(wc -l ${filename} | awk '{ print $1 }')

  if [[ "${binary_result}" -eq "${expected_result}" ]]; then
    echo ' PASS'
  else
    echo ' FAIL'
    echo Binary: "x${binary_result}x"  Expected: "x${expected_result}x"
  fi

done

