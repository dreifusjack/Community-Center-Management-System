#include "ReservationRequest.hpp"
#include <string>

using namespace std;

ReservationRequest::ReservationRequest(const DateTime &dt, const Event::LayoutType &layout, const Event::GuestType &guests, const bool &is_public,
                                       const int &price_per_ticket, const int &duration, const Payment &payment, shared_ptr<User> &requester)
    : dt(dt), layout(layout), guests(guests), is_public(is_public), price_per_ticket(price_per_ticket),
      duration_in_hours(duration), payment(payment), requester(requester) {}

DateTime ReservationRequest::get_dt() const { return dt; }

string ReservationRequest::get_date() const { return dt.get_date_str(); }

string ReservationRequest::get_time() const { return dt.get_time_str(); }

Event::LayoutType ReservationRequest::get_layout() const { return layout; }

Event::GuestType ReservationRequest::get_guest_type() const { return guests; }

int ReservationRequest::get_duration() const { return duration_in_hours; }

int ReservationRequest::get_price_per_ticket() const { return price_per_ticket; }

bool ReservationRequest::get_is_public() const { return is_public; }

Payment ReservationRequest::get_payment() const { return payment; }

shared_ptr<User> ReservationRequest::get_requester() const { return requester; }

Event ReservationRequest::create_event() const
{
  // By default, the Event host and organizer is the person who requested the event
  return Event(dt, layout, guests, is_public, price_per_ticket, duration_in_hours, 40, payment, requester);
}

bool ReservationRequest::operator==(const ReservationRequest &other) const
{
  return dt == other.dt && layout == other.layout && guests == other.guests && is_public == other.is_public;
}

ostream &operator<<(ostream &out, const ReservationRequest &r)
{
  string privacy = r.is_public ? "Public" : "Private";
  string layout_type = r.layout == Event::WEDDING   ? "wedding"
                       : r.layout == Event::MEETING ? "meeting"
                       : r.layout == Event::LECTURE ? "lecture"
                                                    : "dance room";
  out << privacy << " event on " << r.get_date() << " at " << r.get_time() << " for "
      << r.duration_in_hours << " hours, with layout " << layout_type;
  return out;
}
