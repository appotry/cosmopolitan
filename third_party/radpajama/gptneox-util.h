// -*- c++; c-basic-offset:4 -*-
#ifndef GPTNEOX_UTIL_H
#define GPTNEOX_UTIL_H
#include "libc/calls/calls.h"
#include "libc/calls/struct/rlimit.h"
#include "libc/calls/struct/rusage.h"
#include "libc/calls/weirdtypes.h"
#include "libc/errno.h"
#include "libc/runtime/pathconf.h"
#include "libc/runtime/runtime.h"
#include "libc/runtime/sysconf.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/f.h"
#include "libc/sysv/consts/fileno.h"
#include "libc/sysv/consts/madv.h"
#include "libc/sysv/consts/map.h"
#include "libc/sysv/consts/mfd.h"
#include "libc/sysv/consts/mlock.h"
#include "libc/sysv/consts/mremap.h"
#include "libc/sysv/consts/msync.h"
#include "libc/sysv/consts/o.h"
#include "libc/sysv/consts/ok.h"
#include "libc/sysv/consts/posix.h"
#include "libc/sysv/consts/prio.h"
#include "libc/sysv/consts/prot.h"
#include "libc/sysv/consts/rlim.h"
#include "libc/sysv/consts/rlimit.h"
#include "libc/sysv/consts/rusage.h"
#include "libc/time/time.h"
#include "third_party/getopt/getopt.h"
#include "third_party/ggml/llama_util.h"
#include "third_party/libcxx/cerrno"
#include "third_party/libcxx/climits"
#include "third_party/libcxx/cstdarg"
#include "third_party/libcxx/cstdint"
#include "third_party/libcxx/cstdio"
#include "third_party/libcxx/cstdlib"
#include "third_party/libcxx/cstring"
#include "third_party/libcxx/string"
#include "third_party/libcxx/vector"
#include "third_party/musl/crypt.h"
#include "third_party/musl/lockf.h"
// clang-format off

// Internal header to be included only by llama.cpp.
// Contains wrappers around OS interfaces.

#define GPTNEOX_ASSERT(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "GPTNEOX_ASSERT: %s:%d: %s\n", __FILE__, __LINE__, #x); \
            abort(); \
        } \
    } while (0)

#ifdef __GNUC__
#ifdef __MINGW32__
__attribute__((format(gnu_printf, 1, 2)))
#else
__attribute__((format(printf, 1, 2)))
#endif
#endif
static std::string format(const char * fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int size = vsnprintf(NULL, 0, fmt, ap);
    GPTNEOX_ASSERT(size >= 0 && size < INT_MAX);
    std::vector<char> buf(size + 1);
    int size2 = vsnprintf(buf.data(), size + 1, fmt, ap2);
    GPTNEOX_ASSERT(size2 == size);
    va_end(ap2);
    va_end(ap);
    return std::string(buf.data(), size);
}

struct gptneox_file {
    // use FILE * so we don't have to re-open the file to mmap
    FILE * fp;
    size_t size;

    gptneox_file(const char * fname, const char * mode) {
        fp = std::fopen(fname, mode);
        if (fp == NULL) {
            Die("failed to open %s: %s", fname, std::strerror(errno));
        }
        seek(0, SEEK_END);
        size = tell();
        seek(0, SEEK_SET);
    }

    size_t tell() const {
#ifdef _WIN32
        __int64 ret = _ftelli64(fp);
#else
        long ret = std::ftell(fp);
#endif
        GPTNEOX_ASSERT(ret != -1); // this really shouldn't fail
        return (size_t) ret;
    }

    void seek(size_t offset, int whence) {
#ifdef _WIN32
        int ret = _fseeki64(fp, (__int64) offset, whence);
#else
        int ret = std::fseek(fp, (long) offset, whence);
#endif
        GPTNEOX_ASSERT(ret == 0); // same
    }

    void read_raw(void * ptr, size_t size) {
        if (size == 0) {
            return;
        }
        errno = 0;
        std::size_t ret = std::fread(ptr, size, 1, fp);
        if (ferror(fp)) {
            Die("read error: %s", strerror(errno));
        }
        if (ret != 1) {
            Die("unexpectedly reached end of file");
        }
    }

    std::uint32_t read_u32() {
        std::uint32_t ret;
        read_raw(&ret, sizeof(ret));
        return ret;
    }

    std::string read_string(std::uint32_t len) {
        std::vector<char> chars(len);
        read_raw(chars.data(), len);
        return std::string(chars.data(), len);
    }

    void write_raw(const void * ptr, size_t size) {
        if (size == 0) {
            return;
        }
        errno = 0;
        size_t ret = std::fwrite(ptr, size, 1, fp);
        if (ret != 1) {
            Die("write error: %s", strerror(errno));
        }
    }

    void write_u32(std::uint32_t val) {
        write_raw(&val, sizeof(val));
    }

    ~gptneox_file() {
        if (fp) {
            std::fclose(fp);
        }
    }
};

#if defined(_WIN32)
static std::string gptneox_format_win_err(DWORD err) {
    LPSTR buf;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buf, 0, NULL);
    if (!size) {
        return "FormatMessageA failed";
    }
    std::string ret(buf, size);
    LocalFree(buf);
    return ret;
}
#endif

struct gptneox_mmap {
    void * addr;
    size_t size;

    gptneox_mmap(const gptneox_mmap &) = delete;

#ifdef _POSIX_MAPPED_FILES
    static constexpr bool SUPPORTED = true;

    gptneox_mmap(struct gptneox_file * file, bool prefetch = true) {
        size = file->size;
        int fd = fileno(file->fp);
        int flags = MAP_SHARED;
#if defined(__linux__) || defined(__COSMOPOLITAN__)
        flags |= MAP_POPULATE;
#endif
        addr = mmap(NULL, file->size, PROT_READ, flags, fd, 0);
        if (addr == MAP_FAILED) {
            Die("mmap failed: %s", strerror(errno));
        }

        if (prefetch) {
            // Advise the kernel to preload the mapped memory
            if (madvise(addr, file->size, MADV_WILLNEED)) {
                fprintf(stderr, "warning: madvise(.., MADV_WILLNEED) failed: %s\n",
                        strerror(errno));
            }
        }
    }

    ~gptneox_mmap() {
        munmap(addr, size);
    }
#elif defined(_WIN32)
    static constexpr bool SUPPORTED = true;

    gptneox_mmap(struct gptneox_file * file, bool prefetch = true) {
        size = file->size;

        HANDLE hFile = (HANDLE) _get_osfhandle(_fileno(file->fp));

        HANDLE hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
        DWORD error = GetLastError();

        if (hMapping == NULL) {
            Die("CreateFileMappingA failed: %s", gptneox_format_win_err(error).c_str());
        }

        addr = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
        error = GetLastError();
        CloseHandle(hMapping);

        if (addr == NULL) {
            Die("MapViewOfFile failed: %s", gptneox_format_win_err(error).c_str());
        }

        #if _WIN32_WINNT >= _WIN32_WINNT_WIN8
        if (prefetch) {
            // Advise the kernel to preload the mapped memory
            WIN32_MEMORY_RANGE_ENTRY range;
            range.VirtualAddress = addr;
            range.NumberOfBytes = (SIZE_T)size;
            if (!PrefetchVirtualMemory(GetCurrentProcess(), 1, &range, 0)) {
                fprintf(stderr, "warning: PrefetchVirtualMemory failed: %s\n",
                        gptneox_format_win_err(GetLastError()).c_str());
            }
        }
        #else
        #pragma message("warning: You are building for pre-Windows 8; prefetch not supported")
        #endif // _WIN32_WINNT >= _WIN32_WINNT_WIN8
    }

    ~gptneox_mmap() {
        if (!UnmapViewOfFile(addr)) {
            fprintf(stderr, "warning: UnmapViewOfFile failed: %s\n",
                    gptneox_format_win_err(GetLastError()).c_str());
        }
    }
#else
    static constexpr bool SUPPORTED = false;

    gptneox_mmap(struct gptneox_file *) {
        Die("mmap not supported");
    }
#endif
};

// Represents some region of memory being locked using mlock or VirtualLock;
// will automatically unlock on destruction.
struct gptneox_mlock {
    void * addr = NULL;
    size_t size = 0;
    bool failed_already = false;

    gptneox_mlock() {}
    gptneox_mlock(const gptneox_mlock &) = delete;

    ~gptneox_mlock() {
        if (size) {
            raw_unlock(addr, size);
        }
    }

    void init(void * addr) {
        GPTNEOX_ASSERT(this->addr == NULL && this->size == 0);
        this->addr = addr;
    }

    void grow_to(size_t target_size) {
        GPTNEOX_ASSERT(addr);
        if (failed_already) {
            return;
        }
        size_t granularity = lock_granularity();
        target_size = (target_size + granularity - 1) & ~(granularity - 1);
        if (target_size > size) {
            if (raw_lock((uint8_t *) addr + size, target_size - size)) {
                size = target_size;
            } else {
                failed_already = true;
            }
        }
    }

#ifdef _POSIX_MEMLOCK_RANGE
    static constexpr bool SUPPORTED = true;

    size_t lock_granularity() {
        return (size_t) sysconf(_SC_PAGESIZE);
    }

    #ifdef __APPLE__
        #define MLOCK_SUGGESTION \
            "Try increasing the sysctl values 'vm.user_wire_limit' and 'vm.global_user_wire_limit' and/or " \
            "decreasing 'vm.global_no_user_wire_amount'.  Also try increasing RLIMIT_MLOCK (ulimit -l).\n"
    #else
        #define MLOCK_SUGGESTION \
            "Try increasing RLIMIT_MLOCK ('ulimit -l' as root).\n"
    #endif

    bool raw_lock(const void * addr, size_t size) {
        if (!mlock(addr, size)) {
            return true;
        } else {
            char* errmsg = std::strerror(errno);
            bool suggest = (errno == ENOMEM);

            // Check if the resource limit is fine after all
            struct rlimit lock_limit;
            if (suggest && getrlimit(RLIMIT_MEMLOCK, &lock_limit))
                suggest = false;
            if (suggest && (lock_limit.rlim_max > lock_limit.rlim_cur + size))
                suggest = false;

            fprintf(stderr, "warning: failed to mlock %zu-byte buffer (after previously locking %zu bytes): %s\n%s",
                    size, this->size, errmsg, suggest ? MLOCK_SUGGESTION : "");
            return false;
        }
    }

    #undef MLOCK_SUGGESTION

    void raw_unlock(void * addr, size_t size) {
        if (munlock(addr, size)) {
            fprintf(stderr, "warning: failed to munlock buffer: %s\n", std::strerror(errno));
        }
    }
#elif defined(_WIN32)
    static constexpr bool SUPPORTED = true;

    size_t lock_granularity() {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return (size_t) si.dwPageSize;
    }

    bool raw_lock(void * addr, size_t size) {
        for (int tries = 1; ; tries++) {
            if (VirtualLock(addr, size)) {
                return true;
            }
            if (tries == 2) {
                fprintf(stderr, "warning: failed to VirtualLock %zu-byte buffer (after previously locking %zu bytes): %s\n",
                        size, this->size, gptneox_format_win_err(GetLastError()).c_str());
                return false;
            }

            // It failed but this was only the first try; increase the working
            // set size and try again.
            SIZE_T min_ws_size, max_ws_size;
            if (!GetProcessWorkingSetSize(GetCurrentProcess(), &min_ws_size, &max_ws_size)) {
                fprintf(stderr, "warning: GetProcessWorkingSetSize failed: %s\n",
                        gptneox_format_win_err(GetLastError()).c_str());
                return false;
            }
            // Per MSDN: "The maximum number of pages that a process can lock
            // is equal to the number of pages in its minimum working set minus
            // a small overhead."
            // Hopefully a megabyte is enough overhead:
            size_t increment = size + 1048576;
            // The minimum must be <= the maximum, so we need to increase both:
            min_ws_size += increment;
            max_ws_size += increment;
            if (!SetProcessWorkingSetSize(GetCurrentProcess(), min_ws_size, max_ws_size)) {
                fprintf(stderr, "warning: SetProcessWorkingSetSize failed: %s\n",
                        gptneox_format_win_err(GetLastError()).c_str());
                return false;
            }
        }
    }

    void raw_unlock(void * addr, size_t size) {
        if (!VirtualUnlock(addr, size)) {
            fprintf(stderr, "warning: failed to VirtualUnlock buffer: %s\n",
                    gptneox_format_win_err(GetLastError()).c_str());
        }
    }
#else
    static constexpr bool SUPPORTED = false;

    void raw_lock(const void * addr, size_t size) {
        fprintf(stderr, "warning: mlock not supported on this system\n");
    }

    void raw_unlock(const void * addr, size_t size) {}
#endif
};

// Replacement for std::vector<uint8_t> that doesn't require zero-initialization.
struct gptneox_buffer {
    uint8_t * addr = NULL;
    size_t size = 0;

    void resize(size_t size) {
        delete[] addr;
        addr = new uint8_t[size];
        this->size = size;
    }

    ~gptneox_buffer() {
        delete[] addr;
    }
};

#ifdef GGML_USE_CUBLAS
// MISSING #include "ggml-cuda.h"
struct gptneox_ctx_buffer {
    uint8_t * addr = NULL;
    size_t size = 0;

    void resize(size_t size) {
        if (addr) {
            ggml_cuda_host_free(addr);
        }
        addr = (uint8_t *) ggml_cuda_host_malloc(size);
        this->size = size;
    }

    ~gptneox_ctx_buffer() {
        if (addr) {
            ggml_cuda_host_free(addr);
        }
    }
};
#else
typedef gptneox_buffer gptneox_ctx_buffer;
#endif

#endif
