#!/bin/bash

echo "Moving pictures in root..."
mkdir www/pic_del_front

mv www/160417shinsai.jpg www/pic_del_front/
mv www/201.jpg www/pic_del_front/
mv www/21.gif www/pic_del_front/
mv www/210.jpg www/pic_del_front/
mv www/26ozzyscup.jpg www/pic_del_front/
mv www/5ji.jpg www/pic_del_front/
mv www/5ji1.jpg www/pic_del_front/
mv www/8.gif www/pic_del_front/
mv www/angry_worm.jpg www/pic_del_front/
mv www/auction_banar.jpg www/pic_del_front/
mv www/bar2.jpg www/pic_del_front/
mv www/bar_01.gif www/pic_del_front/
mv www/blog.jpg www/pic_del_front/
mv www/card.jpg www/pic_del_front/
mv www/cardinal3.jpg www/pic_del_front/
mv www/corect.jpg www/pic_del_front/
mv www/d_incite.jpg www/pic_del_front/
mv www/double_scat.jpg www/pic_del_front/
mv www/eigyouyotei.jpg www/pic_del_front/
mv www/engine.jpg www/pic_del_front/
mv www/extan9.jpg www/pic_del_front/
mv www/gogo_3ji.jpg www/pic_del_front/
mv www/hari-02.gif www/pic_del_front/
mv www/hidari_ue.gif www/pic_del_front/
mv www/hosi_01.gif www/pic_del_front/
mv www/husen_01.gif www/pic_del_front/
mv www/i-new.gif www/pic_del_front/
mv www/image.jpg www/pic_del_front/
mv www/in_store.jpg www/pic_del_front/
mv www/in_store_now.gif www/pic_del_front/
mv www/kado2.gif www/pic_del_front/
mv www/kanban.jpg www/pic_del_front/
mv www/keitai12.jpg www/pic_del_front/
mv www/korekuto.jpg www/pic_del_front/
mv www/mail-bin.jpg www/pic_del_front/
mv www/mail.jpg www/pic_del_front/
mv www/mail_bin.jpg www/pic_del_front/
mv www/man_naka.gif www/pic_del_front/
mv www/migi_ue.gif www/pic_del_front/
mv www/motegi.jpg www/pic_del_front/
mv www/motegi2.jpg www/pic_del_front/
mv www/nenmatunenshi2015.jpg www/pic_del_front/
mv www/neos.jpg www/pic_del_front/
mv www/new_prop.jpg www/pic_del_front/
mv www/new_swingbait.jpg www/pic_del_front/
mv www/novebor.jpg www/pic_del_front/
mv www/november.jpg www/pic_del_front/
mv www/octobor.jpg www/pic_del_front/
mv www/panicra.jpg www/pic_del_front/
mv www/pick_up.jpg www/pic_del_front/
mv www/pickup_item.gif www/pic_del_front/
mv www/point.jpg www/pic_del_front/
mv www/pop_x_last.jpg www/pic_del_front/
mv www/popx.jpg www/pic_del_front/
mv www/qr_code.jpg www/pic_del_front/
mv www/rarenium.jpg www/pic_del_front/
mv www/rigge.jpg www/pic_del_front/
mv www/ringi2ai.jpg www/pic_del_front/
mv www/rinigi.jpg www/pic_del_front/
mv www/sakana.gif www/pic_del_front/
mv www/sale_dec.jpg www/pic_del_front/
mv www/semi2008.jpg www/pic_del_front/
mv www/september.jpg www/pic_del_front/
mv www/service.jpg www/pic_del_front/
mv www/service3.jpg www/pic_del_front/
mv www/shad5.jpg www/pic_del_front/
mv www/shinchaku.jpg www/pic_del_front/
mv www/shinshun.jpg www/pic_del_front/
mv www/shinsyun-2015.jpg www/pic_del_front/
mv www/shopping_site.jpg www/pic_del_front/
mv www/shukka.jpg www/pic_del_front/
mv www/shut_01.gif www/pic_del_front/
mv www/sokujitsu.jpg www/pic_del_front/
mv www/sokujitsu2.jpg www/pic_del_front/
mv www/sokutatsu.jpg www/pic_del_front/
mv www/spellbound.jpg www/pic_del_front/
mv www/supreame_xt.jpg www/pic_del_front/
mv www/suspend_minnow4.jpg www/pic_del_front/
mv www/suspend_minnow5.jpg www/pic_del_front/
mv www/swing_bait.jpg www/pic_del_front/
mv www/swing_bait2009.jpg www/pic_del_front/
mv www/swingbait.jpg www/pic_del_front/
mv www/top_2010.jpg www/pic_del_front/
mv www/top_2011.jpg www/pic_del_front/
mv www/trout_rod_special.jpg www/pic_del_front/
mv www/twitter.jpg www/pic_del_front/
mv www/vizsla.jpg www/pic_del_front/
mv www/w-jisinn-1.jpg www/pic_del_front/
mv www/w-jisinn-2.jpg www/pic_del_front/
mv www/w-teiden.jpg www/pic_del_front/
mv www/web_shop.gif www/pic_del_front/
mv www/weblog.jpg www/pic_del_front/
mv www/wed999.jpg www/pic_del_front/
mv www/zaiko.jpg www/pic_del_front/

mv www/160731PMX.jpg www/pic_del_front/
mv www/2012wakeari.jpg www/pic_del_front/
mv www/arrow_red_right5.gif www/pic_del_front/
mv www/banner2.gif www/pic_del_front/
mv www/new.gif www/pic_del_front/
mv www/new_cr_code.png www/pic_del_front/
mv www/ozzyswebshop.jpg www/pic_del_front/
mv www/store.jpg www/pic_del_front/
mv www/weblog2.jpg www/pic_del_front/

echo "Done."


echo "Deleting useless files in root..."

rm www/index.ht_
rm www/log_data/menu.20160708.inc
rm www/sub/cart_new.20160801inc
rm www/sub/cart_new2.inc
rm www/sub/goodsname.20160801.inc
rm www/sub/goodsname2.inc
rm www/sub/setup.inc.20160711
rm www/sub/syousai.20160801.inc
rm www/sub/syousai2.inc
rm www/whats_new.ht_
rm www/whats_new.htm
rm www/zzz/readme.txt
rm www/zzz/test.php
# remove folder as well
rm -r www/zzz
rm -rf www/i
rm -rf www/photo
rm -rf www/semi


echo "Done."
