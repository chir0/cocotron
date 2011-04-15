#import <CoreFoundation/CFNumber.h>
#import <Foundation/NSRaise.h>
#import <Foundation/NSCFTypeID.h>
#import <Foundation/NSNumber_CF.h>

struct __CFBoolean {
};

#define ToNSNumber(object) ((NSNumber *)object)

CFTypeID CFBooleanGetTypeID(void) {
   return kNSCFTypeBoolean;
}

CFNumberType CFNumberGetType(CFNumberRef self) {
   if([self isKindOfClass:[NSNumber_CF class]])
    return ((NSNumber_CF *)self)->_type;

   return kCFNumberIntType;
}

Boolean CFBooleanGetValue(CFBooleanRef self) {
   return [ToNSNumber(self) boolValue];
}
