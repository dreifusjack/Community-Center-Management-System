#include "Client.hpp"
#include "Facility.hpp"
#include <limits>
#include <algorithm>

using namespace std;

Client::Client(const string &username, const string &password, const ClientType &client_type)
    : User(username, password), client_type(client_type), booked_hours(0) {}

Client::ClientType Client::get_client_type() const
{
  return client_type;
}

int Client::get_booked_hours() const
{
  return booked_hours;
}

void Client::set_booked_hours(const int &booked_hours)
{
  this->booked_hours = booked_hours;
}

void Client::add_event(const Event &event)
{
  my_events.push_back(event);
}

void Client::remove_event(const Event &event)
{
  my_events.erase(remove(my_events.begin(), my_events.end(), event), my_events.end());
}

void Client::display_my_events()
{
  cout << "My reserved events: " << endl;
  if (my_events.size() == 0)
    cout << "No reserved events." << endl;
  else
    for (const Event &event : my_events)
      cout << event << endl;
}

bool Client::has_overbooked(const int &hours)
{
  if (client_type == CITY)
    return booked_hours + hours > 48;
  else
    return booked_hours + hours > 36; // Organization
}

void Client::display_menu()
{
  cout << "Welcome, " << get_username() << "!" << endl;
  cout << "Please select an option:" << endl;
  cout << "1. View facility schedule" << endl;
  cout << "2. Book an event" << endl;
  cout << "3. View my events" << endl;
  cout << "4. Cancel an event" << endl;
  cout << "5. View my balance" << endl;
  cout << "6. Return to login menu" << endl;
}

void Client::handle_menu_input(Facility &facility)
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
      facility.request_event(make_shared<Client>(*this));
      break;
    case 3:
      display_my_events();
      break;
    case 4:
      facility.cancel_event(make_shared<Client>(*this));
      break;
    case 5:
      User::claim_balance();
      break;
    case 6:
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