<?PHP
//	ログインページ

	$title = "ログイン";

	include("../cone.inc");
	include("./sub/setup.inc");
	include("./sub/array.inc");
	include("./sub/login.inc");

//	session_cache_limiter('nocache');
	session_start();
//	session_register("idpass","customer","blurl","emailpassmemo","ERROR");

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

	//	メイン内容
	$main = login();
	$html = str_replace("<!--MAIN-->",$main,$html);

	//	ページタイトル
	$html = str_replace("<!--TITLE-->",$title,$html);

	//	ヘッド背景画
	include("./sub/headimg.inc");
	$headimg = headimg($HEADIMG);
	$html = str_replace("<!--HEADIMG-->",$headimg,$html);

	//	ヘッドメッセージ
	include("./sub/headmsg.inc");
	$headmsg = headmsg($LOGDATA_DIR,$headmsg_file);
	$html = str_replace("<!--HEADMSG-->",$headmsg,$html);

	//	ログインメッセージ
	include("./sub/loginmsg_resp.inc");
	$burl = "";
	$loginmsg = loginmsg($burl);
	$html = str_replace("<!--LOGIN-->",$loginmsg,$html);

	//	メニュー
	include("./sub/menu.inc");
	$menu = menu($LOGDATA_DIR,$menu_file_resp);
	$html = str_replace("<!--MENU-->",$menu,$html);	//	メニュー

	//	お勧め
	include("./sub/osusume.inc");
	$LIST = "";
	$osusume = osusume($LIST, true);
	$html = str_replace("<!--OSUSUME-->",$osusume,$html);


	echo("$html");

	if ($db) { pg_close($db); }

	exit;

?>
