#include "Citizen.hpp"
#include "Facility.hpp"
#include <limits>
#include <algorithm>

using namespace std;

Citizen::Citizen(const string &username, const string &password, const ResidentStatus &resident_status)
    : User(username, password), resident_status(resident_status), booked_hours(0) {}

Citizen::ResidentStatus Citizen::get_resident_status() const
{
  return resident_status;
}

int Citizen::get_booked_hours() const
{
  return booked_hours;
}

void Citizen::set_booked_hours(const int &booked_hours)
{
  this->booked_hours = booked_hours;
}

void Citizen::display_my_tickets()
{
  cout << "My purchased tickets: " << endl;
  if (my_tickets.size() == 0)
    cout << "No purchased tickets." << endl;
  else
    for (const Ticket &ticket : my_tickets)
      cout << ticket << endl
           << endl;
}

void Citizen::display_my_events()
{
  cout << "My reserved events: " << endl;
  if (my_events.size() == 0)
    cout << "No reserved events." << endl;
  else
    for (const Event &event : my_events)
      cout << event << endl
           << endl;
}

bool Citizen::has_overbooked(const int &hours)
{
  return booked_hours + hours > 24;
}

void Citizen::display_menu()
{
  cout << "Welcome, " << get_username() << "!" << endl;
  cout << "Please select an option:" << endl;
  cout << "1. View facility schedule" << endl;
  cout << "2. Book an event" << endl;
  cout << "3. Buy tickets for an event" << endl;
  cout << "4. View my tickets" << endl;
  cout << "5. View my organized events" << endl;
  cout << "6. Cancel an event" << endl;
  cout << "7. Refund a ticket" << endl;
  cout << "8. View my balance" << endl;
  cout << "9. Return to login menu" << endl;
}

void Citizen::handle_menu_input(Facility &facility)
{
  int option;
  while (true)
  {
    display_menu(); // Display the menu at the start

    cin >> option;
    switch (option)
    {
    case 1:
      facility.display_schedule();
      break;
    case 2:
      facility.request_event(make_shared<Citizen>(*this));
      break;
    case 3:
      facility.request_ticket(make_shared<Citizen>(*this));
      break;
    case 4:
      display_my_tickets();
      break;
    case 5:
      display_my_events();
      break;
    case 6:
      facility.cancel_event(make_shared<Citizen>(*this));
      break;
    case 7:
      facility.refund_ticket(make_shared<Citizen>(*this));
      break;
    case 8:
      User::claim_balance();
      break;
    case 9:
      cout << "Returning to login menu..." << endl;
      return;
    default:
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid option. Please try again." << endl;
      continue; // Skip the rest of the loop and redisplay the menu
    }
    while (true)
    {
      string return_to_menu;
      cout << "Return to menu? (y/n): ";
      cin >> return_to_menu;

      if (return_to_menu == "n")
      {
        cout << "Logging out..." << endl;
        return;
      }
      else if (return_to_menu == "y")
      {
        break; // Break out of the inner loop to redisplay the menu
      }
      else
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid option. Please try again." << endl;
      }
    }
  }
}

void Citizen::add_ticket(const Ticket &ticket)
{
  my_tickets.push_back(ticket);
}

void Citizen::remove_ticket(const Ticket &ticket)
{
  my_tickets.erase(remove(my_tickets.begin(), my_tickets.end(), ticket), my_tickets.end());
}

void Citizen::add_event(const Event &event)
{
  my_events.push_back(event);
}

void Citizen::remove_event(const Event &event)
{
  my_events.erase(remove(my_events.begin(), my_events.end(), event), my_events.end());
}
