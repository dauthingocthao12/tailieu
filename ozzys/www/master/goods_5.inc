<?PHP
//	メーカー登録

	//	テスト設定
	define("SET_TEST","");

	//	メーカーテーブル
	define("TABLE_MAKER",SET_TEST."maker");

	//	商品詳細登録テーブル
	define('TABLE_LIST','list');

	//	db接続
	include "../../cone.inc";
	define("conn_id",$conn_id);

	//	配列読込
	require_once("./array.inc");





function goods_5() {

	unset($ERROR);

	//	処理
	if ($_POST['mode'] == "renew") {
		renew(&$ERROR);
	} elseif ($_POST['mode'] == "chage_maker") {
		chage_maker(&$ERROR);
	} elseif ($_POST['mode'] == "regist") {
		regist_maker(&$ERROR);
	} elseif ($_POST['mode'] == "delete") {
		delete_maker(&$ERROR);
	}

	//	表示
	if ($_POST["maker_num"]) {
		$html = details($ERROR);
	} else {
		$html = ichiran();
	}

	if (conn_id) { pg_close(conn_id); }

	echo $html;

}


//	一覧表示
function ichiran() {
	global $PHP_SELF;

	$html = "";
	$count = 0;

	if ($_POST['mode'] == "delete") {
		$html .= "<b>メーカー名：".$_POST['pos_maker_name']."の削除処理が完了いたしました。</b><br>\n<br>\n";
	}

	$sql  = "select maker_num from ".TABLE_MAKER.
			" GROUP BY maker_num;";
	$result = pg_exec(conn_id,$sql);
	$count = pg_numrows($result);
	$html .= "登録メーカー数：".$count."件<br>\n";
	$html .= "<br>\n";

	if ($count >= 1) {
		$html .= "編集するメーカーの番号をクリックして下さい。<br>\n";

		$html .= "<FORM action='".$PHP_SELF."' method='POST'>\n";
		$html .= "<INPUT type='hidden' name='main' value='".$_POST["main"]."'>\n";
		$html .= "<INPUT type='hidden' name='goods' value='".$_POST["goods"]."'>\n";
		$html .= "<TABLE border='0' cellpadding='3' cellspacing='2' bgcolor='#999999'>\n";
		$html .= "<TR bgcolor='#cccccc'>\n";
		$html .= "<TD>番号</TD>\n";
		$html .= "<TD>メーカー名</TD>\n";
		$html .= "<TD>メーカーURL</TD>\n";
		$html .= "<TD>ポス登録メーカー名(参考)</TD>\n";
		$html .= "</TR>\n";

		$max = $count-1;
		for ($i=0; $i<=$max; $i++) {
			list ($maker_num) = pg_fetch_array($result,$i);
			$sql2 = "select maker_name, maker_url,maker from ".TABLE_MAKER.
					" where maker_num='".$maker_num."';";
			$result2 = pg_exec(conn_id,$sql2);
			$count2 = pg_numrows($result2);
			$max2 = $count2 - 1;
			unset($makers[$maker_num]);
			for ($ii=0; $ii<=$max2; $ii++) {
				list ($maker_name, $maker_url, $maker) = pg_fetch_array($result2,$ii);
				$makers[$maker_num][$maker] = $maker;
			}
			$html .= "<TR bgcolor='#ffffff'>\n";
			$html .= "<TD><INPUT type='submit' name='maker_num' value='".$maker_num."'></TD>\n";
			$html .= "<TD>".$maker_name."</TD>\n";
			if ($maker_url) {
				$html .= "<TD align='center'><A href='".$maker_url."' target='_blank'>".$maker_url."</A></TD>\n";
			} else {
				$html .= "<TD align='center'>---</TD>\n";
			}

			$makers_name_list = "";
			if ($makers[$maker_num]) {
				foreach ($makers[$maker_num] AS $val) {
					$makers_name_list .= $val."<br>\n";
				}
			} else {
				$makers_name_list = "---";
			}
			$html .= "<TD>".$makers_name_list."</TD>\n";
			$html .= "</TR>\n";
		}

		$html .= "</TABLE>\n";
		$html .= "</FORM>\n";

	} else {
		$html .= "現在メーカーは登録されておりません。<BR>\n";
	}

	$mode = "";

	return $html;
}



//	編集画面
function details($ERROR) {
global $PHP_SELF;

//global $main,$goods,$PHP_SELF,$maker_num,$maker_name,$maker_id,$check,$num_h,$makers,$mode;

	$maker_num = $_POST['maker_num'];

	//	メーカー情報取得
	$sql  = "SELECT * FROM ".TABLE_MAKER.
			" WHERE maker_num='".$maker_num."';";
	$result = pg_exec(conn_id,$sql);
	$count = pg_numrows($result);
	$max = $count - 1;
	unset($makers[$maker_num]);
	while ($list=pg_fetch_array($result, NULL, PGSQL_ASSOC)) {
		foreach ($list AS $key => $val) {
			$$key = $val;
			if ($key == "maker") {
				$makers[$maker_num][$val] = $val;
			}
		}
	}

/*
	for ($i=0; $i<=$max; $i++) {
		list ($maker_num, $maker_name, $maker_url, $maker) = pg_fetch_array($result,$i);
		$makers[$maker_num][$maker] = $maker;
	}
*/

	$html = "";
	if ($ERROR) {
		if ($_POST['mode'] == "renew") {
			$maker_name = $_POST["maker_name"];
			$maker_url = $_POST["maker_url"];
		}
		$html .= "<font color=\"#ff0000\"><b>エラー</b></font><br>\n";
		foreach ($ERROR AS $val) {
			$html .= "・".$val."<br>\n";
		}
	} elseif ($_POST['mode'] == "renew") {
		$html .= <<<EOT
<BR>
<b>メーカー情報の変更処理が、完了しました。</b><br>

EOT;
	} elseif ($_POST['mode'] == "chage_maker") {
		$html .= <<<EOT
<BR>
<b>所属メーカーの変更処理が、完了しました。</b><br>

EOT;
	} elseif ($_POST['mode'] == "regist") {
		$html .= <<<EOT
<BR>
<b>新規メーカーの登録処理が、完了しました。</b><br>

EOT;
	}

	$html .= <<<EOT
<BR>
<FORM action='{$PHP_SELF}' method='POST'>
<INPUT type='hidden' name='main' value='{$_POST["main"]}'>
<INPUT type='hidden' name='goods' value='{$_POST["goods"]}'>
<INPUT type='hidden' name='mode' value='renew'>
<INPUT type='hidden' name='maker_num' value='{$maker_num}'>
編集し変更をおしてください。<BR>
<TABLE border="0" bgcolor="#999999" cellpadding="3" cellspacing="2">
    <TR>
      <TD bgcolor="#cccccc" align="center" width="120">メーカー番号</TD>
      <TD bgcolor="#ffffff" width="50" align="center">{$maker_num}</TD>
      <TD bgcolor="#cccccc" width="120" align="center">メーカー名(全角)</TD>
      <TD bgcolor="#ffffff">
       <INPUT type="text" size="30" name="maker_name" value="{$maker_name}">
      </TD>
    </TR>
    <TR>
      <TD bgcolor="#cccccc" align="center">メーカーURL(半角)</TD>
      <TD colspan="3" bgcolor="#ffffff">
       <INPUT type="text" size="50" name="maker_url" value="{$maker_url}">
      </TD>
    </TR>
    <TR>
      <TD colspan="4" bgcolor="#cccccc" align="center">
       <INPUT type="submit" value="変更"> <INPUT type="reset">
      </TD>
    </TR>
</TABLE>
</FORM>
<br>
<FORM action='{$PHP_SELF}' method='POST'>
<INPUT type='hidden' name='main' value='{$_POST["main"]}'>
<INPUT type='hidden' name='goods' value='{$_POST["goods"]}'>
<INPUT type='hidden' name='mode' value='renew'>
<INPUT type='hidden' name='maker_num' value='{$maker_num}'>
所属メーカー変更<br>
登録しているメーカーが違っている場合、変更するメーカーを選択して変更ボタンを押してください。<br>
ただし商品詳細で変更元の商品がある場合は先に変更先のメーカーに変更してから処理を行ってください。<br>
<TABLE border="0" bgcolor="#999999" cellpadding="3" cellspacing="2">
    <TR align="center" bgcolor="#cccccc">
      <TD>ポスデーター登録名</TD>
      <TD>変更先メーカー</TD>
      <TD>新規メーカー登録</TD>
      <TD>削除</TD>
    </TR>
EOT;

	//	全メーカー名取得
	$select_list = "";
	if (!$_POST['chage_maker_num']) { $_POST['chage_maker_num'] = $maker_num; }
	$sql  = "SELECT maker_num, maker_name FROM ".TABLE_MAKER.
			" GROUP BY maker_num, maker_name ORDER BY maker_name;";
	$result = pg_exec(conn_id,$sql);
	while ($list=pg_fetch_array($result, NULL, PGSQL_ASSOC)) {
		$selected = "";
		if ($list['maker_num'] == $_POST['chage_maker_num']) { $selected = "selected"; }
		$select_list .= "<option value='".$list['maker_num']."' $selected>".$list['maker_num'].":".$list['maker_name']."</option>\n";
	}
	if ($select_list) {
		$select_list = "<select name='chage_maker_num'>\n".
						$select_list.
						"</select>\n";
	}

	$makers_name_list = "";
	if ($makers[$maker_num]) {
		$count = count($makers[$maker_num]);
		foreach ($makers[$maker_num] AS $key => $val) {
			$html .= "<TR bgcolor='#ffffff'>\n";
			$html .= "<FORM action='".$PHP_SELF."' method='POST'>\n";
			$html .= "<INPUT type='hidden' name='main' value='".$_POST["main"]."'>\n";
			$html .= "<INPUT type='hidden' name='goods' value='".$_POST["goods"]."'>\n";
			$html .= "<INPUT type='hidden' name='mode' value='chage_maker'>\n";
			$html .= "<INPUT type='hidden' name='maker_num' value='".$maker_num."'>\n";
			$html .= "<INPUT type='hidden' name='pos_maker_name' value='".$val."'>\n";
			$html .= "<INPUT type='hidden' name='maker_count' value='".$count."'>\n";
			$html .= "<td>".$val."</td>\n";
			$html .= "<td>\n".$select_list." <input type='submit' value='変更'></td>\n";
			$html .= "</FORM>\n";
			$html .= "<FORM action='".$PHP_SELF."' method='POST'>\n";
			$html .= "<INPUT type='hidden' name='main' value='".$_POST["main"]."'>\n";
			$html .= "<INPUT type='hidden' name='goods' value='".$_POST["goods"]."'>\n";
			$html .= "<INPUT type='hidden' name='mode' value='regist'>\n";
			$html .= "<INPUT type='hidden' name='maker_num' value='".$maker_num."'>\n";
			$html .= "<INPUT type='hidden' name='pos_maker_name' value='".$val."'>\n";
			$html .= "<INPUT type='hidden' name='maker_count' value='".$count."'>\n";
			if ($count == 1) {
				$html .= "<td>---</td>\n";
			} else {
				$html .= "<td><input type='submit' value='新規メーカー登録'></td>\n";
			}
			$html .= "</FORM>\n";
			$html .= "<FORM action='".$PHP_SELF."' method='POST'>\n";
			$html .= "<INPUT type='hidden' name='main' value='".$_POST["main"]."'>\n";
			$html .= "<INPUT type='hidden' name='goods' value='".$_POST["goods"]."'>\n";
			$html .= "<INPUT type='hidden' name='mode' value='delete'>\n";
			$html .= "<INPUT type='hidden' name='maker_num' value='".$maker_num."'>\n";
			$html .= "<INPUT type='hidden' name='pos_maker_name' value='".$val."'>\n";
			$html .= "<INPUT type='hidden' name='maker_count' value='".$count."'>\n";
			$html .= "<td><input type='submit' value='メーカー削除'></td>\n";
			$html .= "</FORM>\n";
			$html .= "</TR>\n";
		}
	}
	$html .= <<<EOT
</TABLE>
</FORM>
<FORM action='{$PHP_SELF}' method='POST'>
<INPUT type='hidden' name='main' value='{$_POST["main"]}'>
<INPUT type='hidden' name='goods' value='{$_POST["goods"]}'>
<INPUT type="submit" value="戻る">
</FORM>
EOT;

	return $html;
}


//	変更処理
function renew(&$ERROR) {

	if (!$_POST['maker_num']) { $ERROR[] = "変更のメーカー情報が確認できません。"; }
	if (!$_POST['maker_name']) { $ERROR[] = "メーカー名が入力されておりません。"; }
//	if (!$_POST['maker_url']) { $ERROR[] = "メーカーURLが入力されておりません。"; }
	if ($ERROR) { return; }

	$sql  = "UPDATE ".TABLE_MAKER." SET" .
			" maker_name='".$_POST['maker_name']."' ," .
			" maker_url='".$_POST['maker_url']."'" .
			" where maker_num=".$_POST['maker_num'].";";
	if (!pg_exec(conn_id,$sql)) {
		$ERROR[] = "メーカー情報変更処理ができませんでした。";
	}

	return;
}


//	所属メーカー変更処理
function chage_maker(&$ERROR) {

	if (!$_POST['maker_num']) { $ERROR[] = "変更元のメーカー情報が確認できません。"; }
	if (!$_POST['pos_maker_name']) { $ERROR[] = "変更元のポスデーター登録名情報が確認できません。"; }
	if (!$_POST['chage_maker_num']) { $ERROR[] = "変更先のメーカーが選択されておりません。"; }
	if (!$_POST['maker_count']) { $ERROR[] = "変更元のメーカー情報（数）が確認できません。"; }
	if ($_POST['maker_num'] == $_POST['chage_maker_num']) { $ERROR[] = "変更元と変更先のメーカーが一緒です。"; }
	if (!$ERROR) {
		$count = 0;
		$sql  = "SELECT count(*) AS count FROM ".TABLE_LIST.
				" WHERE maker_num='".$_POST['maker_num']."';";
		if ($result = pg_exec(conn_id,$sql)) {
			$list = pg_fetch_array($result, NULL, PGSQL_ASSOC);
			$count = $list['count'];
		}
//		if ($count > 0) { $ERROR[] = "異動元のメーカに商品詳細が登録されております。（".$count."商品）"; }
	}
	if ($ERROR) { return; }

	//	変更先のメーカー情報取得
	$sql  = "SELECT maker_name, maker_url FROM ".TABLE_MAKER.
			" WHERE maker_num='".$_POST['chage_maker_num']."'".
			" LIMIT 1;";
	if ($result = pg_exec(conn_id,$sql)) {
		$list = pg_fetch_array($result, NULL, PGSQL_ASSOC);
		$maker_name = $list['maker_name'];
		$maker_url = $list['maker_url'];
	}

	$sql  = "UPDATE ".TABLE_MAKER." SET" .
			" maker_num='".$_POST['chage_maker_num']."' ," .
			" maker_name='".$maker_name."' ," .
			" maker_url='".$maker_url."'" .
			" where maker_num='".$_POST['maker_num']."'".
			" AND maker='".$_POST['pos_maker_name']."';";
	if (!pg_exec(conn_id,$sql)) {
		$ERROR[] = "メーカー変更処理ができませんでした。";
	} else {
		//	商品データーメーカー番号変更
		//	add ookawara 2009/12/24
		$sql  = "UPDATE ".TABLE_LIST." SET".
				" maker_num='".$_POST['chage_maker_num']."'".
				" WHERE maker_num='".$_POST['maker_num']."';";
		if (!pg_exec(conn_id,$sql)) {
			$ERROR[] = "商品詳細メーカー変更処理ができませんでした。";
		} else {
			$_POST['maker_num'] = $_POST['chage_maker_num'];
		}

//		$_POST['maker_num'] = $_POST['chage_maker_num'];	// del ookawara 2009/12/24
	}

	return;
}


//	新規メーカー登録処理
function regist_maker(&$ERROR) {

	if (!$_POST['maker_num']) { $ERROR[] = "変更元のメーカー情報が確認できません。"; }
	if (!$_POST['pos_maker_name']) { $ERROR[] = "変更元のポスデーター登録名情報が確認できません。"; }
	if (!$_POST['maker_count']) { $ERROR[] = "変更元のメーカー情報（数）が確認できません。"; }
	if ($ERROR) { return; }

	$max_maker_num = 1;
	$sql  = "SELECT MAX(maker_num) AS max FROM ".TABLE_MAKER.";";
	if ($result = pg_exec(conn_id,$sql)) {
		$list = pg_fetch_array($result, NULL, PGSQL_ASSOC);
		$max_maker_num = $list['max'] + 1;
	}

	$sql  = "UPDATE ".TABLE_MAKER." SET" .
			" maker_num='".$max_maker_num."' ," .
			" maker_name='".$_POST['pos_maker_name']."' ," .
			" maker_url=''" .
			" where maker_num='".$_POST['maker_num']."'".
			" AND maker='".$_POST['pos_maker_name']."';";
	if (!pg_exec(conn_id,$sql)) {
		$ERROR[] = "新規メーカー登録処理ができませんでした。";
	} else {
		$_POST['maker_num'] = $max_maker_num;
	}

	return;
}


//	メーカー削除処理
function delete_maker(&$ERROR) {

	if (!$_POST['maker_num']) { $ERROR[] = "変更元のメーカー情報が確認できません。"; }
	if (!$_POST['pos_maker_name']) { $ERROR[] = "変更元のポスデーター登録名情報が確認できません。"; }
	if (!$_POST['maker_count']) { $ERROR[] = "変更元のメーカー情報（数）が確認できません。"; }
	if (!$ERROR && $_POST['maker_count'] == 1) {
		$count = 0;
		$sql  = "SELECT count(*) AS count FROM ".TABLE_LIST.
				" WHERE maker_num='".$_POST['maker_num']."';";
		if ($result = pg_exec(conn_id,$sql)) {
			$list = pg_fetch_array($result, NULL, PGSQL_ASSOC);
			$count = $list['count'];
		}
		if ($count > 0) { $ERROR[] = "商品詳細が登録されておりますので削除できません。（".$count."商品）"; }
	}
	if ($ERROR) { return; }

	$sql  = "DELETE FROM ".TABLE_MAKER.
			" WHERE maker_num='".$_POST['maker_num']."'".
			" AND maker='".$_POST['pos_maker_name']."';";
	if (!pg_exec(conn_id,$sql)) {
		$ERROR[] = "メーカー削除処理ができませんでした。";
	} else {
		unset($_POST["maker_num"]);
	}

	return;
}
?>
