
/* -*- mode: C; c-file-style: "gnu" -*- */

#ifndef BUS_SIGNALS_H
#define BUS_SIGNALS_H

#include <dbus/dbus.h>
#include <dbus/dbus-string.h>
#include <dbus/dbus-sysdeps.h>
#include "connection.h"

typedef enum
{
  BUS_MATCH_MESSAGE_TYPE = 1 << 0,
  BUS_MATCH_INTERFACE    = 1 << 1,
  BUS_MATCH_MEMBER       = 1 << 2,
  BUS_MATCH_SENDER       = 1 << 3,
  BUS_MATCH_DESTINATION  = 1 << 4,
  BUS_MATCH_PATH         = 1 << 5,
  BUS_MATCH_ARGS         = 1 << 6
} BusMatchFlags;

BusMatchRule* bus_match_rule_new   (DBusConnection *matches_go_to);
BusMatchRule* bus_match_rule_ref   (BusMatchRule   *rule);
void          bus_match_rule_unref (BusMatchRule   *rule);

dbus_bool_t bus_match_rule_set_message_type (BusMatchRule *rule,
                                             int           type);
dbus_bool_t bus_match_rule_set_interface    (BusMatchRule *rule,
                                             const char   *interface);
dbus_bool_t bus_match_rule_set_member       (BusMatchRule *rule,
                                             const char   *member);
dbus_bool_t bus_match_rule_set_sender       (BusMatchRule *rule,
                                             const char   *sender);
dbus_bool_t bus_match_rule_set_destination  (BusMatchRule *rule,
                                             const char   *destination);
dbus_bool_t bus_match_rule_set_path         (BusMatchRule *rule,
                                             const char   *path);
dbus_bool_t bus_match_rule_set_arg          (BusMatchRule *rule,
                                             int           arg,
                                             const char   *value);

BusMatchRule* bus_match_rule_parse (DBusConnection   *matches_go_to,
                                    const DBusString *rule_text,
                                    DBusError        *error);

BusMatchmaker* bus_matchmaker_new   (void);
BusMatchmaker* bus_matchmaker_ref   (BusMatchmaker *matchmaker);
void           bus_matchmaker_unref (BusMatchmaker *matchmaker);

dbus_bool_t bus_matchmaker_add_rule             (BusMatchmaker   *matchmaker,
                                                 BusMatchRule    *rule);
dbus_bool_t bus_matchmaker_remove_rule_by_value (BusMatchmaker   *matchmaker,
                                                 BusMatchRule    *value,
                                                 DBusError       *error);
void        bus_matchmaker_remove_rule          (BusMatchmaker   *matchmaker,
                                                 BusMatchRule    *rule);
void        bus_matchmaker_disconnected         (BusMatchmaker   *matchmaker,
                                                 DBusConnection  *disconnected);
dbus_bool_t bus_matchmaker_get_recipients       (BusMatchmaker   *matchmaker,
                                                 BusConnections  *connections,
                                                 DBusConnection  *sender,
                                                 DBusConnection  *addressed_recipient,
                                                 DBusMessage     *message,
                                                 DBusList       **recipients_p);

#endif /* BUS_SIGNALS_H */
