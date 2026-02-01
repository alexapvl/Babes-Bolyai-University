//
//  MainTabView.swift
//  exam_mobile
//
//  Main navigation container with tabs for Main, Reports, and Insights sections.
//  Handles WebSocket notifications and displays toasts.
//

import SwiftUI
import SwiftData

// MARK: - Main Tab View
/// Root view containing the three main sections of the app.
/// Tabs: Main (list), Reports, Insights
struct MainTabView: View {
    
    // MARK: - Environment & State
    
    @Environment(\.modelContext) private var modelContext
    @State private var apiService = APIService()
    @State private var networkMonitor = NetworkMonitor()
    @State private var webSocketService = WebSocketService()
    @State private var alertManager = AlertManager()
    
    @State private var selectedTab = 0
    
    // MARK: - Body
    
    var body: some View {
        TabView(selection: $selectedTab) {
            // MARK: - Main Section Tab
            NavigationStack {
                EntityListView()
            }
            .tabItem {
                Label(AppConfig.mainTabTitle, systemImage: "list.bullet")
            }
            .tag(0)
            
            // MARK: - Reports Section Tab
            NavigationStack {
                ReportsView()
            }
            .tabItem {
                Label(AppConfig.reportsTabTitle, systemImage: "chart.bar")
            }
            .tag(1)
            
            // MARK: - Insights Section Tab
            NavigationStack {
                InsightsView()
            }
            .tabItem {
                Label(AppConfig.insightsTabTitle, systemImage: "lightbulb")
            }
            .tag(2)
        }
        .environment(apiService)
        .environment(networkMonitor)
        .environment(webSocketService)
        .environment(alertManager)
        .alertContainer(alertManager)
        .onAppear {
            // Connect WebSocket when app appears
            webSocketService.connect()
            Logger.log("App started - WebSocket connecting")
        }
        .onChange(of: webSocketService.showNotification) { _, show in
            // Show alert when WebSocket receives a new entity
            if show {
                alertManager.showWebSocketNotification(webSocketService.notificationMessage)
                webSocketService.clearNotification()
            }
        }
        .onChange(of: apiService.lastError) { _, error in
            // Show error alert when API encounters an error
            if let error = error {
                alertManager.showError(error)
                apiService.clearError()
            }
        }
    }
}

// MARK: - Preview
#Preview {
    MainTabView()
        .modelContainer(for: Entity.self, inMemory: true)
}
