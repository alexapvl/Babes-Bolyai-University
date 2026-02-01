//
//  EntityDetailView.swift
//  exam_mobile
//
//  Main Section: Displays detailed information about a single entity.
//  Features: Fetch from server, cache for offline access.
//

import SwiftUI
import SwiftData

// MARK: - Entity Detail View
/// Displays all details of a single entity.
/// Requirement B: View Entity Details (2p)
/// - Uses GET /entity/:id to fetch details
/// - Caches data for offline access
struct EntityDetailView: View {
    
    // MARK: - Properties
    
    let entityId: Int
    
    // MARK: - Environment & State
    
    @Environment(\.modelContext) private var modelContext
    @Environment(APIService.self) private var apiService
    @Environment(NetworkMonitor.self) private var networkMonitor
    @Environment(AlertManager.self) private var alertManager
    
    @Query private var cachedEntities: [Entity]
    
    @State private var isLoading = false
    @State private var fetchedEntity: Entity?
    
    // MARK: - Computed Properties
    
    /// Get the entity from cache or fetched data
    private var entity: Entity? {
        fetchedEntity ?? cachedEntities.first(where: { $0.serverId == entityId })
    }
    
    // MARK: - Body
    
    var body: some View {
        ScrollView {
            if let entity = entity {
                detailContent(for: entity)
            } else if isLoading {
                loadingView
            } else {
                notFoundView
            }
        }
        .navigationTitle("\(AppConfig.entityNameCapitalized) Details")
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            if isLoading {
                ToolbarItem(placement: .navigationBarTrailing) {
                    ProgressView()
                }
            }
        }
        .task {
            await fetchDetails()
        }
    }
    
    // MARK: - Detail Content
    
    @ViewBuilder
    private func detailContent(for entity: Entity) -> some View {
        VStack(spacing: 24) {
            // Header card
            headerCard(for: entity)
            
            // Details section
            detailsSection(for: entity)
            
            Spacer()
        }
        .padding()
    }
    
    /// Header card with amount and date
    private func headerCard(for entity: Entity) -> some View {
        VStack(spacing: 16) {
            // Amount
            Text(entity.formattedAmount)
                .font(.system(size: 48, weight: .bold, design: .rounded))
                .foregroundStyle(.primary)
            
            // Type badge
            Text(entity.type.uppercased())
                .font(.caption)
                .fontWeight(.semibold)
                .padding(.horizontal, 16)
                .padding(.vertical, 6)
                .background(Color.blue)
                .foregroundStyle(.white)
                .clipShape(Capsule())
            
            // Date
            HStack {
                Image(systemName: "calendar")
                    .foregroundStyle(.secondary)
                Text(entity.formattedDate)
                    .foregroundStyle(.secondary)
            }
            .font(.subheadline)
        }
        .frame(maxWidth: .infinity)
        .padding(.vertical, 32)
        .background(
            RoundedRectangle(cornerRadius: 16)
                .fill(Color(.systemBackground))
                .shadow(color: .black.opacity(0.1), radius: 10, x: 0, y: 5)
        )
    }
    
    /// Details section with all entity fields
    private func detailsSection(for entity: Entity) -> some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Details")
                .font(.headline)
                .foregroundStyle(.secondary)
            
            VStack(spacing: 0) {
                detailRow(label: "ID", value: "\(entity.serverId)")
                Divider()
                detailRow(label: "Date", value: entity.date)
                Divider()
                detailRow(label: "Amount", value: entity.formattedAmount)
                Divider()
                detailRow(label: "Type", value: entity.type)
                Divider()
                detailRow(label: "Category", value: entity.category)
                Divider()
                detailRow(label: "Description", value: entity.itemDescription)
            }
            .background(
                RoundedRectangle(cornerRadius: 12)
                    .fill(Color(.systemBackground))
            )
            .clipShape(RoundedRectangle(cornerRadius: 12))
            .shadow(color: .black.opacity(0.05), radius: 5, x: 0, y: 2)
        }
    }
    
    /// Single detail row
    private func detailRow(label: String, value: String) -> some View {
        HStack {
            Text(label)
                .foregroundStyle(.secondary)
            Spacer()
            Text(value)
                .fontWeight(.medium)
                .multilineTextAlignment(.trailing)
        }
        .padding(.horizontal, 16)
        .padding(.vertical, 12)
    }
    
    /// Loading view
    private var loadingView: some View {
        VStack(spacing: 16) {
            ProgressView()
                .scaleEffect(1.5)
            Text("Loading details...")
                .foregroundStyle(.secondary)
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .padding(.top, 100)
    }
    
    /// Not found view
    private var notFoundView: some View {
        VStack(spacing: 16) {
            Image(systemName: "doc.questionmark")
                .font(.system(size: 60))
                .foregroundStyle(.secondary)
            
            Text("\(AppConfig.entityNameCapitalized) not found")
                .font(.title2)
                .foregroundStyle(.secondary)
            
            if !networkMonitor.isConnected {
                Text("Connect to the internet to load details")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
        .padding(.top, 100)
    }
    
    // MARK: - Data Operations
    
    /// Fetch entity details from server
    private func fetchDetails() async {
        // If offline and we have cached data, use it
        if !networkMonitor.isConnected && entity != nil {
            Logger.log("Using cached data for entity \(entityId) (offline)")
            return
        }
        
        guard networkMonitor.isConnected else {
            Logger.log("Cannot fetch details - offline", level: .warning)
            return
        }
        
        isLoading = true
        defer { isLoading = false }
        
        do {
            let dto = try await apiService.fetchOne(id: entityId)
            
            await MainActor.run {
                // Update or insert in cache
                if let existing = cachedEntities.first(where: { $0.serverId == entityId }) {
                    existing.update(from: dto)
                    fetchedEntity = existing
                    Logger.logDatabase(operation: "UPDATE", entity: AppConfig.entityNameSingular, id: entityId)
                } else {
                    let newEntity = Entity(from: dto)
                    modelContext.insert(newEntity)
                    fetchedEntity = newEntity
                    Logger.logDatabase(operation: "INSERT", entity: AppConfig.entityNameSingular, id: entityId)
                }
                
                try? modelContext.save()
            }
            
            Logger.log("Fetched details for entity \(entityId)")
            
        } catch {
            Logger.logNetworkError(error, method: "GET", url: "\(AppConfig.singleEndpoint)/\(entityId)")
            alertManager.showError("Failed to load details: \(error.localizedDescription)")
        }
    }
}

// MARK: - Preview
#Preview {
    NavigationStack {
        EntityDetailView(entityId: 1)
    }
    .modelContainer(for: Entity.self, inMemory: true)
    .environment(APIService())
    .environment(NetworkMonitor())
    .environment(AlertManager())
}
