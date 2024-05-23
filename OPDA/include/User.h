#pragma once
#include <string>
class User
{
private:
	std::string name;
	std::string surname;
	std::string surname_f;
	std::string passport;
	int id;
public:
	User(std::string name, std::string surname, std::string surname_f, std::string passport, int id = 1) :
		name(name), surname(surname), surname_f(surname_f), passport(passport), id(id) {}

	User(const char* name, const char* surname, const char* surname_f, const char* passport, int id = 1) :
		name(name), surname(surname), surname_f(surname_f), passport(passport), id(id) {}

	std::string getName() const { return name; }
	std::string getSurname() const { return surname; }
	std::string getSurname_F() const { return surname_f; }
	std::string getPassport() const { return passport; }
	int getId() const { return id; }

	void setName(std::string new_name) { name = new_name; }
	void setSurname(std::string new_surname) { surname = new_surname; }
	void setSurnameF(std::string new_surname_f) { surname_f = new_surname_f; }
	void setPassport(std::string new_passport) { passport = new_passport; }
	void setId(int new_id) { id = new_id; }
};

