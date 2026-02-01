//
//  AppConfig.swift
//  exam_mobile
//
//  Configuration file for the exam template.
//  MODIFY THIS FILE FOR EACH EXAM - this is the only file you need to change!
//

import Foundation

// MARK: - ⚠️ MODIFY THESE VALUES FOR EACH EXAM ⚠️
struct AppConfig {
    
    // MARK: - Entity Configuration
    // These define the API endpoints and UI labels
    
    /// Singular form of the entity name (used in API endpoints like /order/:id)
    /// Examples: "order", "expense", "ticket", "rental", "log", "fee"
    static let entityNameSingular = "order"
    
    /// Plural form of the entity name (used in API endpoints like /orders)
    /// Examples: "orders", "expenses", "tickets", "rentals", "logs", "fees"
    static let entityNamePlural = "orders"
    
    /// Capitalized form for UI display
    /// Examples: "Order", "Expense", "Ticket", "Rental", "Log", "Fee"
    static let entityNameCapitalized = "Order"
    
    /// Plural capitalized form for UI display
    /// Examples: "Orders", "Expenses", "Tickets", "Rentals", "Logs", "Fees"
    static let entityNamePluralCapitalized = "Orders"
    
    // MARK: - Server Configuration
    // Check the Exam.md file for the correct port number
    
    /// Server port number from Exam.md
    /// Exam 30b (orders): 2625
    /// Exam 30a (expenses): 2624
    /// Exam 26b (tickets): 2623
    /// Exam 26a (rentals): 2622
    /// Exam 20b (logs): 2621
    /// Exam 20a (fees): 2620
    static let serverPort = 2625
    
    // MARK: - Computed URLs (DO NOT MODIFY)
    
    /// Base URL for HTTP requests
    static var baseURL: String {
        "http://localhost:\(serverPort)"
    }
    
    /// WebSocket URL for real-time notifications
    static var wsURL: String {
        "ws://localhost:\(serverPort)"
    }
    
    // MARK: - API Endpoints (DO NOT MODIFY)
    // These are automatically derived from entity names
    
    /// GET all entities endpoint (e.g., /orders)
    static var listEndpoint: String {
        "/\(entityNamePlural)"
    }
    
    /// GET/POST/DELETE single entity endpoint base (e.g., /order)
    static var singleEndpoint: String {
        "/\(entityNameSingular)"
    }
    
    /// GET all entities for reports endpoint (e.g., /allOrders)
    static var allEndpoint: String {
        "/all\(entityNamePluralCapitalized)"
    }
    
    // MARK: - UI Configuration (Optional customization)
    
    /// App title shown in navigation
    static var appTitle: String {
        "\(entityNamePluralCapitalized) Manager"
    }
    
    /// Main section tab title
    static var mainTabTitle: String {
        entityNamePluralCapitalized
    }
    
    /// Reports section tab title
    static var reportsTabTitle: String {
        "Reports"
    }
    
    /// Insights section tab title
    static var insightsTabTitle: String {
        "Insights"
    }
}

// MARK: - Quick Reference for Exam Configuration
/*
 
 EXAM 30b - Restaurant Orders:
 entityNameSingular = "order"
 entityNamePlural = "orders"
 entityNameCapitalized = "Order"
 entityNamePluralCapitalized = "Orders"
 serverPort = 2625
 
 EXAM 30a - Medical Expenses:
 entityNameSingular = "expense"
 entityNamePlural = "expenses"
 entityNameCapitalized = "Expense"
 entityNamePluralCapitalized = "Expenses"
 serverPort = 2624
 
 EXAM 26b - Movie Tickets:
 entityNameSingular = "ticket"
 entityNamePlural = "tickets"
 entityNameCapitalized = "Ticket"
 entityNamePluralCapitalized = "Tickets"
 serverPort = 2623
 
 EXAM 26a - Car Rentals:
 entityNameSingular = "rental"
 entityNamePlural = "rentals"
 entityNameCapitalized = "Rental"
 entityNamePluralCapitalized = "Rentals"
 serverPort = 2622
 
 EXAM 20b - Calorie Logs:
 entityNameSingular = "log"
 entityNamePlural = "logs"
 entityNameCapitalized = "Log"
 entityNamePluralCapitalized = "Logs"
 serverPort = 2621
 
 EXAM 20a - Library Fees:
 entityNameSingular = "fee"
 entityNamePlural = "fees"
 entityNameCapitalized = "Fee"
 entityNamePluralCapitalized = "Fees"
 serverPort = 2620
 
 */
