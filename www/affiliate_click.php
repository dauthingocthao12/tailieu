<?php
/**
 *======================================================================
 * 広告をクリックする時にクリック数を更新するスクリプト
 *======================================================================
 * @author Azet
 * @date 2016-02-19
 */

require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/config.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/common.php");
require_once(dirname(__FILE__) . "/../uranai_lib/libadmin/ad.php");

$ad_id = $_GET['ad_id'];
if($ad_id) {
	$ok = Ad::countClick($ad_id);
	print ($ok)?'OK':'ERR';
}
else {
	print "ERR: ad_id missing!";
}
