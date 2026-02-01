//
//  ReportsView.swift
//  exam_mobile
//
//  Reports Section: Displays monthly totals in descending order.
//  Uses GET /allEntities endpoint.
//

import SwiftUI

// MARK: - Monthly Report Model
/// Represents a monthly total for reporting
struct MonthlyReport: Identifiable {
    let id = UUID()
    let monthYear: String  // Format: "YYYY-MM"
    let total: Double
    let count: Int
    
    /// Formatted month for display (e.g., "January 2026")
    var formattedMonth: String {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy-MM"
        if let date = formatter.date(from: monthYear) {
            formatter.dateFormat = "MMMM yyyy"
            return formatter.string(from: date)
        }
        return monthYear
    }
    
    /// Formatted total amount
    var formattedTotal: String {
        String(format: "$%.2f", total)
    }
}

// MARK: - Reports View
/// Displays monthly totals in descending order.
/// Requirement: Monthly Analysis (1p)
/// - Uses GET /allEntities endpoint
/// - Groups by month, sums amounts
/// - Sorts in descending order
struct ReportsView: View {
    
    // MARK: - Environment & State
    
    @Environment(APIService.self) private var apiService
    @Environment(NetworkMonitor.self) private var networkMonitor
    @Environment(AlertManager.self) private var alertManager
    
    @State private var reports: [MonthlyReport] = []
    @State private var isLoading = false
    @State private var hasLoaded = false
    
    // MARK: - Computed Properties
    
    /// Total across all months
    private var grandTotal: Double {
        reports.reduce(0) { $0 + $1.total }
    }
    
    // MARK: - Body
    
    var body: some View {
        ZStack {
            List {
                // Offline banner
                if !networkMonitor.isConnected {
                    offlineBanner
                }
                
                // Summary section
                if !reports.isEmpty {
                    Section("Summary") {
                        HStack {
                            Text("Total")
                                .font(.headline)
                            Spacer()
                            Text(String(format: "$%.2f", grandTotal))
                                .font(.headline)
                                .foregroundStyle(.blue)
                        }
                    }
                }
                
                // Monthly breakdown
                Section("Monthly Breakdown") {
                    if reports.isEmpty && hasLoaded {
                        Text("No data available")
                            .foregroundStyle(.secondary)
                    } else {
                        ForEach(reports) { report in
                            MonthlyReportRow(report: report)
                        }
                    }
                }
            }
            .listStyle(.insetGrouped)
            .refreshable {
                await fetchReports()
            }
            
            // Loading overlay
            if isLoading {
                ProgressView()
                    .scaleEffect(1.5)
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.black.opacity(0.1))
            }
        }
        .navigationTitle("Monthly Reports")
        .task {
            if !hasLoaded {
                await fetchReports()
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
                Task { await fetchReports() }
            }
            .buttonStyle(.bordered)
            .disabled(!networkMonitor.isConnected)
        }
        .padding(.vertical, 8)
        .listRowBackground(Color.orange.opacity(0.1))
    }
    
    // MARK: - Data Operations
    
    /// Fetch data and compute monthly reports
    private func fetchReports() async {
        guard networkMonitor.isConnected else {
            Logger.log("Cannot fetch reports - offline", level: .warning)
            return
        }
        
        isLoading = true
        defer {
            isLoading = false
            hasLoaded = true
        }
        
        do {
            let dtos = try await apiService.fetchAllForReports()
            
            // Compute monthly totals
            let computed = computeMonthlyReports(from: dtos)
            
            await MainActor.run {
                reports = computed
            }
            
            Logger.log("Computed \(computed.count) monthly reports from \(dtos.count) entities")
            
        } catch is CancellationError {
            // Task was cancelled (e.g., view disappeared during refresh) - ignore
            Logger.log("ReportsView fetch cancelled", level: .debug)
        } catch let urlError as URLError where urlError.code == .cancelled {
            // URL request was cancelled - ignore
            Logger.log("ReportsView URL request cancelled", level: .debug)
        } catch {
            Logger.logNetworkError(error, method: "GET", url: AppConfig.allEndpoint)
            alertManager.showError("Failed to load reports: \(error.localizedDescription)")
        }
    }
    
    /// Compute monthly totals from DTOs
    private func computeMonthlyReports(from dtos: [EntityDTO]) -> [MonthlyReport] {
        // Group by month-year (first 7 characters of date: "YYYY-MM")
        var monthlyData: [String: (total: Double, count: Int)] = [:]
        
        for dto in dtos {
            let monthYear = String(dto.date.prefix(7))
            let existing = monthlyData[monthYear] ?? (total: 0, count: 0)
            monthlyData[monthYear] = (total: existing.total + dto.amount, count: existing.count + 1)
        }
        
        // Convert to MonthlyReport and sort by total descending
        return monthlyData.map { key, value in
            MonthlyReport(monthYear: key, total: value.total, count: value.count)
        }
        .sorted { $0.total > $1.total }
    }
}

// MARK: - Monthly Report Row
/// Row displaying a single month's report
struct MonthlyReportRow: View {
    let report: MonthlyReport
    
    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 4) {
                Text(report.formattedMonth)
                    .font(.headline)
                
                Text("\(report.count) \(report.count == 1 ? "item" : "items")")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
            
            Spacer()
            
            Text(report.formattedTotal)
                .font(.title3)
                .fontWeight(.semibold)
                .foregroundStyle(.primary)
        }
        .padding(.vertical, 4)
    }
}

// MARK: - Preview
#Preview {
    NavigationStack {
        ReportsView()
    }
    .environment(APIService())
    .environment(NetworkMonitor())
    .environment(AlertManager())
}
