# Check for DConf
pkg_check_modules(GLIB "glib-2.0 >= 2.32" REQUIRED)
pkg_check_modules(DCONF "dconf >= 0.12 dconf < 0.13.1 dconf-dbus-1 >= 0.12 dconf-dbus-1 < 0.13.1" REQUIRED)
macro_log_feature(DCONF_FOUND "DConf" "Support for DConf low-level key/value database" "http://live.gnome.org/dconf" TRUE "" "")
