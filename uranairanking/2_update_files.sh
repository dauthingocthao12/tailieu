#!/bin/bash

# FROM root (before develop)

cp develop/uranai_lib/admin/index.php uranai_lib/admin/index.php
cp develop/uranai_lib/bat/cronsum.php uranai_lib/bat/cronsum.php
cp develop/uranai_lib/bat/index.php uranai_lib/bat/index.php
cp develop/uranai_lib/libadmin/account-regist.ctrl.php uranai_lib/libadmin/account-regist.ctrl.php
cp develop/uranai_lib/libadmin/account.ctrl.php uranai_lib/libadmin/account.ctrl.php
cp develop/uranai_lib/libadmin/all_site_links.php uranai_lib/libadmin/all_site_links.php
cp develop/uranai_lib/libadmin/breadcrumb.class.php uranai_lib/libadmin/breadcrumb.class.php
cp develop/uranai_lib/libadmin/calendar.class.php uranai_lib/libadmin/calendar.class.php
cp develop/uranai_lib/libadmin/common.php uranai_lib/libadmin/common.php
cp develop/uranai_lib/libadmin/config.php uranai_lib/libadmin/config.php
cp develop/uranai_lib/libadmin/graph_data.php uranai_lib/libadmin/graph_data.php
cp develop/uranai_lib/libadmin/insert.affiliate.php uranai_lib/libadmin/insert.affiliate.php
rm uranai_lib/libadmin/monthly_ranking.class.php
cp develop/uranai_lib/libadmin/news.class.php uranai_lib/libadmin/news.class.php
cp develop/uranai_lib/libadmin/ranking_past.php uranai_lib/libadmin/ranking_past.php
cp develop/uranai_lib/libadmin/site.php uranai_lib/libadmin/site.php
cp develop/uranai_lib/libadmin/snsapi-mailupdate.class.php uranai_lib/libadmin/snsapi-mailupdate.class.php
cp develop/uranai_lib/libadmin/totalscore.class.php uranai_lib/libadmin/totalscore.class.php
cp develop/uranai_lib/libadmin/uranairanking.class.php uranai_lib/libadmin/uranairanking.class.php
cp develop/uranai_lib/libadmin/utils.smarty.php uranai_lib/libadmin/utils.smarty.php
rm uranai_lib/libadmin/yearly_ranking.class.php
cp develop/uranai_lib/templates/admin/ad-listing.tpl uranai_lib/templates/admin/ad-listing.tpl
cp develop/uranai_lib/templates/admin/log-listing.tpl uranai_lib/templates/admin/log-listing.tpl
cp develop/uranai_lib/templates/admin/main.tpl uranai_lib/templates/admin/main.tpl
cp develop/uranai_lib/templates/admin/news-delete-do.tpl uranai_lib/templates/admin/news-delete-do.tpl
cp develop/uranai_lib/templates/admin/news-delete.tpl uranai_lib/templates/admin/news-delete.tpl
cp develop/uranai_lib/templates/admin/news-input.tpl uranai_lib/templates/admin/news-input.tpl
cp develop/uranai_lib/templates/admin/news-listing.tpl uranai_lib/templates/admin/news-listing.tpl
cp develop/uranai_lib/templates/admin/news-update.tpl uranai_lib/templates/admin/news-update.tpl
cp develop/uranai_lib/templates/user/about.tpl uranai_lib/templates/user/about.tpl
cp develop/uranai_lib/templates/user/account-activate-resend.tpl uranai_lib/templates/user/account-activate-resend.tpl
cp develop/uranai_lib/templates/user/account-activate.tpl uranai_lib/templates/user/account-activate.tpl
cp develop/uranai_lib/templates/user/account-activated-mail.tpl uranai_lib/templates/user/account-activated-mail.tpl
cp develop/uranai_lib/templates/user/account-delete.tpl uranai_lib/templates/user/account-delete.tpl
cp develop/uranai_lib/templates/user/account-form-success.tpl uranai_lib/templates/user/account-form-success.tpl
cp develop/uranai_lib/templates/user/account-form.tpl uranai_lib/templates/user/account-form.tpl
rm uranai_lib/templates/user/account-index.tpl
cp develop/uranai_lib/templates/user/account-intro.tpl uranai_lib/templates/user/account-intro.tpl
cp develop/uranai_lib/templates/user/account-login.tpl uranai_lib/templates/user/account-login.tpl
cp develop/uranai_lib/templates/user/account-logout.tpl uranai_lib/templates/user/account-logout.tpl
cp develop/uranai_lib/templates/user/account-password-lost.tpl uranai_lib/templates/user/account-password-lost.tpl
cp develop/uranai_lib/templates/user/account-unregist.tpl uranai_lib/templates/user/account-unregist.tpl
rm uranai_lib/templates/user/article/20160311.tpl
rm uranai_lib/templates/user/article/20160328.tpl
rm uranai_lib/templates/user/article/20160606.tpl
rm uranai_lib/templates/user/article/20160613.tpl
rm uranai_lib/templates/user/article/20160627.tpl
rm uranai_lib/templates/user/article/20160712.tpl
rm uranai_lib/templates/user/article/20160728.tpl
rm uranai_lib/templates/user/article/20160729.tpl
rm uranai_lib/templates/user/article/20160825.tpl
rm uranai_lib/templates/user/article/20170111.tpl
rm uranai_lib/templates/user/article/20170203.tpl
rm uranai_lib/templates/user/article/20170301.tpl
rm uranai_lib/templates/user/article/20170331.tpl
cp develop/uranai_lib/templates/user/calendar.parts.tpl uranai_lib/templates/user/calendar.parts.tpl
cp develop/uranai_lib/templates/user/company.tpl uranai_lib/templates/user/company.tpl
cp develop/uranai_lib/templates/user/google_analytics.tpl uranai_lib/templates/user/google_analytics.tpl
cp develop/uranai_lib/templates/user/kiyaku.tpl uranai_lib/templates/user/kiyaku.tpl
cp develop/uranai_lib/templates/user/main.tpl uranai_lib/templates/user/main.tpl
cp develop/uranai_lib/templates/user/mainline.parts.tpl uranai_lib/templates/user/mainline.parts.tpl
rm uranai_lib/templates/user/monthly_ranking.tpl
cp develop/uranai_lib/templates/user/policy.tpl uranai_lib/templates/user/policy.tpl
cp develop/uranai_lib/templates/user/ranking-detail.tpl uranai_lib/templates/user/ranking-detail.tpl
cp develop/uranai_lib/templates/user/ranking-index.tpl uranai_lib/templates/user/ranking-index.tpl
cp develop/uranai_lib/templates/user/ranking-past.tpl uranai_lib/templates/user/ranking-past.tpl
rm uranai_lib/templates/user/ranking2016/april.tpl
rm uranai_lib/templates/user/ranking2016/august.tpl
rm uranai_lib/templates/user/ranking2016/december.tpl
rm uranai_lib/templates/user/ranking2016/february.tpl
rm uranai_lib/templates/user/ranking2016/january.tpl
rm uranai_lib/templates/user/ranking2016/july.tpl
rm uranai_lib/templates/user/ranking2016/june.tpl
rm uranai_lib/templates/user/ranking2016/march.tpl
rm uranai_lib/templates/user/ranking2016/may.tpl
rm uranai_lib/templates/user/ranking2016/november.tpl
rm uranai_lib/templates/user/ranking2016/october.tpl
rm uranai_lib/templates/user/ranking2016/september.tpl
rm uranai_lib/templates/user/ranking2016/total.tpl
rm uranai_lib/templates/user/ranking2017/january.tpl
cp develop/uranai_lib/templates/user/sidebar.tpl uranai_lib/templates/user/sidebar.tpl
cp develop/uranai_lib/templates/user/site-list.tpl uranai_lib/templates/user/site-list.tpl
cp develop/uranai_lib/templates/user/whatnew-details.tpl uranai_lib/templates/user/whatnew-details.tpl
cp develop/uranai_lib/templates/user/whatnew-list.tpl uranai_lib/templates/user/whatnew-list.tpl
rm uranai_lib/templates/user/whatnew.tpl
rm uranai_lib/uranai-engine/bootstrap.php
cp develop/uranai_lib/user/index.php uranai_lib/user/index.php
cp develop/www/.htaccess.dev www/.htaccess.dev
cp develop/www/apple-touch-icon.png www/apple-touch-icon.png
cp develop/www/favicon.ico www/favicon.ico
cp develop/www/maintenance.html www/maintenance.html
cp develop/www/user/css/app-main.css www/user/css/app-main.css
cp develop/www/user/css/app-theme.blue-starry-sky.css www/user/css/app-theme.blue-starry-sky.css
rm www/user/css/app-theme.css
cp develop/www/user/img_re/background.jpg www/user/img_re/background.jpg
cp develop/www/user/img_re/member-registration-2.png www/user/img_re/member-registration-2.png
cp develop/www/user/img_re/member-registration.jpg www/user/img_re/member-registration.jpg
cp develop/www/user/img_re/n-aeris.png www/user/img_re/n-aeris.png
cp develop/www/user/img_re/n-aquarius.png www/user/img_re/n-aquarius.png
cp develop/www/user/img_re/n-cancer.png www/user/img_re/n-cancer.png
cp develop/www/user/img_re/n-capricorn.png www/user/img_re/n-capricorn.png
cp develop/www/user/img_re/n-gemini.png www/user/img_re/n-gemini.png
cp develop/www/user/img_re/n-leo.png www/user/img_re/n-leo.png
cp develop/www/user/img_re/n-libra.png www/user/img_re/n-libra.png
cp develop/www/user/img_re/n-pisces.png www/user/img_re/n-pisces.png
cp develop/www/user/img_re/n-sagittarius.png www/user/img_re/n-sagittarius.png
cp develop/www/user/img_re/n-scorpio.png www/user/img_re/n-scorpio.png
cp develop/www/user/img_re/n-taurus.png www/user/img_re/n-taurus.png
cp develop/www/user/img_re/n-virgo.png www/user/img_re/n-virgo.png
cp develop/www/user/img_re/nagarebosi.gif www/user/img_re/nagarebosi.gif
cp develop/www/user/img_re/no1.png www/user/img_re/no1.png
cp develop/www/user/img_re/no10.png www/user/img_re/no10.png
cp develop/www/user/img_re/no11.png www/user/img_re/no11.png
cp develop/www/user/img_re/no12.png www/user/img_re/no12.png
cp develop/www/user/img_re/no2.png www/user/img_re/no2.png
cp develop/www/user/img_re/no3.png www/user/img_re/no3.png
cp develop/www/user/img_re/no4.png www/user/img_re/no4.png
cp develop/www/user/img_re/no5.png www/user/img_re/no5.png
cp develop/www/user/img_re/no6.png www/user/img_re/no6.png
cp develop/www/user/img_re/no7.png www/user/img_re/no7.png
cp develop/www/user/img_re/no8.png www/user/img_re/no8.png
cp develop/www/user/img_re/no9.png www/user/img_re/no9.png
cp develop/www/user/img_re/rank.png www/user/img_re/rank.png
cp develop/www/user/img_re/rank_crown.png www/user/img_re/rank_crown.png
cp develop/www/user/img_re/top-no1.png www/user/img_re/top-no1.png
cp develop/www/user/img_re/top-no10.png www/user/img_re/top-no10.png
cp develop/www/user/img_re/top-no11.png www/user/img_re/top-no11.png
cp develop/www/user/img_re/top-no12.png www/user/img_re/top-no12.png
cp develop/www/user/img_re/top-no2.png www/user/img_re/top-no2.png
cp develop/www/user/img_re/top-no3.png www/user/img_re/top-no3.png
cp develop/www/user/img_re/top-no4.png www/user/img_re/top-no4.png
cp develop/www/user/img_re/top-no5.png www/user/img_re/top-no5.png
cp develop/www/user/img_re/top-no6.png www/user/img_re/top-no6.png
cp develop/www/user/img_re/top-no7.png www/user/img_re/top-no7.png
cp develop/www/user/img_re/top-no8.png www/user/img_re/top-no8.png
cp develop/www/user/img_re/top-no9.png www/user/img_re/top-no9.png
cp develop/www/user/img_re/top-rank.png www/user/img_re/top-rank.png
cp develop/www/user/img_re/year-month-2.png www/user/img_re/year-month-2.png
cp develop/www/user/img_re/year-month.jpg www/user/img_re/year-month.jpg
cp develop/www/user/js/uranai.js www/user/js/uranai.js
cp develop/www/user/img_re/whatnew_201703_1.jpg www/user/img_re/whatnew_201703_1.jpg
cp develop/www/user/img_re/whatnew_201703_2.jpg www/user/img_re/whatnew_201703_2.jpg
cp develop/www/user/img_re/whatnew_201703_3.jpg www/user/img_re/whatnew_201703_3.jpg
