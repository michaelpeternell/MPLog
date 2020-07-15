# Introduction

MPLog is a very lightweight logging framework for Objective C and Swift, written in Objective-C. (If you are looking for a pure Swift version, see my [MPLogSwift](https://github.com/michaelpeternell/MPLogSwift/) library.)

It has less than 200 source lines of code, it is very performant, it has been
production-tested extensively, but it is still much more useful than `NSLog`
or `print`.

Compatible (and tested) with:

- Objective C
- Swift 3
- Swift 4
- Swift 5
- iOS 9, iOS 10, iOS 11, iOS 12, iOS 13
- macOS 10.6+

# Installation

If you want to use MPLog in **Swift**, add MPLog.h, MPLog.m and MPLog.swift to
your project, and also include MPLog.h in your bridging header.

See the included MPLogDemo project for an example use case (deployment target iOS 9).

If you want to use MPLog only from **Objective C**, just add MPLog.h and
MPLog.m to your project and follow the usage instructions in MPLog.h .

# Motivation

Have you ever been frustrated about the limited usefulness of using `NSLog()`
for debugging your code and seeing what is going on in the log messages?
If the answer is `YES`, then `MPLog` is for you!

What do you think about NSLog?

    NSLog(@"(%d)", 1335+2);
    => 2013-10-02 19:21:25.242 dynimg[21068:707] (1337)

Hmm, where did that log message originate from? Hmm, let's just search the
source code for the string 1337... No match? Hmm... We need a decent logging
framework

# Use with Swift

Just use one of the log functions:

- MPLogDebug for verbose log messages
- MPLogInfo for informative messages that are either user-triggered or important
- MPLogWarn for warnings
- MPLogError for errors

Additionally, there are two functions that can be placed at the start of
methods just to indicate that the method started:

- MPLogDebugStart
- MPLogInfoStart

MPLogInfoStart is mostly equivalent to MPLogInfo("Start " + #function).

# Use with Objective C

See the comment section in MPLog.h for detailed usage instructions

# Log message format

MPLog is a logging framework for iOS that allows logging to the console in a
very sophisticated way. There are the following advantages over `NSLog()`:

1. There are four log levels: `DEBUG`, `INFO`, `WARN` and `ERROR`
2. Each log message contains the filename and the linenumber from which
     the log message originated.
3. The date and the name of the binary is missing, and instead, only the
     time is shown (with millisecond precision.)
4. The second column contains an indication of the thread that the message
     came from. This is `M` for the main thread, and the letters `a` to `z`
     for background threads. Letters are reused if necessary and possible.
     If there are more than 26 background threads, however, this column may
     just say `+`. I did a lot of iOS coding and this has never happened to
     me! (Except once, when I tried to test the MPLog system.)

