//
//  WebSocketService.swift
//  exam_mobile
//
//  Handles WebSocket connection for real-time notifications.
//  When a new entity is added on the server, all clients receive the update.
//

import Foundation

// MARK: - WebSocket Service
/// Handles WebSocket connection for real-time notifications.
/// Required by exam: "When a new entity is added, the server will use a WebSocket channel
/// to send the details to all connected clients."
@Observable
final class WebSocketService: NSObject, URLSessionWebSocketDelegate {
    
    // MARK: - Properties
    
    /// Whether the WebSocket is connected
    private(set) var isConnected: Bool = false
    
    /// Last received entity from WebSocket
    private(set) var lastReceivedEntity: EntityDTO?
    
    /// Flag to show notification (reset after displaying)
    var showNotification: Bool = false
    
    // MARK: - Private Properties
    
    private var webSocketTask: URLSessionWebSocketTask?
    private var session: URLSession?
    private let decoder = JSONDecoder()
    private var pingTimer: Timer?
    
    // MARK: - Initialization
    
    override init() {
        super.init()
        // Will connect when connect() is called
    }
    
    deinit {
        disconnect()
    }
    
    // MARK: - Connection Management
    
    /// Connect to the WebSocket server
    func connect() {
        guard webSocketTask == nil else {
            Logger.logWebSocket(event: "Already connected")
            return
        }
        
        guard let url = URL(string: AppConfig.wsURL) else {
            Logger.logWebSocketError(APIError.invalidURL)
            return
        }
        
        Logger.logWebSocket(event: "Connecting to \(AppConfig.wsURL)")
        
        // Create session with delegate to track connection state
        session = URLSession(configuration: .default, delegate: self, delegateQueue: .main)
        webSocketTask = session?.webSocketTask(with: url)
        webSocketTask?.resume()
        
        // Start receiving messages immediately
        receiveMessage()
        
        // Start ping timer to keep connection alive
        startPingTimer()
        
        Logger.logWebSocket(event: "Connection initiated")
    }
    
    /// Disconnect from the WebSocket server
    func disconnect() {
        Logger.logWebSocket(event: "Disconnecting")
        stopPingTimer()
        webSocketTask?.cancel(with: .normalClosure, reason: nil)
        webSocketTask = nil
        session?.invalidateAndCancel()
        session = nil
        isConnected = false
    }
    
    // MARK: - Keep-Alive Ping
    
    /// Start ping timer to keep WebSocket connection alive
    private func startPingTimer() {
        stopPingTimer()
        pingTimer = Timer.scheduledTimer(withTimeInterval: 30, repeats: true) { [weak self] _ in
            self?.sendPing()
        }
    }
    
    /// Stop ping timer
    private func stopPingTimer() {
        pingTimer?.invalidate()
        pingTimer = nil
    }
    
    /// Send ping to keep connection alive
    private func sendPing() {
        webSocketTask?.sendPing { [weak self] error in
            if let error = error {
                Logger.logWebSocketError(error)
                DispatchQueue.main.async {
                    self?.isConnected = false
                    self?.reconnect()
                }
            } else {
                Logger.logWebSocket(event: "Ping successful")
            }
        }
    }
    
    // MARK: - URLSessionWebSocketDelegate
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        Logger.logWebSocket(event: "WebSocket connected")
        DispatchQueue.main.async { [weak self] in
            self?.isConnected = true
        }
    }
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        Logger.logWebSocket(event: "WebSocket disconnected with code: \(closeCode.rawValue)")
        DispatchQueue.main.async { [weak self] in
            self?.isConnected = false
        }
    }
    
    // MARK: - Message Receiving
    
    /// Receive messages from the WebSocket
    private func receiveMessage() {
        webSocketTask?.receive { [weak self] result in
            guard let self = self else { return }
            
            switch result {
            case .success(let message):
                self.handleMessage(message)
                // Continue receiving messages
                self.receiveMessage()
                
            case .failure(let error):
                Logger.logWebSocketError(error)
                DispatchQueue.main.async {
                    self.isConnected = false
                }
                // Try to reconnect after a delay
                DispatchQueue.main.asyncAfter(deadline: .now() + 5) { [weak self] in
                    self?.reconnect()
                }
            }
        }
    }
    
    /// Handle received WebSocket message
    private func handleMessage(_ message: URLSessionWebSocketTask.Message) {
        switch message {
        case .string(let text):
            Logger.logWebSocket(event: "Received message", data: text)
            parseMessage(text)
            
        case .data(let data):
            if let text = String(data: data, encoding: .utf8) {
                Logger.logWebSocket(event: "Received data", data: text)
                parseMessage(text)
            }
            
        @unknown default:
            Logger.logWebSocket(event: "Received unknown message type")
        }
    }
    
    /// Parse the received message as EntityDTO
    private func parseMessage(_ text: String) {
        guard let data = text.data(using: .utf8) else { return }
        
        do {
            let entity = try decoder.decode(EntityDTO.self, from: data)
            DispatchQueue.main.async { [weak self] in
                self?.lastReceivedEntity = entity
                self?.showNotification = true
                Logger.logWebSocket(event: "Parsed entity", data: "ID: \(entity.id), Type: \(entity.type)")
            }
        } catch {
            Logger.logWebSocketError(error)
        }
    }
    
    /// Attempt to reconnect
    private func reconnect() {
        guard !isConnected else { return }
        Logger.logWebSocket(event: "Attempting to reconnect...")
        connect()
    }
    
    // MARK: - Notification Handling
    
    /// Clear the notification flag
    func clearNotification() {
        showNotification = false
        lastReceivedEntity = nil
    }
    
    /// Get a human-readable description of the last received entity
    var notificationMessage: String {
        guard let entity = lastReceivedEntity else { return "" }
        return "New \(AppConfig.entityNameSingular) added: \(entity.description) (\(String(format: "$%.2f", entity.amount)))"
    }
}
