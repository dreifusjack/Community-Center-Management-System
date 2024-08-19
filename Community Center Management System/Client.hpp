#pragma once

#include "Event.hpp"
#include "User.hpp"
#include <vector>

using namespace std;

class Event;
class Facility;

class Client : public User
{
public:
  enum ClientType
  {
    CITY,
    ORGANIZATION
  };
  Client(const string &username, const string &password, const ClientType &client_type);
  ~Client() = default;

  // getters and setters
  /**
   * Gets the ClientType of this Client.
   *
   * @return the ClientType
   */
  ClientType get_client_type() const;
  /**
   * Gets the number of booked hours from this User.
   *
   * @return the number of booked hours from this User
   */
  int get_booked_hours() const;
  /**
   * Sets the number of booked hours for this User.
   *
   * @param booked_hours the new number of booked hours
   */
  void set_booked_hours(const int &booked_hours);

  // helper functions for the menu
  /**
   * Displays the Events that this User has created.
   */
  void display_my_events();
  /**
   * Gets if this User will have overbooked.
   *
   * @param hours the new amount of booked hours
   * @return has the User overbooked with the new amount of booked hours added to their current
   */
  virtual bool has_overbooked(const int &hours) override;
  /**
   * Add an event to the client's list of events.
   */
  void add_event(const Event &event);
  /**
   * Remove an event from the client's list of events.
   */
  void remove_event(const Event &event);

  // menu functions
  /**
   * Displays the menu options for the User.
   */
  virtual void display_menu() override;
  /**
   * Handles menu input for the User.
   */
  virtual void handle_menu_input(Facility &facility) override;

private:
  ClientType client_type;
  vector<Event> my_events; // events that the user has successfully reserved
  int booked_hours;
};