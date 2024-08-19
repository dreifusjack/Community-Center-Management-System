#include "FacilityManager.hpp"
#include <limits>

using namespace std;

FacilityManager::FacilityManager(const string &username, const string &password) : User(username, password) {}

void FacilityManager::display_menu()
{
  cout << "Welcome, " << get_username() << "!" << endl;
  cout << "Please select an option:" << endl;
  cout << "1. View facility schedule" << endl;
  cout << "2. Approve event reservation requests" << endl;
  cout << "3. View balance" << endl;
  cout << "4. Return to login menu" << endl;
}

bool FacilityManager::has_overbooked(const int &hours)
{
  return false;
}

void FacilityManager::handle_menu_input(Facility &facility)
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
      handle_event_approvals(facility);
      break;
    case 3:
      User::claim_balance();
      return;
    case 4:
      cout << "Returning to login menu..." << endl;
      return;
    default:
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid option. Please try again." << endl;
      continue;
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

void FacilityManager::approve_event_request(Facility &facility, const ReservationRequest &request)
{
  // Create an Event from the ReservationRequest
  Event created_event = request.create_event();

  // Remove the event from the pending Events
  facility.remove_pending_event(request);

  // Confirm the Event in the facility
  facility.add_confirmed_event(created_event);

  // add the event to the user's list of events
  if (auto citizen_ptr = dynamic_pointer_cast<Citizen>(request.get_requester()))
  {
    citizen_ptr->add_event(created_event);
  }
  else if (auto client_ptr = dynamic_pointer_cast<Client>(request.get_requester()))
  {
    client_ptr->add_event(created_event);
  }
}

void FacilityManager::handle_event_approvals(Facility &facility)
{
  vector<ReservationRequest> pending_events = facility.get_pending_events();

  if (pending_events.size() == 0)
  {
    cout << "There are no pending events to approve." << endl;
  }
  else
  {
    cout << "Enter the number of a Reservation Request to approve:" << endl;
    for (size_t i = 0; i < pending_events.size(); i++)
      cout << to_string(i + 1) << ": " << pending_events[i] << endl;
    size_t none_option = pending_events.size() + 1;
    cout << to_string(none_option) << ": Approve none of these requests" << endl;

    size_t option;
    while (true)
    {
      cin >> option;
      if (option < 1 || option > none_option)
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid option. Please try again." << endl;
      }
      else
      {
        if (option == none_option)
        {
          cout << "Approving none of these requests." << endl;
          return;
        }
        else
        {
          cout << "Approving event: " << pending_events[option - 1] << endl;
          approve_event_request(facility, pending_events[option - 1]);
          return;
        }
      }
    }
  }
}