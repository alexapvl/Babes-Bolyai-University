//
//  Entity.swift
//  exam_mobile
//
//  Universal data model that works for ALL exam variants.
//  The model structure is identical across all exams - only the semantic meaning changes.
//

import Foundation
import SwiftData

// MARK: - Entity Model
/// Universal entity model that works for all exam variants.
/// Fields are the same across all exams:
/// - orders (restaurant), expenses (medical), tickets (movies),
/// - rentals (cars), logs (calories), fees (library)
@Model
final class Entity: Identifiable {
    
    // MARK: - Properties
    
    /// Unique identifier from the server
    /// Note: Using a separate localId for SwiftData, serverId for API
    @Attribute(.unique) var serverId: Int
    
    /// Date when the transaction occurred (format: "YYYY-MM-DD")
    var date: String
    
    /// The amount/cost value (decimal)
    var amount: Double
    
    /// Type of transaction (e.g., "dine-in", "consultation", "ticket", "rental", "intake", "fine")
    var type: String
    
    /// Category of the item (e.g., "pizza", "dental", "sci-fi", "sedan", "lunch", "late_return")
    var category: String
    
    /// Description of the transaction
    var itemDescription: String
    
    // MARK: - Computed Properties
    
    /// Formatted amount with currency
    var formattedAmount: String {
        String(format: "$%.2f", amount)
    }
    
    /// Parsed date for sorting/grouping
    var parsedDate: Date? {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy-MM-dd"
        return formatter.date(from: date)
    }
    
    /// Month-Year string for grouping in reports (e.g., "2026-01")
    var monthYear: String {
        String(date.prefix(7))
    }
    
    /// Formatted date for display
    var formattedDate: String {
        if let parsed = parsedDate {
            let formatter = DateFormatter()
            formatter.dateStyle = .medium
            return formatter.string(from: parsed)
        }
        return date
    }
    
    // MARK: - Initializer
    
    init(serverId: Int, date: String, amount: Double, type: String, category: String, itemDescription: String) {
        self.serverId = serverId
        self.date = date
        self.amount = amount
        self.type = type
        self.category = category
        self.itemDescription = itemDescription
    }
    
    /// Convenience initializer from DTO
    convenience init(from dto: EntityDTO) {
        self.init(
            serverId: dto.id,
            date: dto.date,
            amount: dto.amount,
            type: dto.type,
            category: dto.category,
            itemDescription: dto.description
        )
    }
    
    /// Update from DTO (for refreshing cached data)
    func update(from dto: EntityDTO) {
        self.date = dto.date
        self.amount = dto.amount
        self.type = dto.type
        self.category = dto.category
        self.itemDescription = dto.description
    }
}

// MARK: - Entity DTO (Data Transfer Object)
/// Used for JSON encoding/decoding with the server.
/// Separate from the SwiftData model to avoid Codable conflicts.
struct EntityDTO: Codable, Identifiable {
    let id: Int
    let date: String
    let amount: Double
    let type: String
    let category: String
    let description: String
    
    // MARK: - Create DTO for POST request (without id)
    struct CreateRequest: Codable {
        let date: String
        let amount: Double
        let type: String
        let category: String
        let description: String
    }
}

// MARK: - Extension for creating request payload
extension Entity {
    /// Convert to DTO for API requests
    var toDTO: EntityDTO {
        EntityDTO(
            id: serverId,
            date: date,
            amount: amount,
            type: type,
            category: category,
            description: itemDescription
        )
    }
    
    /// Create a request payload for POST
    var toCreateRequest: EntityDTO.CreateRequest {
        EntityDTO.CreateRequest(
            date: date,
            amount: amount,
            type: type,
            category: category,
            description: itemDescription
        )
    }
}

// MARK: - Sample Data for Previews
extension Entity {
    static var sample: Entity {
        Entity(
            serverId: 1,
            date: "2026-01-15",
            amount: 25.50,
            type: "delivery",
            category: "pizza",
            itemDescription: "Pepperoni Large"
        )
    }
    
    static var samples: [Entity] {
        [
            Entity(serverId: 1, date: "2026-01-15", amount: 25.50, type: "delivery", category: "pizza", itemDescription: "Pepperoni Large"),
            Entity(serverId: 2, date: "2026-01-14", amount: 15.00, type: "dine-in", category: "sushi", itemDescription: "California Roll"),
            Entity(serverId: 3, date: "2026-02-01", amount: 35.00, type: "takeout", category: "burger", itemDescription: "Double Cheeseburger Combo")
        ]
    }
}
