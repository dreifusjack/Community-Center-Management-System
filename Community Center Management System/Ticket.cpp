#include "Ticket.hpp"

Ticket::Ticket(shared_ptr<Citizen> holder, shared_ptr<Event> event)
    : holder(holder), event(event) {}

string Ticket::get_holder_username() const { return holder->get_username(); }

shared_ptr<Citizen> Ticket::get_holder() const { return holder; }

bool Ticket::operator==(const Ticket &ticket) const
{
  return holder == ticket.holder && event == ticket.event;
}

ostream &operator<<(ostream &out, const Ticket &t)
{
  const Event &e = *(t.event);
  string privacy = e.get_is_public() ? "Public" : "Private";
  string layout_type = e.get_layout() == Event::WEDDING   ? "wedding"
                       : e.get_layout() == Event::MEETING ? "meeting"
                       : e.get_layout() == Event::LECTURE ? "lecture"
                                                          : "dance room";
  out << t.holder->get_username() << "'s Ticket" << endl;
  out << "Time: " << e.get_date() << " at " << e.get_time() << endl;
  out << "Event: " << privacy << " event for " << e.get_duration() << " hours, with layout " << layout_type << endl;
  out << "Cost: $" << e.get_price_per_ticket();
  return out;
}

void Ticket::refund(const int &amount)
{
  holder->add_to_balance(amount);
  // remove the ticket from the user's list of tickets
  holder->remove_ticket(*this);
  // handle updating the facility managers balance
  cout << "Ticket refunded successfully!" << endl;
}