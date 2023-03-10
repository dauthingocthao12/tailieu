<?PHP
/*

	ozzys様
	PDA商品管理システム

	DB接続関連

*/

//	DB接続
function connect_db(&$DB_LIST) {

	//	DB接続
	$connect  = "host=".$DB_LIST["DBHOST"].
				" dbname=".$DB_LIST["DBNAME"].
				" user=".$DB_LIST["DBUSER"].
				" password=".$DB_LIST["DBPASS"];
	if (!$db = pg_connect($connect)) {
		return 1;
	}

	//	文字コード設定
	$sql = "SET NAMES '".$DB_LIST["ENCODE"]."';";
	if (!pg_query($db,$sql)) {
		return 3;
	}

	//	接続リソース
	$DB_LIST["DB"] = $db;

}

//	DB切断
function close_db($db) {

	pg_close($db);

}
?>