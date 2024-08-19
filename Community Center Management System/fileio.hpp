#include "User.hpp"
#include "Event.hpp"
#include "ReservationRequest.hpp"
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace fileio
{
  /**
   * Parses a file into a vector of strings by new lines.
   *
   * @param file_path the file path to parse
   * @return a vector of strings for each line of the file content
   */
  vector<string> parse_file(const string &file_path);

  /**
   * Writes content to a file line by line.
   *
   * @param file_path the file path to write to
   * @param content the content to write to the file, where each item is written on a new line
   */
  void write_to_file(const string &file_path, const vector<string> &content);

  /**
   * Removes CR and LF from the given string.
   *
   * @param str the string to remove CR and LF from
   */
  void sanitize_lines(string &str);
}

namespace user_utils
{
  /**
   * Loads saved users from a saved file.
   *
   * @return the users that have been registered to the app as a vector of User pointers
   */
  vector<shared_ptr<User>> load_saved_users();

  /**
   * Saves a vector of users to a saved file.
   *
   * @param users the users to save to a file
   */
  void save_users(const vector<shared_ptr<User>> &users);

  /**
   * Returns a pointer to a User given a username. Returns nullptr if the User
   * doesn't exist.
   *
   * @param users the User's to search
   * @param s the User's username.
   */
  shared_ptr<User> username_to_user(const vector<shared_ptr<User>> users, const string &s);
}

namespace event_utils
{
  /**
   * Returns the LayoutType from a string.
   *
   * @param s the LayoutType as a string
   * @return the LayoutType
   */
  Event::LayoutType str_to_layout_type(const string &s);

  /**
   * Returns the GuestType from a string.
   *
   * @param s the GuestType as a string
   * @return the GuestType
   */
  Event::GuestType str_to_guest_type(const string &s);

  /**
   * Returns a string from the LayoutType.
   *
   * @param LayoutType the LayoutType
   * @return the string representation
   */
  string layout_type_to_str(Event::LayoutType layoutType);

  /**
   * Returns a string from the GuestType.
   *
   * @param GuestType the LayoutType
   * @return the string representation
   */
  string guest_type_to_str(Event::GuestType guestType);

  /**
   * Returns a bool from a string if the event is public or not.
   *
   * @param s the public/private as a string
   * @return is the event public
   */
  bool str_to_is_public(const string &s);

  /**
   * Loads confirmed events from a file.
   *
   * @param users the Users in the program
   * @return the Events that have been previously confirmed
   */
  vector<Event> load_confirmed_events(const vector<shared_ptr<User>> &users);

  /**
   * Saves confirmed events to a file.
   *
   * @param events the confirmed Events to save
   */
  void save_confirmed_events(const vector<Event> &events);

  /**
   * Loads pending events from a file.
   *
   * @param users the Users in the program
   * @return the Events that are pending confirmation
   */
  vector<ReservationRequest> load_pending_events(const vector<shared_ptr<User>> &users);

  /**
   * Saves pending events to a file.
   *
   * @param events the pending Events to save
   */
  void save_pending_events(const vector<ReservationRequest> &events);
}
