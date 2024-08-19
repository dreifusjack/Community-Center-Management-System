#include "Facility.hpp"
#include "DateTime.hpp"
#include "prompt.hpp"
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

Facility::Facility(shared_ptr<User> manager, const DateTime &dt) : manager(manager), mock_dt(dt) {}

vector<Event> Facility::get_confirmed_events() const { return confirmed_events; }

vector<ReservationRequest> Facility::get_pending_events() const { return pending_events; }

void Facility::add_confirmed_event(const Event &event)
{
  confirmed_events.push_back(event);
}

void Facility::remove_confirmed_event(const Event &event)
{
  confirmed_events.erase(remove(confirmed_events.begin(), confirmed_events.end(), event), confirmed_events.end());
}

void Facility::add_pending_event(const ReservationRequest &event)
{
  pending_events.push_back(event);
}

void Facility::remove_pending_event(const ReservationRequest &event)
{
  pending_events.erase(remove(pending_events.begin(), pending_events.end(), event), pending_events.end());
}

double Facility::calculate_event_cost(const shared_ptr<User> &requester, const int &duration) const
{
  if (dynamic_pointer_cast<Citizen>(requester))
  {
    if (dynamic_pointer_cast<Citizen>(requester)->get_resident_status() == Citizen::ResidentStatus::RESIDENT)
    {
      return 10 + (duration * 10);
    }
    else if (dynamic_pointer_cast<Citizen>(requester)->get_resident_status() == Citizen::ResidentStatus::NONRESIDENT)
    {
      return 10 + (duration * 15);
    }
  }
  else if (dynamic_pointer_cast<Client>(requester))
  {
    if (dynamic_pointer_cast<Client>(requester)->get_client_type() == Client::ClientType::CITY)
    {
      return 10 + (duration * 5);
    }
    else if (dynamic_pointer_cast<Client>(requester)->get_client_type() == Client::ClientType::ORGANIZATION)
    {
      return 10 + (duration * 20);
    }
  }
  return 0;
}

void Facility::request_event(shared_ptr<User> requester)
{
  // prompts the user to enter data for requested event, creates an instance of if the time doesnt collide with another event ReservationRequest,
  cout << "Event Request:" << endl;
  cout << "Enter the date to be request (MM/DD/YYYY): ";
  string date = prompt::get_user_date_input();

  cout << "Enter the hour of the time for the event (i.e. 8 for 08:00, 22 for 22:00): ";
  string time = prompt::get_user_time_input();
  DateTime dt(date, time);

  // Get the Duration of the event
  cout << "Enter the duration of the event in hours: ";
  int duration;
  while (true)
  {
    cin >> duration;
    if (!cin.good() || duration < 1 || duration > dt.hours_until_facility_close())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid duration. Please try again." << endl;
    }
    else
    {
      break;
    }
  }

  // Before creating reservation request, check if the event is available and if the user has overbooked
  for (const auto &event : confirmed_events)
  {
    // logic needs to be changed to check if the requested time is within the duration of the event
    if (event.get_date() == date && event.get_time() == time)
    {
      cout << "Event already booked at that time!" << endl;
      return;
    }
  }
  if (requester->has_overbooked(duration))
  {
    cout << "You have overbooked!" << endl;
    return;
  }

  // cast the requester to see if its a Citizen or Client
  if (dynamic_pointer_cast<Citizen>(requester))
  {
    cout << "Enter the layout type (1. Meeting, 2. Lecture, 3. Dance, 4. Wedding): ";
  }
  else if (dynamic_pointer_cast<Client>(requester))
  {
    cout << "Enter the layout type (1. Meeting, 2. Lecture, 3. Dance): ";
  }
  int layout_type;
  while (true)
  {
    cin >> layout_type;
    if (layout_type == 1)
    {
      layout_type = Event::LayoutType::MEETING;
      break;
    }
    else if (layout_type == 2)
    {
      layout_type = Event::LayoutType::LECTURE;
      break;
    }
    else if (layout_type == 3)
    {
      layout_type = Event::LayoutType::DANCEROOM;
      break;
    }
    else if (layout_type == 4)
    {
      layout_type = Event::LayoutType::WEDDING;
      break;
    }
    else
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid layout type. Please try again." << endl;
    }
  }

  cout << "Enter the guest-type (1. Residents, 2. Non-Residents, 3. Both): ";
  int guest_type;
  while (true)
  {
    cin >> guest_type;
    if (guest_type == 1)
    {
      guest_type = Event::GuestType::RESIDENTS;
      break;
    }
    else if (guest_type == 2)
    {
      guest_type = Event::GuestType::NONRESIDENTS;
      break;
    }
    else if (guest_type == 3)
    {
      guest_type = Event::GuestType::BOTH;
      break;
    }
    else
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid guest type. Please try again." << endl;
    }
  }

  cout << "Is the event public? (1. Yes, 2. No): ";
  int is_public_int;
  bool is_public;
  while (true)
  {
    cin >> is_public_int;
    if (is_public_int != 1 && is_public_int != 2)
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid public type. Please enter 1 or 2." << endl;
    }
    else
    {
      is_public = (is_public_int == 1);
      break;
    }
  }

  int price_per_ticket;
  if (is_public)
  {
    cout << "Enter the price per ticket for the event ($): ";
    cin >> price_per_ticket;
    while (true)
    {
      if (price_per_ticket < 0)
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid price. Please try again." << endl;
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    price_per_ticket = 0;
  }

  // Calculate the price of the event and prompt the user for payment
  double total = calculate_event_cost(requester, duration);
  cout << "Your total is $" << total << ".\n";
  cout << "Enter the payment information below:" << endl;
  cout << "Enter the card number: ";
  long card_number;
  while (true)
  {
    cin >> card_number;
    if (card_number < 1000000000000000 || card_number > 9999999999999999)
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid card number (must be 16 numbers). Please try again." << endl;
    }
    else
    {
      break;
    }
  }
  cout << "Enter the CVV: ";
  int cvv;
  cin >> cvv;
  while (true)
  {
    if (cvv < 100 || cvv > 999)
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid CVV. Please try again." << endl;
    }
    else
    {
      break;
    }
  }
  cout << "Enter the expiration date (MM/YY): ";
  string expiration_date;
  while (true)
  {
    cin >> expiration_date;
    if (expiration_date.length() != 5 && expiration_date[2] != '/')
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid expiration date. Please try again." << endl;
    }
    else
    {
      int month = stoi(date.substr(0, 2));
      int year = stoi(date.substr(3, 2));
      if ((year < 24 || (month < 6 && year <= 24)))
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid date. The date must be later than 05/24. Please try again" << endl;
      }
      else
      {
        break;
      }
    }
  }
  // create instance of payment object with inputed card information
  Payment payment(total, card_number, cvv, expiration_date);

  // Count the duration of this event against the user's booked hours
  if (auto citizen_ptr = dynamic_pointer_cast<Citizen>(requester))
    citizen_ptr->set_booked_hours(citizen_ptr->get_booked_hours() + duration);
  else if (auto client_ptr = dynamic_pointer_cast<Client>(requester))
    client_ptr->set_booked_hours(client_ptr->get_booked_hours() + duration);

  manager->add_to_balance(total);
  cout << "Event requested successfully!" << endl;
  ReservationRequest request(dt, static_cast<Event::LayoutType>(layout_type),
                             static_cast<Event::GuestType>(guest_type), is_public, price_per_ticket, duration, payment, requester);
  this->add_pending_event(request);
}

void Facility::cancel_event(shared_ptr<User> requester)
{
  /**
   * Events canceled within 24 do not get refuned
   * Events canceled within a week get 1% penalty
   * Events canceled before a week get no penalty
   * The service charge in not refundable
   */
  // case the requester to display their events
  if (auto citizen_ptr = dynamic_pointer_cast<Citizen>(requester))
    citizen_ptr->display_my_events();
  else if (auto client_ptr = dynamic_pointer_cast<Client>(requester))
    client_ptr->display_my_events();

  cout << "Enter the date of the event to cancel (MM/DD/YYYY): ";
  string date = prompt::get_user_date_input();
  cout << "Enter the time of the event to cancel (i.e. 8 for 08:00, 22 for 22:00): ";
  string time = prompt::get_user_time_input();
  DateTime event_dt(date, time);

  // Validate current time is before the event
  if (event_dt < mock_dt)
  {
    cout << "Cannot cancel an event that has already occurred (current time is after inputted time)." << endl;
    return;
  }

  for (const Event &event : confirmed_events)
  {
    if (event.get_dt() == event_dt)
    {
      double refund;

      int time_difference = event_dt.hours_difference(mock_dt);
      if (time_difference > 168) // More than a week
        refund = event.get_payment().get_amount() - 10;
      else if (time_difference > 24 && time_difference <= 168) // Between 24 hours and a week
        refund = (event.get_payment().get_amount() - 10) * 0.99;
      else // Less than 24 hours
        refund = 0;

      cout << "You will be refunded $" << refund << " for the event." << endl;

      // Remove the event from the user's booked hours and list of events
      if (auto citizen_ptr = dynamic_pointer_cast<Citizen>(requester))
      {
        citizen_ptr->set_booked_hours(citizen_ptr->get_booked_hours() - event.get_duration());
        citizen_ptr->remove_event(event);
      }
      else if (auto client_ptr = dynamic_pointer_cast<Client>(requester))
      {
        client_ptr->set_booked_hours(client_ptr->get_booked_hours() - event.get_duration());
        client_ptr->remove_event(event);
      }

      // Money to be paid
      manager->subtract_from_balance(refund);

      // Refund the user
      event.get_organizer()->add_to_balance(refund);

      // Refund the tickets
      for (auto &ticket : event.get_tickets())
      {
        manager->subtract_from_balance(event.get_price_per_ticket());
        ticket.refund(event.get_price_per_ticket());
      }

      // Remove the event from the confirmed events
      this->remove_confirmed_event(event);

      cout << "Event canceled successfully!" << endl;
      return;
    }
  }
  cout << "Event not found." << endl;
}

void Facility::request_ticket(shared_ptr<User> requester)
{
  auto citizen_ptr = dynamic_pointer_cast<Citizen>(requester);
  display_schedule();
  cout << "Enter the date of the event to request a ticket for (MM/DD/YYYY): ";
  string date = prompt::get_user_date_input();
  cout << "Enter the time of the event to request a ticket for (i.e. 8 for 08:00, 22 for 22:00): ";
  string time = prompt::get_user_time_input();
  DateTime event_dt(date, time);

  for (auto &event : confirmed_events)
  {
    if (event.get_dt() == event_dt)
    {
      if (!event.get_is_public())
      {
        cout << "This event is private and does not have tickets for sale." << endl;
        return;
      }
      // check if the user already has a ticket for this event
      for (const auto &ticket : event.get_tickets())
      {
        if (ticket.get_holder_username() == citizen_ptr->get_username())
        {
          cout << "You already have a ticket for this event." << endl;
          return;
        }
      }
      if (event.get_guest_type() == Event::GuestType::RESIDENTS && citizen_ptr->get_resident_status() == Citizen::ResidentStatus::NONRESIDENT)
      {
        cout << "This event is for residents only." << endl;
        return;
      }
      if (event.get_guest_type() == Event::GuestType::NONRESIDENTS && citizen_ptr->get_resident_status() == Citizen::ResidentStatus::RESIDENT)
      {
        cout << "This event is for non-residents only." << endl;
        return;
      }
      double total = event.get_price_per_ticket();
      cout << "Your total is $" << total << ".\n";
      cout << "Enter the payment information below:" << endl;
      cout << "Enter the card number: ";
      long card_number;
      while (true)
      {
        cin >> card_number;
        if (card_number < 1000000000000000 || card_number > 9999999999999999)
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Invalid card number (must be 16 numbers). Please try again." << endl;
        }
        else
        {
          break;
        }
      }
      cout << "Enter the CVV: ";
      int cvv;
      while (true)
      {
        cin >> cvv;
        if (cvv < 100 || cvv > 999)
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Invalid CVV. Please try again." << endl;
        }
        else
        {
          break;
        }
      }
      cout << "Enter the expiration date (MM/YY): ";
      string expiration_date;
      while (true)
      {
        cin >> expiration_date;
        if (expiration_date.length() != 5 || expiration_date[2] != '/')
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Invalid expiration date. Please try again." << endl;
        }
        else
        {
          int month = stoi(expiration_date.substr(0, 2));
          int year = stoi(expiration_date.substr(3, 2));
          if (year < 24 || (year == 24 && month <= 5))
          {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid date. The date must be later than 05/24. Please try again" << endl;
          }
          else
          {
            break;
          }
        }
      }

      if (event.get_tickets().size() >= static_cast<size_t>(event.get_capacity()))
      {
        cout << "This event is sold out, you have been added to the waitlist." << endl;
        event.add_to_waitlist(citizen_ptr);
        return;
      }
      else
      {
        auto new_ticket = make_shared<Ticket>(citizen_ptr, make_shared<Event>(event));
        event.add_ticket(*new_ticket);
        citizen_ptr->add_ticket(*new_ticket);
        manager->add_to_balance(total);
        cout << "Ticket purchased successfully!" << endl;
        return;
      }
    }
  }
  cout << "Event not found." << endl;
}

void Facility::refund_ticket(shared_ptr<User> requester)
{
  auto citizen_ptr = dynamic_pointer_cast<Citizen>(requester);
  citizen_ptr->display_my_tickets();

  cout << "Enter the date of the ticket to refund (MM/DD/YYYY): ";
  string date = prompt::get_user_date_input();
  cout << "Enter the time of the ticket to cancel (i.e. 8 for 08:00, 22 for 22:00): ";
  string time = prompt::get_user_time_input();
  DateTime event_dt(date, time);

  // Validate current time is before the ticket
  if (event_dt < mock_dt)
  {
    cout << "Cannot refund a ticket that has already occurred (current time is after inputted time)." << endl;
    return;
  }

  for (Event &event : confirmed_events)
  {
    if (event.get_dt() == event_dt)
    {
      for (Ticket &ticket : event.get_tickets())
      {
        if (ticket.get_holder_username() == citizen_ptr->get_username())
        {
          manager->subtract_from_balance(event.get_price_per_ticket());
          event.remove_ticket(ticket);
          ticket.refund(event.get_price_per_ticket());

          // checks if the waitlist is not empty if so it will create a ticket for the first person in the waitlist
          if (!event.get_waitlist().empty())
          {
            shared_ptr<Citizen> citizen = event.get_waitlist().front();
            auto new_ticket = Ticket(citizen, make_shared<Event>(event));
            event.add_ticket(new_ticket);
            citizen->add_ticket(new_ticket);
            event.get_waitlist().pop();
            manager->add_to_balance(event.get_price_per_ticket());
          }
          return;
        }
      }
    }
  }
  cout << "Ticket not found." << endl;
}

void Facility::display_schedule() const
{
  cout << "Facility Schedule:" << endl;
  for (const auto &event : confirmed_events)
  {
    if (event.get_dt().is_same_day_or_after(mock_dt))
      cout << event << endl;
  }
}

void Facility::load_saved_confirmed_events(const vector<Event> &events)
{
  for (const auto &e : events)
    this->add_confirmed_event(e);
}

void Facility::load_saved_pending_events(const vector<ReservationRequest> &events)
{
  for (const auto &e : events)
    this->add_pending_event(e);
}
