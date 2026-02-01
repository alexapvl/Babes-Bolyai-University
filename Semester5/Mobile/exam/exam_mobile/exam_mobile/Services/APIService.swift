//
//  APIService.swift
//  exam_mobile
//
//  Handles all HTTP API communication with the server.
//  Endpoints are automatically derived from AppConfig.
//

import Foundation

// MARK: - API Error
/// Custom error type for API operations
enum APIError: LocalizedError {
    case invalidURL
    case networkError(Error)
    case serverError(Int, String)
    case decodingError(Error)
    case noData
    case offline
    
    var errorDescription: String? {
        switch self {
        case .invalidURL:
            return "Invalid URL"
        case .networkError(let error):
            return "Network error: \(error.localizedDescription)"
        case .serverError(let code, let message):
            return "Server error (\(code)): \(message)"
        case .decodingError(let error):
            return "Failed to decode response: \(error.localizedDescription)"
        case .noData:
            return "No data received from server"
        case .offline:
            return "No internet connection"
        }
    }
}

// MARK: - API Service
/// Handles all HTTP communication with the server.
/// All endpoints are derived from AppConfig - no changes needed per exam.
@Observable
final class APIService {
    
    // MARK: - Properties
    
    /// Loading state for UI progress indicators
    private(set) var isLoading: Bool = false
    
    /// Last error message for UI display
    private(set) var lastError: String?
    
    // MARK: - Private Properties
    
    private let session: URLSession
    private let decoder: JSONDecoder
    private let encoder: JSONEncoder
    
    // MARK: - Initialization
    
    init() {
        let config = URLSessionConfiguration.default
        config.timeoutIntervalForRequest = 30
        config.timeoutIntervalForResource = 60
        self.session = URLSession(configuration: config)
        self.decoder = JSONDecoder()
        self.encoder = JSONEncoder()
    }
    
    // MARK: - CRUD Operations
    
    /// GET all entities (e.g., GET /orders)
    /// Used in Main Section - List View
    func fetchAll() async throws -> [EntityDTO] {
        let url = "\(AppConfig.baseURL)\(AppConfig.listEndpoint)"
        return try await performRequest(method: "GET", url: url)
    }
    
    /// GET single entity by ID (e.g., GET /order/:id)
    /// Used in Main Section - Detail View
    func fetchOne(id: Int) async throws -> EntityDTO {
        let url = "\(AppConfig.baseURL)\(AppConfig.singleEndpoint)/\(id)"
        return try await performRequest(method: "GET", url: url)
    }
    
    /// POST create new entity (e.g., POST /order)
    /// Used in Main Section - Add View (online only)
    func create(_ request: EntityDTO.CreateRequest) async throws -> EntityDTO {
        let url = "\(AppConfig.baseURL)\(AppConfig.singleEndpoint)"
        let body = try encoder.encode(request)
        return try await performRequest(method: "POST", url: url, body: body)
    }
    
    /// DELETE entity by ID (e.g., DELETE /order/:id)
    /// Used in Main Section - Delete (online only)
    func delete(id: Int) async throws -> EntityDTO {
        let url = "\(AppConfig.baseURL)\(AppConfig.singleEndpoint)/\(id)"
        return try await performRequest(method: "DELETE", url: url)
    }
    
    /// GET all entities for reports (e.g., GET /allOrders)
    /// Used in Reports and Insights sections
    func fetchAllForReports() async throws -> [EntityDTO] {
        let url = "\(AppConfig.baseURL)\(AppConfig.allEndpoint)"
        return try await performRequest(method: "GET", url: url)
    }
    
    // MARK: - Private Request Handler
    
    private func performRequest<T: Decodable>(method: String, url urlString: String, body: Data? = nil) async throws -> T {
        // Update loading state
        await MainActor.run { isLoading = true }
        defer { Task { @MainActor in isLoading = false } }
        
        // Validate URL
        guard let url = URL(string: urlString) else {
            Logger.logNetworkError(APIError.invalidURL, method: method, url: urlString)
            throw APIError.invalidURL
        }
        
        // Create request
        var request = URLRequest(url: url)
        request.httpMethod = method
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        if let body = body {
            request.httpBody = body
        }
        
        // Log request
        Logger.logRequest(method: method, url: urlString, body: body.flatMap { String(data: $0, encoding: .utf8) })
        
        let startTime = Date()
        
        do {
            // Perform request
            let (data, response) = try await session.data(for: request)
            
            let duration = Date().timeIntervalSince(startTime)
            
            // Check HTTP response
            guard let httpResponse = response as? HTTPURLResponse else {
                throw APIError.noData
            }
            
            // Log response
            Logger.logResponse(method: method, url: urlString, statusCode: httpResponse.statusCode, duration: duration)
            
            // Handle error status codes
            if httpResponse.statusCode >= 400 {
                let errorMessage = String(data: data, encoding: .utf8) ?? "Unknown error"
                let error = APIError.serverError(httpResponse.statusCode, errorMessage)
                await MainActor.run { lastError = error.localizedDescription }
                throw error
            }
            
            // Decode response
            do {
                let decoded = try decoder.decode(T.self, from: data)
                await MainActor.run { lastError = nil }
                return decoded
            } catch {
                Logger.logNetworkError(error, method: method, url: urlString)
                throw APIError.decodingError(error)
            }
            
        } catch let error as APIError {
            throw error
        } catch {
            Logger.logNetworkError(error, method: method, url: urlString)
            await MainActor.run { lastError = error.localizedDescription }
            throw APIError.networkError(error)
        }
    }
    
    // MARK: - Error Clearing
    
    func clearError() {
        lastError = nil
    }
}
