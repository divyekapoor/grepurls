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
#include <pegtl/contrib/uri.hh>

#include <gflags/gflags.h>

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
    struct iana_scheme :
      sor<
pegtl_istring_t("aaas"),
pegtl_istring_t("aaa"),
pegtl_istring_t("about"),
pegtl_istring_t("acap"),
pegtl_istring_t("acct"),
pegtl_istring_t("acr"),
pegtl_istring_t("adiumxtra"),
pegtl_istring_t("afp"),
pegtl_istring_t("afs"),
pegtl_istring_t("aim"),
pegtl_istring_t("appdata"),
pegtl_istring_t("apt"),
pegtl_istring_t("attachment"),
pegtl_istring_t("aw"),
pegtl_istring_t("barion"),
pegtl_istring_t("beshare"),
pegtl_istring_t("bitcoin"),
pegtl_istring_t("blob"),
pegtl_istring_t("bolo"),
pegtl_istring_t("callto"),
pegtl_istring_t("cap"),
pegtl_istring_t("chrome-extension"),
pegtl_istring_t("chrome"),
pegtl_istring_t("cid"),
pegtl_istring_t("coaps"),
pegtl_istring_t("coap"),
pegtl_istring_t("com-eventbrite-attendee"),
pegtl_istring_t("content"),
pegtl_istring_t("crid"),
pegtl_istring_t("cvs"),
pegtl_istring_t("data"),
pegtl_istring_t("dav"),
pegtl_istring_t("dict"),
pegtl_istring_t("dis"),
pegtl_istring_t("dlna-playcontainer"),
pegtl_istring_t("dlna-playsingle"),
pegtl_istring_t("dns"),
pegtl_istring_t("dntp"),
pegtl_istring_t("dtn"),
pegtl_istring_t("dvb"),
pegtl_istring_t("ed2k"),
pegtl_istring_t("example"),
pegtl_istring_t("facetime"),
pegtl_istring_t("fax"),
pegtl_istring_t("feedready"),
pegtl_istring_t("feed"),
pegtl_istring_t("filesystem"),
pegtl_istring_t("file"),
pegtl_istring_t("finger"),
pegtl_istring_t("fish"),
pegtl_istring_t("ftp"),
pegtl_istring_t("geo"),
pegtl_istring_t("gg"),
pegtl_istring_t("git"),
pegtl_istring_t("gizmoproject"),
pegtl_istring_t("gopher"),
pegtl_istring_t("go"),
pegtl_istring_t("gtalk"),
pegtl_istring_t("h323"),
pegtl_istring_t("ham"),
pegtl_istring_t("hcp"),
pegtl_istring_t("https"),
pegtl_istring_t("http"),
pegtl_istring_t("iax"),
pegtl_istring_t("icap"),
pegtl_istring_t("icon"),
pegtl_istring_t("imap"),
pegtl_istring_t("im"),
pegtl_istring_t("info"),
pegtl_istring_t("iotdisco"),
pegtl_istring_t("ipn"),
pegtl_istring_t("ipps"),
pegtl_istring_t("ipp"),
pegtl_istring_t("irc6"),
pegtl_istring_t("ircs"),
pegtl_istring_t("irc"),
pegtl_istring_t("iris.beep"),
pegtl_istring_t("iris.lwz"),
pegtl_istring_t("iris.xpcs"),
pegtl_istring_t("iris.xpc"),
pegtl_istring_t("iris"),
pegtl_istring_t("isostore"),
pegtl_istring_t("itms"),
pegtl_istring_t("jabber"),
pegtl_istring_t("jar"),
pegtl_istring_t("jms"),
pegtl_istring_t("keyparc"),
pegtl_istring_t("lastfm"),
pegtl_istring_t("ldaps"),
pegtl_istring_t("ldap"),
pegtl_istring_t("magnet"),
pegtl_istring_t("mailserver"),
pegtl_istring_t("mailto"),
pegtl_istring_t("maps"),
pegtl_istring_t("market"),
pegtl_istring_t("message"),
pegtl_istring_t("mid"),
pegtl_istring_t("mms"),
pegtl_istring_t("modem"),
pegtl_istring_t("ms-access"),
pegtl_istring_t("ms-drive-to"),
pegtl_istring_t("ms-excel"),
pegtl_istring_t("ms-getoffice"),
pegtl_istring_t("ms-help"),
pegtl_istring_t("ms-infopath"),
pegtl_istring_t("ms-media-stream-id"),
pegtl_istring_t("ms-powerpoint"),
pegtl_istring_t("ms-project"),
pegtl_istring_t("ms-publisher"),
pegtl_istring_t("ms-search-repair"),
pegtl_istring_t("ms-secondary-screen-controller"),
pegtl_istring_t("ms-secondary-screen-setup"),
pegtl_istring_t("ms-settings-airplanemode"),
pegtl_istring_t("ms-settings-bluetooth"),
pegtl_istring_t("ms-settings-camera"),
pegtl_istring_t("ms-settings-cellular"),
pegtl_istring_t("ms-settings-cloudstorage"),
pegtl_istring_t("ms-settings-emailandaccounts"),
pegtl_istring_t("ms-settings-language"),
pegtl_istring_t("ms-settings-location"),
pegtl_istring_t("ms-settings-lock"),
pegtl_istring_t("ms-settings-nfctransactions"),
pegtl_istring_t("ms-settings-notifications"),
pegtl_istring_t("ms-settings-power"),
pegtl_istring_t("ms-settings-privacy"),
pegtl_istring_t("ms-settings-proximity"),
pegtl_istring_t("ms-settings-screenrotation"),
pegtl_istring_t("ms-settings-wifi"),
pegtl_istring_t("ms-settings-workplace"),
pegtl_istring_t("ms-settings"),
pegtl_istring_t("ms-spd"),
pegtl_istring_t("ms-transit-to"),
pegtl_istring_t("ms-visio"),
pegtl_istring_t("ms-walk-to"),
pegtl_istring_t("ms-word"),
pegtl_istring_t("msnim"),
pegtl_istring_t("msrps"),
pegtl_istring_t("msrp"),
pegtl_istring_t("mtqp"),
pegtl_istring_t("mumble"),
pegtl_istring_t("mupdate"),
pegtl_istring_t("mvn"),
pegtl_istring_t("news"),
pegtl_istring_t("nfs"),
pegtl_istring_t("nih"),
pegtl_istring_t("ni"),
pegtl_istring_t("nntp"),
pegtl_istring_t("notes"),
pegtl_istring_t("oid"),
pegtl_istring_t("opaquelocktoken"),
pegtl_istring_t("pack"),
pegtl_istring_t("palm"),
pegtl_istring_t("paparazzi"),
pegtl_istring_t("pkcs11"),
pegtl_istring_t("platform"),
pegtl_istring_t("pop"),
pegtl_istring_t("pres"),
pegtl_istring_t("prospero"),
pegtl_istring_t("proxy"),
pegtl_istring_t("psyc"),
pegtl_istring_t("query"),
pegtl_istring_t("rediss"),
pegtl_istring_t("redis"),
pegtl_istring_t("reload"),
pegtl_istring_t("resource"),
pegtl_istring_t("res"),
pegtl_istring_t("rmi"),
pegtl_istring_t("rsync"),
pegtl_istring_t("rtmfp"),
pegtl_istring_t("rtmp"),
pegtl_istring_t("rtsps"),
pegtl_istring_t("rtspu"),
pegtl_istring_t("rtsp"),
pegtl_istring_t("secondlife"),
pegtl_istring_t("service"),
pegtl_istring_t("session"),
pegtl_istring_t("sftp"),
pegtl_istring_t("sgn"),
pegtl_istring_t("shttp"),
pegtl_istring_t("sieve"),
pegtl_istring_t("sips"),
pegtl_istring_t("sip"),
pegtl_istring_t("skype"),
pegtl_istring_t("smb"),
pegtl_istring_t("sms"),
pegtl_istring_t("smtp"),
pegtl_istring_t("snews"),
pegtl_istring_t("snmp"),
pegtl_istring_t("soap.beeps"),
pegtl_istring_t("soap.beep"),
pegtl_istring_t("soldat"),
pegtl_istring_t("spotify"),
pegtl_istring_t("ssh"),
pegtl_istring_t("steam"),
pegtl_istring_t("stuns"),
pegtl_istring_t("stun"),
pegtl_istring_t("submit"),
pegtl_istring_t("svn"),
pegtl_istring_t("tag"),
pegtl_istring_t("teamspeak"),
pegtl_istring_t("teliaeid"),
pegtl_istring_t("telnet"),
pegtl_istring_t("tel"),
pegtl_istring_t("tftp"),
pegtl_istring_t("things"),
pegtl_istring_t("thismessage"),
pegtl_istring_t("tip"),
pegtl_istring_t("tn3270"),
pegtl_istring_t("turns"),
pegtl_istring_t("turn"),
pegtl_istring_t("tv"),
pegtl_istring_t("udp"),
pegtl_istring_t("unreal"),
pegtl_istring_t("urn"),
pegtl_istring_t("ut2004"),
pegtl_istring_t("v-event"),
pegtl_istring_t("vemmi"),
pegtl_istring_t("ventrilo"),
pegtl_istring_t("videotex"),
pegtl_istring_t("view-source"),
pegtl_istring_t("wais"),
pegtl_istring_t("webcal"),
pegtl_istring_t("wss"),
pegtl_istring_t("ws"),
pegtl_istring_t("wtai"),
pegtl_istring_t("wyciwyg"),
pegtl_istring_t("xcon-userid"),
pegtl_istring_t("xcon"),
pegtl_istring_t("xfire"),
pegtl_istring_t("xmlrpc.beeps"),
pegtl_istring_t("xmlrpc.beep"),
pegtl_istring_t("xmpp"),
pegtl_istring_t("xri"),
pegtl_istring_t("ymsgr"),
pegtl_istring_t("z39.50r"),
pegtl_istring_t("z39.50s"),
pegtl_istring_t("z39.50")
> {};
    struct iana_URI : seq<iana_scheme, one<':'>, hier_part, opt_query, opt_fragment> {};

    // The state associated with the URI parse.
    // (available to all actions).
    struct URIState {
      std::string uri;
    };

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
  gflags::SetVersionString("0.9");
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
