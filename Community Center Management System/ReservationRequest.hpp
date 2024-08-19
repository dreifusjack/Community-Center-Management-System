#pragma once

#include "Event.hpp"
#include "Payment.hpp"
#include "User.hpp"
#include "DateTime.hpp"
#include <memory>

using namespace std;

class ReservationRequest
{
private:
  DateTime dt;
  Event::LayoutType layout;
  Event::GuestType guests;
  bool is_public;
  int price_per_ticket;
  int duration_in_hours;
  Payment payment;
  shared_ptr<User> requester;

public:
  ReservationRequest(const DateTime &dt, const Event::LayoutType &layout, const Event::GuestType &guests, const bool &is_public,
                     const int &price_per_ticket, const int &duration, const Payment &payment, shared_ptr<User> &requester);
  ~ReservationRequest() = default;

  // getters
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
  Event::LayoutType get_layout() const;
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
   * Gets the price per ticket of this Event.
   */
  int get_price_per_ticket() const;
  /**
   * Gets the duration of this Event.
   *
   * @return the duration in hours
   */
  int get_duration() const;
  /**
   * Gets the Payment for this Event.
   *
   * @return the Payment
   */
  Payment get_payment() const;
  /**
   * Gets the requester of this ReservationRequest.
   */
  shared_ptr<User> get_requester() const;

  /**
   * Creates an Event from this ReservationRequest.
   */
  Event create_event() const;

  /**
   * Operator overload for ReservationRequest ==.
   */
  bool operator==(const ReservationRequest &other) const;
  /**
   * Operator overload for ReservationRequest <<.
   */
  friend ostream &operator<<(ostream &out, const ReservationRequest &r);
};