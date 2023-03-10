<?PHP
//	データー更新
function goods_9() {
global $mode;

	if ($mode == "update")	{ $msg = update($msg); }
	first($msg);

}

//	商品詳細一覧
function first($msg) {
global $PHP_SELF,$main,$goods,$mode;

	if (!$mode) {
		echo <<<ALPHA
商品情報を更新をした場合必ず下記のキャッシュ削除を押して下さい。<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mode" value="update">
<INPUT type="submit" value="キャッシュ削除">
</FORM>

ALPHA;
	}
	else {
		echo("$msg<BR>\n");
	}
}

//	詳細削除記録
function update($msg) {
include "../../cone.inc";

	cash();
	$msg = "キャッシュデーターを削除しました。<BR>\n";

	return $msg;
}



//	キャッシュデーター削除
function cash() {

	$ST_DIR = "../log";
	$strDir = opendir($ST_DIR);
	while($ID1 = readdir($strDir)) {
##		if (eregi("\.htm",$ID1)) {
		if (preg_match("/\.htm/i",$ID1)) {
			$file = "$ST_DIR/$ID1";
			unlink($file);
		}
	}

}
?>
