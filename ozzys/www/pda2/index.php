<?PHP
/*

	ozzys様
	PDA商品管理システム

*/

	//	設定ファイル読込
	define(SET_DIR,"./sub/");
	include_once(SET_DIR."main_lib.php");
	include_once(SET_DIR."main_array.php");

	//	DB接続
	if (defined("DBHOST")) { $DB_LIST["DBHOST"] = DBHOST; }
	if (defined("DBUSER")) { $DB_LIST["DBUSER"] = DBUSER; }
	if (defined("DBNAME")) { $DB_LIST["DBNAME"] = DBNAME; }
	if (defined("DBPASS")) { $DB_LIST["DBPASS"] = DBPASS; }
	$DB_LIST["ENCODE"] = DB_ENCODE;
	if (connect_db(&$DB_LIST)) {
		//	エラーが有った場合障害ページに飛ばす
		$url = "/";
//		header ("Location: $url\n\n");
//		exit;
	}
	define("DB",$DB_LIST["DB"]);

	//	基本サブルーチン

	//	文字コード整頓
	set_request(DISPLAY_ENCODE);	//	0:元の文字コード設定

	//	ページ作成
	switch (MODE) {
		case "search";	//	商品検索
			include_once(SET_DIR.SEARCH_FILE);
			$html = make_html();
			break;
		case "check";	//	確認商品
			include_once(SET_DIR.CHECK_FILE);
			$html = make_html();
			break;
		default;	//	TOPページ
			include_once(SET_DIR.INDEX_FILE);
			$html = make_html();
			break;
	}

	//	表示変換
	$html = change_code_html($html,DISPLAY_ENCODE);		//	0:変換する文字	1:変換後の文字コード

	//	表示
	output_html($html);

	//	DB切断
	if (defined("DB")) { close_db(DB); }

	exit;

?>