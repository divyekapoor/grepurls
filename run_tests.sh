#!/bin/bash
#
# Author: Divye Kapoor (divyekapoor@gmail.com)
#
# Runs tests on the grepurls binary.
# The tests could be a bit better, but this is what works right now.

set -e

binary=./grepurls
flags=--schemes=

for f in `ls testdata`; do
  filename=testdata/${f}
  echo -n Running ${filename}...

  # Use shell redirection to supply input
  redirect_output=/tmp/grepurls.redirect.out
  ${binary} ${flags} < ${filename} > ${redirect_output}

  # Use direct filename read to supply input and pipe it to wc -l
  file_output=/tmp/grepurls.file.out
  ${binary} ${flags} ${filename} > ${file_output}

  # Diff outputs.
  diff_redirect_output=/tmp/grepurls.redirect.diff
  diff_file_output=/tmp/grepurls.file.diff
  diff_flags='-y --suppress-common-lines'
  (diff ${diff_flags} ${redirect_output} expected/${f} > ${diff_redirect_output} && \
    diff ${diff_flags} ${file_output} expected/${f} > ${diff_file_output} && echo ' PASS') || \
    (echo ' FAIL' && cat ${diff_redirect_output} ${diff_redirect_output})
done

for f in `ls faildata`; do
  filename=faildata/${f}
  echo -n Running ${filename}...

  redirect_result=$(${binary} < ${filename} | wc -l)
  file_result=$(${binary} ${filename} | wc -l)
  expected_result=0
  if [[ "${redirect_result}" -eq "${expected_result}" && "${file_result}" -eq "${expected_result}" ]]; then
    echo ' PASS'
  else
    echo ' FAIL'
    echo Redirect: "x${redirect_result}x" Direct: "x${file_result}x" Expected: "x${expected_result}x"
  fi
done

