//
//  Logger.swift
//  exam_mobile
//
//  Utility for logging all server and database interactions.
//  Required by exam: "all interactions (server or DB) will log a message"
//

import Foundation
import os.log

// MARK: - Logger
/// Centralized logging utility for the app.
/// Logs all server and database interactions as required by the exam.
struct Logger {
    
    // MARK: - Log Categories
    
    private static let subsystem = Bundle.main.bundleIdentifier ?? "exam_mobile"
    
    private static let networkLogger = os.Logger(subsystem: subsystem, category: "Network")
    private static let databaseLogger = os.Logger(subsystem: subsystem, category: "Database")
    private static let webSocketLogger = os.Logger(subsystem: subsystem, category: "WebSocket")
    private static let generalLogger = os.Logger(subsystem: subsystem, category: "General")
    
    // MARK: - Log Levels
    
    enum Level: String {
        case debug = "🔍 DEBUG"
        case info = "ℹ️ INFO"
        case warning = "⚠️ WARNING"
        case error = "❌ ERROR"
        case success = "✅ SUCCESS"
    }
    
    // MARK: - General Logging
    
    /// Log a general message
    static func log(_ message: String, level: Level = .info) {
        let formattedMessage = "[\(level.rawValue)] \(message)"
        generalLogger.log("\(formattedMessage)")
        #if DEBUG
        print(formattedMessage)
        #endif
    }
    
    // MARK: - Network Logging
    
    /// Log a network request
    static func logRequest(method: String, url: String, body: Any? = nil) {
        var message = "[REQUEST] \(method) \(url)"
        if let body = body {
            message += " | Body: \(body)"
        }
        networkLogger.log("\(message)")
        #if DEBUG
        print("🌐 \(message)")
        #endif
    }
    
    /// Log a network response
    static func logResponse(method: String, url: String, statusCode: Int, duration: TimeInterval) {
        let message = "[RESPONSE] \(method) \(url) | Status: \(statusCode) | Duration: \(String(format: "%.2f", duration * 1000))ms"
        networkLogger.log("\(message)")
        #if DEBUG
        print("🌐 \(message)")
        #endif
    }
    
    /// Log a network error
    static func logNetworkError(_ error: Error, method: String, url: String) {
        let message = "[ERROR] \(method) \(url) | Error: \(error.localizedDescription)"
        networkLogger.error("\(message)")
        #if DEBUG
        print("🌐 ❌ \(message)")
        #endif
    }
    
    // MARK: - Database Logging
    
    /// Log a database operation
    static func logDatabase(operation: String, entity: String, id: Int? = nil) {
        var message = "[DB] \(operation) \(entity)"
        if let id = id {
            message += " (id: \(id))"
        }
        databaseLogger.log("\(message)")
        #if DEBUG
        print("💾 \(message)")
        #endif
    }
    
    /// Log a database error
    static func logDatabaseError(_ error: Error, operation: String) {
        let message = "[DB ERROR] \(operation) | Error: \(error.localizedDescription)"
        databaseLogger.error("\(message)")
        #if DEBUG
        print("💾 ❌ \(message)")
        #endif
    }
    
    // MARK: - WebSocket Logging
    
    /// Log a WebSocket event
    static func logWebSocket(event: String, data: Any? = nil) {
        var message = "[WS] \(event)"
        if let data = data {
            message += " | Data: \(data)"
        }
        webSocketLogger.log("\(message)")
        #if DEBUG
        print("🔌 \(message)")
        #endif
    }
    
    /// Log a WebSocket error
    static func logWebSocketError(_ error: Error) {
        let message = "[WS ERROR] \(error.localizedDescription)"
        webSocketLogger.error("\(message)")
        #if DEBUG
        print("🔌 ❌ \(message)")
        #endif
    }
}
