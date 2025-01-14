/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:4;tab-width:8;coding:utf-8 -*-│
│ vi: set et ft=c ts=4 sts=4 sw=4 fenc=utf-8                               :vi │
╞══════════════════════════════════════════════════════════════════════════════╡
│ Python 3                                                                     │
│ https://docs.python.org/3/license.html                                       │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/calls/calls.h"
#include "libc/dce.h"
#include "libc/errno.h"
#include "libc/stdio/stdio.h"
#include "libc/str/locale.h"
#include "libc/sysv/consts/s.h"
#include "third_party/python/Include/abstract.h"
#include "third_party/python/Include/bytesobject.h"
#include "third_party/python/Include/ceval.h"
#include "third_party/python/Include/fileutils.h"
#include "third_party/python/Include/import.h"
#include "third_party/python/Include/modsupport.h"
#include "third_party/python/Include/osdefs.h"
#include "third_party/python/Include/patchlevel.h"
#include "third_party/python/Include/pgenheaders.h"
#include "third_party/python/Include/pydebug.h"
#include "third_party/python/Include/pyerrors.h"
#include "third_party/python/Include/pylifecycle.h"
#include "third_party/python/Include/pymem.h"
#include "third_party/python/Include/pythonrun.h"
#include "third_party/python/Include/sysmodule.h"
#include "third_party/python/Include/yoink.h"
#include "third_party/python/pyconfig.h"

__static_yoink("PyInit__codecs");      // for pylifecycle.o
__static_yoink("PyInit__collections"); // for pylifecycle.o
__static_yoink("PyInit__functools");   // for pylifecycle.o
__static_yoink("PyInit__locale");      // for pylifecycle.o
__static_yoink("PyInit__operator");    // for pylifecycle.o
__static_yoink("PyInit__signal");      // for pylifecycle.o
__static_yoink("PyInit__sre");         // for pylifecycle.o
__static_yoink("PyInit__stat");        // for pylifecycle.o
__static_yoink("PyInit_errno");        // for pylifecycle.o
__static_yoink("PyInit_itertools");    // for pylifecycle.o

PYTHON_YOINK("site");                // for pylifecycle.o
PYTHON_YOINK("struct");              // for memoryobject.o

PYTHON_YOINK("io");
PYTHON_YOINK("encodings.aliases");
PYTHON_YOINK("encodings.latin_1");
PYTHON_YOINK("encodings.utf_8");

PYTHON_YOINK("site");
PYTHON_YOINK("_sysconfigdata_m_cosmo_x86_64_cosmo");

PYTHON_YOINK("_bootlocale");
PYTHON_YOINK("_warnings");
PYTHON_YOINK("_locale");
PYTHON_YOINK("locale");
PYTHON_YOINK("runpy");

/* Python interpreter main program */

#if defined(MS_WINDOWS)
#define PYTHONHOMEHELP "<prefix>\\python{major}{minor}"
#else
#define PYTHONHOMEHELP "<prefix>/lib/pythonX.X"
#endif

#include "third_party/python/Include/pygetopt.h"

#define COPYRIGHT \
    "Type \"help\", \"copyright\", \"credits\" or \"license\" " \
    "for more information."

/* For Py_GetArgcArgv(); set by main() */
static wchar_t **orig_argv;
static int  orig_argc;

/* command line options */
#define BASE_OPTS L"bBc:dEhiIJm:OqRsStuvVW:xX:?"

#define PROGRAM_OPTS BASE_OPTS

/* Short usage message (with %s for argv0) */
static const char usage_line[] =
"usage: %ls [option] ... [-c cmd | -m mod | file | -] [arg] ...\n";

/* Long usage message, split into parts < 512 bytes */
static const char usage_1[] = "\
Options and arguments (and corresponding environment variables):\n\
-b     : issue warnings about str(bytes_instance), str(bytearray_instance)\n\
         and comparing bytes/bytearray with str. (-bb: issue errors)\n\
-B     : don't write .pyc files on import; also PYTHONDONTWRITEBYTECODE=x\n\
-c cmd : program passed in as string (terminates option list)\n\
-d     : debug output from parser; also PYTHONDEBUG=x\n\
-E     : ignore PYTHON* environment variables (such as PYTHONPATH)\n\
-h     : print this help message and exit (also --help)\n\
";
static const char usage_2[] = "\
-i     : inspect interactively after running script; forces a prompt even\n\
         if stdin does not appear to be a terminal; also PYTHONINSPECT=x\n\
-I     : isolate Python from the user's environment (implies -E and -s)\n\
-m mod : run library module as a script (terminates option list)\n\
-O     : remove assert and __debug__-dependent statements; add .opt-1 before\n\
         .pyc extension; also PYTHONOPTIMIZE=x\n\
-OO    : do -O changes and also discard docstrings; add .opt-2 before\n\
         .pyc extension\n\
-q     : don't print version and copyright messages on interactive startup\n\
-s     : don't add user site directory to sys.path; also PYTHONNOUSERSITE\n\
-S     : don't imply 'import site' on initialization\n\
";
static const char usage_3[] = "\
-u     : force the binary I/O layers of stdout and stderr to be unbuffered;\n\
         stdin is always buffered; text I/O layer will be line-buffered;\n\
         also PYTHONUNBUFFERED=x\n\
-v     : verbose (trace import statements); also PYTHONVERBOSE=x\n\
         can be supplied multiple times to increase verbosity\n\
-V     : print the Python version number and exit (also --version)\n\
         when given twice, print more information about the build\n\
-W arg : warning control; arg is action:message:category:module:lineno\n\
         also PYTHONWARNINGS=arg\n\
-x     : skip first line of source, allowing use of non-Unix forms of #!cmd\n\
-X opt : set implementation-specific option\n\
";
static const char usage_4[] = "\
file   : program read from script file\n\
-      : program read from stdin (default; interactive mode if a tty)\n\
arg ...: arguments passed to program in sys.argv[1:]\n\n\
Other environment variables:\n\
PYTHONSTARTUP: file executed on interactive startup (no default)\n\
PYTHONPATH   : '%lc'-separated list of directories prefixed to the\n\
               default module search path.  The result is sys.path.\n\
";
static const char usage_5[] =
"PYTHONHOME   : alternate <prefix> directory (or <prefix>%lc<exec_prefix>).\n"
"               The default module search path uses %s.\n"
"PYTHONCASEOK : ignore case in 'import' statements (Windows).\n"
"PYTHONIOENCODING: Encoding[:errors] used for stdin/stdout/stderr.\n"
"PYTHONFAULTHANDLER: dump the Python traceback on fatal errors.\n";
static const char usage_6[] =
"PYTHONHASHSEED: if this variable is set to 'random', a random value is used\n"
"   to seed the hashes of str, bytes and datetime objects.  It can also be\n"
"   set to an integer in the range [0,4294967295] to get hash values with a\n"
"   predictable seed.\n"
"PYTHONMALLOC: set the Python memory allocators and/or install debug hooks\n"
"   on Python memory allocators. Use PYTHONMALLOC=debug to install debug\n"
"   hooks.\n";

static int
usage(int exitcode, const wchar_t* program)
{
    FILE *f = exitcode ? stderr : stdout;

    fprintf(f, usage_line, program);
    if (exitcode)
        fprintf(f, "Try `python -h' for more information.\n");
    else {
        fputs(usage_1, f);
        fputs(usage_2, f);
        fputs(usage_3, f);
        fprintf(f, usage_4, (wint_t)DELIM);
        fprintf(f, usage_5, (wint_t)DELIM, PYTHONHOMEHELP);
        fputs(usage_6, f);
    }
    return exitcode;
}

static void RunStartupFile(PyCompilerFlags *cf)
{
    char *startup = Py_GETENV("PYTHONSTARTUP");
    if (startup != NULL && startup[0] != '\0') {
        FILE *fp = _Py_fopen(startup, "r");
        if (fp != NULL) {
            (void) PyRun_SimpleFileExFlags(fp, startup, 0, cf);
            PyErr_Clear();
            fclose(fp);
        } else {
            int save_errno;

            save_errno = errno;
            PySys_WriteStderr("Could not open PYTHONSTARTUP\n");
            errno = save_errno;
            PyErr_SetFromErrnoWithFilename(PyExc_IOError,
                            startup);
            PyErr_Print();
            PyErr_Clear();
        }
    }
}

static void RunInteractiveHook(void)
{
    PyObject *sys, *hook, *result;
    sys = PyImport_ImportModule("sys");
    if (sys == NULL)
        goto error;
    hook = PyObject_GetAttrString(sys, "__interactivehook__");
    Py_DECREF(sys);
    if (hook == NULL)
        PyErr_Clear();
    else {
        result = PyObject_CallObject(hook, NULL);
        Py_DECREF(hook);
        if (result == NULL)
            goto error;
        else
            Py_DECREF(result);
    }
    return;

error:
    PySys_WriteStderr("Failed calling sys.__interactivehook__\n");
    PyErr_Print();
    PyErr_Clear();
}


static int RunModule(wchar_t *modname, int set_argv0)
{
    PyObject *module, *runpy, *runmodule, *runargs, *result;
    runpy = PyImport_ImportModule("runpy");
    if (runpy == NULL) {
        fprintf(stderr, "Could not import runpy module\n");
        PyErr_Print();
        return -1;
    }
    runmodule = PyObject_GetAttrString(runpy, "_run_module_as_main");
    if (runmodule == NULL) {
        fprintf(stderr, "Could not access runpy._run_module_as_main\n");
        PyErr_Print();
        Py_DECREF(runpy);
        return -1;
    }
    module = PyUnicode_FromWideChar(modname, wcslen(modname));
    if (module == NULL) {
        fprintf(stderr, "Could not convert module name to unicode\n");
        PyErr_Print();
        Py_DECREF(runpy);
        Py_DECREF(runmodule);
        return -1;
    }
    runargs = Py_BuildValue("(Oi)", module, set_argv0);
    if (runargs == NULL) {
        fprintf(stderr,
            "Could not create arguments for runpy._run_module_as_main\n");
        PyErr_Print();
        Py_DECREF(runpy);
        Py_DECREF(runmodule);
        Py_DECREF(module);
        return -1;
    }
    result = PyObject_Call(runmodule, runargs, NULL);
    if (result == NULL) {
        PyErr_Print();
    }
    Py_DECREF(runpy);
    Py_DECREF(runmodule);
    Py_DECREF(module);
    Py_DECREF(runargs);
    if (result == NULL) {
        return -1;
    }
    Py_DECREF(result);
    return 0;
}

static PyObject *
AsImportPathEntry(wchar_t *filename)
{
    PyObject *sys_path0 = NULL, *importer;

    sys_path0 = PyUnicode_FromWideChar(filename, wcslen(filename));
    if (sys_path0 == NULL)
        goto error;

    importer = PyImport_GetImporter(sys_path0);
    if (importer == NULL)
        goto error;

    if (importer == Py_None) {
        Py_DECREF(sys_path0);
        Py_DECREF(importer);
        return NULL;
    }
    Py_DECREF(importer);
    return sys_path0;

error:
    Py_XDECREF(sys_path0);
    PySys_WriteStderr("Failed checking if argv[0] is an import path entry\n");
    PyErr_Print();
    PyErr_Clear();
    return NULL;
}


static int
RunMainFromImporter(PyObject *sys_path0)
{
    PyObject *sys_path;
    int sts;

    /* Assume sys_path0 has already been checked by AsImportPathEntry,
     * so put it in sys.path[0] and import __main__ */
    sys_path = PySys_GetObject("path");
    if (sys_path == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to get sys.path");
        goto error;
    }
    sts = PyList_Insert(sys_path, 0, sys_path0);
    if (sts) {
        sys_path0 = NULL;
        goto error;
    }

    sts = RunModule(L"__main__", 0);
    return sts != 0;

error:
    Py_XDECREF(sys_path0);
    PyErr_Print();
    return 1;
}

static int
run_command(wchar_t *command, PyCompilerFlags *cf)
{
    PyObject *unicode, *bytes;
    int ret;

    unicode = PyUnicode_FromWideChar(command, -1);
    if (unicode == NULL)
        goto error;
    bytes = PyUnicode_AsUTF8String(unicode);
    Py_DECREF(unicode);
    if (bytes == NULL)
        goto error;
    ret = PyRun_SimpleStringFlags(PyBytes_AsString(bytes), cf);
    Py_DECREF(bytes);
    return ret != 0;

error:
    PySys_WriteStderr("Unable to decode the command from the command line:\n");
    PyErr_Print();
    return 1;
}

static int
run_file(FILE *fp, const wchar_t *filename, PyCompilerFlags *p_cf)
{
    PyObject *unicode, *bytes = NULL;
    char *filename_str;
    int run;

    /* call pending calls like signal handlers (SIGINT) */
    if (Py_MakePendingCalls() == -1) {
        PyErr_Print();
        return 1;
    }

    if (filename) {
        unicode = PyUnicode_FromWideChar(filename, wcslen(filename));
        if (unicode != NULL) {
            bytes = PyUnicode_EncodeFSDefault(unicode);
            Py_DECREF(unicode);
        }
        if (bytes != NULL)
            filename_str = PyBytes_AsString(bytes);
        else {
            PyErr_Clear();
            filename_str = "<encoding error>";
        }
    }
    else
        filename_str = "<stdin>";

    run = PyRun_AnyFileExFlags(fp, filename_str, filename != NULL, p_cf);
    Py_XDECREF(bytes);
    return run != 0;
}


/* Main program */

int
Py_Main(int argc, wchar_t **argv)
{
    int c;
    int sts;
    wchar_t *command = NULL;
    wchar_t *filename = NULL;
    wchar_t *module = NULL;
    FILE *fp = stdin;
    char *p;
#ifdef MS_WINDOWS
    wchar_t *wp;
#endif
    int skipfirstline = 0;
    int stdin_is_interactive = 0;
    int help = 0;
    int version = 0;
    int saw_unbuffered_flag = 0;
    char *opt;
    PyCompilerFlags cf;
    PyObject *main_importer_path = NULL;
    PyObject *warning_option = NULL;
    PyObject *warning_options = NULL;

    (void)opt;

    cf.cf_flags = 0;

    orig_argc = argc;           /* For Py_GetArgcArgv() */
    orig_argv = argv;

    /* Hash randomization needed early for all string operations
       (including -W and -X options). */
    _PyOS_opterr = 0;  /* prevent printing the error in 1st pass */
    while ((c = _PyOS_GetOpt(argc, argv, PROGRAM_OPTS)) != EOF) {
        if (c == 'm' || c == 'c') {
            /* -c / -m is the last option: following arguments are
               not interpreter options. */
            break;
        }
        if (c == 'E') {
            Py_IgnoreEnvironmentFlag++;
            break;
        }
    }

#if IsModeDbg()
    opt = Py_GETENV("PYTHONMALLOC");
    if (_PyMem_SetupAllocators(opt) < 0) {
        fprintf(stderr,
                "Error in PYTHONMALLOC: unknown allocator \"%s\"!\n", opt);
        exit(1);
    }
#endif

    _PyRandom_Init();

    PySys_ResetWarnOptions();
    _PyOS_ResetGetOpt();

    while ((c = _PyOS_GetOpt(argc, argv, PROGRAM_OPTS)) != EOF) {
        if (c == 'c') {
            size_t len;
            /* -c is the last option; following arguments
               that look like options are left for the
               command to interpret. */

            len = wcslen(_PyOS_optarg) + 1 + 1;
            command = (wchar_t *)PyMem_RawMalloc(sizeof(wchar_t) * len);
            if (command == NULL)
                Py_FatalError(
                   "not enough memory to copy -c argument");
            wcscpy(command, _PyOS_optarg);
            command[len - 2] = '\n';
            command[len - 1] = 0;
            break;
        }

        if (c == 'm') {
            /* -m is the last option; following arguments
               that look like options are left for the
               module to interpret. */
            module = _PyOS_optarg;
            break;
        }

        switch (c) {
        case 'b':
            Py_BytesWarningFlag++;
            break;

        case 'd':
            Py_DebugFlag++;
            break;

        case 'i':
            Py_InspectFlag++;
            Py_InteractiveFlag++;
            break;

        case 'I':
            Py_IsolatedFlag++;
            Py_NoUserSiteDirectory++;
            Py_IgnoreEnvironmentFlag++;
            break;

        /* case 'J': reserved for Jython */

        case 'O':
            Py_OptimizeFlag++;
            break;

        case 'B':
            Py_DontWriteBytecodeFlag++;
            break;

        case 's':
            Py_NoUserSiteDirectory++;
            break;

        case 'S':
            Py_NoSiteFlag++;
            break;

        case 'E':
            /* Already handled above */
            break;

        case 't':
            /* ignored for backwards compatibility */
            break;

        case 'u':
            Py_UnbufferedStdioFlag = 1;
            saw_unbuffered_flag = 1;
            break;

        case 'v':
            Py_VerboseFlag++;
            break;

        case 'x':
            skipfirstline = 1;
            break;

        case 'h':
        case '?':
            help++;
            break;

        case 'V':
            version++;
            break;

        case 'W':
            if (warning_options == NULL)
                warning_options = PyList_New(0);
            if (warning_options == NULL)
                Py_FatalError("failure in handling of -W argument");
            warning_option = PyUnicode_FromWideChar(_PyOS_optarg, -1);
            if (warning_option == NULL)
                Py_FatalError("failure in handling of -W argument");
            if (PyList_Append(warning_options, warning_option) == -1)
                Py_FatalError("failure in handling of -W argument");
            Py_DECREF(warning_option);
            break;

        case 'X':
            PySys_AddXOption(_PyOS_optarg);
            break;

        case 'q':
            Py_QuietFlag++;
            break;

        case 'R':
            /* Ignored */
            break;

        /* This space reserved for other options */

        default:
            return usage(2, argv[0]);
            /*NOTREACHED*/

        }
    }

    if (help)
        return usage(0, argv[0]);

    if (version) {
        printf("Python %s\n", version >= 2 ? Py_GetVersion() : PY_VERSION);
        return 0;
    }

    if (!Py_InspectFlag &&
        (p = Py_GETENV("PYTHONINSPECT")) && *p != '\0')
        Py_InspectFlag = 1;
    if (!saw_unbuffered_flag &&
        (p = Py_GETENV("PYTHONUNBUFFERED")) && *p != '\0')
        Py_UnbufferedStdioFlag = 1;

    if (!Py_NoUserSiteDirectory &&
        (p = Py_GETENV("PYTHONNOUSERSITE")) && *p != '\0')
        Py_NoUserSiteDirectory = 1;

#ifdef MS_WINDOWS
    if (!Py_IgnoreEnvironmentFlag && (wp = _wgetenv(L"PYTHONWARNINGS")) &&
        *wp != L'\0') {
        wchar_t *buf, *warning, *context = NULL;

        buf = (wchar_t *)PyMem_RawMalloc((wcslen(wp) + 1) * sizeof(wchar_t));
        if (buf == NULL)
            Py_FatalError(
               "not enough memory to copy PYTHONWARNINGS");
        wcscpy(buf, wp);
        for (warning = wcstok_s(buf, L",", &context);
             warning != NULL;
             warning = wcstok_s(NULL, L",", &context)) {
            PySys_AddWarnOption(warning);
        }
        PyMem_RawFree(buf);
    }
#else
    if ((p = Py_GETENV("PYTHONWARNINGS")) && *p != '\0') {
        char *buf, *oldloc;
        PyObject *unicode;

        /* settle for strtok here as there's no one standard
           C89 wcstok */
        buf = (char *)PyMem_RawMalloc(strlen(p) + 1);
        if (buf == NULL)
            Py_FatalError(
               "not enough memory to copy PYTHONWARNINGS");
        strcpy(buf, p);
        oldloc = _PyMem_RawStrdup(setlocale(LC_ALL, NULL));
        setlocale(LC_ALL, "");
        for (p = strtok(buf, ","); p != NULL; p = strtok(NULL, ",")) {
#if defined(__APPLE__) || defined(__COSMOPOLITAN__)
            /* Use utf-8 on Mac OS X */
            unicode = PyUnicode_FromString(p);
#else
            unicode = PyUnicode_DecodeLocale(p, "surrogateescape");
#endif
            if (unicode == NULL) {
                /* ignore errors */
                PyErr_Clear();
                continue;
            }
            PySys_AddWarnOptionUnicode(unicode);
            Py_DECREF(unicode);
        }
        setlocale(LC_ALL, oldloc);
        PyMem_RawFree(oldloc);
        PyMem_RawFree(buf);
    }
#endif
    if (warning_options != NULL) {
        Py_ssize_t i;
        for (i = 0; i < PyList_GET_SIZE(warning_options); i++) {
            PySys_AddWarnOptionUnicode(PyList_GET_ITEM(warning_options, i));
        }
    }

    if (command == NULL && module == NULL && _PyOS_optind < argc &&
        wcscmp(argv[_PyOS_optind], L"-") != 0)
    {
        filename = argv[_PyOS_optind];
    }

    stdin_is_interactive = Py_FdIsInteractive(stdin, (char *)0);

#if defined(MS_WINDOWS) || defined(__CYGWIN__)
    /* don't translate newlines (\r\n <=> \n) */
    _setmode(fileno(stdin), O_BINARY);
    _setmode(fileno(stdout), O_BINARY);
    _setmode(fileno(stderr), O_BINARY);
#endif

    if (Py_UnbufferedStdioFlag) {
#ifdef HAVE_SETVBUF
        setvbuf(stdin,  (char *)NULL, _IONBF, BUFSIZ);
        setvbuf(stdout, (char *)NULL, _IONBF, BUFSIZ);
        setvbuf(stderr, (char *)NULL, _IONBF, BUFSIZ);
#else /* !HAVE_SETVBUF */
        setbuf(stdin,  (char *)NULL);
        setbuf(stdout, (char *)NULL);
        setbuf(stderr, (char *)NULL);
#endif /* !HAVE_SETVBUF */
    }
    else if (Py_InteractiveFlag) {
#ifdef MS_WINDOWS
        /* Doesn't have to have line-buffered -- use unbuffered */
        /* Any set[v]buf(stdin, ...) screws up Tkinter :-( */
        setvbuf(stdout, (char *)NULL, _IONBF, BUFSIZ);
#else /* !MS_WINDOWS */
#ifdef HAVE_SETVBUF
        setvbuf(stdin,  (char *)NULL, _IOLBF, BUFSIZ);
        setvbuf(stdout, (char *)NULL, _IOLBF, BUFSIZ);
#endif /* HAVE_SETVBUF */
#endif /* !MS_WINDOWS */
        /* Leave stderr alone - it should be unbuffered anyway. */
    }

    if (!IsXnu()) {
        Py_SetProgramName(argv[0]);
    } else {
        /* On MacOS X, when the Python interpreter is embedded in an
           application bundle, it gets executed by a bootstrapping script
           that does os.execve() with an argv[0] that's different from the
           actual Python executable. This is needed to keep the Finder happy,
           or rather, to work around Apple's overly strict requirements of
           the process name. However, we still need a usable sys.executable,
           so the actual executable path is passed in an environment variable.
           See Lib/plat-mac/bundlebuiler.py for details about the bootstrap
           script. */
        if ((p = Py_GETENV("PYTHONEXECUTABLE")) && *p != '\0') {
            wchar_t* buffer;
            size_t len = strlen(p) + 1;
            buffer = PyMem_RawMalloc(len * sizeof(wchar_t));
            if (buffer == NULL) {
                Py_FatalError(
                    "not enough memory to copy PYTHONEXECUTABLE");
            }
            mbstowcs(buffer, p, len);
            Py_SetProgramName(buffer);
            /* buffer is now handed off - do not free */
        } else {
#ifdef WITH_NEXT_FRAMEWORK
            char* pyvenv_launcher = getenv("__PYVENV_LAUNCHER__");
            if (pyvenv_launcher && *pyvenv_launcher) {
                /* Used by Mac/Tools/pythonw.c to forward
                 * the argv0 of the stub executable
                 */
                wchar_t* wbuf = Py_DecodeLocale(pyvenv_launcher, NULL);
                if (wbuf == NULL) {
                    Py_FatalError("Cannot decode __PYVENV_LAUNCHER__");
                }
                Py_SetProgramName(wbuf);
                /* Don't free wbuf, the argument to Py_SetProgramName
                 * must remain valid until Py_FinalizeEx is called.
                 */
            } else {
                Py_SetProgramName(argv[0]);
            }
#else /* WITH_NEXT_FRAMEWORK */
            Py_SetProgramName(argv[0]);
#endif /* WITH_NEXT_FRAMEWORK */
        }
    }
    Py_Initialize();
    Py_XDECREF(warning_options);

    if (!Py_QuietFlag && (Py_VerboseFlag ||
                        (command == NULL && filename == NULL &&
                         module == NULL && stdin_is_interactive))) {
        fprintf(stderr, "Python %s on %s\n",
            Py_GetVersion(), Py_GetPlatform());
        if (!Py_NoSiteFlag)
            fprintf(stderr, "%s\n", COPYRIGHT);
    }

    if (command != NULL) {
        /* Backup _PyOS_optind and force sys.argv[0] = '-c' */
        _PyOS_optind--;
        argv[_PyOS_optind] = L"-c";
    }

    if (module != NULL) {
        /* Backup _PyOS_optind and force sys.argv[0] = '-m'*/
        _PyOS_optind--;
        argv[_PyOS_optind] = L"-m";
    }

    if (filename != NULL) {
        main_importer_path = AsImportPathEntry(filename);
    }

    if (main_importer_path != NULL) {
        /* Let RunMainFromImporter adjust sys.path[0] later */
        PySys_SetArgvEx(argc-_PyOS_optind, argv+_PyOS_optind, 0);
    } else {
        /* Use config settings to decide whether or not to update sys.path[0] */
        PySys_SetArgv(argc-_PyOS_optind, argv+_PyOS_optind);
    }

    if ((Py_InspectFlag || (command == NULL && filename == NULL && module == NULL)) &&
        isatty(fileno(stdin)) &&
        !Py_IsolatedFlag) {
        PyObject *v;
        v = PyImport_ImportModule("readline");
        if (v == NULL)
            PyErr_Clear();
        else
            Py_DECREF(v);
    }

    if (command) {
        sts = run_command(command, &cf);
        PyMem_RawFree(command);
    } else if (module) {
        sts = (RunModule(module, 1) != 0);
    }
    else {

        if (filename == NULL && stdin_is_interactive) {
            Py_InspectFlag = 0; /* do exit on SystemExit */
            RunStartupFile(&cf);
            RunInteractiveHook();
        }
        /* XXX */

        sts = -1;               /* keep track of whether we've already run __main__ */

        if (main_importer_path != NULL) {
            sts = RunMainFromImporter(main_importer_path);
        }

        if (sts==-1 && filename != NULL) {
            fp = _Py_wfopen(filename, L"r");
            if (fp == NULL) {
                char *cfilename_buffer;
                const char *cfilename;
                int err = errno;
                cfilename_buffer = Py_EncodeLocale(filename, NULL);
                if (cfilename_buffer != NULL)
                    cfilename = cfilename_buffer;
                else
                    cfilename = "<unprintable file name>";
                fprintf(stderr, "%ls: can't open file '%s': [Errno %d] %s\n",
                    argv[0], cfilename, err, strerror(err));
                if (cfilename_buffer)
                    PyMem_Free(cfilename_buffer);
                return 2;
            }
            else if (skipfirstline) {
                int ch;
                /* Push back first newline so line numbers
                   remain the same */
                while ((ch = getc(fp)) != EOF) {
                    if (ch == '\n') {
                        (void)ungetc(ch, fp);
                        break;
                    }
                }
            }
            {
                struct _Py_stat_struct sb;
                if (_Py_fstat_noraise(fileno(fp), &sb) == 0 &&
                    S_ISDIR(sb.st_mode)) {
                    fprintf(stderr,
                            "%ls: '%ls' is a directory, cannot continue\n",
                            argv[0], filename);
                    fclose(fp);
                    return 1;
                }
            }
        }

        if (sts == -1)
            sts = run_file(fp, filename, &cf);
    }

    /* Check this environment variable at the end, to give programs the
     * opportunity to set it from Python.
     */
    if (!Py_InspectFlag &&
        (p = Py_GETENV("PYTHONINSPECT")) && *p != '\0')
    {
        Py_InspectFlag = 1;
    }

    if (Py_InspectFlag && stdin_is_interactive &&
        (filename != NULL || command != NULL || module != NULL)) {
        Py_InspectFlag = 0;
        RunInteractiveHook();
        /* XXX */
        sts = PyRun_AnyFileFlags(stdin, "<stdin>", &cf) != 0;
    }

    if (Py_FinalizeEx() < 0) {
        /* Value unlikely to be confused with a non-error exit status or
        other special meaning */
        sts = 120;
    }

#ifdef __INSURE__
    /* Insure++ is a memory analysis tool that aids in discovering
     * memory leaks and other memory problems.  On Python exit, the
     * interned string dictionaries are flagged as being in use at exit
     * (which it is).  Under normal circumstances, this is fine because
     * the memory will be automatically reclaimed by the system.  Under
     * memory debugging, it's a huge source of useless noise, so we
     * trade off slower shutdown for less distraction in the memory
     * reports.  -baw
     */
    _Py_ReleaseInternedUnicodeStrings();
#endif /* __INSURE__ */

    return sts;
}

/* this is gonna seem *real weird*, but if you put some other code between
   Py_Main() and Py_GetArgcArgv() you will need to adjust the test in the
   while statement in Misc/gdbinit:ppystack */

/* Make the *original* argc/argv available to other modules.
   This is rare, but it is needed by the secureware extension. */

void
Py_GetArgcArgv(int *argc, wchar_t ***argv)
{
    *argc = orig_argc;
    *argv = orig_argv;
}
