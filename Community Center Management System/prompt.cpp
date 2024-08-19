#include "prompt.hpp"
#include <string>
#include <limits>
#include <iostream>

using namespace std;

namespace prompt
{
  string get_user_date_input()
  {
    string date;
    while (true)
    {
      cin >> date;
      if (date.length() != 10 && date[2] != '/' && date[5] != '/')
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid date format. Please try again." << endl;
      }
      else
        break;
    }
    return date;
  }

  string get_user_time_input()
  {
    while (true)
    {
      string input;
      cin >> input;
      try
      {
        int time_num = stoi(input);
        if (time_num < 8 || time_num > 23)
        {
          cout << "Facility is closed at this time, try a new time." << endl;
        }
        else
        {
          return (time_num < 10 ? "0" : "") + to_string(time_num) + ":00";
        }
      }
      catch (invalid_argument &)
      {
        cout << "Invalid input. Please enter a valid time (e.g. 9 for 9am or 20 for 22:00, 10pm)." << endl;
      }
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }
}
