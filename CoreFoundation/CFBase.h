/* Copyright (c) 2007 Christopher J. W. Lloyd

Permission is hereby granted,free of charge,to any person obtaining a copy of this software and associated documentation files (the "Software"),to deal in the Software without restriction,including without limitation the rights to use,copy,modify,merge,publish,distribute,sublicense,and/or sell copies of the Software,and to permit persons to whom the Software is furnished to do so,subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",WITHOUT WARRANTY OF ANY KIND,EXPRESS OR IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE,ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#import <sys/types.h>
#import <stdlib.h>
#import <ctype.h>
#import <errno.h>
#import <float.h>
#import <limits.h>
#import <math.h>
#import <stdarg.h>
#import <stddef.h>
#import <stdio.h>
#import <string.h>
#import <assert.h>
#import <stdint.h>
#import <time.h>

#ifdef __cplusplus

#if defined(__WIN32__)
#if defined(COREFOUNDATION_INSIDE_BUILD)
#define COREFOUNDATION_EXPORT extern "C" __declspec(dllexport)
#else
#define COREFOUNDATION_EXPORT extern "C" __declspec(dllimport) 
#endif
#else
#define COREFOUNDATION_EXPORT extern "C"
#endif

#else

#if defined(__WIN32__)
#if defined(COREFOUNDATION_INSIDE_BUILD)
#define COREFOUNDATION_EXPORT __declspec(dllexport) extern
#else
#define COREFOUNDATION_EXPORT __declspec(dllimport) extern
#endif
#else
#define COREFOUNDATION_EXPORT extern
#endif

#endif // __cplusplus

/* Apple's Foundation imports CoreGraphics in order to get some of the basic CG* types, unfortunately
   this is a hassle on platforms where you just want to use Foundation, so we put them in CoreFoundation and see what happens
*/
 
enum
{
	kCFNotFound = LONG_MAX
};

#ifdef __LP64__
typedef double CGFloat;
#define CGFLOAT_MIN DBL_MIN
#define CGFLOAT_MAX DBL_MAX
#else
typedef float CGFloat;
#define CGFLOAT_MIN FLT_MIN
#define CGFLOAT_MAX FLT_MAX
#endif

typedef struct CGPoint {
   CGFloat x;
   CGFloat y;
} CGPoint;

typedef struct CGSize {
   CGFloat width;
   CGFloat height;
} CGSize;

typedef struct CGRect {
   CGPoint origin;
   CGSize  size;
} CGRect;
 

typedef unsigned short UniChar;
typedef unsigned long UTF32Char;
typedef float Float32;
typedef double Float64;
// ---

typedef unsigned CFUInteger;
typedef int CFInteger;
typedef int8_t SInt8;
typedef uint8_t UInt8;
typedef int16_t SInt16;
typedef uint16_t UInt16;
typedef int32_t SInt32;
typedef uint32_t UInt32;
typedef int64_t SInt64;
typedef uint64_t UInt64;

typedef const void *CFTypeRef;
typedef CFUInteger CFTypeID;
typedef CFUInteger CFHashCode;
typedef char       Boolean;
typedef CFInteger  CFIndex;
typedef CFUInteger CFOptionFlags;

typedef SInt32 OSStatus;
typedef SInt16 OSErr;

typedef UInt32 FourCharCode;
typedef FourCharCode OSType;

typedef struct {
   CFIndex location;
   CFIndex length;
} CFRange;

static inline CFRange CFRangeMake(CFIndex loc,CFIndex len){
   CFRange result={loc,len};
   
   return result;
}

#ifndef TRUE
#define TRUE ((Boolean)1)
#endif

#ifndef FALSE
#define FALSE ((Boolean)0)
#endif

typedef enum  {
   kCFCompareLessThan  = -1,
   kCFCompareEqualTo   = 0,
   kCFCompareGreaterThan= 1
} CFComparisonResult;

typedef CFComparisonResult (*CFComparatorFunction)(const void *value,const void *other,void *context);

typedef struct CFAllocator *CFAllocatorRef;

#import <CoreFoundation/CFString.h>

typedef void       *(*CFAllocatorAllocateCallBack)(CFIndex size,CFOptionFlags hint,void *info);
typedef CFStringRef (*CFAllocatorCopyDescriptionCallBack)(const void *info);
typedef void        (*CFAllocatorDeallocateCallBack)(void *ptr,void *info);
typedef CFIndex     (*CFAllocatorPreferredSizeCallBack)(CFIndex size,CFOptionFlags hint,void *info);
typedef void       *(*CFAllocatorReallocateCallBack)(void *ptr,CFIndex size,CFOptionFlags hint,void *info);
typedef void        (*CFAllocatorReleaseCallBack)(const void *info);
typedef const void *(*CFAllocatorRetainCallBack)(const void *info);

typedef struct {
   CFIndex                            version;
   void                              *info;
   CFAllocatorRetainCallBack          retain;
   CFAllocatorReleaseCallBack         release;
   CFAllocatorCopyDescriptionCallBack copyDescription;
   CFAllocatorAllocateCallBack        allocate;
   CFAllocatorReallocateCallBack      reallocate;
   CFAllocatorDeallocateCallBack      deallocate;
   CFAllocatorPreferredSizeCallBack   preferredSize;
} CFAllocatorContext;

COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorDefault;
COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorSystemDefault;
COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorMalloc;
COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorMallocZone;
COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorNull;
COREFOUNDATION_EXPORT const CFAllocatorRef kCFAllocatorUseContext;

COREFOUNDATION_EXPORT CFAllocatorRef CFAllocatorGetDefault(void);
COREFOUNDATION_EXPORT void           CFAllocatorSetDefault(CFAllocatorRef self);

COREFOUNDATION_EXPORT CFTypeID       CFAllocatorGetTypeID(void);

COREFOUNDATION_EXPORT CFAllocatorRef CFAllocatorCreate(CFAllocatorRef self,CFAllocatorContext *context);

COREFOUNDATION_EXPORT void           CFAllocatorGetContext(CFAllocatorRef self,CFAllocatorContext *context);
COREFOUNDATION_EXPORT CFIndex        CFAllocatorGetPreferredSizeForSize(CFAllocatorRef self,CFIndex size,CFOptionFlags hint);

COREFOUNDATION_EXPORT void          *CFAllocatorAllocate(CFAllocatorRef self,CFIndex size,CFOptionFlags hint);
COREFOUNDATION_EXPORT void           CFAllocatorDeallocate(CFAllocatorRef self,void *ptr);
COREFOUNDATION_EXPORT void          *CFAllocatorReallocate(CFAllocatorRef self,void *ptr,CFIndex size,CFOptionFlags hint);


COREFOUNDATION_EXPORT CFTypeID       CFGetTypeID(CFTypeRef self);

COREFOUNDATION_EXPORT CFTypeRef      CFRetain(CFTypeRef self);
COREFOUNDATION_EXPORT void           CFRelease(CFTypeRef self);
COREFOUNDATION_EXPORT CFIndex        CFGetRetainCount(CFTypeRef self);

COREFOUNDATION_EXPORT CFAllocatorRef CFGetAllocator(CFTypeRef self);

COREFOUNDATION_EXPORT CFHashCode     CFHash(CFTypeRef self);
COREFOUNDATION_EXPORT Boolean        CFEqual(CFTypeRef self,CFTypeRef other);
COREFOUNDATION_EXPORT CFStringRef    CFCopyTypeIDDescription(CFTypeID typeID);
COREFOUNDATION_EXPORT CFStringRef    CFCopyDescription(CFTypeRef self);
COREFOUNDATION_EXPORT CFTypeRef      CFMakeCollectable(CFTypeRef self);

#ifndef MACH

// mach/mach_types.h

typedef int kern_return_t;

#define KERN_SUCCESS 0
#define KERN_FAILURE 5

#ifndef _MACH_PORT_T
#define _MACH_PORT_T
typedef int mach_port_t;
#endif


// mach/mach_time.h

typedef struct mach_timebase_info {
   uint32_t numer;
   uint32_t denom;
} mach_timebase_info_data_t,*mach_timebase_info_t;

uint64_t      mach_absolute_time(void);
kern_return_t mach_timebase_info(mach_timebase_info_t timebase);
#endif

#ifdef WINDOWS
COREFOUNDATION_EXPORT unsigned int sleep(unsigned int seconds);
//COREFOUNDATION_EXPORT int usleep(long useconds);
COREFOUNDATION_EXPORT size_t strlcpy(char *dst, const char *src, size_t size);
COREFOUNDATION_EXPORT char *strnstr(const char *s1,const char *s2,size_t n);
COREFOUNDATION_EXPORT void bzero(void *ptr,size_t size);
COREFOUNDATION_EXPORT void bcopy(const void *s1, void *s2, size_t n);
COREFOUNDATION_EXPORT int bcmp(const void *s1, void *s2, size_t n);
COREFOUNDATION_EXPORT int mkstemps(char *tmplt,int suffixlen);
#endif


