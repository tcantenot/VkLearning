#ifndef COMPILER_WARNINGS_HPP
#define COMPILER_WARNINGS_HPP

// Based on: https://stackoverflow.com/a/36175016
#define DIAG_STR(s) #s
#define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)

#ifdef _MSC_VER
#define DIAG_DO_PRAGMA(x) __pragma(x)
#define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(warning(x))
#else
#define DIAG_DO_PRAGMA(x) _Pragma(#x)
#define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)
#endif

#if defined(__clang__)
	#define DISABLE_WARNING_CLANG(clang_option) DIAG_PRAGMA(clang,push) DIAG_PRAGMA(clang,ignored DIAG_JOINSTR(-W,clang_option))
	#define ENABLE_WARNING_CLANG(clang_option)  DIAG_PRAGMA(clang,pop)
	#define DISABLE_WARNING_GCC(x)
	#define ENABLE_WARNING_GCC(x)
	#define DISABLE_WARNING_MSVC(x)
	#define ENABLE_WARNING_MSVC(x)
#elif defined(__GNUC__)
	#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
		#define DISABLE_WARNING_GCC(gcc_option) DIAG_PRAGMA(GCC,push) DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
		#define ENABLE_WARNING_GCC(gcc_option)  DIAG_PRAGMA(GCC,pop)
	#else
		#define DISABLE_WARNING_GCC(gcc_option) DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
		#define ENABLE_WARNING_GCC(gcc_option)  DIAG_PRAGMA(GCC,warning DIAG_JOINSTR(-W,gcc_option))
	#endif
	#define DISABLE_WARNING_CLANG(x)
	#define ENABLE_WARNING_CLANG(x)
	#define DISABLE_WARNING_MSVC(x)
	#define ENABLE_WARNING_MSVC(x)
#elif defined(_MSC_VER)
	#define DISABLE_WARNING_MSVC(msvc_errorcode)  DIAG_PRAGMA(msvc,push) DIAG_DO_PRAGMA(warning(disable:##msvc_errorcode))
	#define ENABLE_WARNING_MSVC(msvc_errorcode) DIAG_PRAGMA(msvc,pop)
	#define DISABLE_WARNING_GCC(x)
	#define ENABLE_WARNING_GCC(x)
	#define DISABLE_WARNING_CLANG(x)
	#define ENABLE_WARNING_CLANG(x)
#endif

#if defined(__clang__)
	#define DISABLE_WARNING(gcc_unused,clang_option,msvc_unused) DISABLE_WARNING_CLANG(clang_option)
	#define ENABLE_WARNING(gcc_unused,clang_option,msvc_unused)  ENABLE_WARNING_CLANG(clang_option)
#elif defined(__GNUC__)
	#define DISABLE_WARNING(gcc_unused,clang_option,msvc_unused) DISABLE_WARNING_GCC(gcc_unused)
	#define ENABLE_WARNING(gcc_unused,clang_option,msvc_unused)  ENABLE_WARNING_GCC(gcc_unused)
#elif defined(_MSC_VER)
	#define DISABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode) DIAG_PRAGMA(msvc,push) DIAG_DO_PRAGMA(warning(disable:##msvc_errorcode))
	#define ENABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode)  ENABLE_WARNING_MSVC(msvc_errorcode)
#endif

#endif //COMPILER_WARNINGS_HPP
