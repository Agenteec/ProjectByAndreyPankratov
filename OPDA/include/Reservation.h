#pragma once
class Reservation
{
private:
	int id, user_id, route_id, carr_num, seat;
public:
	Reservation(int user_id, int route_id, int carr_num, int seat, int id = 1) :
		user_id(user_id), route_id(route_id), carr_num(carr_num), seat(seat), id(id) {}

	int getUserId() const { return user_id; }
	int getRouteId() const { return route_id; }
	int getCarriageNumber() const { return carr_num; }
	int getSeat() const { return seat; }
	int getId() const { return id; }

	void setUserId(int new_user_id) { user_id = new_user_id; }
	void setRouteId(int new_route_id) { route_id = new_route_id; }
	void setCarriageNumber(int new_carr_num) { carr_num = new_carr_num; }
	void setSeat(int new_seat) { seat = new_seat; }
	void setId(int new_id) { id = new_id; }
};

