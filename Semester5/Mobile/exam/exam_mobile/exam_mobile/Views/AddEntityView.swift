//
//  AddEntityView.swift
//  exam_mobile
//
//  Main Section: Form to create a new entity.
//  Feature: Online only - cannot add while offline.
//

import SwiftUI
import SwiftData

// MARK: - Add Entity View
/// Form for creating a new entity.
/// Requirement C: Add a new entity (1p)
/// - Uses POST /entity to create
/// - Available online only
struct AddEntityView: View {
    
    // MARK: - Environment & State
    
    @Environment(\.dismiss) private var dismiss
    @Environment(\.modelContext) private var modelContext
    @Environment(APIService.self) private var apiService
    @Environment(NetworkMonitor.self) private var networkMonitor
    @Environment(AlertManager.self) private var alertManager
    
    // MARK: - Form Fields
    
    @State private var date: Date = Date()
    @State private var amount: String = ""
    @State private var type: String = ""
    @State private var category: String = ""
    @State private var description: String = ""
    
    @State private var isSubmitting = false
    @State private var showValidationError = false
    
    // MARK: - Callback
    
    /// Called after successfully adding an entity
    var onAdd: (() -> Void)?
    
    // MARK: - Computed Properties
    
    /// Check if form is valid
    private var isFormValid: Bool {
        !amount.isEmpty && Double(amount) != nil && !type.isEmpty
    }
    
    /// Format date for API (YYYY-MM-DD)
    private var formattedDate: String {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy-MM-dd"
        return formatter.string(from: date)
    }
    
    // MARK: - Body
    
    var body: some View {
        NavigationStack {
            Form {
                // Offline warning
                if !networkMonitor.isConnected {
                    Section {
                        HStack {
                            Image(systemName: "wifi.slash")
                                .foregroundStyle(.orange)
                            Text("Adding is not available while offline")
                                .foregroundStyle(.secondary)
                        }
                    }
                }
                
                // Date field
                Section("Date") {
                    DatePicker("Select date", selection: $date, displayedComponents: .date)
                        .datePickerStyle(.graphical)
                }
                
                // Amount field
                Section("Amount") {
                    HStack {
                        Text("$")
                            .foregroundStyle(.secondary)
                        TextField("0.00", text: $amount)
                            .keyboardType(.decimalPad)
                    }
                }
                
                // Type field
                Section("Type") {
                    TextField("e.g., delivery, dine-in, takeout", text: $type)
                        .textInputAutocapitalization(.never)
                    
                    // Quick suggestions based on common exam types
                    Text("Common types: delivery, dine-in, takeout, consultation, medicine, ticket, rental, intake, burn, fine, membership")
                        .font(.caption)
                        .foregroundStyle(.secondary)
                }
                
                // Category field
                Section("Category") {
                    TextField("e.g., pizza, sushi, burger", text: $category)
                        .textInputAutocapitalization(.never)
                    
                    Text("Common categories: pizza, sushi, burger, dental, cardio, action, drama, sedan, suv, lunch, running")
                        .font(.caption)
                        .foregroundStyle(.secondary)
                }
                
                // Description field
                Section("Description") {
                    TextField("Enter description", text: $description, axis: .vertical)
                        .lineLimit(3...6)
                }
            }
            .navigationTitle("Add \(AppConfig.entityNameCapitalized)")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .cancellationAction) {
                    Button("Cancel") {
                        dismiss()
                    }
                }
                
                ToolbarItem(placement: .confirmationAction) {
                    Button("Save") {
                        Task { await saveEntity() }
                    }
                    .disabled(!isFormValid || !networkMonitor.isConnected || isSubmitting)
                }
            }
            .overlay {
                if isSubmitting {
                    ProgressView("Saving...")
                        .padding()
                        .background(
                            RoundedRectangle(cornerRadius: 12)
                                .fill(.ultraThinMaterial)
                        )
                }
            }
            .alert("Validation Error", isPresented: $showValidationError) {
                Button("OK", role: .cancel) { }
            } message: {
                Text("Please fill in all required fields (amount and type are required)")
            }
        }
    }
    
    // MARK: - Save Operation
    
    /// Save the new entity to the server
    private func saveEntity() async {
        guard isFormValid else {
            showValidationError = true
            return
        }
        
        guard networkMonitor.isConnected else {
            alertManager.showError("Cannot save while offline")
            return
        }
        
        isSubmitting = true
        defer { isSubmitting = false }
        
        // Create request payload
        let request = EntityDTO.CreateRequest(
            date: formattedDate,
            amount: Double(amount) ?? 0,
            type: type.trimmingCharacters(in: .whitespaces),
            category: category.isEmpty ? "general" : category.trimmingCharacters(in: .whitespaces),
            description: description.trimmingCharacters(in: .whitespaces)
        )
        
        do {
            let createdDTO = try await apiService.create(request)
            
            // Add to local cache
            await MainActor.run {
                let newEntity = Entity(from: createdDTO)
                modelContext.insert(newEntity)
                try? modelContext.save()
                Logger.logDatabase(operation: "INSERT", entity: AppConfig.entityNameSingular, id: createdDTO.id)
            }
            
            Logger.log("Created new \(AppConfig.entityNameSingular) with id \(createdDTO.id)")
            
            // Notify and dismiss
            onAdd?()
            dismiss()
            
        } catch {
            Logger.logNetworkError(error, method: "POST", url: AppConfig.singleEndpoint)
            alertManager.showError("Failed to save: \(error.localizedDescription)")
        }
    }
}

// MARK: - Preview
#Preview {
    AddEntityView()
        .modelContainer(for: Entity.self, inMemory: true)
        .environment(APIService())
        .environment(NetworkMonitor())
        .environment(AlertManager())
}
