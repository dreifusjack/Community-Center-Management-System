#pragma once

#include "User.hpp"
#include "Event.hpp"
#include "ReservationRequest.hpp"
#include "FacilityManager.hpp"
#include "DateTime.hpp"
#include <vector>
#include <memory>

class Facility
{
private:
  vector<Event> confirmed_events;
  vector<ReservationRequest> pending_events; // events that are waiting for approval by the facility manager
  shared_ptr<User> manager;
  DateTime mock_dt;

public:
  Facility(shared_ptr<User> manager, const DateTime &dt);
  ~Facility() = default;

  /**
   * Gets the confirmed events in the Facility.
   *
   * @return the confirmed events in the Facility.
   */
  vector<Event> get_confirmed_events() const;
  /**
   * Gets the pending events in the Facility.
   *
   * @return the pending events in the Facility
   */
  vector<ReservationRequest> get_pending_events() const;

  // system backend functions
  /**
   * Adds an event as a confirmed event to this Facility.
   *
   * @param event the event to be added to the confirmed events
   */
  void add_confirmed_event(const Event &event);
  /**
   * Removes an event from the confirmed events of this Facility.
   *
   * @param event the event to be removed to the confirmed events
   */
  void remove_confirmed_event(const Event &event);
  /**
   * Adds an event as a pending event to be confirmed by the FacilityManager to this Facility.
   *
   * @param event the event to be added to the pending events
   */
  void add_pending_event(const ReservationRequest &event);
  /**
   * Removes a pending event from this Facility.
   *
   * @param event the event to be removed from the pending events
   */
  void remove_pending_event(const ReservationRequest &event);
  /**
   * Creates a ReservationRequest for an Event for the given User.
   *
   * @param requester the user making the reservation request
   */
  void request_event(shared_ptr<User> requester);
  /**
   * Cancels an Event.
   *
   * @param requester the user making the cancellation request
   */
  void cancel_event(shared_ptr<User> requester);
  /**
   *
   */
  void request_ticket(shared_ptr<User> requester);
  /**
   * Cancels a Ticket in an Event.
   *
   * @param requester the user attempting to cancel the ticket
   */
  void refund_ticket(shared_ptr<User> requester);
  /**
   * Calculates the cost of reserving an Event.
   * There is a service charge of $10, and an hourly rate of $10 for Residents, $15 for Non-Residents,
   * $20 for Organizations, and $5 for the City.
   *
   * @param requester the user that reserved the Event
   * @param duration the duration of the event (in hours)
   */
  double calculate_event_cost(const shared_ptr<User> &requester, const int &duration) const;

  /**
   * Displays the current schedule of the Facility.
   */
  void display_schedule() const;

  // methods for updating the Facility from state persistence
  /**
   * Loads a vector of Events into this Facility's confirmed Events.
   *
   * @param events the events to load
   */
  void load_saved_confirmed_events(const vector<Event> &events);
  /**
   * Loads a vector of ReservationRequest into this Facility's pending Events.
   *
   * @param events the events to load
   */
  void load_saved_pending_events(const vector<ReservationRequest> &events);
};
