#pragma once

#include "User.hpp"
#include "Event.hpp"
#include "Payment.hpp"
#include "Citizen.hpp"
#include <memory>

using namespace std;

class Event;
class Citizen;

class Ticket
{
private:
  shared_ptr<Citizen> holder;
  shared_ptr<Event> event;

public:
  Ticket(shared_ptr<Citizen> holder, shared_ptr<Event> event);
  ~Ticket() = default;

  // getters
  /**
   * Gets the username of the holder of this Ticket.
   */
  string get_holder_username() const;
  /**
   * Gets the holder of this Ticket.
   */
  shared_ptr<Citizen> get_holder() const;

  /**
   * Refunds the ticket to the holder's account
   */
  void refund(const int &amount);

  // overload operators
  bool operator==(const Ticket &ticket) const;
  friend ostream &operator<<(ostream &out, const Ticket &t);
};