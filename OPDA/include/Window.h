#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <imgui_stdlib.h>
#include "SQLManager.h"
#include "CustomButton.h"
#include <set>

class Window
{
private:
	SQLManager dbmanager;

	std::vector <User> users;
	std::vector <Route> routes;
	std::vector <Reservation> reservations;

	std::vector <Route> availableRoutes;

	CustomButton* mainLayoutButton;
	CustomButton* mainLayoutButtonStatistic;
	CustomButton* fillTable;
	sf::Text mainLayoutText;
	sf::RenderWindow window;
	bool windowIsOpen{ true };
	const char* U(const char8_t* str)
	{
		return reinterpret_cast<const char*>(str);
	}
	void mainLoop() {
		sf::Clock clock;
		while (window.isOpen() && windowIsOpen) {
			sf::Event event;
			while (window.pollEvent(event)) {
				ImGui::SFML::ProcessEvent(window, event);
				mainLayoutButton->update(window, event);
				mainLayoutButtonStatistic->update(window, event);
				fillTable->update(window, event);
				if (event.type == sf::Event::Closed) {
					window.close();
				}
			}
			ImGui::SFML::Update(window, clock.restart());
			window.clear(sf::Color::White);
			// ImGui::ShowDemoWindow();
			if (viewTicketBuy) {
				ticketBuy();
			}
			if (viewMainLayout) {
				mainLayout();
			}
			if (viewStatistics) {
				statistics();
			}
			ImGui::SFML::Render(window);
			window.display();
		}
		ImGui::SFML::Shutdown();
	}

	void mainLayout() {
		window.draw(mainLayoutText);
		mainLayoutText.move(10, 0);
		mainLayoutText.setFillColor(sf::Color(160, 220, 255, 128));
		window.draw(mainLayoutText);
		mainLayoutText.move(10, 0);
		mainLayoutText.setFillColor(sf::Color(160, 220, 255, 64));
		window.draw(mainLayoutText);
		mainLayoutText.move(-20, 0);
		mainLayoutText.setFillColor(sf::Color(121, 200, 255));
		mainLayoutButtonStatistic->draw(window);
		fillTable->draw(window);
		mainLayoutButton->draw(window);
	}

	void ticketBuy() {
		ImGui::SetNextWindowSizeConstraints(ImVec2(1024, 768), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::SetNextWindowSize(ImVec2(1024, 768), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(215.0/255.f, 255.0/255.f, 242.0/255.f, 1.00f);
		ImGui::Begin(U(u8"Покупка билетов"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

		static bool selectingRoute = true;
		static bool selectingSeat = false;

		if (selectingRoute) {
			std::vector <std::string> departures, arrivals;
			std::set <std::string> unique_departures_set, unique_arrivals_set; // уникальные отправление + прибытие

			for (auto route : routes) {
				unique_departures_set.insert(route.getDeparture());
				unique_arrivals_set.insert(route.getArrival());
			}

			departures.assign(unique_departures_set.begin(), unique_departures_set.end());
			arrivals.assign(unique_arrivals_set.begin(), unique_arrivals_set.end());

			static int item_current_departure = -1, item_current_arrival = -1;

			ImGui::SetCursorPos(ImVec2(420, 30));
			ImGui::Text(U(u8"Выбор маршрута"));

			ImGui::SetCursorPos(ImVec2(80, 100));
			ImGui::Text(U(u8"Откуда"));
			ImGui::SetCursorPos(ImVec2(80, 130));
			ImGui::SetNextItemWidth(250);

			if (ImGui::BeginCombo("##ОткудаCombo", (item_current_departure >= 0 ? departures[item_current_departure].c_str() : ""))) {
				for (int i = 0; i < departures.size(); i++)
				{
					bool isSelectedDep = (item_current_departure == i);
					if (ImGui::Selectable(departures[i].c_str(), isSelectedDep)) {
						item_current_departure = i;
					}
					if (isSelectedDep) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SetCursorPos(ImVec2(360, 100));
			ImGui::Text(U(u8"Куда"));
			ImGui::SetCursorPos(ImVec2(360, 130));
			ImGui::SetNextItemWidth(250);

			if (ImGui::BeginCombo("##КудаCombo", (item_current_arrival >= 0 ? arrivals[item_current_arrival].c_str() : ""))) {
				for (int i = 0; i < arrivals.size(); i++)
				{
					bool isSelectedArr = (item_current_arrival == i);
					if (ImGui::Selectable(arrivals[i].c_str(), isSelectedArr)) {
						item_current_arrival = i;
					}
					if (isSelectedArr) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			static bool searchResults = false;
			static int selectedRouteId = 0;

			ImGui::SetCursorPos(ImVec2(640, 120));
			if (ImGui::Button(U(u8"Поиск маршрута"), ImVec2(140, 40))) {
				updateData();
				searchResults = true;
				availableRoutes.clear();
				for (auto route : routes) {
					if (departures[item_current_departure] == route.getDeparture() &&
						arrivals[item_current_arrival] == route.getArrival()) {
						availableRoutes.push_back(route);
					}
				}
			}
			if (searchResults) {
				ImGui::SetCursorPos(ImVec2(80, 300));
				ImGui::Text(U(u8"Доступные маршруты"));
				ImGui::SetCursorPos(ImVec2(80, 330));
				if (ImGui::BeginTable("##table", 6)) {
					ImGui::TableSetupColumn(U(u8"Отправление"));
					ImGui::TableSetupColumn(U(u8"Прибытие"));
					ImGui::TableSetupColumn(U(u8"Время отправления"));
					ImGui::TableSetupColumn(U(u8"Время прибытия"));
					ImGui::TableSetupColumn(U(u8"Цена"));
					ImGui::TableSetupColumn("");
					ImGui::TableHeadersRow();
					for (auto availableroute : availableRoutes) {
						// std::cout << availableroute.getDeparture() << " - " << availableroute.getArrival() << std::endl;
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", availableroute.getDeparture().c_str());
						ImGui::TableSetColumnIndex(1);
						ImGui::Text("%s", availableroute.getArrival().c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text("%s", availableroute.getDepartureTime().c_str());
						ImGui::TableSetColumnIndex(3);
						ImGui::Text("%s", availableroute.getArrivalTime().c_str());
						ImGui::TableSetColumnIndex(4);
						ImGui::Text("%f", availableroute.getPrice());
						ImGui::TableSetColumnIndex(5);
						std::string buttonName = "";
						buttonName += reinterpret_cast<const char*>(u8"Выбрать##");
						buttonName += std::to_string(availableroute.getId());
						if (ImGui::Button(buttonName.c_str(), ImVec2(80, 22))) {
							selectedRouteId = availableroute.getId();
							selectingRoute = false;
							selectingSeat = true;
							// std::cout << selectedRouteId << std::endl;
						}
					}
					ImGui::EndTable();
				}
			}
			// ImGui::SameLine();
		}
		if (selectingSeat) {
			ImGui::SetCursorPos(ImVec2(420, 30));
			ImGui::Text(U(u8"Выбор места"));

		}
		ImGui::End();
	}

	void statistics() {

	}

	void initGUI() {
		mainLayoutText.setFont(sv::Fonts::arial);
		mainLayoutText.setCharacterSize(92);
		mainLayoutText.setPosition(285, 200);
		mainLayoutText.setFillColor(sf::Color(121, 200, 255));
		mainLayoutText.setString("IT Railways");
		auto tbc = [this]() {
			viewTicketBuy = true;
			closeMainLayout();
		};
		auto sc = [this]() {
			viewStatistics = true;
			closeMainLayout();
		};
		auto ft = [this]() {
			
			
			
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Москва"), U(u8"Санкт-Петербург"), U(u8"05:55"), U(u8"09:40"),
				758, 4500.00, std::string(13 * 54, '0') , 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Москва"), U(u8"Санкт-Петербург"), U(u8"00:00"), U(u8"06:40"),
				142, 3200.00, std::string(13 * 54, '0'), 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Москва"), U(u8"Санкт-Петербург"), U(u8"12:55"), U(u8"14:40"),
				754, 5000.00, std::string(13 * 54, '0'), 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Москва"), U(u8"Санкт-Петербург"), U(u8"18:55"), U(u8"21:40"),
				254, 4000.00, std::string(13 * 54, '0'), 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Урюпинск"), U(u8"Санкт-Петербург"), U(u8"21:10"), U(u8"23:40"),
				466, 2500.00, std::string(13 * 54, '0'), 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Москва"), U(u8"Урюпинск"), U(u8"16:20"), U(u8"21:40"),
				866, 4000.00, std::string(13 * 54, '0'), 54, 13));
			dbmanager.addDataToDatabaseRoutes(Route(U(u8"Мытищи"), U(u8"Санкт-Петербург"), U(u8"18:55"), U(u8"21:40"),
				477, 4000.00, std::string(13 * 54, '0'), 54, 13));
			//std::cout << seats << std::endl;

		};
		mainLayoutButton = new CustomButton(sf::Vector2f(240, 80), sf::Vector2f(392, 422), tbc, L"Покупка билетов", 24);
		mainLayoutButtonStatistic = new CustomButton(sf::Vector2f(240, 80), sf::Vector2f(392, 522), sc, L"Статистика", 24);
		fillTable = new CustomButton(sf::Vector2f(80, 25), sf::Vector2f(0, 0), ft, L"TableFill", 14);
	}

	void closeMainLayout() {
		viewMainLayout = false;
		mainLayoutButton->setEnabled(false);
		mainLayoutButtonStatistic->setEnabled(false);
		updateData();
	}

	void updateData() {
		users.clear();
		routes.clear();
		reservations.clear();

		users = dbmanager.getListOfUsers();
		routes = dbmanager.getListOfRoutes();
		reservations = dbmanager.getListOfReservations();
	}

	bool viewTicketBuy{ false };
	bool viewMainLayout{ true };
	bool viewStatistics{ false };
public:

	Window(): dbmanager(), window(sf::VideoMode(1024,768), L"IT Railways") {
		if (!ImGui::SFML::Init(window)) return;

		users = dbmanager.getListOfUsers();
		routes = dbmanager.getListOfRoutes();
		reservations = dbmanager.getListOfReservations();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 18, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		ImGui::SFML::UpdateFontTexture();
		ImGui::StyleColorsLight();

		initGUI();
	}
	void open() {
		windowIsOpen = true;
		mainLoop();
	}

	void close() {
		windowIsOpen = false;
	}
};

