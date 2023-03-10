<?PHP
//	商品詳細登録

	include "../../cone.inc";

function goods_12() {
	global $main,$goods;
	include './array.inc';

	if ($_POST['mode']) { define(MODE, $_POST['mode']); }
	if ($_POST['list_num']) { define(LIST_NUM, $_POST['list_num']); }

	if (MODE == "update") {
		update($ERROR);
	} elseif (MODE == "削除") {
		delete($ERROR);
	}

	if (!$ERROR && MODE == "update") {
		detaile_html($ERROR);
	} elseif (!$ERROR && MODE == "削除") {
		default_html($ERROR);
	} elseif (LIST_NUM > 0) {
		detaile_html($ERROR);
	} else {
		default_html($ERROR);
	}

}



//	一覧画面
function default_html($ERROR) {
	global $PHP_SELF,$main,$goods,$view,$views,$view_s,$view_e,$page,$max,$VIEW_NUM;
	global $maker, $bunrui, $s_goods;	//	add ookawara 2014/02/07
	include './array.inc';

	$html = "";

	//	add ookawara 2014/02/07 start
	$where = "";
	//	メーカー
	if ($maker > 0) {
		$where .= " AND list.maker_num='".$maker."'";
	}
	//	分類
	if ($bunrui > 0) {
		$where .= " AND list.class_m='".$bunrui."'";
	}
	//	商品番号＆JANコード
	$goods_jan_num = trim($_POST['goods_jan_num']);
	if ($goods_jan_num != "") {
		if ($goods_jan_num <= "2147483647") {
			$where .= " AND (list.list_num='".$goods_jan_num."' OR list.pluid='".$goods_jan_num."')";
		} else {
			$where .= " AND list.pluid='".$goods_jan_num."'";
		}
	}
	//	add ookawara 2014/02/07 end


	//	該当商品数
	$count = 0;
	$sql  = "SELECT count(list.*) AS count FROM list list".
			" LEFT JOIN goods goods ON goods.pluid=list.pluid".
			" LEFT JOIN (SELECT maker_num, maker_name FROM maker GROUP BY maker_num, maker_name) maker_list ON maker_list.maker_num=list.maker_num".
			" WHERE goods.pluid IS NULL".
			$where.	//	add ookawara 2014/02/07
			" AND list.state='0';";
	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$count = $list['count'];
	}

	if ($count < 1) {
		echo "該当する商品は有りません。";
		return;
	}


	//	分類リスト作成
	$CLASS_L = array();
	$CLASS_NAME = array();
	$sql  = "SELECT * FROM class".
			" ORDER BY class_m;";
	if ($result = pg_query(DB, $sql)) {
		while ($list = pg_fetch_array($result)) {
			$class_l_ = $list[class_l];
			$class_m_ = $list[class_m];
			$class_m_n_ = $list[class_m_n];
			$CLASS_L[$class_m_] = $class_l_;
			$CLASS_NAME[$class_m_] = $class_m_n_;
		}
	}

	$views = $VIEW_NUM[$view];
	$page_all = ceil($count / $views);
	if (!$page) { $page = 1; }
	$start = $views * ($page-1);
	$end = $views;
	$limit = " OFFSET ".$start." LIMIT ".$end;
	$max = $count - 1;
	$view_s = $views * ($page-1);
	$view_e = ($views * $page)-1;
	if ($view_e > $max) { $view_e = $max; }

	echo <<<EOT
<form action='{$PHP_SELF}' method='POST'>
<input type="hidden" name="mode" value="syou">
<input type="hidden" name="main" value="{$main}">
<input type="hidden" name="goods" value="{$goods}">
<input type="hidden" name="s_goods" value="{$s_goods}">
<input type="hidden" name="view" value="{$view}">
<input type="hidden" name="main_r" value="{$main}">
<input type="hidden" name="goods_r" value="{$goods}">
<input type="hidden" name="page" value="{$page}">
JANコードを変更、または削除したい商品の番号をクリックして下さい。<BR>
該当商品数：{$count}件 Page : {$page}/{$page_all} <BR>
<table border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
<tr bgcolor="#cccccc">
<th>商品番号</th>
<th>JANコード</th>
<th>名前</th>
<th>メーカー名</th>
<th>カラー</th>
<th>サイズ</th>
<th>分類</th>
</tr>

EOT;

	$sql  = "SELECT".
			" list.list_num,".
			" list.pluid,".
			" list.goods_name,".
			" list.size,".
			" list.color,".
			" list.maker_num,".
			" list.class_m,".
			" maker_list.maker_name".
			" FROM list list".
			" LEFT JOIN goods goods ON goods.pluid=list.pluid".
			" LEFT JOIN (SELECT maker_num, maker_name FROM maker GROUP BY maker_num, maker_name) maker_list ON maker_list.maker_num=list.maker_num".
			" WHERE goods.pluid IS NULL".
			$where.	//	add ookawara 2014/02/07
			" AND list.state='0'".
			" ORDER BY list_num DESC".
			$limit.";";
	if ($result = pg_query(DB, $sql)) {
		WHILE ($list = pg_fetch_array($result)) {
			$list_num = $list['list_num'];
			$pluid = $list['pluid'];
			$goods_name = $list['goods_name'];
			$size = $list['size'];
			if (!$size) { $size = "---"; }
			$color = $list['color'];
			if (!$color) { $color = "---"; }
			$maker_num = $list['maker_num'];
			$class_name = "";
			$class_m = $list['class_m'];
			$class_l = $CLASS_L[$class_m];
			$class_name = $CLASS_N[$class_l]."：".$CLASS_NAME[$class_m];
			$maker_name = $list['maker_name'];

			echo "<tr bgcolor='#ffffff'>\n";
			echo "<td><input type='submit' name='list_num' value='".$list_num."'></td>\n";
			echo "<td>".$pluid."</td>\n";
			echo "<td>".$goods_name."</td>\n";
			echo "<td>".$maker_name."</td>\n";
			echo "<td align='center'>".$color."</td>\n";
			echo "<td align='center'>".$size."</td>\n";
			echo "<td>".$class_name."</td>\n";
			echo "</tr>\n";

		}
	}

	echo <<<EOT
</table>
</form>

EOT;

	//ページ処理
	if ($max > $view_e || $view_s >= $views) { next_p_g_1(); }

	return;
}



//	詳細登録画面
function detaile_html() {
	global $PHP_SELF,$main,$goods,$view,$views,$view_s,$view_e,$page,$max,$VIEW_NUM;
	include './array.inc';

	if (!$_POST['list_num']) {
		$ERROR[] = "JANコードを変更、または削除したい商品の番号が確認出来ません。";
		ERROR(&$ERROR);
	}

	//	詳細データーあるかチェック
	$list_num = "";
	$sql  = "SELECT * FROM list".
			" WHERE list_num='".$_POST['list_num']."'".
			" AND state='0'".
			" LIMIT 1;";
	if ($result = pg_query(DB, $sql)) {
		$list = pg_fetch_array($result);
		$list_num	= $list['list_num'];
		$pluid		= $list['pluid'];
		$goods_name	= $list['goods_name'];
##		$goods_name	= ereg_replace("#","＃",$goods_name);
		$goods_name	= preg_replace("/#/","＃",$goods_name);
		$comment	= nl2br($list['comment']);
		$size		= $list['size'];
		$color		= $list['color'];
		$picture	= $list['picture'];
		$maker_num	= $list['maker_num'];
		$class_m	= $list['class_m'];
		$display	= $list['display'];
		$c_link		= $list['c_link'];
	}

	//	メーカー番号取得
	if ($maker_num) {
		$sql  = "SELECT maker_name FROM maker".
				" WHERE maker_num='".$maker_num."'".
				" LIMIT 1;";
		if ($result = pg_query(DB, $sql)) {
			$list = pg_fetch_array($result);
			$maker_name = $list['maker_name'];
		}
	}

	if ($picture) { $pic_file = "../pic/$picture"; }
	$dis1 = "";
	$dis2 = "";
	if ($display == 0 || $display == 1)  { $display_msg = "表示する"; } else { $display_msg = "表示しない"; }


	echo <<<EOT
	<FORM action='{$PHP_SELF}' method="POST" enctype='multipart/form-data'>
	<INPUT type="hidden" name="mode" value="update">
	<INPUT type="hidden" name="main" value="{$main}">
	<INPUT type="hidden" name="goods" value="{$goods}">
	<INPUT type='hidden' name='view' value='{$view}'>
	<INPUT type='hidden' name='list_num' value='{$list_num}'>
	<INPUT type="hidden" name="main_r" value="{$main}">
	<INPUT type="hidden" name="goods_r" value="{$goods}">
	<INPUT type="hidden" name="page" value="{$page}">
	<INPUT type="hidden" name="list_num" value="{$list_num}">

EOT;

 	echo <<<EOT
JANコードを修正し変更ボタンを押してください。<br>
削除する場合は、削除ボタンをクリックしてください。<input type='submit' name='mode' value='削除' /><br>
     <TABLE border='0' cellpadding='3' cellspacing='2' bgcolor='#999999' width='600px'>
        <TBODY>
          <TR bgcolor='#ffffff'>
            <TD>商品番号</TD>
            <TD>{$list_num}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>JANコード</TD>
            <TD><input type='text' size='30' name='pluid' value='$pluid' /></TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD width='100px'>商品名</TD>
            <TD>{$goods_name}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>メーカー</TD>
            <TD>{$maker_name}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>画　像</TD>
            <TD>

EOT;

		if (file_exists($pic_file)) {
			echo <<<EOT
            <P align='center'><IMG src='{$pic_file}' border='0'></P>

EOT;
		}

		echo <<<EOT
            </TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>コメント</TD>
            <TD>{$comment}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>カラー</TD>
            <TD>{$color}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>サイズ</TD>
            <TD>{$size}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>分類</TD>
            <TD>

EOT;

		$sql  = "SELECT class_m, class_m_n FROM class".
				" WHERE class_m='$class_m';";
		if ($result = pg_query(DB, $sql)) {
			$list = pg_fetch_array($result);
			$class_m_ = $list[class_m];
			$class_m_n_ = $list[class_m_n];
			echo "$class_m_n_($class_m_)\n";
		}

		echo <<<EOT
            </TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>商品一覧表示</TD>
            <TD>{$display_msg}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD>カラー画像LINK</TD>
            <TD>{$c_link}</TD>
          </TR>
          <TR bgcolor='#ffffff'>
            <TD colspan='2'><INPUT type='submit' value='変更'></TD>
          </TR>
        </TBODY>
      </TABLE>
      </FORM>
		<FORM action="{$PHP_SELF}" method="POST">
			<INPUT type="hidden" name="main" value="{$main}">
			<INPUT type="hidden" name="goods" value="{$goods}">
			<INPUT type="hidden" name="view" value="{$view}">
			<INPUT type="hidden" name="main_r" value="{$main}">
			<INPUT type="hidden" name="goods_r" value="{$goods}">
			<INPUT type="hidden" name="page" value="{$page}">
			<INPUT type="submit" value="戻る">
		</FORM>

EOT;


	pg_close(DB);

}


//	更新
function update($ERROR) {

	if (!$_POST['list_num']) {
		$ERROR[] = "JANコードを変更したい商品の番号が確認出来ません。";
	}

	$pluid = $_POST['pluid'];
	## $pluid = mb_convert_kana($pluid, 'as', 'EUC-JP');
	$pluid = mb_convert_kana($pluid, 'as', 'UTF-8');
	$pluid = trim($pluid);
	if (!$pluid) {
		$ERROR[] = "JANコードが確認出来ません。";
	}

	if ($ERROR) {
		ERROR(&$ERROR);
		return;
	}

	$sql = "UPDATE list SET" .
			" pluid='".$pluid."',".
			" update_time=now()" .
			" WHERE list_num=".$_POST['list_num'].";";
	if (pg_exec(DB, $sql)) {
		echo "JANコード変更完了致しました。<br>\n<br>\n";
		return;
	} else {
		echo "JANコード変更出来ませんでした。<br>\n<br>\n";
		return;
	}

	pg_close(DB);

}


//	削除
function delete($ERROR) {

	if (!$_POST['list_num']) {
		$ERROR[] = "削除したい商品の番号が確認出来ません。";
		ERROR(&$ERROR);
		return;
	}

	$sql = "UPDATE list SET" .
			" state='1',".
			" update_time=now()" .
			" WHERE list_num=".$_POST['list_num'].";";
	if (pg_exec(DB, $sql)) {
		echo "削除完了致しました。<br>\n<br>\n";
		return;
	} else {
		echo "削除完了出来ませんでした。<br>\n<br>\n";
		return;
	}

	pg_close(DB);

}
?>
