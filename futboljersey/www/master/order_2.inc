<?PHP
//	請求書管理
function order_2() {
global $conn_id;
global $main,$order,$mode; //_POST
include "../../cone.inc";

	$mode = $_POST['mode'];
	$mode = stripslashes($mode);

	if ($mode == "delete") { list($msg,$ERROR) = delete(); }

	$html = first_00($main,$order,$mode,$msg,$ERROR);


	echo($html);

	// mysqli_close($conn_id);


}


function first_00($main,$order,$mode,$msg,$ERROR) {

	if ($ERROR) {
		ERROR($ERROR);
	}
	else {
		$html .= <<<WAKABA
<br>
$msg

WAKABA;
	}

	//	新規登録部分
	$html .= <<<WAKABA
<table border="0">
  <tbody>
    <tr>
      <td>新規登録する場合は、新規登録ボタンを押してください。</td>
      <td>
      <form action="bill.php" method="GET" target="bill">
      <input type="hidden" name="mode" value="yahoo">
      <input type="submit" value="Yahoo新規作成">
      </td></form>
      <td>
      <form action="bill.php" method="GET" target="bill">
      <input type="hidden" name="mode" value="rakuten">
      <input type="submit" value="楽天新規作成">
      </td></form>
      <td>
      <form action="bill.php" method="GET" target="bill">
      <input type="submit" value="新規作成">
      </td></form>
    </tr>
  </tbody>
</table>
<br>

WAKABA;

	//	検索部分
	$html .= search($main,$order,$mode);

	//	リスト部分
	$html .= bill_list($main,$order);

	return $html;

}



//	検索部分
function search($main,$order,$mode) {
global $PHP_SELF,$SITE_TYPE_L;
	$page_num = $_POST['page_num'];
	if ($mode == "リセット") {
		unset($_SESSION['SEARCH']);
		$page_num = 1;
	}
	elseif ($mode == "表示") {
		$s_order_num = $_POST['s_order_num'];
		$s_site_type = $_POST['s_site_type'];
		$s_bill_num = $_POST['s_bill_num'];
		$s_name = $_POST['s_name'];
		$view_num = $_POST['view_num'];
		$page_num = 1;

#		$s_order_num = mb_convert_kana($s_order_num,"ns","EUC-JP");
		$s_order_num = mb_convert_kana($s_order_num,"ns","UTF-8");
		$s_order_num = trim($s_order_num);
#		$s_bill_num = mb_convert_kana($s_bill_num,"ns","EUC-JP");
		$s_bill_num = mb_convert_kana($s_bill_num,"ns","UTF-8");
		$s_bill_num = trim($s_bill_num);
#		$s_name = mb_convert_kana($s_name,"asKV","EUC-JP");
		$s_name = mb_convert_kana($s_name,"asKV","UTF-8");
		$s_name = trim($s_name);

		$_SESSION['SEARCH']['s_order_num'] = $s_order_num;
		$_SESSION['SEARCH']['s_site_type'] = $s_site_type;
		$_SESSION['SEARCH']['s_bill_num'] = $s_bill_num;
		$_SESSION['SEARCH']['s_name'] = $s_name;
		$_SESSION['SEARCH']['view_num'] = $view_num;
		$_SESSION['SEARCH']['page_num'] = $page_num;
	}
	elseif ($_SESSION['SEARCH'] || $page_num > 0) {
		$s_order_num = $_SESSION['SEARCH']['s_order_num'];
		$s_site_type = $_SESSION['SEARCH']['s_site_type'];
		$s_bill_num = $_SESSION['SEARCH']['s_bill_num'];
		$s_name = $_SESSION['SEARCH']['s_name'];
		$view_num = $_SESSION['SEARCH']['view_num'];
		if (!$page_num) {
			$page_num = $_SESSION['SEARCH']['page_num'];
		}
		else {
			$_SESSION['SEARCH']['page_num'] = $page_num;
		}
	}

	//	ショップ
	if (!$s_site_type) { $selected = "selected"; } else { $selected = ""; }
	$l_s_site_type = "        <option value=\"\" $selected>全て</option>\n";
	$max = count($SITE_TYPE_L);
	for($i=1; $i<$max; $i++) {
		if ($i == $s_site_type) { $selected = "selected"; } else { $selected = ""; }
		$l_s_site_type .= "        <option value=\"$i\" $selected>$SITE_TYPE_L[$i]</option>\n";
	}

	//	表示数
	if (!$view_num) { $view_num = 1; }
	for($i=1; $i<=10; $i++) {
		if ($i == $view_num) { $selected = "selected"; } else { $selected = ""; }
		$ii = $i * 10;
		$l_view_num .= "        <option value=\"$i\" $selected>$ii</option>\n";
	}

	$html = <<<WAKABA
<form action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="order" value="$order">
<INPUT type="hidden" name="order_r" value="$order">
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>注文番号</td>
      <td>サイト</td>
      <td>請求書番号</td>
      <td>お名前</td>
      <td>表示数</td>
      <td></td>
    </tr>
    <tr bgcolor="#ffffff">
      <td><input size="30" type="text" name="s_order_num" value="$s_order_num"></td>
      <td><select name="s_site_type">
$l_s_site_type
      </select></td>
      <td><input size="12" type="text" name="s_bill_num" value="$s_bill_num"></td>
      <td><input size="20" type="text" name="s_name" value="$s_name"></td>
      <td><select name="view_num">
$l_view_num
      </select></td>
      <td><input type="submit" name="mode" value="表示">　<input type="submit" name="mode" value="リセット"></td>
    </tr>
  </tbody>
</table>
</form>

WAKABA;

	return $html;

}



//	リスト一覧
function bill_list($main,$order) {
global $PHP_SELF,$conn_id,$SITE_TYPE_L;

	if ($_SESSION['SEARCH']) {
		$s_order_num = $_SESSION['SEARCH']['s_order_num'];
		$s_site_type = $_SESSION['SEARCH']['s_site_type'];
		$s_bill_num = $_SESSION['SEARCH']['s_bill_num'];
		$s_name = $_SESSION['SEARCH']['s_name'];
		$view_num = $_SESSION['SEARCH']['view_num'];
		$page_num = $_SESSION['SEARCH']['page_num'];
	}

	if (!$page_num) { $page_num = 1; }
	if (!$view_num) { $view_num = 1; }

	if ($s_order_num) {
		$where .= " AND order_num='$s_order_num'";
	}
	if ($s_site_type) {
		$where .= " AND site_type='$s_site_type'";
	}
	if ($s_bill_num) {
		$where .= " AND bill_num='$s_bill_num'";
	}
	if ($s_name) {
		$where .= " AND name like '%$s_name%'";
	}

	$sql  = "SELECT count(*) as count FROM bill_base WHERE state='0' $where;";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$count = $list['count'];
	}

	if ($count > 0) {
		$views = $view_num * 10;
		$s = $views * ($page_num - 1) + 1;
		$f = $views * $page_num;
		if ($count < $f) { $f = $count; }
		$max_page = ceil($count/$views);
		$hyouji_msg = $count . "件中 " .  $s . " - " .  $f . "件";
		if ($max_page > 1) { $hyouji_msg .= " $page_num/$max_page" . "Page"; }

		$html = <<<WAKABA
<br>
表示・変更する場合は表示・変更ボタンを、削除する場合は削除ボタンを押してください。<br>
削除は確認なしで削除されます。<br>
該当数：$hyouji_msg<br>
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>注文番号</td>
      <td>サイト</td>
      <td>日付</td>
      <td>お名前</td>
      <td>住所</td>
      <td>請求書番号</td>
      <td>表示</td>
      <td>削除</td>
    </tr>

WAKABA;

		$offset = ($page_num - 1) * $views;
		$limit_max = $offset + $views;
		if ($count < $limit_max) {
			$limit = $count % $views;
		}
		else {
			$limit = $views;
		}
		if ($offset > 0) {
			$offset1 = " OFFSET $offset";
		}
		$limit_num = "  LIMIT $limit $offset1";

		$sql = "SELECT * FROM bill_base" .
				" WHERE state='0' $where ORDER BY update_date DESC $limit_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			WHILE ($list = mysqli_fetch_array($result)) {
				$order_num = $list['order_num'];
				$site_type = $list['site_type'];
				$bill_date = $list['bill_date'];
				$name = $list['name'];
				$add1 = $list['add1'];
				$add2 = $list['add2'];
				$bill_num = $list['bill_num'];

				$bill_num_ = sprintf("%06d",$bill_num);

				$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td>$order_num</td>
      <td>$SITE_TYPE_L[$site_type]</td>
      <td>$bill_date</td>
      <td>$name</td>
      <td>$add1<br>
      $add2</td>
      <td align="center">$bill_num_</td>
      <td>
      <form action="bill.php" method="POST" target="bill">
      <input type="hidden" name="bill_num" value="$bill_num">
      <input type="submit" value="表示・変更">
     </td></form>
      <td>
      <form action="$PHP_SELF" method="POST">
      <input type="hidden" name="main" value="$main">
      <input type="hidden" name="main_r" value="$main">
      <input type="hidden" name="order" value="$order">
      <input type="hidden" name="order_r" value="$order">
      <input type="hidden" name="mode" value="delete">
      <input type="hidden" name="bill_num" value="$bill_num">
      <input type="submit" value="削除">
     </td></form>
    </tr>

WAKABA;
			}
		}

		$html .= <<<WAKABA
  </tbody>
</table>
</form>

WAKABA;

		//	ページ処理
		$s = ($page_num-1) * $views + 1;
		$e = $page_num * $views;
		if ($count < $e) { $e = $count; }
		if ($page_num >= 2 || $page_num < $max_page) {
			$html .= <<<WAKABA
<TABLE border="0">
  <TBODY>
    <TR>

WAKABA;
			if ($page_num >= 2) {
				$b_page = $page_num - 1;
				$html .= <<<WAKABA
      <TD align="center">
      <FORM action="$PHP_SELF" method="POST">
      <input type="hidden" name="main" value="$main">
      <input type="hidden" name="main_r" value="$main">
      <input type="hidden" name="order" value="$order">
      <input type="hidden" name="order_r" value="$order">
      <input type="hidden" name="page_num" value="$b_page">
      <input type="submit" value="前の $views 件">
      </TD></FORM>

WAKABA;
			}

			if ($page_num < $max_page) {
				$n_page = $page_num + 1;
				$n_view = $count - $e;
				if ($n_view > $views) { $n_view = $views; }
				$html .= <<<WAKABA
      <TD align="center">
      <FORM action="$PHP_SELF" method="POST">
      <input type="hidden" name="main" value="$main">
      <input type="hidden" name="main_r" value="$main">
      <input type="hidden" name="order" value="$order">
      <input type="hidden" name="order_r" value="$order">
      <input type="hidden" name="page_num" value="$n_page">
      <input type="submit" value="次の $n_view 件">
      </TD></FORM>

WAKABA;
			}

			$html .= <<<WAKABA
    </TR>
  </TBODY>
</TABLE>
<BR>

WAKABA;

		}
	}
	else {
		$html = <<<WAKABA
登録されている請求書はございません。<br>

WAKABA;
	}

	return $html;

}



//	削除処理
function delete() {
global $conn_id;

	$bill_num = $_POST['bill_num'];
	if (!$bill_num) { $ERROR[] = "請求書番号が確認出来ません。"; }

	if (!$ERROR) {
		$sql  = "UPDATE bill_base SET state='2', update_date=now() WHERE bill_num='$bill_num';";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "基本情報を削除出来ませんでした。"; }
	}
	if (!$ERROR) {
		$sql  = "UPDATE bill_list SET state='2' WHERE bill_num='$bill_num' AND state='0';";
		if (!$result = mysqli_query($conn_id,$sql)) { $ERROR[] = "詳細情報を削除出来ませんでした。"; }
	}

	if (!$ERROR) {
		$msg = "請求書番号：{$bill_num}を削除しました。<br>\n";
	}

	return array($msg,$ERROR);

}
?>
