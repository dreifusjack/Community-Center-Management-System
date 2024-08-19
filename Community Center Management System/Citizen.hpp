#pragma once

#include "User.hpp"
#include "Ticket.hpp"
#include "Event.hpp"
#include <vector>

using namespace std;

// forward declarations
class Ticket;
class Event;

class Citizen : public User
{
public:
  enum ResidentStatus
  {
    RESIDENT,
    NONRESIDENT
  };
  Citizen(const string &username, const string &password, const ResidentStatus &resident_status);
  ~Citizen() = default;

  // getters and setters
  /**
   * Gets the ResidentStatus of this Citizen.
   *
   * @return the ResidentStatus
   */
  ResidentStatus get_resident_status() const;
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
   * Displays the User's tickets for Events.
   */
  void display_my_tickets();
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
   * Adds a Ticket to this User's list of tickets.
   */
  void add_ticket(const Ticket &ticket);

  /**
   * Removes a Ticket from this User's list of tickets.
   */
  void remove_ticket(const Ticket &ticket);

  /**
   * Adds an Event to this User's list of events.
   */
  void add_event(const Event &event);

  /**
   * Removes an Event from this User's list of events.
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
  ResidentStatus resident_status;
  vector<Ticket> my_tickets; // tickets that the user has successfully bought
  vector<Event> my_events;   // events that the user has successfully reserved
  int booked_hours;
};