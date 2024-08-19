#pragma once

#include "User.hpp"
#include "Citizen.hpp"
#include "Client.hpp"
#include "Ticket.hpp"
#include "Payment.hpp"
#include "DateTime.hpp"
#include <memory>
#include <vector>
#include <queue>

// Forward declarations
class User;
class Citizen;
class Ticket;

class Event
{
public:
  enum LayoutType
  {
    WEDDING,
    MEETING,
    LECTURE,
    DANCEROOM
  };

  enum GuestType
  {
    RESIDENTS,
    NONRESIDENTS,
    BOTH
  };
  Event(const DateTime &dt, const LayoutType &layout, const GuestType &guest_type, const bool &is_public,
        const int &price_per_ticket, const int &duration_in_hours, const int &capacity, const Payment &payment, const shared_ptr<User> &organizer);

  // getters and setters
  /**
   * Gets the DateTime of this Event.
   *
   * @return the DateTime
   */
  DateTime get_dt() const;
  /**
   * Gets the date of this Event.
   *
   * @return the date
   */
  string get_date() const;
  /**
   * Gets the time of this Event.
   *
   * @return the time
   */
  string get_time() const;
  /**
   * Gets the LayoutType of this Event.
   *
   * @return the LayoutType
   */
  LayoutType get_layout() const;
  /**
   * Gets the organizer of this Event.
   *
   * @return the organizer of this Event
   */
  shared_ptr<User> get_organizer() const;
  /**
   * Gets the duration of this Event.
   *
   * @return the duration in hours
   */
  int get_duration() const;
  /**
   * Gets the price per ticket of this Event.
   */
  int get_price_per_ticket() const;
  /**
   * Gets the tickets of this Event.
   */
  vector<Ticket> get_tickets() const;
  /**
   * Gets the waitlist of this Event.
   */
  queue<shared_ptr<Citizen>> get_waitlist() const;
  /**
   * Gets the capacity of this Event.
   */
  int get_capacity() const;
  /**
   * Gets the GuestType of this Event.
   *
   * @return the GuestType
   */
  Event::GuestType get_guest_type() const;
  /**
   * Gets the is_public status of this Event.
   */
  bool get_is_public() const;
  /**
   * Gets the Payment for this Event.
   *
   * @return the Payment
   */
  Payment get_payment() const;
  /**
   * Remove a ticket from the event.
   */
  void remove_ticket(const Ticket &ticket);
  /**
   * Adds the citizen to the waitlist.
   */
  void add_to_waitlist(const shared_ptr<Citizen> &citizen);
  /**
   * Adds a ticket to the event.
   */
  void add_ticket(const Ticket &ticket);

  /**
   * Operator overload for Event ==.
   */
  bool operator==(const Event &event) const;
  /**
   * Operator overload for Event <<.
   */
  friend ostream &operator<<(ostream &out, const Event &e);

  // methods for updating the event from state persistence
  /**
   * Adds ticket holders to this event given a vector of Tickets.
   *
   * @param tickets the ticket holders to this event
   */
  void load_ticket_holders(const vector<Ticket> &tickets);
  /**
   * Adds Citizens to this event that are on the waitlist given a vector of Citizens.
   *
   * @param waitlist the Citizens on the waitlist
   */
  void load_waitlist(const vector<shared_ptr<Citizen>> &waitlist);

private:
  // event information
  DateTime dt;
  LayoutType layout;
  GuestType guest_type;
  bool is_public;
  int price_per_ticket;
  int duration_in_hours;
  int capacity;

  // event objects
  Payment payment;
  shared_ptr<User> organizer;
  vector<Ticket> tickets;
  queue<shared_ptr<Citizen>> waitlist;
};