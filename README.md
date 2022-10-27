debug keyring

#编译方法
1. mkdir build
2. cd build
3. cmake ..
4. make

#调试方法
#说明 -1表示“”，3表示系统密码
1. 加密： ./keyring -1 3
2. 解密： ./keyring 3 -1

#参考
https://mail.gnome.org/archives/commits-list/2015-July/msg06536.html
https://libsoup.org/gio/GDBusProxy.html#g-dbus-proxy-call-sync