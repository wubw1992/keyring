#include <libsecret/secret.h>
#include <stdio.h>

static const char *PasswordSecretValueContentType = "text/plain";
static const char *LoginKeyringPath = "/org/freedesktop/secrets/collection/login";

int main(int argc, char **argv) {
  gchar *current = "";
  gchar *newPass = "";

  printf(" argc length: %d \n", argc);

  if (argc < 3) {
    printf(" argv too short \n");
    return -1;
  }

  current = *argv[1];
  newPass = *argv[2];

  //加密
//  current = "";
//  newPass = "3";

  //解密
//  current = "3";
//  newPass = "";

  if (current == 0x2d) {
      current = "";
  } else if (current != '\0') {
      current = argv[1];
    }

  if (newPass == 0x2d) {
      newPass = "";
  } else if (newPass != '\0') {
      newPass = argv[2];
  }

  GError *err = NULL;
  SecretService *service = NULL;
  GDBusConnection *bus = NULL;
  SecretValue *currentValue = NULL;
  SecretValue *newPassValue = NULL;
  GVariant *ret = NULL;

  do {
    service = secret_service_get_sync(SECRET_SERVICE_OPEN_SESSION, NULL, &err);
    if (service == NULL) {
      printf(" 11 failed to get session bus: %s", err->message);
      break;
    }

    SecretCollection *collection = secret_collection_for_alias_sync(
        service, SECRET_COLLECTION_DEFAULT, SECRET_COLLECTION_NONE, NULL, &err);
    if (err != NULL) {
      break;
    }

    if (collection == NULL) {
      printf("default secret collection not exists");
      break;
    }

    currentValue = secret_value_new(current, strlen(current), PasswordSecretValueContentType);
    newPassValue = secret_value_new(newPass, strlen (newPass), PasswordSecretValueContentType);

    bus = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &err);
    if (bus == NULL) {
      printf("22 failed to get session bus: %s", err->message);
      break;
    }

    ret = g_dbus_connection_call_sync(
        bus, "org.gnome.keyring", "/org/freedesktop/secrets",
        "org.gnome.keyring.InternalUnsupportedGuiltRiddenInterface",
        "ChangeWithMasterPassword",
        g_variant_new("(o@(oayays)@(oayays))", LoginKeyringPath,
                      secret_service_encode_dbus_secret(service, currentValue),
                      secret_service_encode_dbus_secret(service, newPassValue)),
        NULL, G_DBUS_CALL_FLAGS_NONE, G_MAXINT, NULL, &err);

    if (err != NULL) {
      printf(" 33 failed to get session bus: %s", err->message);
      break;
    }
  } while (0);

  if (err != NULL)
    g_error_free(err);
  if (service != NULL)
    g_object_unref(NULL);
  if (bus != NULL)
    g_object_unref(bus);
  if (currentValue != NULL)
    secret_value_unref(currentValue);
  if (newPassValue != NULL)
    secret_value_unref(newPassValue);
  if (ret != NULL)
    g_variant_unref(ret);
}
