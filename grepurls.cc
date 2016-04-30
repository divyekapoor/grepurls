// A simple binary to grep URLs out of text.
//
// Based on the URI class contrib'd to the excellent PEGTL library.
//
// Licenced under the MIT licence as per the original code.
//
// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <iostream>
#include <string>
#include <iomanip>
#include <ios>
#include <vector>
#include <fstream>

#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/trace.hh>

#include <gflags/gflags.h>

#include "uri_state.h"
#include "uri.h"

const char usage[] =
R"(A utility that extracts URLs from input.

Typical usage:
  grepurls [-hv] [file...]
  ugrep [-hv] [file...]

If no files are supplied on the command line, we read the input from stdin.)";

DECLARE_bool(help);       // Defined by the gflags library.
DECLARE_bool(helpshort);  // Defined by the gflags library.
DECLARE_bool(version);    // Defined by the gflags library.
DEFINE_bool(v, false, "Display the version of the binary.");
DEFINE_bool(h, false, "Display help.");

namespace pegtl {
  namespace uri {
    // The semantic actions associated with each parse rule.
    // The default base action class.
    template<typename Rule>
    struct URIActions : pegtl::nothing<Rule> {};

    // Action for the URI class.
    // (saves the parsed URI to URIState.uri)
    template<>
    struct URIActions<URI> {
      static void apply(const pegtl::input& input,
                        URIState& parsed_uri) {
        parsed_uri.uri.assign(input.string());
      }
    };

    template<>
    struct URIActions<iana_URI> {
      static void apply(const pegtl::input& input,
                        URIState& parsed_uri) {
        parsed_uri.uri.assign(input.string());
      }
    };

  }
}


using grammar = pegtl::until<pegtl::uri::iana_URI>;

using pegtl::uri::URIActions;
using pegtl::uri::URIState;

// Should be compiled down quite nicely. :)
const std::vector<std::vector<char>> brackets = {{'{', '}'}, {'[', ']'}, {'(', ')'},
  {'<', '>'}, {'"', '"'}, {'\'', '\''}};
bool IsMatchingBracket(char lhs, char rhs) {
  for (size_t i = 0; i < brackets.size(); ++i) {
    if (lhs == brackets[i][0] && rhs == brackets[i][1]) return true;
  }
  return false;
}

void grepurl(const std::string& thunk) {
  // Trim.
  size_t start = 0, end = thunk.size();
  for (; start < thunk.size() && end > 0 &&
      IsMatchingBracket(thunk[start], thunk[end - 1]); ++start, --end) {}

  // Match.
  URIState state;
  bool parse_result = pegtl::parse<grammar, URIActions>(
      thunk.data() + start, thunk.data() + end, "stdin", state);

  // Output.
  if (parse_result && !state.uri.empty()) {
    std::cout << state.uri << '\n';
  }
}

template<class IStream>
void process(IStream& in) {
  std::string thunk;
  while (in >> thunk) {
    grepurl(thunk);
  }
}


int main(int argc, char* argv[]) {
  // Handle command line flags.
  // Slightly uglier than necessary due to:
  // https://github.com/gflags/gflags/issues/43
  gflags::SetUsageMessage(usage);
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
  if (FLAGS_v) {
    FLAGS_version = true;
  }
  if (FLAGS_help || FLAGS_h) {
    FLAGS_help = false;
    FLAGS_helpshort = true;
  }
  gflags::HandleCommandLineHelpFlags();

  // Fast IO.
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  /* Analysis */
  // const size_t issues_found = pegtl::analyze<grammar>();
  // std::cout << "Grammar issues found: " << issues_found << '\n';

  // Argv contains just the filenames now.
  const bool files_found = (argc > 1);
  if (!files_found) {
    process(std::cin);
    return 0;
  }

  bool atleast_one_file_opened = false;
  for (int i = 1; i < argc; ++i) {
    std::ifstream fin(argv[i]);
    if (fin) {
      atleast_one_file_opened = true;
      process(fin);
    }
  }

  return atleast_one_file_opened ? 0 : 1;
}
