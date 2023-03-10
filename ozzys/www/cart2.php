<?PHP
//	会員登録・変更

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/array.inc");
	include("./sub/cart_new2.inc");
	include("./sub/mail.inc");
	include("./sub/souryou.inc");

//	session_cache_limiter('public');
	session_start();
//	session_register("idpass","customer","b_url","id","check_list","ERROR");
##	session_register("customer");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ひな形読込
	$html = "";
	if (file_exists("$hina_file")) {
		$fp = fopen ("$hina_file", "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}

	//	メイン内容
	list($main,$title) = cart();
##	$html = eregi_replace("<!--MAIN-->",$main,$html);
	$html = preg_replace("/<!--MAIN-->/i",$main,$html);
	$title .= " オジーズ";

	//	ページタイトル
##	$html = eregi_replace("<!--TITLE-->",$title,$html);
	$html = preg_replace("/<!--TITLE-->/i",$title,$html);

	//	ヘッド背景画
	include("./sub/headimg.inc");
	$headimg = headimg($HEADIMG);
##	$html = eregi_replace("<!--HEADIMG-->",$headimg,$html);
	$html = preg_replace("/<!--HEADIMG-->/i",$headimg,$html);

	//	ヘッドメッセージ
	include("./sub/headmsg.inc");
	$headmsg = headmsg($LOGDATA_DIR,$headmsg_file);
##	$html = eregi_replace("<!--HEADMSG-->",$headmsg,$html);
	$html = preg_replace("/<!--HEADMSG-->/i",$headmsg,$html);

	//	ログインメッセージ
	include("./sub/loginmsg.inc");
	$burl = "";
	$loginmsg = loginmsg($burl);
##	$html = eregi_replace("<!--LOGIN-->",$loginmsg,$html);
	$html = preg_replace("/<!--LOGIN-->/i",$loginmsg,$html);

	//	メニュー
	include("./sub/menu.inc");
	$menu = menu($LOGDATA_DIR,$menu_file);
##	$html = eregi_replace("<!--MENU-->",$menu,$html);	//	メニュー
	$html = preg_replace("/<!--MENU-->/i",$menu,$html);	//	メニュー

	//	お勧め
	include("./sub/osusume.inc");
	$LIST = "";
	$osusume = osusume($LIST);
##	$html = eregi_replace("<!--OSUSUME-->",$osusume,$html);
	$html = preg_replace("/<!--OSUSUME-->/i",$osusume,$html);


	echo("$html");

	if ($db) { pg_close($db); }

/*
$session_id = session_id();
echo("<font color=\"#ffffff\">$session_id</font><br>\n");
*/

	exit;

?>
