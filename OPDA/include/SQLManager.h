#pragma once
#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "Route.h"
#include "User.h"
#include "Reservation.h"

class SQLManager
{
public:
	SQLManager() {
		sqlite3* db;
		char* errorMessage = 0;
		int exitCode;
		exitCode = sqlite3_open("trains.db", &db);
		if (exitCode) {
			std::cerr << "Error open trains.db! " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		const char* sqlCreateTableUsers = "CREATE TABLE IF NOT EXISTS Users("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"name TEXT NOT NULL, "
			"surname TEXT NOT NULL, "
			"surname_f TEXT NOT NULL, "
			"passport TEXT NOT NULL);";
		exitCode = sqlite3_exec(db, sqlCreateTableUsers, 0, 0, &errorMessage);
		if (exitCode != SQLITE_OK) {
			std::cerr << "Error create table Users: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
		}
		const char* sqlCreateTableRoutes = "CREATE TABLE IF NOT EXISTS Routes("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"departure TEXT NOT NULL, "
			"arrival TEXT NOT NULL, "
			"departureTime TEXT NOT NULL, "
			"arrivalTime TEXT NOT NULL, "
			"trainNumber INTEGER NOT NULL, "
			"price REAL NOT NULL, "
			"seats TEXT NOT NULL, "
			"amountOfSeats INT NOT NULL, "
			"carriages INTEGER NOT NULL);";
		exitCode = sqlite3_exec(db, sqlCreateTableRoutes, 0, 0, &errorMessage);
		if (exitCode != SQLITE_OK) {
			std::cerr << "Error create table Routes: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
		}
		const char* sqlCreateTableReservation = "CREATE TABLE IF NOT EXISTS Reservation("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"user_id INTEGER NOT NULL, "
			"route_id INTEGER NOT NULL, "
			"carr_num INTEGER NOT NULL, "
			"seat INTEGER NOT NULL);";
		exitCode = sqlite3_exec(db, sqlCreateTableReservation, 0, 0, &errorMessage);
		if (exitCode != SQLITE_OK) {
			std::cerr << "Error create table Users: " << errorMessage << std::endl;
			sqlite3_free(errorMessage);
		}
	}
	bool addDataToDatabaseUsers(const User& user) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;

		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return false;
		}

		// Создание SQL-запроса для вставки данных
		std::string sql = "INSERT INTO Users (name, surname, surname_f, passport) VALUES ('" + user.getName() + "', '" + user.getSurname() + "', '" + user.getSurname_F() + "', '" + user.getPassport() + "'); ";

		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return false;
		}

		// Закрытие базы данных
		sqlite3_close(db);
		return true;
	}

	bool addDataToDatabaseRoutes(const Route& route) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;

		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return false;
		}

		// Создание SQL-запроса для вставки данных
		std::string sql = "INSERT INTO Routes (departure, arrival, departureTime, arrivalTime, trainNumber, price, seats, amountOfSeats, carriages) VALUES ('" + route.getDeparture() + "', '" + route.getArrival() + "', '" + route.getDepartureTime() + "', '" + route.getArrivalTime() + "', " + std::to_string(route.getTrainNumber()) + ", " + std::to_string(route.getPrice()) + ", '" + route.getSeats() + "', " + std::to_string(route.getAmountOfSeats()) + ", " + std::to_string(route.getCarriages()) + "); ";

		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return false;
		}

		// Закрытие базы данных
		sqlite3_close(db);
		return true;
	}

	bool addDataToDatabaseReservation(const Reservation& reserve) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;

		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return false;
		}

		// Создание SQL-запроса для вставки данных
		std::string sql = "INSERT INTO Reservation (user_id, route_id, carr_num, seat) VALUES (" + std::to_string(reserve.getUserId()) + ", " + std::to_string(reserve.getRouteId()) + ", " + std::to_string(reserve.getCarriageNumber()) + ", " + std::to_string(reserve.getSeat()) + "); ";

		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return false;
		}

		// Закрытие базы данных
		sqlite3_close(db);
		return true;
	}

	std::vector <Reservation> getListOfReservations(){
		std::vector <Reservation> reservations;
		std::string sql = "SELECT * FROM Reservation;";
		sqlite3* db;
		sqlite3_stmt* stmt = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Выполнение SQL-запроса
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
		if (rc == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				reservations.push_back(Reservation(sqlite3_column_int(stmt, 1),
					sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3),
					sqlite3_column_int(stmt, 4), sqlite3_column_int(stmt, 0)));
			}
		}
		// Закрытие базы данных
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return reservations;
	}

	void updateReservation(Reservation reserve) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "UPDATE Reservation SET user_id = " + std::to_string(reserve.getUserId()) + ", route_id = " + std::to_string(reserve.getRouteId()) + ", carr_num = " + std::to_string(reserve.getCarriageNumber()) + ", seat = " + std::to_string(reserve.getSeat()) + "WHERE id = " + std::to_string(reserve.getId()) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}

	void deleteReservation(int id) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "DELETE FROM Reservation WHERE id = " + std::to_string(id) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}

	std::vector <User> getListOfUsers() {
		std::vector <User> users;
		std::string sql = "SELECT * FROM Users;";
		sqlite3* db;
		sqlite3_stmt* stmt = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Выполнение SQL-запроса
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
		if (rc == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				users.push_back(User(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)), sqlite3_column_int(stmt, 0)));
			}
		}
		// Закрытие базы данных
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return users;
	}

	void updateUser(User user) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "UPDATE Users SET name = " + user.getName() + ", surname = " + user.getSurname() + ", surname_f = " + user.getSurname_F() + ", passport = " + user.getPassport() + "WHERE id = " + std::to_string(user.getId()) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}

	void deleteUser(int id) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "DELETE FROM Users WHERE id = " + std::to_string(id) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}

	std::vector <Route> getListOfRoutes() {
		std::vector <Route> routes;
		std::string sql = "SELECT * FROM Routes;";
		sqlite3* db;
		sqlite3_stmt* stmt = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Выполнение SQL-запроса
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
		if (rc == SQLITE_OK) {
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				routes.push_back(Route(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
					sqlite3_column_int(stmt, 5), sqlite3_column_double(stmt, 6),
					reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)),
					sqlite3_column_int(stmt, 8), sqlite3_column_int(stmt, 9),
					sqlite3_column_int(stmt, 0)));
			}
		}
		// Закрытие базы данных
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return routes;
	}

	void updateRoute(Route route) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "UPDATE Routes SET departure = " + route.getDeparture() + ", arrival = " + route.getArrival() + ", departureTime = " + route.getDepartureTime() + ", arrivalTime = " + route.getArrivalTime() + ", trainNumber = " + std::to_string(route.getTrainNumber()) + ", price = " + std::to_string(route.getPrice()) + ", seats = " + route.getSeats() + ", amountOfSeats = " + std::to_string(route.getAmountOfSeats()) + ", carriages = " + std::to_string(route.getCarriages()) + "WHERE id = " + std::to_string(route.getId()) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}

	void deleteRoute(int id) {
		sqlite3* db;
		char* errMsg = 0;
		int rc;
		// Открытие базы данных
		rc = sqlite3_open("trains.db", &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		}
		// Создание SQL-запроса для вставки данных
		std::string sql = "DELETE FROM Routes WHERE id = " + std::to_string(id) + ";";
		// Выполнение SQL-запроса
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
		}
		// Закрытие базы данных
		sqlite3_close(db);
	}
};



