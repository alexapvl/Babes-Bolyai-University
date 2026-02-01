//
//  AlertManager.swift
//  exam_mobile
//
//  Simple alert manager for displaying errors and notifications to the user.
//  Uses native SwiftUI alerts.
//

import SwiftUI

// MARK: - Alert Item
/// Represents an alert to display to the user
struct AlertItem: Identifiable {
    let id = UUID()
    let title: String
    let message: String
    let type: AlertType
    
    enum AlertType {
        case error
        case success
        case info
        case websocket
    }
}

// MARK: - Alert Manager
/// Observable manager for displaying alerts
@Observable
final class AlertManager {
    
    // MARK: - Properties
    
    /// Current alert to display (nil if none)
    var currentAlert: AlertItem?
    
    /// Trigger to refresh data after WebSocket notification is dismissed
    /// Increment this to signal views to refresh their data
    var refreshTrigger: Int = 0
    
    // MARK: - Show Methods
    
    /// Show a success alert
    func showSuccess(_ message: String) {
        currentAlert = AlertItem(title: "Success", message: message, type: .success)
    }
    
    /// Show an error alert
    func showError(_ message: String) {
        currentAlert = AlertItem(title: "Error", message: message, type: .error)
    }
    
    /// Show an info alert
    func showInfo(_ message: String) {
        currentAlert = AlertItem(title: "Info", message: message, type: .info)
    }
    
    /// Show a WebSocket notification alert
    func showWebSocketNotification(_ message: String) {
        currentAlert = AlertItem(title: "New Notification", message: message, type: .websocket)
    }
    
    /// Dismiss the current alert
    /// If it was a WebSocket notification, trigger a refresh
    func dismiss() {
        let wasWebSocket = currentAlert?.type == .websocket
        currentAlert = nil
        
        // Trigger refresh if dismissing a WebSocket notification
        if wasWebSocket {
            refreshTrigger += 1
        }
    }
}

// MARK: - Alert View Modifier
/// View modifier that adds alert display capability to any view
struct AlertModifier: ViewModifier {
    @Bindable var alertManager: AlertManager
    
    func body(content: Content) -> some View {
        content
            .alert(
                alertManager.currentAlert?.title ?? "",
                isPresented: Binding(
                    get: { alertManager.currentAlert != nil },
                    set: { if !$0 { alertManager.dismiss() } }
                ),
                presenting: alertManager.currentAlert
            ) { _ in
                Button("OK", role: .cancel) {
                    alertManager.dismiss()
                }
            } message: { alert in
                Text(alert.message)
            }
    }
}

// MARK: - View Extension
extension View {
    /// Add alert display capability to the view
    func alertContainer(_ manager: AlertManager) -> some View {
        modifier(AlertModifier(alertManager: manager))
    }
}
