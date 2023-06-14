#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <iterator>
#include <locale>
#include <array>
#include <variant>
#include <Windows.h>

#pragma once

namespace fs = std::filesystem;
using json = nlohmann::json;

class colorDB {
public:
	std::string database_name;
	void create_colorDB(std::string database_name);
} database_obj;


std::string current_date_time() {
	auto time_now = std::chrono::system_clock::now();
	return std::format("{:%Y/%m/%d %H:%M:%S}", time_now);
}

void create_database_config(std::string database_name) {
	auto start_time = std::chrono::high_resolution_clock::now();
	std::cout << "Creating database config file...";

	json config_json;
	config_json["creation_date"] = current_date_time();
	config_json["seed"] = 123456789;
	config_json["database_name"] = database_name;

	std::ofstream config(std::format("./{}/config.json", database_name));
	config << std::setw(4) << config_json << std::endl;

	auto elapse_time = std::chrono::high_resolution_clock::now() - start_time;
	std::cout << " " << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time).count() << "ms\n";
	std::cout << "Config file has been created!\n";
}

void modify_config_file(std::array<std::string, 3> config_items, std::array<std::string, 3> config_item_keys, int size) {
	//bool finish_file = false;
	//json override_config;
	//std::ofstream config(std::format("./{}/config.json", config_items[1]));
	//while (!finish_file) {
	//	for (int i = 1; i < size; i++) {
	//		std::string response;
	//		std::cout << config_item_keys[i] << ": ";
	//		std::cin >> response;
	//		if (response != "") config_item_keys[i] = response;
	//		else override_config[config_item_keys[i]] = config_items[i];
	//	}
	//	finish_file = true;
	//}
	//config << std::setw(4) << override_config << std::endl;
}

void display_items(std::array<std::string, 3> config_items, std::array<std::string, 3> config_item_keys, int size) {
	system("CLS");
	for (int i = 0; i < size; i++) {
		std::cout << config_item_keys[i] << ": " << config_items[i] << '\n';
	}
	Sleep(5000);

	//std::string user_action;
	//std::cout << "\nDo you want to change any settings?\n";
	//std::cin >> user_action;
	//if (user_action == "y") modify_config_file(config_item_keys, config_item_keys, 3);

	system("CLS");
}

void establish_connection() {

}

void connect_to_database(std::string database_name) {
	system("CLS");
	std::ifstream read_config_file(std::format("./{}/config.json", database_name));
	json config_file;
	read_config_file >> config_file;

	std::string config_creation_date = config_file["creation_date"];
	std::string config_database_name = config_file["database_name"];
	int config_seed = config_file["seed"];

	std::array<std::string, 3> config_item_keys = { "creation_date", "database_name", "seed" };
	std::array<std::string, 3> config_items = { config_creation_date, config_database_name, std::to_string(config_seed)};
	display_items(config_items, config_item_keys, 3);
	std::cout << "Establishing connection to database...\n";
}

bool check_for_config_file(std::string database_name) {
	std::cout << "Checking for config file...\n";
	if (fs::exists(std::format("./{}/config.json", database_name))) {
		std::cout << "Config file found!\n";
		return true;
	}
	std::cout << "Config file was not found, generating one now.\n";
	return false;
}

void colorDB::create_colorDB(std::string database_name) {
	auto start_time = std::chrono::high_resolution_clock::now();
	std::cout << "Initializing database...";

	std::string file_directory_name = std::format("./{}", database_name);
	if (fs::exists(file_directory_name)) {
		std::cout << "\nDatabase already exists!\n";
		if (check_for_config_file(database_name)) connect_to_database(database_name);
		else {
			create_database_config(database_name);
			connect_to_database(database_name);
		};
	}
	else if (fs::create_directories(file_directory_name)) {
		auto elapse_time = std::chrono::high_resolution_clock::now() - start_time;
		std::cout << " " << std::chrono::duration_cast<std::chrono::milliseconds>(elapse_time).count() << "ms\n";
		create_database_config(database_name);
		std::cout << std::format("Database {} has been created!\n", database_name);
	}
	else {
		std::cout << "\nDatabase was not created!\n";
	}
}