#!/bin/bash
#
# Author: Divye Kapoor (divyekapoor@gmail.com)
#
# Runs tests on the grepurls binary.
# The tests could be a bit better, but this is what works right now.

set -e

binary=./grepurls

for f in `ls testdata`; do
  filename=testdata/${f}
  echo -n Running ${filename}...

  # Use shell redirection to supply input and pipe it to wc -l.
  redirect_result=$(${binary} < ${filename} | wc -l)

  # Use direct filename read to supply input and pipe it to wc -l
  file_result=$(${binary} ${filename} | wc -l)

  # Expected result: all urls match.
  # Sadly, we don't have negative tests yet.
  expected_result=$(wc -l ${filename} | awk '{ print $1 }')

  if [[ "${redirect_result}" -eq "${expected_result}" && "${file_result}" -eq "${expected_result}" ]]; then
    echo ' PASS'
  else
    echo ' FAIL'
    echo Redirect: "x${redirect_result}x" Direct: ${file_result}  Expected: "x${expected_result}x"
  fi

done

