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

#include <pegtl.hh>
#include <pegtl/trace.hh>
#include <pegtl/contrib/uri.hh>

using grammar = pegtl::uri::URI;

namespace pegtl {
  namespace uri {
    struct URIState {
      std::string uri;
    };

    // The default base action class.
    template<typename Rule>
    struct URIActions : pegtl::nothing<Rule> {};

    // Action for the URI class.
    template<>
    struct URIActions<URI> {
      static void apply(const pegtl::input& input,
                        URIState& parsed_uri) {
        parsed_uri.uri.assign(input.string());
      }
    };
  }
}

using pegtl::uri::URIActions;
using pegtl::uri::URIState;

// Should be compiled down quite nicely. :)
char brackets[] = {'{', '}', '[', ']', '(', ')', '<', '>', '"', '\''};
bool IsBracket(char c) {
  for (int i = 0; i < sizeof(brackets) / sizeof(char); ++i) {
    if (c == brackets[i]) return true;
  }
  return false;
}

int main(int argc, char* argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string thunk;
  while (std::cin >> thunk) {
    // Trim.
    int start = 0, end = thunk.size();
    for (; start < thunk.size() && IsBracket(thunk[start]); ++start) {}
    for (; end > 0 && IsBracket(thunk[end - 1]); --end) {}

    URIState state;
    bool parse_result = pegtl::parse<grammar, URIActions>(
        thunk.data() + start, thunk.data() + end, "stdin", state);
    if (parse_result) {
      std::cout << state.uri << '\n';
    }
  }
  return 0;
}
