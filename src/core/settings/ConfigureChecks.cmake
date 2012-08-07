# Check for DConf
pkg_check_modules(GLIB "glib-2.0 >= 2.32" REQUIRED)
pkg_check_modules(DCONF dconf-dbus-1 REQUIRED)
macro_log_feature(DCONF_FOUND "DConf" "Support for DConf low-level key/value database" "http://live.gnome.org/dconf" TRUE "" "")
