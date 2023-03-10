<?PHP
//	商品csv作成
function goods_10() {
global $mode;

	if ($mode == "make") { list($msg,$ERROR) = make_csv(); }
	elseif ($mode == "delete") { list($msg,$ERROR) = delete_csv(); }

	first_00($msg,$ERROR);

}


//	詳細一覧
function first_00($msg,$ERROR) {
include './array.inc';
global $PHP_SELF,$main,$goods;

	if ($ERROR) {
		$errors = "<br>\n<b><font color=\"#ff0000\">エラー</font></b><br>\n";
		foreach ($ERROR AS $VAL) {
			$errors .= "・" . $VAL . "<br>\n";
		}
	}


	echo <<<WAKABA
$errors
$msg
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="main" value="$main">
<input type="hidden" name="main_r" value="$main">
<input type="hidden" name="goods" value="$goods">
<input type="hidden" name="goods_r" value="$goods">
<input type="hidden" name="mode" value="make">
新規のファイルを作成する場合は、csv作成ボタンを押して下さい。<br>
<input type="submit" value="csv作成">
</form>

WAKABA;

	//	フォルダー内ファイル取得
	$DIR = "csv";
	list($LIST,$ERROR) = read_dir($DIR);

	if ($LIST) {
		foreach ($LIST AS $VAL) {
			if (!$VAL) { continue; }
			$url = "./csv/$VAL";

			$l_file_name .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td><input type="checkbox" name="file_names[]" value="$VAL"></td>
      <td><a href="$url" target="_blank">$VAL</a></td>
    </tr>

WAKABA;
		}
	}

	if ($l_file_name) {
	echo <<<WAKABA
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="main" value="$main">
<input type="hidden" name="main_r" value="$main">
<input type="hidden" name="goods" value="$goods">
<input type="hidden" name="goods_r" value="$goods">
<input type="hidden" name="mode" value="delete">
ファイル名をクリックし保存して閲覧して下さい。<br>
ファイルを削除する場合はチェックを入れ削除ボタンを押して下さい。<br>
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td></td>
      <td>ファイル名</td>
    </tr>
$l_file_name
    <tr bgcolor="#ffffff" align="center">
      <td colspan="2"><input type="submit" value="削除"></td>
    </tr>
  </tbody>
</table>
</form>

WAKABA;

	}
	else {
		echo <<<WAKABA
<br>
今現在作成されたファイルは有りません。<br>

WAKABA;
	}

}



//	csv作成
function make_csv() {
include './array.inc';
include "../../cone.inc";

	//	詳細取得
//	$sql  = "SELECT * FROM list;";	//	2009/04/25	del ookawara
	//	2009/04/25	add ookawara
	$sql  = "SELECT * FROM list WHERE state!='1';";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$list_num_ = $list['list_num'];
			$pluid_ = $list['pluid'];
			$display_ = $list['display'];

			$DETALES[$pluid_][list_num] = $list_num_;
			$DETALES[$pluid_][display] = $display_;
		}
	}


	$csv = "\"PLUID\",\"商品番号\",\"商品名\",\"中分類コード\",\"在庫数\",\"詳細登録\",\"表示・非表示\",\n";

	//	商品情報
//	$sql  = "SELECT * FROM goods ORDER BY maker_id, class_m, pluid;";	//	2009/04/25	del ookawara
	$sql  = "SELECT * FROM goods ORDER BY maker, class_m, pluid;";	//	2009/04/25	add ookawara
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$pluid_ = $list['pluid'];
			$goods_name_ = $list['goods_name'];
			$stock_ = $list['stock'];
			$class_m_ = $list['class_m'];

			$list_num_ = $DETALES[$pluid_][list_num];
			$display_ = $DETALES[$pluid_][display];

			$syousai_ = "登録済み";
			$display_msg = "表示";
			if (!$list_num_) {
				$syousai_ = "未登録";
				$display_msg = "-----";
			}
			elseif ($display_ == 1) {
				$display_msg = "非表示";
			}

			$csv .= "\"$pluid_\",\"$list_num_\",\"$goods_name_\",\"$class_m_\",\"$stock_\",\"$syousai_\",\"$display_msg\",\n";
		}
	}

	//	s-jis変換
	## $csv = mb_convert_encoding($csv,"SJIS","EUC-JP");
	$csv = mb_convert_encoding($csv,"SJIS","UTF-8");

	$dates = date("Y") . date("m") . date("d") . date("H") . date("i") . date("s");

	$file = "./csv/goods_$dates.csv";
	$OUT = fopen($file,"w");
	fwrite($OUT,$csv);
	fclose($OUT);
	@chmod($file,0666);

	$msg = "<br>\n<b>csv(goods_$dates.csv)を作成完了しました。</b><br>\n";

	pg_close($conn_id);

	return array($msg,$ERROR);

}




//	ファイル削除
function delete_csv() {

	$FILE_NAMES = $_POST['file_names'];
	if (!$FILE_NAMES) { $ERROR[] = "削除するファイルが選択されておりません。"; }

	if (!$ERROR && $FILE_NAMES) {
		foreach ($FILE_NAMES AS $VAL) {
			$file = "./csv/$VAL";
			if (file_exists($file)) {
				unlink($file);
			}
		}
	}

	if (!$ERROR) {
		$msg = "<br>\n<b>ファイル削除完了しました。</b><br>\n";
	}

	return array($msg,$ERROR);

}



//	フォルダー内ファイル取得
function read_dir($DIR) {

	if (!file_exists($DIR)) { $ERROR = "ファイルを取得するフォルダーが見つかりません。"; }

	if (!$ERROR) {
		$strDir = opendir($DIR);
		while($strFle=readdir($strDir)) {
##			if (ereg("^[.]",$strFle)) { continue; }
			if (preg_match("/^[.]/",$strFle)) { continue; }
			$LIST[] = $strFle;
		}
	}

	if (!$LIST) { $ERROR = "フォルダー内にファイルが存在しません。"; }

	return array($LIST,$ERROR);

}
?>
