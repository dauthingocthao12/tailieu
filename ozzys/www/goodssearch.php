<?PHP
//	検索表示プログラム

	$title = "商品検索";

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/array.inc");
	include("./sub/goodssearch.inc");

//	session_cache_limiter('nocache');
	session_start();
//	session_register("idpass","customer");

	$PHP_SELF = $_SERVER['PHP_SELF'];

	//	ひな形読込
	$html = "";
	if (file_exists($hina_file)) {
		$fp = fopen ($hina_file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}

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
	include("./sub/loginmsg_resp.inc");
	$burl = "";
	$loginmsg = loginmsg($burl);
##	$html = eregi_replace("<!--LOGIN-->",$loginmsg,$html);
	$html = preg_replace("/<!--LOGIN-->/i",$loginmsg,$html);

	//	メニュー
	include("./sub/menu.inc");
	$menu = menu($LOGDATA_DIR,$menu_file_resp);
##	$html = eregi_replace("<!--MENU-->",$menu,$html);	//	メニュー
	$html = preg_replace("/<!--MENU-->/i",$menu,$html);	//	メニュー

	//	メイン内容
	$main = goodssearch($word);
##	$html = eregi_replace("<!--MAIN-->",$main,$html);
	$html = preg_replace("/<!--MAIN-->/i",$main,$html);

	//	お勧め
	include("./sub/osusume.inc");
	$LIST = "";
	$osusume = osusume($LIST, true);
##	$html = eregi_replace("<!--OSUSUME-->",$osusume,$html);
	$html = preg_replace("/<!--OSUSUME-->/i",$osusume,$html);


	echo("$html");

	if ($db) { pg_close($db); }

	exit;

?>
