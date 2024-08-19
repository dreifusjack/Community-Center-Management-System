#include "Event.hpp"
#include "Payment.hpp"
#include <algorithm>

Event::Event(const DateTime &dt, const LayoutType &layout, const GuestType &guest_type, const bool &is_public,
             const int &price_per_ticket, const int &duration_in_hours, const int &capacity, const Payment &payment, const shared_ptr<User> &organizer)
    : dt(dt), layout(layout), guest_type(guest_type), is_public(is_public), price_per_ticket(price_per_ticket),
      duration_in_hours(duration_in_hours), capacity(capacity), payment(payment), organizer(organizer) {}

DateTime Event::get_dt() const { return dt; }

string Event::get_date() const { return dt.get_date_str(); }

string Event::get_time() const { return dt.get_time_str(); }

Event::LayoutType Event::get_layout() const { return layout; }

shared_ptr<User> Event::get_organizer() const { return organizer; }

int Event::get_duration() const { return duration_in_hours; }

int Event::get_price_per_ticket() const { return price_per_ticket; }

int Event::get_capacity() const { return capacity; }

Event::GuestType Event::get_guest_type() const { return guest_type; }

bool Event::get_is_public() const { return is_public; }

Payment Event::get_payment() const { return payment; }

vector<Ticket> Event::get_tickets() const { return tickets; }

queue<shared_ptr<Citizen>> Event::get_waitlist() const { return waitlist; }

void Event::remove_ticket(const Ticket &ticket)
{
  tickets.erase(remove(tickets.begin(), tickets.end(), ticket), tickets.end());
}

void Event::add_to_waitlist(const shared_ptr<Citizen> &citizen)
{
  waitlist.push(citizen);
}

void Event::add_ticket(const Ticket &ticket)
{
  tickets.push_back(ticket);
}

bool Event::operator==(const Event &other) const
{
  return dt == other.dt && layout == other.layout && guest_type == other.guest_type && is_public == other.is_public;
}

ostream &operator<<(ostream &out, const Event &e)
{
  string privacy = e.is_public ? "Public" : "Private";
  string layout_type = e.layout == Event::WEDDING   ? "wedding"
                       : e.layout == Event::MEETING ? "meeting"
                       : e.layout == Event::LECTURE ? "lecture"
                                                    : "dance room";
  out << privacy << " event on " << e.get_date() << " at " << e.get_time() << " for "
      << e.duration_in_hours << " hours, with layout " << layout_type << ", price per ticket: $" << e.price_per_ticket << endl;
  out << "- Organizer: " << e.organizer->get_username() << endl;
  out << "- Attendees: ";
  for (size_t i = 0; i < e.tickets.size(); i++)
  {
    out << e.tickets[i].get_holder_username();
    if (i < e.tickets.size() - 1)
      out << ", ";
  }
  return out;
}

void Event::load_ticket_holders(const vector<Ticket> &tickets)
{
  for (const Ticket &t : tickets)
    this->add_ticket(t);
}

void Event::load_waitlist(const vector<shared_ptr<Citizen>> &waitlist)
{
  for (const shared_ptr<Citizen> &c : waitlist)
    this->add_to_waitlist(c);
}
