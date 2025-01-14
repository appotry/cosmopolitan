/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:4;tab-width:8;coding:utf-8 -*-│
│ vi: set et ft=c ts=4 sts=4 sw=4 fenc=utf-8                               :vi │
╞══════════════════════════════════════════════════════════════════════════════╡
│ Python 3                                                                     │
│ https://docs.python.org/3/license.html                                       │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "third_party/python/Include/yoink.h"
#include "third_party/python/runpythonmodule.h"
#include "libc/cosmo.h"

PYTHON_YOINK("xed");
PYTHON_YOINK("xterm");

PYTHON_YOINK("encodings");
PYTHON_YOINK("encodings.aliases");
PYTHON_YOINK("encodings.ascii");
PYTHON_YOINK("encodings.idna");
PYTHON_YOINK("encodings.latin_1");
PYTHON_YOINK("encodings.utf_8");
PYTHON_YOINK("encodings.utf_8_sig");
PYTHON_YOINK("encodings.utf_16");
PYTHON_YOINK("encodings.utf_16_be");
PYTHON_YOINK("encodings.utf_16_le");
PYTHON_YOINK("encodings.utf_32");
PYTHON_YOINK("encodings.utf_32_be");
PYTHON_YOINK("encodings.utf_32_le");
PYTHON_YOINK("encodings.uu_codec");
PYTHON_YOINK("encodings.undefined");
PYTHON_YOINK("encodings.hex_codec");
PYTHON_YOINK("encodings.base64_codec");
PYTHON_YOINK("encodings.unicode_escape");
PYTHON_YOINK("encodings.unicode_internal");
PYTHON_YOINK("encodings.raw_unicode_escape");
PYTHON_YOINK("encodings.zlib_codec");
PYTHON_YOINK("encodings.big5");
PYTHON_YOINK("encodings.big5hkscs");
PYTHON_YOINK("encodings.bz2_codec");
PYTHON_YOINK("encodings.charmap");
PYTHON_YOINK("encodings.cp037");
PYTHON_YOINK("encodings.cp1006");
PYTHON_YOINK("encodings.cp1026");
PYTHON_YOINK("encodings.cp1125");
PYTHON_YOINK("encodings.cp1140");
PYTHON_YOINK("encodings.cp1250");
PYTHON_YOINK("encodings.cp1251");
PYTHON_YOINK("encodings.cp1252");
PYTHON_YOINK("encodings.cp1253");
PYTHON_YOINK("encodings.cp1254");
PYTHON_YOINK("encodings.cp1255");
PYTHON_YOINK("encodings.cp1256");
PYTHON_YOINK("encodings.cp1257");
PYTHON_YOINK("encodings.cp1258");
PYTHON_YOINK("encodings.cp273");
PYTHON_YOINK("encodings.cp424");
PYTHON_YOINK("encodings.cp437");
PYTHON_YOINK("encodings.cp500");
PYTHON_YOINK("encodings.cp720");
PYTHON_YOINK("encodings.cp737");
PYTHON_YOINK("encodings.cp775");
PYTHON_YOINK("encodings.cp850");
PYTHON_YOINK("encodings.cp852");
PYTHON_YOINK("encodings.cp855");
PYTHON_YOINK("encodings.cp856");
PYTHON_YOINK("encodings.cp857");
PYTHON_YOINK("encodings.cp858");
PYTHON_YOINK("encodings.cp860");
PYTHON_YOINK("encodings.cp861");
PYTHON_YOINK("encodings.cp862");
PYTHON_YOINK("encodings.cp863");
PYTHON_YOINK("encodings.cp864");
PYTHON_YOINK("encodings.cp865");
PYTHON_YOINK("encodings.cp866");
PYTHON_YOINK("encodings.cp869");
PYTHON_YOINK("encodings.cp874");
PYTHON_YOINK("encodings.cp875");
PYTHON_YOINK("encodings.cp932");
PYTHON_YOINK("encodings.cp949");
PYTHON_YOINK("encodings.cp950");
PYTHON_YOINK("encodings.euc_jis_2004");
PYTHON_YOINK("encodings.euc_jisx0213");
PYTHON_YOINK("encodings.euc_jp");
PYTHON_YOINK("encodings.euc_kr");
PYTHON_YOINK("encodings.gb18030");
PYTHON_YOINK("encodings.gb2312");
PYTHON_YOINK("encodings.gbk");
PYTHON_YOINK("encodings.hp_roman8");
PYTHON_YOINK("encodings.hz");
PYTHON_YOINK("encodings.iso2022_jp");
PYTHON_YOINK("encodings.iso2022_jp_1");
PYTHON_YOINK("encodings.iso2022_jp_2");
PYTHON_YOINK("encodings.iso2022_jp_2004");
PYTHON_YOINK("encodings.iso2022_jp_3");
PYTHON_YOINK("encodings.iso2022_jp_ext");
PYTHON_YOINK("encodings.iso2022_kr");
PYTHON_YOINK("encodings.iso8859_1");
PYTHON_YOINK("encodings.iso8859_10");
PYTHON_YOINK("encodings.iso8859_11");
PYTHON_YOINK("encodings.iso8859_13");
PYTHON_YOINK("encodings.iso8859_14");
PYTHON_YOINK("encodings.iso8859_15");
PYTHON_YOINK("encodings.iso8859_16");
PYTHON_YOINK("encodings.iso8859_2");
PYTHON_YOINK("encodings.iso8859_3");
PYTHON_YOINK("encodings.iso8859_4");
PYTHON_YOINK("encodings.iso8859_5");
PYTHON_YOINK("encodings.iso8859_6");
PYTHON_YOINK("encodings.iso8859_7");
PYTHON_YOINK("encodings.iso8859_8");
PYTHON_YOINK("encodings.iso8859_9");
PYTHON_YOINK("encodings.johab");
PYTHON_YOINK("encodings.koi8_r");
PYTHON_YOINK("encodings.koi8_t");
PYTHON_YOINK("encodings.koi8_u");
PYTHON_YOINK("encodings.kz1048");
PYTHON_YOINK("encodings.mac_arabic");
PYTHON_YOINK("encodings.mac_centeuro");
PYTHON_YOINK("encodings.mac_croatian");
PYTHON_YOINK("encodings.mac_cyrillic");
PYTHON_YOINK("encodings.mac_farsi");
PYTHON_YOINK("encodings.mac_greek");
PYTHON_YOINK("encodings.mac_iceland");
PYTHON_YOINK("encodings.mac_latin2");
PYTHON_YOINK("encodings.mac_roman");
PYTHON_YOINK("encodings.mac_romanian");
PYTHON_YOINK("encodings.mac_turkish");
PYTHON_YOINK("encodings.palmos");
PYTHON_YOINK("encodings.ptcp154");
PYTHON_YOINK("encodings.punycode");
PYTHON_YOINK("encodings.quopri_codec");
PYTHON_YOINK("encodings.rot_13");
PYTHON_YOINK("encodings.shift_jis");
PYTHON_YOINK("encodings.shift_jis_2004");
PYTHON_YOINK("encodings.shift_jisx0213");
PYTHON_YOINK("encodings.tis_620");
PYTHON_YOINK("encodings.utf_7");

#ifndef TINY
PYTHON_YOINK("pydoc_data.topics");
__static_yoink(".python/pydoc_data/_pydoc.css");
#endif

/* todo(jart): delete nonessential yoinks */

__static_yoink("_PyImport_Inittab__bisect");
__static_yoink("_PyImport_Inittab__codecs_cn");
__static_yoink("_PyImport_Inittab__codecs_hk");
__static_yoink("_PyImport_Inittab__codecs_iso2022");
__static_yoink("_PyImport_Inittab__codecs_jp");
__static_yoink("_PyImport_Inittab__codecs_kr");
__static_yoink("_PyImport_Inittab__codecs_tw");
__static_yoink("_PyImport_Inittab__csv");
__static_yoink("_PyImport_Inittab__datetime");
__static_yoink("_PyImport_Inittab__decimal");
__static_yoink("_PyImport_Inittab__elementtree");
__static_yoink("_PyImport_Inittab__json");
__static_yoink("_PyImport_Inittab__lsprof");
__static_yoink("_PyImport_Inittab__multibytecodec");
__static_yoink("_PyImport_Inittab__multiprocessing");
__static_yoink("_PyImport_Inittab__opcode");
__static_yoink("_PyImport_Inittab__pickle");
__static_yoink("_PyImport_Inittab__posixsubprocess");
__static_yoink("_PyImport_Inittab__random");
__static_yoink("_PyImport_Inittab__socket");
__static_yoink("_PyImport_Inittab__sqlite3");
__static_yoink("_PyImport_Inittab__stat");
__static_yoink("_PyImport_Inittab__struct");
__static_yoink("_PyImport_Inittab__symtable");
__static_yoink("_PyImport_Inittab__testcapi");
__static_yoink("_PyImport_Inittab__tracemalloc");
__static_yoink("_PyImport_Inittab_array");
__static_yoink("_PyImport_Inittab_atexit");
__static_yoink("_PyImport_Inittab_audioop");
__static_yoink("_PyImport_Inittab_binascii");
__static_yoink("_PyImport_Inittab_cmath");
__static_yoink("_PyImport_Inittab_faulthandler");
__static_yoink("_PyImport_Inittab_fcntl");
__static_yoink("_PyImport_Inittab_fpectl");
__static_yoink("_PyImport_Inittab_grp");
__static_yoink("_PyImport_Inittab_math");
__static_yoink("_PyImport_Inittab_mmap");
__static_yoink("_PyImport_Inittab_parser");
__static_yoink("_PyImport_Inittab_pwd");
__static_yoink("_PyImport_Inittab_pyexpat");
__static_yoink("_PyImport_Inittab_resource");
__static_yoink("_PyImport_Inittab_select");
__static_yoink("_PyImport_Inittab_syslog");
__static_yoink("_PyImport_Inittab_termios");
__static_yoink("_PyImport_Inittab_time");
__static_yoink("_PyImport_Inittab_unicodedata");
__static_yoink("_PyImport_Inittab_zipimport");

PYTHON_YOINK("__future__");
PYTHON_YOINK("_bootlocale");
PYTHON_YOINK("_collections_abc");
PYTHON_YOINK("_compat_pickle");
PYTHON_YOINK("_compression");
PYTHON_YOINK("_dummy_thread");
PYTHON_YOINK("_markupbase");
PYTHON_YOINK("_pyio");
PYTHON_YOINK("_sitebuiltins");
PYTHON_YOINK("_strptime");
PYTHON_YOINK("_sysconfigdata_m_cosmo_x86_64_cosmo");
PYTHON_YOINK("_threading_local");
PYTHON_YOINK("_weakrefset");
PYTHON_YOINK("abc");
PYTHON_YOINK("argparse");
PYTHON_YOINK("ast");
PYTHON_YOINK("base64");
PYTHON_YOINK("bdb");
PYTHON_YOINK("binhex");
PYTHON_YOINK("bisect");
PYTHON_YOINK("calendar");
PYTHON_YOINK("chunk");
PYTHON_YOINK("cmd");
PYTHON_YOINK("code");
PYTHON_YOINK("codecs");
PYTHON_YOINK("colorsys");
PYTHON_YOINK("configparser");
PYTHON_YOINK("contextlib");
PYTHON_YOINK("copy");
PYTHON_YOINK("copyreg");
PYTHON_YOINK("csv");
PYTHON_YOINK("datetime");
PYTHON_YOINK("decimal");
PYTHON_YOINK("difflib");
PYTHON_YOINK("doctest");
PYTHON_YOINK("dummy_threading");
PYTHON_YOINK("enum");
PYTHON_YOINK("filecmp");
PYTHON_YOINK("fileinput");
PYTHON_YOINK("fnmatch");
PYTHON_YOINK("formatter");
PYTHON_YOINK("fractions");
PYTHON_YOINK("functools");
PYTHON_YOINK("genericpath");
PYTHON_YOINK("getopt");
PYTHON_YOINK("getpass");
PYTHON_YOINK("gettext");
PYTHON_YOINK("glob");
PYTHON_YOINK("hashlib");
PYTHON_YOINK("heapq");
PYTHON_YOINK("hmac");
PYTHON_YOINK("imghdr");
PYTHON_YOINK("imp");
PYTHON_YOINK("io");
PYTHON_YOINK("ipaddress");
PYTHON_YOINK("keyword");
PYTHON_YOINK("linecache");
PYTHON_YOINK("locale");
PYTHON_YOINK("macpath");
PYTHON_YOINK("macurl2path");
PYTHON_YOINK("mimetypes");
PYTHON_YOINK("modulefinder");
PYTHON_YOINK("netrc");
PYTHON_YOINK("ntpath");
PYTHON_YOINK("nturl2path");
PYTHON_YOINK("numbers");
PYTHON_YOINK("opcode");
PYTHON_YOINK("operator");
PYTHON_YOINK("optparse");
PYTHON_YOINK("os");
PYTHON_YOINK("pathlib");
PYTHON_YOINK("pickle");
PYTHON_YOINK("pickletools");
PYTHON_YOINK("pipes");
PYTHON_YOINK("pkgutil");
PYTHON_YOINK("platform");
PYTHON_YOINK("plistlib");
PYTHON_YOINK("posixpath");
PYTHON_YOINK("pprint");
PYTHON_YOINK("pstats");
PYTHON_YOINK("pty");
PYTHON_YOINK("pyclbr");
PYTHON_YOINK("queue");
PYTHON_YOINK("quopri");
PYTHON_YOINK("random");
PYTHON_YOINK("re");
PYTHON_YOINK("reprlib");
PYTHON_YOINK("runpy");
PYTHON_YOINK("sched");
PYTHON_YOINK("secrets");
PYTHON_YOINK("selectors");
PYTHON_YOINK("shelve");
PYTHON_YOINK("shlex");
PYTHON_YOINK("shutil");
PYTHON_YOINK("signal");
PYTHON_YOINK("site");
PYTHON_YOINK("sndhdr");
PYTHON_YOINK("socket");
PYTHON_YOINK("socketserver");
PYTHON_YOINK("sre_compile");
PYTHON_YOINK("sre_constants");
PYTHON_YOINK("sre_parse");
PYTHON_YOINK("stat");
PYTHON_YOINK("statistics");
PYTHON_YOINK("string");
PYTHON_YOINK("stringprep");
PYTHON_YOINK("struct");
PYTHON_YOINK("subprocess");
PYTHON_YOINK("symbol");
PYTHON_YOINK("symtable");
PYTHON_YOINK("sysconfig");
PYTHON_YOINK("tabnanny");
PYTHON_YOINK("tempfile");
PYTHON_YOINK("textwrap");
PYTHON_YOINK("this");
PYTHON_YOINK("threading");
PYTHON_YOINK("token");
PYTHON_YOINK("tokenbucket");
PYTHON_YOINK("tokenize");
PYTHON_YOINK("trace");
PYTHON_YOINK("traceback");
PYTHON_YOINK("tty");
PYTHON_YOINK("types");
PYTHON_YOINK("typing");
PYTHON_YOINK("uu");
PYTHON_YOINK("uuid");
PYTHON_YOINK("warnings");
PYTHON_YOINK("weakref");
PYTHON_YOINK("webbrowser");
PYTHON_YOINK("xdrlib");

PYTHON_YOINK("aifc");
PYTHON_YOINK("wave");
PYTHON_YOINK("sunau");

PYTHON_YOINK("dis");
PYTHON_YOINK("codeop");
PYTHON_YOINK("compileall");
PYTHON_YOINK("py_compile");

PYTHON_YOINK("cgi");
PYTHON_YOINK("pdb");
PYTHON_YOINK("cgitb");
PYTHON_YOINK("pydoc");
PYTHON_YOINK("timeit");
PYTHON_YOINK("profile");
PYTHON_YOINK("inspect");
PYTHON_YOINK("cProfile");
PYTHON_YOINK("tracemalloc");

PYTHON_YOINK("bz2");
PYTHON_YOINK("gzip");
PYTHON_YOINK("zipapp");
PYTHON_YOINK("ftplib");
PYTHON_YOINK("tarfile");
PYTHON_YOINK("zipfile");
PYTHON_YOINK("telnetlib");
PYTHON_YOINK("antigravity");

PYTHON_YOINK("collections");
PYTHON_YOINK("collections.abc");

PYTHON_YOINK("json");
PYTHON_YOINK("json.decoder");
PYTHON_YOINK("json.encoder");
PYTHON_YOINK("json.scanner");
PYTHON_YOINK("json.tool");

PYTHON_YOINK("html");
PYTHON_YOINK("html.entities");
PYTHON_YOINK("html.parser");

PYTHON_YOINK("http");
PYTHON_YOINK("http.client");
PYTHON_YOINK("http.cookiejar");
PYTHON_YOINK("http.cookies");
PYTHON_YOINK("http.server");

PYTHON_YOINK("email");
PYTHON_YOINK("email._encoded_words");
PYTHON_YOINK("email._header_value_parser");
PYTHON_YOINK("email._parseaddr");
PYTHON_YOINK("email._policybase");
PYTHON_YOINK("email.base64mime");
PYTHON_YOINK("email.charset");
PYTHON_YOINK("email.contentmanager");
PYTHON_YOINK("email.encoders");
PYTHON_YOINK("email.errors");
PYTHON_YOINK("email.feedparser");
PYTHON_YOINK("email.generator");
PYTHON_YOINK("email.header");
PYTHON_YOINK("email.headerregistry");
PYTHON_YOINK("email.iterators");
PYTHON_YOINK("email.message");
PYTHON_YOINK("email.mime");
PYTHON_YOINK("email.mime.application");
PYTHON_YOINK("email.mime.audio");
PYTHON_YOINK("email.mime.base");
PYTHON_YOINK("email.mime.image");
PYTHON_YOINK("email.mime.message");
PYTHON_YOINK("email.mime.multipart");
PYTHON_YOINK("email.mime.nonmultipart");
PYTHON_YOINK("email.mime.text");
PYTHON_YOINK("email.parser");
PYTHON_YOINK("email.policy");
PYTHON_YOINK("email.quoprimime");
PYTHON_YOINK("email.utils");
__static_yoink(".python/email/architecture.rst");

PYTHON_YOINK("importlib");
PYTHON_YOINK("importlib.abc");
PYTHON_YOINK("importlib.machinery");
PYTHON_YOINK("importlib.util");

PYTHON_YOINK("logging");
PYTHON_YOINK("logging.config");
PYTHON_YOINK("logging.handlers");

PYTHON_YOINK("urllib");
PYTHON_YOINK("urllib.error");
PYTHON_YOINK("urllib.parse");
PYTHON_YOINK("urllib.request");
PYTHON_YOINK("urllib.response");
PYTHON_YOINK("urllib.robotparser");

PYTHON_YOINK("wsgiref");
PYTHON_YOINK("wsgiref.handlers");
PYTHON_YOINK("wsgiref.headers");
PYTHON_YOINK("wsgiref.simple_server");
PYTHON_YOINK("wsgiref.util");
PYTHON_YOINK("wsgiref.validate");

PYTHON_YOINK("sqlite3");
PYTHON_YOINK("sqlite3.dbapi2");
PYTHON_YOINK("sqlite3.dump");

PYTHON_YOINK("xml");
PYTHON_YOINK("xml.dom.NodeFilter");
PYTHON_YOINK("xml.dom");
PYTHON_YOINK("xml.dom.domreg");
PYTHON_YOINK("xml.dom.expatbuilder");
PYTHON_YOINK("xml.dom.minicompat");
PYTHON_YOINK("xml.dom.minidom");
PYTHON_YOINK("xml.dom.pulldom");
PYTHON_YOINK("xml.dom.xmlbuilder");
PYTHON_YOINK("xml.etree.ElementInclude");
PYTHON_YOINK("xml.etree.ElementPath");
PYTHON_YOINK("xml.etree.ElementTree");
PYTHON_YOINK("xml.etree");
PYTHON_YOINK("xml.etree.cElementTree");
PYTHON_YOINK("xml.parsers");
PYTHON_YOINK("xml.parsers.expat");
PYTHON_YOINK("xml.sax");
PYTHON_YOINK("xml.sax._exceptions");
PYTHON_YOINK("xml.sax.expatreader");
PYTHON_YOINK("xml.sax.handler");
PYTHON_YOINK("xml.sax.saxutils");
PYTHON_YOINK("xml.sax.xmlreader");
PYTHON_YOINK("xmlrpc");
PYTHON_YOINK("xmlrpc.client");
PYTHON_YOINK("xmlrpc.server");

PYTHON_YOINK("multiprocessing");
PYTHON_YOINK("multiprocessing.connection");
PYTHON_YOINK("multiprocessing.context");
PYTHON_YOINK("multiprocessing.dummy");
PYTHON_YOINK("multiprocessing.dummy.connection");
PYTHON_YOINK("multiprocessing.forkserver");
PYTHON_YOINK("multiprocessing.heap");
PYTHON_YOINK("multiprocessing.managers");
PYTHON_YOINK("multiprocessing.pool");
PYTHON_YOINK("multiprocessing.popen_fork");
PYTHON_YOINK("multiprocessing.popen_forkserver");
PYTHON_YOINK("multiprocessing.popen_spawn_posix");
PYTHON_YOINK("multiprocessing.popen_spawn_win32");
PYTHON_YOINK("multiprocessing.process");
PYTHON_YOINK("multiprocessing.queues");
PYTHON_YOINK("multiprocessing.reduction");
PYTHON_YOINK("multiprocessing.resource_sharer");
PYTHON_YOINK("multiprocessing.semaphore_tracker");
PYTHON_YOINK("multiprocessing.spawn");
PYTHON_YOINK("multiprocessing.synchronize");
PYTHON_YOINK("multiprocessing.util");

PYTHON_YOINK("unittest");
PYTHON_YOINK("unittest.__main__");
PYTHON_YOINK("unittest.case");
PYTHON_YOINK("unittest.loader");
PYTHON_YOINK("unittest.main");
PYTHON_YOINK("unittest.mock");
PYTHON_YOINK("unittest.result");
PYTHON_YOINK("unittest.runner");
PYTHON_YOINK("unittest.signals");
PYTHON_YOINK("unittest.suite");
PYTHON_YOINK("unittest.util");

PYTHON_YOINK("smtpd");
PYTHON_YOINK("poplib");
PYTHON_YOINK("imaplib");
PYTHON_YOINK("mailbox");
PYTHON_YOINK("mailcap");
PYTHON_YOINK("smtplib");
PYTHON_YOINK("nntplib");

PYTHON_YOINK("asdl");

PYTHON_YOINK("_thread");
PYTHON_YOINK("_thread.LockType");
PYTHON_YOINK("_thread.RLock");
PYTHON_YOINK("_thread.TIMEOUT_MAX");
PYTHON_YOINK("_thread._count");
PYTHON_YOINK("_thread._local");
PYTHON_YOINK("_thread._set_sentinel");
PYTHON_YOINK("_thread.allocate");
PYTHON_YOINK("_thread.allocate_lock");
PYTHON_YOINK("_thread.error");
PYTHON_YOINK("_thread.exit");
PYTHON_YOINK("_thread.exit_thread");
PYTHON_YOINK("_thread.get_ident");
PYTHON_YOINK("_thread.interrupt_main");
PYTHON_YOINK("_thread.stack_size");
PYTHON_YOINK("_thread.start_new");
PYTHON_YOINK("_thread.start_new_thread");
PYTHON_YOINK("concurrent");
PYTHON_YOINK("concurrent.futures");
PYTHON_YOINK("concurrent.futures._base");
PYTHON_YOINK("concurrent.futures.process");
PYTHON_YOINK("concurrent.futures.thread");
PYTHON_YOINK("asynchat");
PYTHON_YOINK("asyncore");
PYTHON_YOINK("asyncio");
PYTHON_YOINK("asyncio.base_events");
PYTHON_YOINK("asyncio.base_futures");
PYTHON_YOINK("asyncio.base_subprocess");
PYTHON_YOINK("asyncio.base_tasks");
PYTHON_YOINK("asyncio.compat");
PYTHON_YOINK("asyncio.constants");
PYTHON_YOINK("asyncio.coroutines");
PYTHON_YOINK("asyncio.events");
PYTHON_YOINK("asyncio.futures");
PYTHON_YOINK("asyncio.locks");
PYTHON_YOINK("asyncio.log");
PYTHON_YOINK("asyncio.proactor_events");
PYTHON_YOINK("asyncio.protocols");
PYTHON_YOINK("asyncio.queues");
PYTHON_YOINK("asyncio.selector_events");
PYTHON_YOINK("asyncio.sslproto");
PYTHON_YOINK("asyncio.streams");
PYTHON_YOINK("asyncio.subprocess");
PYTHON_YOINK("asyncio.tasks");
PYTHON_YOINK("asyncio.transports");
PYTHON_YOINK("asyncio.unix_events");

int
main(int argc, char **argv)
{
    LoadZipArgs(&argc, &argv);
    return RunPythonModule(argc, argv);
}
