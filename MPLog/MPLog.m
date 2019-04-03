//
//  MPLog.m
//
//  Copyright (c) 2019 Michael Peternell. All rights reserved.
//

#import "MPLog.h"

#if _MP_LOG_ENABLED

FILE *_MPLog_file = NULL;


@interface MPThreadIdAtom : NSObject {
    int number;
}

@property (readonly) int number; //0..25 (or 100)
@property (readonly) char letter; //a..z (or "+")

// returns a new ID. if we run out of IDs, it returns an atom with number 100 (letter "+")
// If an atom is destroyed, it's number will be reused.
+ (MPThreadIdAtom *)newId;

@end


static NSMutableArray *Availability = nil;
static int CurrentNumber = 0;

@implementation MPThreadIdAtom

+ (void)initialize {
    if([self class] == [MPThreadIdAtom class]) {
        Availability = [[NSMutableArray alloc] initWithCapacity:26];
        for(int i=0; i<26; i++) {
            [Availability addObject:@YES];
        }
    }
}

- (id)initWithInt:(int)num {
    self->number = num;
    return self;
}

- (int)number {
    return number;
}

- (char)letter {
    if(number == 100) {
        return '+';
    }
    return 'a'+number;
}

- (void)dealloc {
    if(number != 100) {
        @synchronized([MPThreadIdAtom class]) {
            Availability[number] = @YES;
        }
    }
}

+ (MPThreadIdAtom *)newId {
    @synchronized([MPThreadIdAtom class]) {
        for(int i=0; i<26; i++) {
            int the_number = (CurrentNumber + i) % 26;
            if([Availability[the_number] boolValue]) {
                MPThreadIdAtom *atom = [[MPThreadIdAtom alloc] initWithInt:the_number];
                CurrentNumber = (the_number + 1) % 26;
                Availability[the_number] = @NO;
                return atom;
            }
        }
    }
    return [[MPThreadIdAtom alloc] initWithInt:100];
}

@end


// returns the current "thread ID"
// This is defined as:
//   'M' if the current thread is the main thread
//   The letters 'a' to 'z' for background threads.
//   If we run out of lowercase letters, the letters are recycled. If there are
//   more than 26 background threads that do logging simultanuously, some
//   threads will get the special thread ID '+'.
static char getThreadId() {
    static NSString *ThreadIdKey = @"MPLog_ThreadId";
    NSThread *t = [NSThread currentThread];
    if([t isMainThread])
        return 'M';
    MPThreadIdAtom *atom = t.threadDictionary[ThreadIdKey];
    if(!atom) {
        atom = [MPThreadIdAtom newId];
        t.threadDictionary[ThreadIdKey] = atom;
    }
    return atom.letter;
}


void _MPLog_impl_swift(NSString *message, const char *log_type, const char *filename, int linenumber) {
    static NSDateFormatter *dateFormatter;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        dateFormatter = [NSDateFormatter new];
        [dateFormatter setDateFormat:@"HH:mm:ss.SSS"];
    });
    
    const char *basename = filename;
    for(const char *basename_tentative = filename; *basename_tentative != 0; basename_tentative++) {
        if(*basename_tentative == '/') {
            basename = basename_tentative+1;
        }
    }
    
    NSDate *now = [NSDate new];
    NSString *dateString = [dateFormatter stringFromDate:now];
    
    NSString *fullString = [[NSString alloc] initWithFormat:@"%@ %c %-5s %s(%d): %@\n",
                            dateString, getThreadId(), log_type, basename, linenumber, message];
    const char *fullString_c = [fullString cStringUsingEncoding:NSUTF8StringEncoding];
    
    if(_MPLog_file) {
        fputs(fullString_c, _MPLog_file);
        fflush(_MPLog_file);
    } else {
        fputs(fullString_c, stderr);
    }
}

void _MPLog_impl(const char *filename, int linenumber, const char *log_type, NSString *format, ...) {
    va_list args;
    va_start(args, format);
    NSString *userString = [[NSString alloc] initWithFormat:format arguments:args];
    va_end(args);
    _MPLog_impl_swift(userString, log_type, filename, linenumber);
}

#endif
