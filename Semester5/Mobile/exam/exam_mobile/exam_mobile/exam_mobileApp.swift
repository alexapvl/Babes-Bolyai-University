//
//  exam_mobileApp.swift
//  exam_mobile
//
//  Main app entry point.
//  Sets up SwiftData container and launches MainTabView.
//

import SwiftUI
import SwiftData

@main
struct exam_mobileApp: App {
    
    // MARK: - SwiftData Container
    
    /// Shared model container for offline data persistence
    /// Uses Entity model for caching server data
    var sharedModelContainer: ModelContainer = {
        let schema = Schema([
            Entity.self,
        ])
        let modelConfiguration = ModelConfiguration(
            schema: schema,
            isStoredInMemoryOnly: false
        )
        
        do {
            return try ModelContainer(for: schema, configurations: [modelConfiguration])
        } catch {
            fatalError("Could not create ModelContainer: \(error)")
        }
    }()
    
    // MARK: - App Body
    
    var body: some Scene {
        WindowGroup {
            MainTabView()
        }
        .modelContainer(sharedModelContainer)
    }
}

// MARK: - App Configuration Notes
/*
 
 This app uses the following architecture:
 
 1. SwiftData for offline data persistence
    - Entity model stores cached data from the server
    - Data persists between app launches
    - Automatically syncs with server when online
 
 2. Services (injected via @Environment):
    - APIService: Handles all HTTP requests
    - NetworkMonitor: Tracks online/offline status
    - WebSocketService: Receives real-time notifications
    - ToastManager: Displays notifications and errors
 
 3. Views:
    - MainTabView: Root view with 3 tabs
    - EntityListView: Main list with CRUD operations
    - EntityDetailView: Detailed view of single entity
    - AddEntityView: Form for creating new entities
    - ReportsView: Monthly totals analysis
    - InsightsView: Top 3 categories
 
 To adapt for a different exam:
 1. Open Config/AppConfig.swift
 2. Change entity names and port number
 3. That's it! The rest adapts automatically.
 
 */
