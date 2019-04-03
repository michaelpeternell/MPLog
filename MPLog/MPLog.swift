//
//  MPLog.swift
//  Mines1337
//
//  Copyright (c) 2019 Michael Peternell. All rights reserved.
//

import Foundation

#if DEBUG

func MPLogDebug(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    _MPLog_impl_swift(message, "DEBUG", filename, linenumber)
}

func MPLogInfo(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    _MPLog_impl_swift(message, "INFO", filename, linenumber)
}

func MPLogWarn(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    _MPLog_impl_swift(message, "WARN", filename, linenumber)
}

func MPLogError(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    _MPLog_impl_swift(message, "ERROR", filename, linenumber)
}

func MPLogDebugStart(_ myself: Any? = nil, filename: String = #file, linenumber: CInt = #line, fName: String = #function) {
    let message: String
    if let myself = myself {
        message = "Start \(fName) (\(myself))"
    } else {
        message = "Start \(fName)"
    }
    MPLogDebug(message, filename: filename, linenumber: linenumber)
}

func MPLogInfoStart(_ myself: Any? = nil, filename: String = #file, linenumber: CInt = #line, fName: String = #function) {
    let message: String
    if let myself = myself {
        message = "Start \(fName) (\(myself))"
    } else {
        message = "Start \(fName)"
    }
    MPLogInfo(message, filename: filename, linenumber: linenumber)
}

#else

func MPLogDebug(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    // disabled in Release mode
}

func MPLogInfo(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    // disabled in Release mode
}

func MPLogWarn(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    // disabled in Release mode
}

func MPLogError(_ message: String, filename: String = #file, linenumber: CInt = #line) {
    // disabled in Release mode
}

func MPLogDebugStart(_ myself: Any? = nil, filename: String = #file, linenumber: CInt = #line, fName: String = #function) {
    // disabled in Release mode
}

func MPLogInfoStart(_ myself: Any? = nil, filename: String = #file, linenumber: CInt = #line, fName: String = #function) {
    // disabled in Release mode
}

#endif
