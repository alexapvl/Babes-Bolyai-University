//
//  InsightsView.swift
//  exam_mobile
//
//  Insights Section: Displays top 3 categories by amount.
//  Uses GET /allEntities endpoint.
//

import SwiftUI

// MARK: - Category Insight Model
/// Represents a category's total for insights
struct CategoryInsight: Identifiable {
    let id = UUID()
    let category: String
    let total: Double
    let count: Int
    let rank: Int
    
    /// Formatted total amount
    var formattedTotal: String {
        String(format: "$%.2f", total)
    }
    
    /// Medal emoji based on rank
    var medal: String {
        switch rank {
        case 1: return "🥇"
        case 2: return "🥈"
        case 3: return "🥉"
        default: return ""
        }
    }
    
    /// Color based on rank
    var rankColor: Color {
        switch rank {
        case 1: return .yellow
        case 2: return .gray
        case 3: return .orange
        default: return .secondary
        }
    }
}

// MARK: - Insights View
/// Displays top 3 categories by total amount.
/// Requirement: Top Categories (1p)
/// - Uses GET /allEntities endpoint
/// - Groups by category, sums amounts
/// - Shows top 3 in descending order
struct InsightsView: View {
    
    // MARK: - Environment & State
    
    @Environment(APIService.self) private var apiService
    @Environment(NetworkMonitor.self) private var networkMonitor
    @Environment(AlertManager.self) private var alertManager
    
    @State private var insights: [CategoryInsight] = []
    @State private var isLoading = false
    @State private var hasLoaded = false
    
    // MARK: - Body
    
    var body: some View {
        ZStack {
            List {
                // Offline banner
                if !networkMonitor.isConnected {
                    offlineBanner
                }
                
                // Top categories section
                Section("Top Categories") {
                    if insights.isEmpty && hasLoaded {
                        Text("No data available")
                            .foregroundStyle(.secondary)
                    } else {
                        ForEach(insights) { insight in
                            CategoryInsightCard(insight: insight)
                        }
                    }
                }
            }
            .listStyle(.insetGrouped)
            .refreshable {
                await fetchInsights()
            }
            
            // Loading overlay
            if isLoading {
                ProgressView()
                    .scaleEffect(1.5)
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.black.opacity(0.1))
            }
        }
        .navigationTitle("Insights")
        .task {
            if !hasLoaded {
                await fetchInsights()
            }
        }
    }
    
    // MARK: - Subviews
    
    /// Offline banner
    private var offlineBanner: some View {
        HStack {
            Image(systemName: "wifi.slash")
                .foregroundStyle(.orange)
            
            Text("You are offline")
                .font(.subheadline)
            
            Spacer()
            
            Button("Retry") {
                Task { await fetchInsights() }
            }
            .buttonStyle(.bordered)
            .disabled(!networkMonitor.isConnected)
        }
        .padding(.vertical, 8)
        .listRowBackground(Color.orange.opacity(0.1))
    }
    
    // MARK: - Data Operations
    
    /// Fetch data and compute category insights
    private func fetchInsights() async {
        guard networkMonitor.isConnected else {
            Logger.log("Cannot fetch insights - offline", level: .warning)
            return
        }
        
        isLoading = true
        defer {
            isLoading = false
            hasLoaded = true
        }
        
        do {
            let dtos = try await apiService.fetchAllForReports()
            
            // Compute top 3 categories
            let computed = computeTopCategories(from: dtos)
            
            await MainActor.run {
                insights = computed
            }
            
            Logger.log("Computed top \(computed.count) categories from \(dtos.count) entities")
            
        } catch is CancellationError {
            // Task was cancelled (e.g., view disappeared during refresh) - ignore
            Logger.log("InsightsView fetch cancelled", level: .debug)
        } catch let urlError as URLError where urlError.code == .cancelled {
            // URL request was cancelled - ignore
            Logger.log("InsightsView URL request cancelled", level: .debug)
        } catch {
            Logger.logNetworkError(error, method: "GET", url: AppConfig.allEndpoint)
            alertManager.showError("Failed to load insights: \(error.localizedDescription)")
        }
    }
    
    /// Compute top 3 categories from DTOs
    private func computeTopCategories(from dtos: [EntityDTO]) -> [CategoryInsight] {
        // Group by category
        var categoryData: [String: (total: Double, count: Int)] = [:]
        
        for dto in dtos {
            let category = dto.category.isEmpty ? "general" : dto.category
            let existing = categoryData[category] ?? (total: 0, count: 0)
            categoryData[category] = (total: existing.total + dto.amount, count: existing.count + 1)
        }
        
        // Sort by total descending and take top 3
        let sorted = categoryData
            .sorted { $0.value.total > $1.value.total }
            .prefix(3)
        
        // Convert to CategoryInsight with ranks
        return sorted.enumerated().map { index, item in
            CategoryInsight(
                category: item.key,
                total: item.value.total,
                count: item.value.count,
                rank: index + 1
            )
        }
    }
}

// MARK: - Category Insight Card
/// Card displaying a single category insight
struct CategoryInsightCard: View {
    let insight: CategoryInsight
    
    var body: some View {
        HStack(spacing: 16) {
            // Rank badge
            ZStack {
                Circle()
                    .fill(insight.rankColor.opacity(0.2))
                    .frame(width: 50, height: 50)
                
                Text(insight.medal)
                    .font(.title2)
            }
            
            // Category info
            VStack(alignment: .leading, spacing: 4) {
                Text(insight.category.capitalized)
                    .font(.headline)
                
                Text("\(insight.count) \(insight.count == 1 ? "item" : "items")")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
            
            Spacer()
            
            // Total amount
            VStack(alignment: .trailing, spacing: 4) {
                Text(insight.formattedTotal)
                    .font(.title3)
                    .fontWeight(.bold)
                
                Text("#\(insight.rank)")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
        .padding()
        .background(
            RoundedRectangle(cornerRadius: 16)
                .fill(Color(.systemBackground))
                .shadow(color: .black.opacity(0.1), radius: 8, x: 0, y: 4)
        )
    }
}

// MARK: - Preview
#Preview {
    NavigationStack {
        InsightsView()
    }
    .environment(APIService())
    .environment(NetworkMonitor())
    .environment(AlertManager())
}
