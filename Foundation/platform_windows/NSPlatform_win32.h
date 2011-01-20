/* Copyright (c) 2006-2007 Christopher J. W. Lloyd

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#import <Foundation/NSPlatform.h>

#import <windows.h>

@class NSParentDeathMonitor_win32;

@interface NSPlatform_win32 : NSPlatform {
   NSParentDeathMonitor_win32 *_parentDeathMonitor;
}

@end

void _Win32Assert(const char *code,int line,const char *file);

#define Win32Assert(call) _Win32Assert(call,__LINE__,__FILE__)

NSTimeInterval Win32TimeIntervalFromFileTime(FILETIME fileTime);
void Win32ThreadSleepForTimeInterval(NSTimeInterval interval);

BOOL NSPlatformGreaterThanOrEqualToWindowsXP(void);
BOOL NSPlatformGreaterThanOrEqualToWindows2000(void);
