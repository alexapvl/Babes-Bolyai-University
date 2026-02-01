//
//  EntityListView.swift
//  exam_mobile
//
//  Main Section: Displays the list of all entities.
//  Features: Offline support, retry button, swipe-to-delete, navigation to detail.
//

import SwiftUI
import SwiftData

// MARK: - Entity List View
/// Displays the list of all entities with offline support.
/// Requirement A: View the list of entities
/// Requirement D: Delete an entity (swipe action)
struct EntityListView: View {
    
    // MARK: - Environment & State
    
    @Environment(\.modelContext) private var modelContext
    @Environment(APIService.self) private var apiService
    @Environment(NetworkMonitor.self) private var networkMonitor
    @Environment(AlertManager.self) private var alertManager
    
    @Query(sort: \Entity.date, order: .reverse) private var cachedEntities: [Entity]
    
    @State private var isLoading = false
    @State private var hasLoaded = false
    @State private var showAddSheet = false
    
    // MARK: - Body
    
    var body: some View {
        ZStack {
            // Main content
            List {
                // Offline banner
                if !networkMonitor.isConnected {
                    offlineBanner
                }
                
                // Entity list
                ForEach(cachedEntities) { entity in
                    NavigationLink(destination: EntityDetailView(entityId: entity.serverId)) {
                        EntityRowView(entity: entity)
                    }
                }
                .onDelete(perform: deleteEntities)
            }
            .listStyle(.insetGrouped)
            .refreshable {
                await fetchEntities()
            }
            
            // Loading overlay
            if isLoading {
                ProgressView()
                    .scaleEffect(1.5)
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.black.opacity(0.1))
            }
            
            // Empty state
            if cachedEntities.isEmpty && !isLoading && hasLoaded {
                emptyStateView
            }
        }
        .navigationTitle(AppConfig.entityNamePluralCapitalized)
        .toolbar {
            ToolbarItem(placement: .navigationBarTrailing) {
                Button(action: { showAddSheet = true }) {
                    Image(systemName: "plus")
                }
                .disabled(!networkMonitor.isConnected)
            }
        }
        .sheet(isPresented: $showAddSheet) {
            AddEntityView {
                // Refresh list after adding
                Task { await fetchEntities() }
            }
        }
        .task {
            // Fetch on first appear
            if !hasLoaded {
                await fetchEntities()
            }
        }
        .onChange(of: alertManager.refreshTrigger) { _, _ in
            // Refresh list when WebSocket notification is dismissed
            Task { await fetchEntities() }
        }
    }
    
    // MARK: - Subviews
    
    /// Offline banner with retry button
    private var offlineBanner: some View {
        HStack {
            Image(systemName: "wifi.slash")
                .foregroundStyle(.orange)
            
            Text("You are offline")
                .font(.subheadline)
            
            Spacer()
            
            Button("Retry") {
                Task { await fetchEntities() }
            }
            .buttonStyle(.bordered)
            .disabled(!networkMonitor.isConnected)
        }
        .padding(.vertical, 8)
        .listRowBackground(Color.orange.opacity(0.1))
    }
    
    /// Empty state when no entities exist
    private var emptyStateView: some View {
        VStack(spacing: 16) {
            Image(systemName: "tray")
                .font(.system(size: 60))
                .foregroundStyle(.secondary)
            
            Text("No \(AppConfig.entityNamePlural) yet")
                .font(.title2)
                .foregroundStyle(.secondary)
            
            if networkMonitor.isConnected {
                Button(action: { showAddSheet = true }) {
                    Label("Add \(AppConfig.entityNameCapitalized)", systemImage: "plus")
                }
                .buttonStyle(.borderedProminent)
            } else {
                Text("Connect to the internet to add \(AppConfig.entityNamePlural)")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
    }
    
    // MARK: - Data Operations
    
    /// Fetch entities from the server and cache locally
    private func fetchEntities() async {
        guard networkMonitor.isConnected else {
            Logger.log("Cannot fetch - offline", level: .warning)
            return
        }
        
        isLoading = true
        defer { 
            isLoading = false
            hasLoaded = true
        }
        
        do {
            let dtos = try await apiService.fetchAll()
            
            // Update local cache
            await MainActor.run {
                updateCache(with: dtos)
            }
            
            Logger.log("Fetched \(dtos.count) \(AppConfig.entityNamePlural)")
            
        } catch is CancellationError {
            // Task was cancelled (e.g., view disappeared during refresh) - ignore
            Logger.log("EntityListView fetch cancelled", level: .debug)
        } catch let urlError as URLError where urlError.code == .cancelled {
            // URL request was cancelled - ignore
            Logger.log("EntityListView URL request cancelled", level: .debug)
        } catch {
            Logger.logNetworkError(error, method: "GET", url: AppConfig.listEndpoint)
            alertManager.showError("Failed to load \(AppConfig.entityNamePlural): \(error.localizedDescription)")
        }
    }
    
    /// Update the local SwiftData cache with fetched DTOs
    private func updateCache(with dtos: [EntityDTO]) {
        // Create a map of existing entities by serverId
        let existingMap = Dictionary(uniqueKeysWithValues: cachedEntities.map { ($0.serverId, $0) })
        
        // Track which IDs we received from the server
        let receivedIds = Set(dtos.map { $0.id })
        
        // Update or insert entities
        for dto in dtos {
            if let existing = existingMap[dto.id] {
                existing.update(from: dto)
                Logger.logDatabase(operation: "UPDATE", entity: AppConfig.entityNameSingular, id: dto.id)
            } else {
                let newEntity = Entity(from: dto)
                modelContext.insert(newEntity)
                Logger.logDatabase(operation: "INSERT", entity: AppConfig.entityNameSingular, id: dto.id)
            }
        }
        
        // Remove entities that no longer exist on the server
        for entity in cachedEntities {
            if !receivedIds.contains(entity.serverId) {
                modelContext.delete(entity)
                Logger.logDatabase(operation: "DELETE (sync)", entity: AppConfig.entityNameSingular, id: entity.serverId)
            }
        }
        
        try? modelContext.save()
    }
    
    /// Delete entities (swipe action)
    private func deleteEntities(at offsets: IndexSet) {
        guard networkMonitor.isConnected else {
            alertManager.showError("Cannot delete while offline")
            return
        }
        
        for index in offsets {
            let entity = cachedEntities[index]
            
            Task {
                do {
                    // Delete on server
                    _ = try await apiService.delete(id: entity.serverId)
                    
                    // Delete locally
                    await MainActor.run {
                        modelContext.delete(entity)
                        try? modelContext.save()
                        Logger.logDatabase(operation: "DELETE", entity: AppConfig.entityNameSingular, id: entity.serverId)
                    }
                    
                    // alertManager.showSuccess("\(AppConfig.entityNameCapitalized) deleted")
                    
                } catch {
                    alertManager.showError("Failed to delete: \(error.localizedDescription)")
                }
            }
        }
    }
}

// MARK: - Entity Row View
/// Single row in the entity list
struct EntityRowView: View {
    let entity: Entity
    
    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 4) {
                Text(entity.itemDescription)
                    .font(.headline)
                    .lineLimit(1)
                
                HStack(spacing: 8) {
                    Text(entity.type)
                        .font(.caption)
                        .padding(.horizontal, 8)
                        .padding(.vertical, 2)
                        .background(Color.blue.opacity(0.1))
                        .foregroundStyle(.blue)
                        .clipShape(Capsule())
                    
                    Text(entity.category)
                        .font(.caption)
                        .foregroundStyle(.secondary)
                }
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text(entity.formattedAmount)
                    .font(.headline)
                    .foregroundStyle(.primary)
                
                Text(entity.formattedDate)
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
        .padding(.vertical, 4)
    }
}

// MARK: - Preview
#Preview {
    NavigationStack {
        EntityListView()
    }
    .modelContainer(for: Entity.self, inMemory: true)
    .environment(APIService())
    .environment(NetworkMonitor())
    .environment(AlertManager())
}
