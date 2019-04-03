//
//  MPLog.h
//
//  Copyright (c) 2019 Michael Peternell. All rights reserved.
//

#import <Foundation/Foundation.h>

// MPLog, logging framework for iOS and macOS
// Version 1.0

//
// Documentation for Objective C
//

// ************
// INTRODUCTION
// ************

// Have you ever been frustrated about the limited usefulness of using NSLog()
// for debugging your code and seeing what is going on in the log messages?
//
// If the answer is YES, then MPLog is for you!
//
// What do you think about NSLog?
//   NSLog(@"(%d)", 1335+2);
//   => 2013-10-02 19:21:25.242 dynimg[21068:707] (1337)
// Hmm, where did that log message originate from? Hmm, let's just search the
// source code for the string 1337... No match? Hmm... We need a decent logging
// framework
//
// MPLog comes to the rescue:
//   SET_LOG_LEVEL(MP_LOG_LEVEL_DEBUG);
//   MPLogDebug(@"(%d)", 1335+2);
//   => 19:21:25.235 M DEBUG MPAppDelegate.m(17): (1337)
// Hmm, why is there a funny log message in MPAppDelegate.m, line 17?
//

// ***********
// EXPLANATION
// ***********

// MPLog is a logging framework for iOS that allows logging to the console in a
// very sophisticated way. There are the following advantages over NSLog():
//
//   1. There are four log levels: DEBUG, INFO, WARN and ERROR
//   2. Each log message contains the filename and the linenumber from which
//      the log message originated.
//   3. The date and the name of the binary is missing, and instead, only the
//      time is shown (with millisecond precision.)
//   4. The second column contains an indication of the thread that the message
//      came from. This is 'M' for the main thread, and the letters 'a' to 'z'
//      for background threads. Letters are reused if necessary and possible.
//      If there are more than 26 background threads, however, this column may
//      just say '+'. I did a lot of iOS coding and this has never happened to
//      me! (Except once, when I tried to test the MPLog system.)
//
// In order for MPLog to work, you have to call SET_LOG_LEVEL on the top of your
// .m file. This macro just creates a static variable that contains the current
// log level. If you set the log level to MP_LOG_LEVEL_DEBUG, all log messages
// are displayed on the console. This is useful for developing.
//   SET_LOG_LEVEL(MP_LOG_LEVEL_DEBUG);
// Once you finished developing your class, you may raise the log level to
//   SET_LOG_LEVEL(MP_LOG_LEVEL_WARN);
// If you like. With MP_LOG_LEVEL_WARN, only warnings and errors are shown.

// **********
// LOG LEVELS
// **********

// There are four log levels:
//   DEBUG
//     Used for messages of all kinds, this can be very verbose.
//     SET_LOG_LEVEL(MP_LOG_LEVEL_DEBUG);
//     MPLogDebug(@"Successfully initialized new foobar object: %p", self);
//   INFO
//     Should be used for messages that are somewhat informative, and that
//     happen infrequently. Maybe as a response to user interaction. A timer
//     that fires every second probably shouldn't emit INFO messages when it
//     fires.
//     SET_LOG_LEVEL(MP_LOG_LEVEL_INFO);
//     ^^If you set the log level to INFO, all DEBUG messages will be ommitted
//     but all other messages are still shown.
//     MPLogInfo(@"Initializing In-App-Purchase system...");
//   WARN
//     This should be used for messages that denote a suboptimal condition,
//     like a bad server response or if the user entered a wrong username and
//     password, but not for bugs in the app!
//     SET_LOG_LEVEL(MP_LOG_LEVEL_WARN);
//     MPLogWarn(@"Password is to short. Starting funny error animation...");
//   ERROR
//     This should be used for bugs in the app. E.g.
//     if(!bar) {
//        MPLogError(@"bar is nil! You must not call this method with a nil argument!");
//        return 0;
//     }
//     SET_LOG_LEVEL(MP_LOG_LEVEL_ERROR);
// BTW, if you forget to set the log level on the top of your compilation unit,
// a compile time error will result. Note that you really shouldn't call
// SET_LOG_LEVEL in a header file! Always place your SET_LOG_LEVEL call to the
// top of the compilation unit, which usually means: on the first few lines of
// your .m file.

// *****************
// FUNCTION STARTED!
// *****************

// There ar two convenience functions:
//   MPLogDebugStart()
//     ^^it emits a debug message, that says that your function just started.
//     The log message includes the name of the function. E.g.:
//     19:21:25.241 M DEBUG MPAppDelegate.m(47): Start -[MPAppDelegate applicationDidBecomeActive:]
//   MPLogInfoStart()
//     Same as MPLogDebugStart, but with an INFO message. I use this function
//     often in my -viewWillAppear view controller methods.
//     19:21:25.241 M INFO  MPAppDelegate.m(47): Start -[MPAppDelegate applicationDidBecomeActive:]

// *************
// CONFIGURATION
// *************

// You can enable or disable logging currently. By default MPLog will log to
// stderr (if enabled). You can also log to any filehandle instead. The file
// should be opened in mode "a" (append), anything else would not work as expected.
// (Search for _MPLog_file)



//
// Configuration
//

// Enable or disable MP-Logging.
// To enable MP-Logging, set _MP_LOG_ENABLED to 1

#ifdef DEBUG
#  define _MP_LOG_ENABLED 1
#else
#  define _MP_LOG_ENABLED 0
#endif



//
// Implementation - Constants
//

#define MP_LOG_ONLY_DEBUG   0x01
#define MP_LOG_ONLY_INFO    0x02
#define MP_LOG_ONLY_WARN    0x04
#define MP_LOG_ONLY_ERROR   0x08

#define MP_LOG_LEVEL_DEBUG  (MP_LOG_ONLY_DEBUG | MP_LOG_ONLY_INFO | MP_LOG_ONLY_WARN | MP_LOG_ONLY_ERROR)
#define MP_LOG_LEVEL_INFO   (MP_LOG_ONLY_INFO | MP_LOG_ONLY_WARN | MP_LOG_ONLY_ERROR)
#define MP_LOG_LEVEL_WARN   (MP_LOG_ONLY_WARN | MP_LOG_ONLY_ERROR)
#define MP_LOG_LEVEL_ERROR  (MP_LOG_ONLY_ERROR)

#include <stdio.h>

//
// Implementation - Logging enabled
//

#if _MP_LOG_ENABLED

#define SET_LOG_LEVEL(l) static int PLEASE_CALL_SET_LOG_LEVEL_FIRST = (l)

#define MPLogDebug(...) _MPLog_raw("DEBUG", MP_LOG_ONLY_DEBUG, ##__VA_ARGS__)
#define MPLogInfo(...) _MPLog_raw("INFO", MP_LOG_ONLY_INFO, ##__VA_ARGS__)
#define MPLogWarn(...) _MPLog_raw("WARN", MP_LOG_ONLY_WARN, ##__VA_ARGS__)
#define MPLogError(...) _MPLog_raw("ERROR", MP_LOG_ONLY_ERROR, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void _MPLog_impl(const char *filename, int linenumber, const char *log_type, NSString *format, ...) NS_FORMAT_FUNCTION(4,5) NS_SWIFT_UNAVAILABLE("Use _MPLog_impl_swift in Swift");
void _MPLog_impl_swift(NSString *message, const char *log_type, const char *filename_obj, int linenumber);

#define _MPLog_raw(type_str, type_flag, ...) do { if(PLEASE_CALL_SET_LOG_LEVEL_FIRST & type_flag) { _MPLog_impl(__FILE__, __LINE__, type_str, ##__VA_ARGS__); } } while(0)

/// The file to log to. Defaults to NULL. If this variable is NULL, MPLog will
/// log to stderr.
extern FILE *_MPLog_file;

#ifdef __cplusplus
}
#endif


//
// Implementation - Logging disabled
//

#else

#define SET_LOG_LEVEL(l)

#define MPLogDebug(...) do { } while(0)
#define MPLogInfo(...) do { } while(0)
#define MPLogWarn(...) do { } while(0)
#define MPLogError(...) do { } while(0)


#endif



//
// Implementation - Convenience functions
//

#define MPLogDebugStart() MPLogDebug(@"Start %s", __FUNCTION__)
#define MPLogInfoStart() MPLogInfo(@"Start %s", __FUNCTION__)
