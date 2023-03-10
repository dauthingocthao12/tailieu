<?PHP
/*
 * 注意 (2017-01-17)
 *
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 * 同時に修正されるファイル：
 *  - menulist.php
 *  - menulist_resp.php
 * ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 */

//	メニュー MENU

	include("./sub/setup.inc");
	include("./sub/menu.inc");

	$menu = menu($LOGDATA_DIR,$menu_file);

	echo($menu);

	exit;

?>
