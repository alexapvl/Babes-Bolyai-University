package com.quiz.util;

import com.zaxxer.hikari.HikariConfig;
import com.zaxxer.hikari.HikariDataSource;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.SQLException;

public class DatabaseUtil {
  private static final String DB_URL = "jdbc:mysql://localhost:3306/quiz_app?useSSL=false&serverTimezone=UTC";
  private static final String DB_USERNAME = "root";
  private static final String DB_PASSWORD = ""; // Change this to your MySQL root password

  private static HikariDataSource dataSource;

  static {
    try {
      HikariConfig config = new HikariConfig();
      config.setJdbcUrl(DB_URL);
      config.setUsername(DB_USERNAME);
      config.setPassword(DB_PASSWORD);
      config.setDriverClassName("com.mysql.cj.jdbc.Driver");

      // Pool configuration
      config.setMaximumPoolSize(20);
      config.setMinimumIdle(5);
      config.setConnectionTimeout(30000);
      config.setIdleTimeout(600000);
      config.setMaxLifetime(1800000);

      // Connection test query
      config.setConnectionTestQuery("SELECT 1");

      dataSource = new HikariDataSource(config);
    } catch (Exception e) {
      throw new RuntimeException("Failed to initialize database connection pool", e);
    }
  }

  public static Connection getConnection() throws SQLException {
    return dataSource.getConnection();
  }

  public static DataSource getDataSource() {
    return dataSource;
  }

  public static void closeDataSource() {
    if (dataSource != null && !dataSource.isClosed()) {
      dataSource.close();
    }
  }
}