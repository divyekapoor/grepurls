#ifndef GREPURLS_URI_STATE_H
#define GREPURLS_URI_STATE_H

#include <string>

namespace pegtl {
  namespace uri {

    // The state associated with the URI parse.
    // (available to all actions).
    struct URIState {
      std::string uri;
      char opening_context;
    };

  }
}

#endif
