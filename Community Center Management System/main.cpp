#include "Facility.hpp"
#include "fileio.hpp"
#include "prompt.hpp"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

void display_login();
shared_ptr<User> login(const vector<shared_ptr<User>> &users);
shared_ptr<User> register_user(const vector<shared_ptr<User>> &users);
void handle_login_option(int option, vector<shared_ptr<User>> &users, shared_ptr<User> &logged_in_user);

int main()
{
  vector<shared_ptr<User>> users = user_utils::load_saved_users();

  // Load the FacilityManager
  auto manager_ptr = find_if(users.begin(), users.end(), [](const shared_ptr<User> &user)
                             { return user->get_username() == "BradStevens"; });

  // Get the mock time for the program
  cout << "Before running this program, you must enter a Date and Time to simulate when this program is being "
       << "run relative to events that occur." << endl;
  cout << "Enter the date (MM/DD/YYYY): ";
  string mock_date = prompt::get_user_date_input();
  cout << "Enter the time (e.g. 8 for 8am, 22 for 22:00, 11pm): ";
  string mock_time = prompt::get_user_time_input();
  DateTime mock_dt(mock_date, mock_time);

  Facility facility(*manager_ptr, mock_dt);
  facility.load_saved_confirmed_events(event_utils::load_confirmed_events(users));
  facility.load_saved_pending_events(event_utils::load_pending_events(users));

  cout << "\nWelcome to the Newton Community Center!" << endl;
  while (true)
  {
    display_login();

    int login_option;
    shared_ptr<User> logged_in_user = nullptr;
    while (true)
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin >> login_option;
      handle_login_option(login_option, users, logged_in_user);
      if (logged_in_user != nullptr)
        break;
    }
    logged_in_user->handle_menu_input(facility);
  }

  // Save data
  user_utils::save_users(users);
  event_utils::save_confirmed_events(facility.get_confirmed_events());
  event_utils::save_pending_events(facility.get_pending_events());

  return EXIT_SUCCESS;
}

/**
 * Displays all login options for a user.
 */
void display_login()
{
  cout << "Please select an option:" << endl;
  cout << "1. Login" << endl;
  cout << "2. Register" << endl;
  cout << "3. Exit" << endl;
}

/**
 * Attempts to log in a user, prompting for their username and password.
 *
 * @param users all of the users registered in the system
 * @return a pointer to the logged in user
 */
shared_ptr<User> login(const vector<shared_ptr<User>> &users)
{
  while (true)
  {
    cout << "Please enter your username (or type 'menu' to return to login menu): ";
    string username;
    cin >> username;
    if (username == "menu")
    {
      return nullptr;
    }
    auto user_it = find_if(users.begin(), users.end(), [&username](const shared_ptr<User> &user)
                           { return user->get_username() == username; });
    if (user_it == users.end())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Username not found. Please try again or type 'menu'." << endl;
      continue;
    }
    for (int attempts = 0; attempts < 3; ++attempts)
    {
      cout << "Please enter your password: ";
      string password;
      cin >> password;
      if ((*user_it)->get_password() == password)
      {
        cout << "Login successful!" << endl;
        return *user_it;
      }
      else
      {
        cout << "Incorrect password. ";
        if (attempts < 2)
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Please try again." << endl;
        }
        else
        {
          cout << "Returning to menu." << endl;
          return nullptr;
        }
      }
    }
  }
}

/**
 * Attempts to register a user, prompting for a username and password.
 *
 * @param users all of the users registered in the system
 * @return the newly created user as a pointer
 */
shared_ptr<User> register_user(const vector<shared_ptr<User>> &users)
{
  while (true)
  {
    cout << "Please enter your username (or type 'menu' to return to login menu): ";
    string username;
    cin >> username;
    if (username == "menu")
    {
      return nullptr;
    }
    if (find_if(users.begin(), users.end(), [&username](const shared_ptr<User> &user)
                { return user->get_username() == username; }) != users.end())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Username already exists. Please try again." << endl;
      continue;
    }
    cout << "Please enter your password: ";
    string password;
    cin >> password;

    cout << "Press 1 for citizen or 2 for client: ";
    int user_type;
    cin >> user_type;
    if (user_type != 1 && user_type != 2)
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid option. Try again." << endl;
      continue;
    }
    if (user_type == 1)
    {
      cout << "Press 1 for resident or 2 for non-resident: ";
      int resident_status;
      cin >> resident_status;
      if (resident_status != 1 && resident_status != 2)
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid option. Try again." << endl;
        continue;
      }
      Citizen::ResidentStatus status = (resident_status == 1) ? Citizen::RESIDENT : Citizen::NONRESIDENT;
      return make_shared<Citizen>(username, password, status);
    }
    else
    {
      cout << "Press 1 for city-member or 2 for organization-member: ";
      int client_type;
      cin >> client_type;
      if (client_type != 1 && client_type != 2)
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid option. Try again." << endl;
        continue;
      }
      Client::ClientType type = (client_type == 1) ? Client::CITY : Client::ORGANIZATION;
      return make_shared<Client>(username, password, type);
    }
  }
}

/**
 * Handles an authentication method (login, register, exit).
 *
 * @param option the authentication method (1. login, 2. register, 3. exit)
 * @param users all of the users registered in the system
 * @param logged_in_user a pointer that holds the currently logged in user
 */
void handle_login_option(int option, vector<shared_ptr<User>> &users, shared_ptr<User> &logged_in_user)
{
  switch (option)
  {
  case 1:
    logged_in_user = login(users);
    if (logged_in_user != nullptr)
    {
      return;
    }
    else
    {
      display_login();
    }
    break;
  case 2:
  {
    shared_ptr<User> created_user = register_user(users);
    if (created_user != nullptr)
    {
      users.push_back(created_user);
      cout << "Registration successful!" << endl;
    }
    else
    {
      cout << "Returning to menu." << endl;
    }
    display_login();
    break;
  }
  case 3:
    cout << "Goodbye!" << endl;
    user_utils::save_users(users); // Save any newly created users
    exit(EXIT_SUCCESS);
  default:
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid option. Please try again." << endl;
    display_login();
    break;
  }
}
