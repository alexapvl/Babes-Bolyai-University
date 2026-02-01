//
//  NetworkMonitor.swift
//  exam_mobile
//
//  Monitors network connectivity status.
//  Used to determine if the app is online or offline.
//

import Foundation
import Network
import SwiftUI

// MARK: - Network Monitor
/// Observable class that monitors network connectivity.
/// Use this to check if the device is online before making API calls.
@Observable
final class NetworkMonitor {
    
    // MARK: - Properties
    
    /// Whether the device is currently connected to the network
    private(set) var isConnected: Bool = true
    
    /// The type of connection (wifi, cellular, etc.)
    private(set) var connectionType: ConnectionType = .unknown
    
    /// The underlying NWPathMonitor
    private let monitor = NWPathMonitor()
    
    /// Queue for monitoring
    private let queue = DispatchQueue(label: "NetworkMonitor")
    
    // MARK: - Connection Type
    
    enum ConnectionType {
        case wifi
        case cellular
        case wiredEthernet
        case unknown
    }
    
    // MARK: - Initialization
    
    init() {
        startMonitoring()
    }
    
    deinit {
        stopMonitoring()
    }
    
    // MARK: - Monitoring
    
    /// Start monitoring network changes
    func startMonitoring() {
        monitor.pathUpdateHandler = { [weak self] path in
            DispatchQueue.main.async {
                self?.isConnected = path.status == .satisfied
                self?.updateConnectionType(path)
                
                // Log network status change
                Logger.log("Network status changed: \(path.status == .satisfied ? "Online" : "Offline")")
            }
        }
        monitor.start(queue: queue)
    }
    
    /// Stop monitoring network changes
    func stopMonitoring() {
        monitor.cancel()
    }
    
    // MARK: - Private Methods
    
    private func updateConnectionType(_ path: NWPath) {
        if path.usesInterfaceType(.wifi) {
            connectionType = .wifi
        } else if path.usesInterfaceType(.cellular) {
            connectionType = .cellular
        } else if path.usesInterfaceType(.wiredEthernet) {
            connectionType = .wiredEthernet
        } else {
            connectionType = .unknown
        }
    }
}

// MARK: - Environment Key
/// Environment key for injecting NetworkMonitor
private struct NetworkMonitorKey: EnvironmentKey {
    static let defaultValue = NetworkMonitor()
}

extension EnvironmentValues {
    var networkMonitor: NetworkMonitor {
        get { self[NetworkMonitorKey.self] }
        set { self[NetworkMonitorKey.self] = newValue }
    }
}
