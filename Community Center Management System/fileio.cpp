#include "fileio.hpp"
#include "User.hpp"
#include "Citizen.hpp"
#include "Client.hpp"
#include "FacilityManager.hpp"
#include "Event.hpp"
#include "ReservationRequest.hpp"
#include "DateTime.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

namespace fileio
{
  vector<string> parse_file(const string &file_path)
  {
    ifstream infile(file_path);
    if (!infile.is_open())
    {
      cout << "Error reading file: " << file_path << endl;
      cout << "Default to empty content." << endl;
      return {};
    }

    vector<string> file_content; // The content of the file as a vector of strings

    string line;
    while (getline(infile, line))
    {
      file_content.push_back(line);
    }

    infile.close();
    return file_content;
  }

  void write_to_file(const string &file_path, const vector<string> &content)
  {
    ofstream outfile(file_path);

    if (!outfile.is_open())
      cout << "Error write to file: " << file_path << endl;
    else
    {
      for (const string &line : content)
        outfile << line << endl;

      outfile.close();
    }
  }

  void sanitize_lines(string &str)
  {
    str.erase(remove(str.begin(), str.end(), '\r'), str.end()); // deletes CR
    str.erase(remove(str.begin(), str.end(), '\n'), str.end()); // deletes LF
  }
}

namespace user_utils
{
  vector<shared_ptr<User>> load_saved_users()
  {
    vector<shared_ptr<User>> users;

    vector<string> user_strings = fileio::parse_file("program_data/users.csv");
    for (const string &user_str : user_strings)
    {
      stringstream ss(user_str);
      string item;
      char token = ',';

      string user_type_str;
      string user_name;
      string user_password;
      string user_type_status_str = "";

      // Read user type, first name, last name
      getline(ss, user_type_str, token);
      getline(ss, user_name, token);
      getline(ss, user_password, token);

      if (user_type_str == "FACILITY_MANAGER")
      {
        users.push_back(make_shared<FacilityManager>(user_name, user_password));
      }
      else
      {
        // Read user status
        getline(ss, user_type_status_str, token);

        if (user_type_str == "CITIZEN")
        {
          auto type = user_type_status_str == "RESIDENT" ? Citizen::RESIDENT : Citizen::NONRESIDENT;
          users.push_back(make_shared<Citizen>(user_name, user_password, type));
        }
        else
        {
          auto type = user_type_status_str == "CITY" ? Client::CITY : Client::ORGANIZATION;
          users.push_back(make_shared<Client>(user_name, user_password, type));
        }
      }
    }

    return users;
  }

  void save_users(const vector<shared_ptr<User>> &users)
  {
    vector<string> user_strings;

    for (const auto &user_ptr : users)
    {
      if (auto casted_user_ptr = dynamic_pointer_cast<FacilityManager>(user_ptr))
        user_strings.push_back("FACILITY_MANAGER," + casted_user_ptr->get_username() + "," + casted_user_ptr->get_password());
      else if (auto casted_user_ptr = dynamic_pointer_cast<Citizen>(user_ptr))
        user_strings.push_back("CITIZEN," + casted_user_ptr->get_username() + "," + casted_user_ptr->get_password() + "," + (casted_user_ptr->get_resident_status() == Citizen::RESIDENT ? "RESIDENT" : "NON_RESIDENT"));
      else if (auto casted_user_ptr = dynamic_pointer_cast<Client>(user_ptr))
        user_strings.push_back("CLIENT," + casted_user_ptr->get_username() + "," + casted_user_ptr->get_password() + "," + (casted_user_ptr->get_client_type() == Client::CITY ? "CITY" : "ORGANIZATION"));
    }

    fileio::write_to_file("program_data/users.csv", user_strings);
  }

  shared_ptr<User> username_to_user(const vector<shared_ptr<User>> users, const string &s)
  {
    for (const shared_ptr<User> &user : users)
    {
      if (user->get_username() == s)
        return user;
    }

    return nullptr;
  }
}

namespace event_utils
{
  Event::LayoutType str_to_layout_type(const string &s)
  {
    return s == "WEDDING"   ? Event::WEDDING
           : s == "MEETING" ? Event::MEETING
           : s == "LECTURE" ? Event::LECTURE
                            : Event::DANCEROOM;
  }

  Event::GuestType str_to_guest_type(const string &s)
  {
    return s == "RESIDENTS"      ? Event::RESIDENTS
           : s == "NONRESIDENTS" ? Event::NONRESIDENTS
                                 : Event::BOTH;
  }

  bool str_to_is_public(const string &s)
  {
    return s == "public";
  }

  string layout_type_to_str(Event::LayoutType layoutType)
  {
    return layoutType == Event::WEDDING   ? "WEDDING"
           : layoutType == Event::MEETING ? "MEETING"
           : layoutType == Event::LECTURE ? "LECTURE"
                                          : "DANCEROOM";
  }

  string guest_type_to_str(Event::GuestType guestType)
  {
    return guestType == Event::RESIDENTS      ? "RESIDENTS"
           : guestType == Event::NONRESIDENTS ? "NONRESIDENTS"
                                              : "BOTH";
  }

  vector<Event> load_confirmed_events(const vector<shared_ptr<User>> &users)
  {
    vector<Event> events;

    vector<string> event_strings = fileio::parse_file("program_data/confirmed_events.csv");
    for (size_t i = 1; i < event_strings.size(); i++) // Ignore header line
    {
      const string &event_str = event_strings[i];

      stringstream ss(event_str);
      string item;
      char token = ',';

      string date;
      string time;
      string layout_str;
      string guest_type_str;
      string is_public_str;
      string price;
      string duration;
      string payment_amount;
      string cc;
      string cvv;
      string expiration_date;
      string organizer_username;
      string tickets_str;
      string waitlist_str;

      getline(ss, date, token);
      getline(ss, time, token);
      getline(ss, layout_str, token);
      getline(ss, guest_type_str, token);
      getline(ss, is_public_str, token);
      getline(ss, price, token);
      getline(ss, duration, token);
      getline(ss, payment_amount, token);
      getline(ss, cc, token);
      getline(ss, cvv, token);
      getline(ss, expiration_date, token);
      getline(ss, organizer_username, token);
      getline(ss, tickets_str, token);  // May be empty
      getline(ss, waitlist_str, token); // May be empty

      // Transform data from strings to appropriate types
      DateTime dt(date, time);
      Event::LayoutType layout = event_utils::str_to_layout_type(layout_str);
      Event::GuestType guest_type = event_utils::str_to_guest_type(guest_type_str);
      bool is_public = event_utils::str_to_is_public(is_public_str);
      shared_ptr<User> organizer = user_utils::username_to_user(users, organizer_username);

      // Create the Payment and Event
      Payment payment(stod(payment_amount), stol(cc), stoi(cvv), expiration_date);
      Event saved_event = Event(dt, layout, guest_type, is_public, stoi(price),
                                stoi(duration), 40, payment, organizer);

      // Transform ticket strings to Tickets
      vector<Ticket> tickets;
      fileio::sanitize_lines(tickets_str);
      if (!tickets_str.empty())
      {
        stringstream ss_tickets(tickets_str);
        string user;
        while (getline(ss_tickets, user, ';'))
          tickets.push_back(Ticket(dynamic_pointer_cast<Citizen>(user_utils::username_to_user(users, user)), make_shared<Event>(saved_event)));
      }
      saved_event.load_ticket_holders(tickets);

      // Add these tickets to the Citizen's tickets
      for (const Ticket &t : tickets)
      {
        shared_ptr<Citizen> ticket_holder = t.get_holder();
        ticket_holder->add_ticket(t);
      }

      // Transform waitlist string to Citizens on the waitlist
      vector<shared_ptr<Citizen>> citizens_on_waitlist;
      fileio::sanitize_lines(waitlist_str);
      if (!waitlist_str.empty())
      {
        stringstream ss_waitlist(waitlist_str);
        string current_username;
        while (getline(ss_waitlist, current_username, ';'))
          citizens_on_waitlist.push_back(dynamic_pointer_cast<Citizen>(user_utils::username_to_user(users, current_username)));
      }
      saved_event.load_waitlist(citizens_on_waitlist);

      // Add this event to the organizer's booked events
      if (auto citizen_ptr = dynamic_pointer_cast<Citizen>(organizer))
        citizen_ptr->add_event(saved_event);
      else if (auto client_ptr = dynamic_pointer_cast<Client>(organizer))
        client_ptr->add_event(saved_event);

      events.push_back(saved_event);
    }

    return events;
  }

  void save_confirmed_events(const vector<Event> &events)
  {
    vector<string> event_strings;

    // Add header
    event_strings.push_back("DATE,TIME,LAYOUT,GUEST_TYPE,IS_PUBLIC,PRICE,DURATION,PAYMENT_AMOUNT,CC,CVV,EXPIRY,ORGANIZER,TICKETS,WAITLIST");

    for (const auto &event : events)
    {
      Payment payment = event.get_payment();
      string payment_str = to_string(payment.get_amount()) + "," + to_string(payment.get_card_number()) + "," +
                           to_string(payment.get_cvv()) + "," + payment.get_expiry_date();

      vector<Ticket> tickets = event.get_tickets();
      string ticket_str = "";
      for (size_t i = 0; i < tickets.size(); i++)
      {
        ticket_str += tickets[i].get_holder_username();
        if (i < tickets.size() - 1)
          ticket_str += ";";
      }
      queue<shared_ptr<Citizen>> waitlist_queue = event.get_waitlist();
      vector<shared_ptr<Citizen>> waitlist;
      // convert queue to vector
      while (!waitlist_queue.empty())
      {
        waitlist.push_back(waitlist_queue.front());
        waitlist_queue.pop();
      }
      string waitlist_str = "";
      for (size_t i = 0; i < waitlist.size(); i++)
      {
        waitlist_str += waitlist[i]->get_username();
        if (i < waitlist.size() - 1)
          waitlist_str += ";";
      }
      string event_str = event.get_date() + "," + event.get_time() + "," + layout_type_to_str(event.get_layout()) + "," +
                         guest_type_to_str(event.get_guest_type()) + "," + (event.get_is_public() ? "public" : "private") + "," +
                         to_string(event.get_price_per_ticket()) + "," + to_string(event.get_duration()) + "," + payment_str + "," +
                         event.get_organizer()->get_username() + "," + ticket_str + "," + waitlist_str;

      event_strings.push_back(event_str);
    }

    fileio::write_to_file("program_data/confirmed_events.csv", event_strings);
  }

  vector<ReservationRequest> load_pending_events(const vector<shared_ptr<User>> &users)
  {
    vector<ReservationRequest> events;

    vector<string> event_strings = fileio::parse_file("program_data/pending_events.csv");
    for (size_t i = 1; i < event_strings.size(); i++) // Ignore header line
    {
      const string &event_str = event_strings[i];

      stringstream ss(event_str);
      string item;
      char token = ',';

      string date;
      string time;
      string layout_str;
      string guest_type_str;
      string is_public_str;
      string price;
      string duration;
      string payment_amount;
      string cc;
      string cvv;
      string expiration_date;
      string organizer_username;

      getline(ss, date, token);
      getline(ss, time, token);
      getline(ss, layout_str, token);
      getline(ss, guest_type_str, token);
      getline(ss, is_public_str, token);
      getline(ss, price, token);
      getline(ss, duration, token);
      getline(ss, payment_amount, token);
      getline(ss, cc, token);
      getline(ss, cvv, token);
      getline(ss, expiration_date, token);
      getline(ss, organizer_username, token);

      // Transform data from strings to appropriate types
      DateTime dt(date, time);
      Event::LayoutType layout = event_utils::str_to_layout_type(layout_str);
      Event::GuestType guest_type = event_utils::str_to_guest_type(guest_type_str);
      bool is_public = event_utils::str_to_is_public(is_public_str);
      shared_ptr<User> organizer = user_utils::username_to_user(users, organizer_username);

      // Create the Payment and ReservationRequest
      Payment payment(stod(payment_amount), stol(cc), stoi(cvv), expiration_date);
      ReservationRequest pending_event = ReservationRequest(dt, layout, guest_type, is_public, stoi(price),
                                                            stoi(duration), payment, organizer);

      events.push_back(pending_event);
    }

    return events;
  }

  void save_pending_events(const vector<ReservationRequest> &events)
  {
    vector<string> event_strings;

    // Add header
    event_strings.push_back("DATE,TIME,LAYOUT,GUEST_TYPE,IS_PUBLIC,PRICE,DURATION,PAYMENT_AMOUNT,CC,CVV,EXPIRY,ORGANIZER");

    for (const auto &event : events)
    {
      Payment payment = event.get_payment();
      string payment_str = to_string(payment.get_amount()) + "," + to_string(payment.get_card_number()) + "," +
                           to_string(payment.get_cvv()) + "," + payment.get_expiry_date();

      string event_str = event.get_date() + "," + event.get_time() + "," + layout_type_to_str(event.get_layout()) + "," +
                         guest_type_to_str(event.get_guest_type()) + "," + (event.get_is_public() ? "public" : "private") + "," +
                         to_string(event.get_price_per_ticket()) + "," + to_string(event.get_duration()) + "," + payment_str + "," +
                         event.get_requester()->get_username();

      event_strings.push_back(event_str);
    }

    fileio::write_to_file("program_data/pending_events.csv", event_strings);
  }

}
