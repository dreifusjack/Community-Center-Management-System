#pragma once

#include "User.hpp"
#include "Event.hpp"
#include "ReservationRequest.hpp"
#include "Ticket.hpp"
#include "Payment.hpp"
#include "Facility.hpp"
#include <memory>

class FacilityManager : public User
{
public:
  FacilityManager(const string &username, const string &password);
  ~FacilityManager() = default;

  // managment functions
  /**
   * Approves an event in the Facility.
   *
   * @param facility the Facility
   * @param request the ReservationRequest to approve
   */
  void approve_event_request(Facility &facility, const ReservationRequest &request);
  /**
   * Displays all events pending approval in the Facility and allows the User to choose one to approve.
   *
   * @param facility the Facility
   */
  void handle_event_approvals(Facility &facility);
  virtual bool has_overbooked(const int &hours) override;

  // menu functions
  /**
   * Displays the menu options for the User.
   */
  virtual void display_menu() override;
  /**
   * Handles menu input for the User.
   */
  virtual void handle_menu_input(Facility &facility) override;
};