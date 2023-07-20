#ifndef CONFIG_H
#define CONFIG_H

#include <setup.h>
// http://192.168.8.136/
//  AP Name, password
#define AP_NAME "Fume extractor"            // access point name
#define PAGE_TITLE "Fume extractor"         // page title
#define AP_PASS "ap_password"               // access point password
#define ADMIN_PAGE_USERNAME "user";         // http://[local ip]/_ac/update - username
#define ADMIN_PAGE_PASSWORD "pass";         // http://[local ip]/_ac/update - password
#define ENABLE_PASSWORD_PROTECTED_WEBSITE 0 // 1 enables password protected admin pages (only update, AP, etc), but not toggle
#define TOGGLE_ONBOARD_LED_FOR_TESTING 0    // 1 enables onboard LED for testing

#define relay_turn_off_relay_delay_ms_def 10000
#define relay_turn_on_relay_delay_ms 400
#define SSalive_max_timeout 300

// min time should be 100ms, but this is too little i think
// https://www.aliexpress.com/item/1005001325775411.html?spm=a2g0o.productlist.0.0.6a29621a9WTjbV&algo_pvid=06c2e948-c5b8-403f-b119-8eb7f267489f&algo_exp_id=06c2e948-c5b8-403f-b119-8eb7f267489f-35&pdp_ext_f=%7B%22sku_id%22%3A%2212000015699711430%22%7D&pdp_npi=2%40dis%21EUR%219.37%219.37%21%21%21%21%21%40210318c216638713780245224eada9%2112000015699711430%21sea&curPageLogUid=qTCD6KBQCo5h
// https://www.aliexpress.com/item/1005002863717374.html?spm=a2g0o.order_detail.0.0.f264f19cra8hvC
/*
io5 relay
io2 TXd
io3 RX
gpio16 LED

input pins
GPIO 12
GPIO14
*/

#endif