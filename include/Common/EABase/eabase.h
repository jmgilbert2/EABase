/*-----------------------------------------------------------------------------
 * eabase.h
 *
 * Copyright (c) Electronic Arts Inc. All rights reserved.
 *---------------------------------------------------------------------------*/


#ifndef INCLUDED_eabase_H
#define INCLUDED_eabase_H


// Identify the compiler and declare the EA_COMPILER_xxxx defines
#include <EABase/config/eacompiler.h>

// Identify traits which this compiler supports, or does not support
#include <EABase/config/eacompilertraits.h>

// Identify the platform and declare the EA_xxxx defines
#include <EABase/config/eaplatform.h>

#if defined(EA_PRAGMA_ONCE_SUPPORTED)
	#pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif

// Always include version.h for backwards compatibility.
#include <EABase/version.h>

// Define common SI unit macros
#include <EABase/eaunits.h>


// ------------------------------------------------------------------------
// Include stddef.h for size_t, ptrdiff_t, NULL and offsetof.

#include <stddef.h>

// ------------------------------------------------------------------------
// Include assert.h on C11 supported compilers so we may allow static_assert usage
// http://en.cppreference.com/w/c/error/static_assert
// C11 standard(ISO / IEC 9899:2011) :
// 7.2/3 Diagnostics <assert.h>(p : 186)
#if !defined(__cplusplus) && defined(__STDC_VERSION__)  && __STDC_VERSION__ >= 201100L
	#include <assert.h>
#endif



// ------------------------------------------------------------------------
// C99 integer types and macros (int32_t, INT64_C, UINT8_MAX, PRId64, etc.).
// C++11 and later provide the limit, constant and format macros without
// __STDC_LIMIT_MACROS, __STDC_CONSTANT_MACROS or __STDC_FORMAT_MACROS.

#include <inttypes.h> // PRId8, SCNd8, etc.
#include <stdint.h>   // int32_t, INT64_C, UINT8_MAX, etc.
#include <math.h>     // float_t, double_t, etc.
#include <float.h>    // FLT_EVAL_METHOD.


// ------------------------------------------------------------------------
// ssize_t
// signed equivalent to size_t.
// This is defined by GCC (except the QNX implementation of GCC) but not by other compilers.
//
#if !defined(__GNUC__)
	// As of this writing, all non-GCC compilers significant to us implement 
	// uintptr_t the same as size_t. However, this isn't guaranteed to be 
	// so for all compilers, as size_t may be based on int, long, or long long.
	#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
		#define _SSIZE_T_
		#define _SSIZE_T_DEFINED

		#if defined(_MSC_VER) && (EA_PLATFORM_PTR_SIZE == 8)
			typedef __int64 ssize_t;
		#else
			typedef long ssize_t;
		#endif
	#endif
#else
	#include <sys/types.h>
#endif


// ------------------------------------------------------------------------
// Character types
//
#if defined(EA_COMPILER_MSVC)
	#if defined(EA_WCHAR_T_NON_NATIVE)
	   // In this case, wchar_t is not defined unless we include 
	   // wchar.h or if the compiler makes it built-in.
	   #ifdef EA_COMPILER_MSVC
		  #pragma warning(push, 3)
	   #endif
	   #include <wchar.h>
	   #ifdef EA_COMPILER_MSVC
		  #pragma warning(pop)
	   #endif
	#endif
#endif


// ------------------------------------------------------------------------
// char8_t  -- The compiler's char8_t type (C++20 and later). Compilers
//             without char8_t get `typedef char char8_t`.
//
// char16_t, char32_t -- The compiler's types.
//
// EA_CHAR8_UNIQUE
// EA_CHAR16_NATIVE
// EA_CHAR32_NATIVE
// EA_WCHAR_UNIQUE
//
// The EA_WCHAR_UNIQUE symbol is defined to 1 if wchar_t is distinct from
// char8_t, char16_t, and char32_t, and defined to 0 if not. In some cases, 
// if the compiler does not support char16_t/char32_t, one of these two types 
// is typically a typedef or define of wchar_t. For compilers that support 
// the C++11 unicode character types often overloads must be provided to 
// support existing code that passes a wide char string to a function that 
// takes a unicode string.
//
// The EA_CHAR8_UNIQUE symbol is defined to 1 if char8_t is distinct type 
// from char in the type system, and defined to 0 if otherwise.

#if !defined(EA_CHAR16_NATIVE)
	// To do: Change this to be based on EA_COMPILER_NO_NEW_CHARACTER_TYPES.
	#if defined(_MSC_VER) && (_MSC_VER >= 1600) && defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) && _HAS_CHAR16_T_LANGUAGE_SUPPORT // VS2010+
		#define EA_CHAR16_NATIVE 1
	#elif defined(EA_COMPILER_CLANG) && defined(EA_COMPILER_CPP11_ENABLED)
		#if __has_feature(cxx_unicode_literals)
			#define EA_CHAR16_NATIVE 1
		#elif (EA_COMPILER_VERSION >= 300) && !(defined(EA_PLATFORM_IPHONE) || defined(EA_PLATFORM_OSX))
			#define EA_CHAR16_NATIVE 1
		#elif defined(EA_PLATFORM_APPLE)
			#define EA_CHAR16_NATIVE 1 
		#else
			#define EA_CHAR16_NATIVE 0
		#endif
	#elif defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 404) && defined(__CHAR16_TYPE__) && defined(EA_COMPILER_CPP11_ENABLED)// EDG 4.4+.
		#define EA_CHAR16_NATIVE 1
	#elif defined(EA_COMPILER_GNUC) && (EA_COMPILER_VERSION >= 4004) && !defined(EA_COMPILER_EDG) && (defined(EA_COMPILER_CPP11_ENABLED) || defined(__STDC_VERSION__)) // g++ (C++ compiler) 4.4+ with -std=c++0x or gcc (C compiler) 4.4+ with -std=gnu99
		#define EA_CHAR16_NATIVE 1
	#else
		#define EA_CHAR16_NATIVE 0
	#endif
#endif

#if !defined(EA_CHAR32_NATIVE)                    // Microsoft currently ties char32_t language support to char16_t language support. So we use CHAR16_T here.
	// To do: Change this to be based on EA_COMPILER_NO_NEW_CHARACTER_TYPES.
	#if defined(_MSC_VER) && (_MSC_VER >= 1600) && defined(_HAS_CHAR16_T_LANGUAGE_SUPPORT) && _HAS_CHAR16_T_LANGUAGE_SUPPORT // VS2010+
		#define EA_CHAR32_NATIVE 1
	#elif defined(EA_COMPILER_CLANG) && defined(EA_COMPILER_CPP11_ENABLED)
		#if __has_feature(cxx_unicode_literals)
			#define EA_CHAR32_NATIVE 1
		#elif (EA_COMPILER_VERSION >= 300) && !(defined(EA_PLATFORM_IPHONE) || defined(EA_PLATFORM_OSX))
			#define EA_CHAR32_NATIVE 1
		#elif defined(EA_PLATFORM_APPLE)
			#define EA_CHAR32_NATIVE 1 
		#else
			#define EA_CHAR32_NATIVE 0
		#endif
	#elif defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 404) && defined(__CHAR32_TYPE__) && defined(EA_COMPILER_CPP11_ENABLED)// EDG 4.4+.
		#define EA_CHAR32_NATIVE 1
	#elif defined(EA_COMPILER_GNUC) && (EA_COMPILER_VERSION >= 4004) && !defined(EA_COMPILER_EDG) && (defined(EA_COMPILER_CPP11_ENABLED) || defined(__STDC_VERSION__)) // g++ (C++ compiler) 4.4+ with -std=c++0x or gcc (C compiler) 4.4+ with -std=gnu99
		#define EA_CHAR32_NATIVE 1
	#else
		#define EA_CHAR32_NATIVE 0
	#endif
#endif


#if EA_CHAR16_NATIVE || EA_CHAR32_NATIVE
	#define EA_WCHAR_UNIQUE 1
#else
	#define EA_WCHAR_UNIQUE 0
#endif


// EA_CHAR8_UNIQUE
//
// Check for char8_t support in the cpp type system. Moving forward from c++20,
// the char8_t type allows users to overload function for character encoding.   
//
// EA_CHAR8_UNIQUE is 1 when the type is a unique in the type system and 
// can there be used as a valid overload. EA_CHAR8_UNIQUE is 0 otherwise.
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0482r6.html
//
#ifdef __cpp_char8_t
	#define EA_CHAR8_UNIQUE 1
#else
	#define EA_CHAR8_UNIQUE 0
#endif


#if !EA_CHAR8_UNIQUE
	typedef char char8_t;
#endif


// CHAR8_MIN, CHAR8_MAX, etc.
//
#define EA_LIMITS_DIGITS_S(T)  ((sizeof(T) * 8) - 1)
#define EA_LIMITS_DIGITS_U(T)  ((sizeof(T) * 8))
#define EA_LIMITS_DIGITS(T)    ((EA_LIMITS_IS_SIGNED(T) ? EA_LIMITS_DIGITS_S(T) : EA_LIMITS_DIGITS_U(T)))
#define EA_LIMITS_IS_SIGNED(T) ((T)(-1) < 0)
#define EA_LIMITS_MIN_S(T)     ((T)((T)1 << EA_LIMITS_DIGITS_S(T)))
#define EA_LIMITS_MIN_U(T)     ((T)0)
#define EA_LIMITS_MIN(T)       ((EA_LIMITS_IS_SIGNED(T) ? EA_LIMITS_MIN_S(T) : EA_LIMITS_MIN_U(T)))
#define EA_LIMITS_MAX_S(T)     ((T)(((((T)1 << (EA_LIMITS_DIGITS(T) - 1)) - 1) << 1) + 1))
#define EA_LIMITS_MAX_U(T)     ((T)~(T)0)
#define EA_LIMITS_MAX(T)       ((EA_LIMITS_IS_SIGNED(T) ? EA_LIMITS_MAX_S(T) : EA_LIMITS_MAX_U(T)))

#if !defined(CHAR8_MIN)
	#define CHAR8_MIN EA_LIMITS_MIN(char8_t)
#endif

#if !defined(CHAR8_MAX)
	#define CHAR8_MAX EA_LIMITS_MAX(char8_t)
#endif

#if !defined(CHAR16_MIN)
	#define CHAR16_MIN EA_LIMITS_MIN(char16_t)
#endif

#if !defined(CHAR16_MAX)
	#define CHAR16_MAX EA_LIMITS_MAX(char16_t)
#endif

#if !defined(CHAR32_MIN)
	#define CHAR32_MIN EA_LIMITS_MIN(char32_t)
#endif

#if !defined(CHAR32_MAX)
	#define CHAR32_MAX EA_LIMITS_MAX(char32_t)
#endif



// EA_CHAR8 / EA_CHAR16 / EA_CHAR32 / EA_WCHAR 
//
// Supports usage of portable string constants.
//
// Example usage:
//     const char16_t* str = EA_CHAR16("Hello world");
//     const char32_t* str = EA_CHAR32("Hello world");
//     const char16_t  c   = EA_CHAR16('\x3001');
//     const char32_t  c   = EA_CHAR32('\x3001');
//
#ifndef EA_CHAR8
	#if EA_CHAR8_UNIQUE
		#define EA_CHAR8(s) u8 ## s
	#else
		#define EA_CHAR8(s) s
	#endif
#endif

#ifndef EA_WCHAR
	#define EA_WCHAR_(s) L ## s
	#define EA_WCHAR(s)  EA_WCHAR_(s)
#endif

#ifndef EA_CHAR16
	#if EA_CHAR16_NATIVE && !defined(_MSC_VER) // Microsoft doesn't support char16_t string literals.
		#define EA_CHAR16_(s) u ## s
		#define EA_CHAR16(s)  EA_CHAR16_(s)
	#elif (EA_WCHAR_SIZE == 2)
		#if defined(_MSC_VER) && (_MSC_VER >= 1900) && defined(__cplusplus) // VS2015 supports u"" string literals.
			#define EA_CHAR16_(s) u ## s
			#define EA_CHAR16(s)  EA_CHAR16_(s)
		#else
			#define EA_CHAR16_(s) L ## s
			#define EA_CHAR16(s)  EA_CHAR16_(s)
		#endif
	#else
		//#define EA_CHAR16(s) // Impossible to implement efficiently.
	#endif
#endif

#ifndef EA_CHAR32
	#if EA_CHAR32_NATIVE && !defined(_MSC_VER) // Microsoft doesn't support char32_t string literals.
		#define EA_CHAR32_(s) U ## s
		#define EA_CHAR32(s)  EA_CHAR32_(s)
	#elif (EA_WCHAR_SIZE == 2)
		#if defined(_MSC_VER) && (_MSC_VER >= 1900) && defined(__cplusplus) // VS2015 supports u"" string literals.
			#define EA_CHAR32_(s) U ## s
			#define EA_CHAR32(s)  EA_CHAR32_(s)
		#else
			//#define EA_CHAR32(s) // Impossible to implement.
		#endif
	#elif (EA_WCHAR_SIZE == 4)
		#define EA_CHAR32_(s) L ## s
		#define EA_CHAR32(s)  EA_CHAR32_(s)
	#else
		#error Unexpected size of wchar_t
	#endif
#endif

// EAText8 / EAText16
//
// Provided for backwards compatibility with older code.
//
#if defined(EABASE_ENABLE_EATEXT_MACROS)
	#define EAText8(x)   x
	#define EAChar8(x)   x

	#define EAText16(x)  EA_CHAR16(x)
	#define EAChar16(x)  EA_CHAR16(x)
#endif




// ------------------------------------------------------------------------
// EAArrayCount
//
// Returns the count of items in a built-in C array. This is a common technique
// which is often used to help properly calculate the number of items in an 
// array at runtime in order to prevent overruns, etc.
//
// Example usage:
//     int array[75];
//     size_t arrayCount = EAArrayCount(array); // arrayCount is 75.
//
#if defined(EA_COMPILER_NO_CONSTEXPR)
	#ifndef EAArrayCount
		#define EAArrayCount(x) (sizeof(x) / sizeof(x[0]))
	#endif
#else
	// This C++11 version is a little smarter than the macro version above; 
	// it can tell the difference between arrays and pointers. Other simpler
	// templated versions have failed in various subtle ways.

	template <typename T, size_t N>
	char (&EAArraySizeHelper(T (&x)[N]))[N];

	template <typename T, size_t N>
	char (&EAArraySizeHelper(T (&&x)[N]))[N];

	#define EAArrayCount(x) (sizeof(EAArraySizeHelper(x)))
#endif


// ------------------------------------------------------------------------
// EA_IS_ENABLED
//
// EA_IS_ENABLED is intended to be used for detecting if compile time features are enabled or disabled.
//
// It has some advantages over using a standard #if or #ifdef tests:
//	1) Fails to compile when passes numeric macro values. Valid options are strictly enabled or disabled.
//	2) Fails to compile when passed undefined macro values rather than disabling by default
//	3) Fails to compile when the passed macro is defined to but empty
//
// To use the macro, the calling code should create a define for the feature to enable or disable.  This feature define
// must be set to either EA_ENABLED or EA_DISABLED.  (Do not try to set the feature define directly to some other
// value.)
//
// Note: These macros are analogous to the Frostbite macro FB_USING used in combination with FB_OFF / FB_ON and are
// designed to be compatible to support gradual migration.
//
// Example usage:
//
//      // The USER_PROVIDED_FEATURE_DEFINE should be defined as either
//      // EA_ENABLED or EA_DISABLED.
//      #define USER_PROVIDED_FEATURE_DEFINE EA_ENABLED
//
//      #if EA_IS_ENABLED(USER_PROVIDED_FEATURE_DEFINE)
//          // USER_PROVIDED_FEATURE_DEFINE is enabled
//      #else
//          // USER_PROVIDED_FEATURE_DEFINE is disabled
//      #endif
//
#define EA_ENABLED              111-
#define EA_DISABLED             333-
// NOTE: Numeric values for x will produce a parse error while empty values produce a divide by zero, and the test is a bool for proper negation behavior
#define EA_IS_ENABLED(x) (333 == 333 * 111 / ((x 0) * (((x 0) == 333 ? 1 : 0) + ((x 0) == 111 ? 1 : 0))))



// Define int128_t / uint128_t types.
// NOTE(rparolin):  include file at the end because we want all the signed integral types defined.
#ifdef __cplusplus
	#include <EABase/int128.h>
#endif


#if !defined(EA_GET_INSTRUCTION_POINTER)
	#if defined(EA_COMPILER_CLANG) || defined(EA_COMPILER_GNUC)
		#define EA_GET_INSTRUCTION_POINTER() []()EA_NO_INLINE{ return __builtin_return_address(0); }()
	#elif defined(EA_COMPILER_MSVC) && !defined(_MANAGED)
		#include <intrin.h>
		#define EA_GET_INSTRUCTION_POINTER() []()EA_NO_INLINE{ return _ReturnAddress(); }()
	#else
		#define EA_GET_INSTRUCTION_POINTER() nullptr
	#endif
#endif


#endif // Header include guard




