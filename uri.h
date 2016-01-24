// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/
//
// This file is almost completely derived from the PEGTL contrib file except
// for a few minor modifications (notably, a reduction in the characters
// allowed in the sub_delims set). This allows better parsing of URIs
// embedded in various forms of text and the IANA scheme set to ensure that
// only valid URIs with given schemes are parsed (else spurious URLs are
// detected in text).

#ifndef GREPURLS_URI_H
#define GREPURLS_URI_H

#include <pegtl/rules.hh>
#include <pegtl/ascii.hh>
#include <pegtl/utf8.hh>
#include <pegtl/contrib/abnf.hh>

namespace pegtl
{
   namespace uri
   {
      // URI grammar according to RFC 3986.

      // This grammar is a direct PEG translation of the original URI grammar.
      // It should be considered experimental -- in case of any issues, in particular
      // missing anchor rules for actions, please contact the developers.

      // Note that this grammar has multiple top-level rules.

      using namespace abnf;

      using dot = one< '.' >;
      using colon = one< ':' >;

      struct dec_octet : sor< one< '0' >,
                              rep_min_max< 1, 2, DIGIT >,
                              seq< one< '1' >, DIGIT, DIGIT >,
                              seq< one< '2' >, range< '0', '4' >, DIGIT >,
                              seq< string< '2', '5' >, range< '0', '5' > > > {};

      struct IPv4address : seq< dec_octet, dot, dec_octet, dot, dec_octet, dot, dec_octet > {};

      struct h16 : rep_min_max< 1, 4, HEXDIG > {};
      struct ls32 : sor< seq< h16, colon, h16 >, IPv4address > {};

      struct dcolon : two< ':' > {};

      struct IPv6address : sor< seq<                                               rep< 6, h16, colon >, ls32 >,
                                seq<                                       dcolon, rep< 5, h16, colon >, ls32 >,
                                seq< opt< h16                           >, dcolon, rep< 4, h16, colon >, ls32 >,
                                seq< opt< h16,     opt<    colon, h16 > >, dcolon, rep< 3, h16, colon >, ls32 >,
                                seq< opt< h16, rep_opt< 2, colon, h16 > >, dcolon, rep< 2, h16, colon >, ls32 >,
                                seq< opt< h16, rep_opt< 3, colon, h16 > >, dcolon,         h16, colon,   ls32 >,
                                seq< opt< h16, rep_opt< 4, colon, h16 > >, dcolon,                       ls32 >,
                                seq< opt< h16, rep_opt< 5, colon, h16 > >, dcolon,                       h16  >,
                                seq< opt< h16, rep_opt< 6, colon, h16 > >, dcolon                             > > {};

      struct gen_delims : one< ':', '/', '?', '#', '[', ']', '@', '\''  > {};
      struct sub_delims : one< '!', '$', '&', '(', ')', '*', '+', ',', ';', '=' > {};

      struct unreserved : sor< ALPHA, DIGIT, one< '-', '.', '_', '~' > > {};
      struct reserved : sor< gen_delims, sub_delims > {};

      struct IPvFuture : if_must< one< 'v' >, plus< HEXDIG >, dot, plus< sor< unreserved, sub_delims, colon > > > {};

      struct IP_literal : if_must< one< '[' >, sor< IPvFuture, IPv6address >, one< ']' > > {};

      struct pct_encoded : if_must< one< '%' >, HEXDIG, HEXDIG > {};
      struct pchar : sor< unreserved, pct_encoded, sub_delims, one< ':', '@' > > {};

      struct query : star< sor< pchar, one< '/', '?' > > > {};
      struct fragment : star< sor< pchar, one< '/', '?' > > > {};

      struct segment : star< pchar > {};
      struct segment_nz : plus< pchar > {};
      struct segment_nz_nc : plus< sor< unreserved, pct_encoded, sub_delims, one< '@' > > > {}; // non-zero-length segment without any colon ":"

      struct path_abempty : star< one< '/' >, segment > {};
      struct path_absolute : seq< one< '/' >, opt< segment_nz, star< one< '/' >, segment > > > {};
      struct path_noscheme : seq< segment_nz_nc, star< one< '/' >, segment > > {};
      struct path_rootless : seq< segment_nz, star< one< '/' >, segment > > {};
      struct path_empty : success {};

      struct path : sor< path_noscheme,     // begins with a non-colon segment
                         path_rootless,     // begins with a segment
                         path_absolute,     // begins with "/" but not "//"
                         path_abempty > {}; // begins with "/" or is empty

      struct reg_name : star< sor< unreserved, pct_encoded, sub_delims > > {};

      struct port : star< DIGIT > {};
      struct host : sor< IP_literal, IPv4address, reg_name > {};
      struct userinfo : star< sor< unreserved, pct_encoded, sub_delims, colon > > {};
      struct authority : seq< opt< userinfo, one< '@' > >, host, opt< colon, port > > {};

      struct scheme : seq< ALPHA, star< sor< ALPHA, DIGIT, one< '+', '-', '.' > > > > {};

      using dslash = two< '/' >;
      using opt_query = opt< if_must< one< '?' >, query > >;
      using opt_fragment = opt< if_must< one< '#' >, fragment > >;

      struct hier_part : sor< if_must< dslash, authority, path_abempty >, path_rootless, path_absolute, path_empty > {};
      struct relative_part : sor< if_must< dslash, authority, path_abempty >, path_noscheme, path_absolute, path_empty > {};
      struct relative_ref : seq< relative_part, opt_query, opt_fragment > {};

      // Newly added. Note: order is very important (longest match first) since
      // the or is sequenced. If you put aaa before aaas, then aaas will never get to
      // match.
      struct iana_scheme : sor<
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

      struct URI : seq< scheme, one< ':' >, hier_part, opt_query, opt_fragment > {};
      struct URI_reference : sor< URI, relative_ref > {};
      struct absolute_URI : seq< scheme, one< ':' >, hier_part, opt_query > {};
      struct iana_URI : seq<iana_scheme, one<':'>, hier_part, opt_query, opt_fragment> {};

   } // uri

} // pegtl

#endif
