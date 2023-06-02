<?PHP
//	アフェリエイト管理
//　ポイント修正
function aff_2() {
global $PHP_SELF,$conn_id;
include "../../cone.inc";

	$ap_num = $_POST['ap_num'];

	if ($ap_num) {
		detaile($ap_num);
	}
	else {
		main_list();
	}

}



//	一覧
function main_list() {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$main_r = $_POST['main_r'];
	$aff = $_POST['aff'];
	$aff_r = $_POST['aff_r'];
	$aff_number = $_POST['aff_number'];
#	$aff_number = mb_convert_kana($aff_number,"n","EUC-JP");
	$aff_number = mb_convert_kana($aff_number,"n","UTF-8");
	$aff_number = trim($aff_number);
	$sells_number = $_POST['sells_number'];
#	$sells_number = mb_convert_kana($sells_number,"n","EUC-JP");
	$sells_number = mb_convert_kana($sells_number,"n","UTF-8");
	$sells_number = trim($sells_number);
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];
	if (!$view) { $view = 1; }
	if (!$page) { $page = 1; }

	unset($where);
	if ($aff_number) { $where .= " AND ap.af_num='$aff_number'"; }
	if ($sells_number) { $where .= " AND ap.sells_num='$sells_number'"; }
	if ($hlist == "" || $hlist == 1) { $DESC = "DESC"; }

	//	件数
	$sql  = "SELECT distinct ap.ap_num FROM $appoint_table ap, $sells_table c, $member_table a" .
			" WHERE ap.sells_num=c.sells_num AND a.kojin_num=c.kojin_num AND c.send='1' $where;";
	if ($result = mysqli_query($conn_id,$sql)) {
		$max = mysqli_num_rows($result);
	}

	if ($max > 0) {
		$search_view = 20 * $view;
		$max_page = ceil($max/$search_view);
		$s = $search_view * ($page - 1);
		$e = ($search_view * $page) - 1;
		if ($e >= $max) { $e = $max - 1; }
		$view_s = $s + 1;
		$view_e = $e+1;

		$page_msg = $max . "件条件に当てはまる商品がありました。 ";
		if ($max_page > 1) { $page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "Page"; }

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="sells_number" value="$sells_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
情報を変更したい処理番号をクリックして下さい。<br />
※発送処理が完了してない場合は一覧には表示されません。<br />
該当数:{$max}件　{$page}/{$max_page} PAGE
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>処理番号</td>
      <td>注文番号</td>
      <td>注文者名</td>
      <td>ポイント</td>
      <td>&nbsp;率&nbsp;</td>
      <td>注文日</td>
      <td>発送日</td>
      <td>アフェリエイター番号</td>
    </tr>

WAKABA;

		$offset = ($page - 1) * $search_view;
		$limit_max = $offset + $search_view;
		if ($max < $limit_max) {
			$limit = $max % $search_view;
		}
		else {
			$limit = $search_view;
		}
		$limit_num = " LIMIT $limit";
		if ($offset != 0) {
			$limit_num .= " OFFSET $offset";
		}

		$sql  = "SELECT distinct ap.ap_num, ap.sells_num, a.name_s, a.name_n," .
				" ap.point, ap.p_ritu, ap.order_day, ap.send_day, ap.af_num" .
				" FROM $appoint_table ap, $sells_table c, $member_table a" .
				" WHERE ap.sells_num=c.sells_num AND a.kojin_num=c.kojin_num AND c.send='1' $where" .
				" ORDER BY ap.order_day $DESC, ap.send_day $DESC $limit_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$ap_num = $list['ap_num'];
				$sells_num = $list['sells_num'];
				$name_s = $list['name_s'];
				$name_n = $list['name_n'];
				$point = $list['point'];
				$p_ritu = $list['p_ritu'];
				$order_day = $list['order_day'];
				$send_day = $list['send_day'];
				$af_num = $list['af_num'];

				$point = number_format($point);

				if (!$send_day) { $send_day = "----------"; }

				echo <<<WAKABA
    <tr bgcolor="#ffffff" align="center">
      <td><input type="submit" name="ap_num" value="$ap_num"></td>
      <td>$sells_num</td>
      <td>$name_s $name_n</td>
      <td align="right">{$point}pt</td>
      <td align="right">{$p_ritu}%</td>
      <td>$order_day</td>
      <td>$send_day</td>
      <td>$af_num</td>
    </tr>

WAKABA;

			}
		}

		echo <<<WAKABA
  </tbody>
</table>
</form>

WAKABA;


		//	ページ処理
		if ($max_page > 1) {

			echo <<<WAKABA
      <TABLE border="0">
        <TBODY>
          <TR bgcolor="#ffffff">

WAKABA;

			if ($page != 1) {
				$page_b = $page-1;

				echo <<<WAKABA
            <TD align="center">
            <form action="$PHP_SELF" method="POST">
            <input type="hidden" name="main" value="$main">
            <input type="hidden" name="main_r" value="$main">
            <input type="hidden" name="aff" value="$aff">
            <input type="hidden" name="aff_r" value="$aff">
            <input type="hidden" name="aff_number" value="$aff_number">
            <input type="hidden" name="sells_number" value="$sells_number">
            <input type="hidden" name="hlist" value="$hlist">
            <input type="hidden" name="view" value="$view">
            <input type="hidden" name="page" value="$page_b">
            <input type="submit" value="前の $search_view 件">
            </TD></form>

WAKABA;
			}

			if ($max_page != $page && $max_page != 1) {
				$page_n = $page + 1;
				$view_n = $max-($page*$search_view);
				if ($view_n > $view) { $view_n = $search_view; } else { $view_n = $view_n; }
				echo <<<WAKABA
            <TD align="center">
            <form action="$PHP_SELF" method="POST">
            <input type="hidden" name="main" value="$main">
            <input type="hidden" name="main_r" value="$main">
            <input type="hidden" name="aff" value="$aff">
            <input type="hidden" name="aff_r" value="$aff">
            <input type="hidden" name="aff_number" value="$aff_number">
            <input type="hidden" name="sells_number" value="$sells_number">
            <input type="hidden" name="hlist" value="$hlist">
            <input type="hidden" name="view" value="$view">
            <input type="hidden" name="page" value="$page_n">
            <input type="submit" value="次の $view_n 件">
            </TD></form>

WAKABA;
			}

			echo <<<WAKABA
　         </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

		}


	}
	else {
		echo<<<WAKABA
今現在登録されてる情報はありません。<br>

WAKABA;

	}

}



//	変更ページ
function detaile($ap_num) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$mode = $_POST['mode'];
	if ($mode == "change") { list($errors,$msg) = change($ap_num); }

	$main = $_POST['main'];
	$main_r = $_POST['main_r'];
	$aff = $_POST['aff'];
	$aff_r = $_POST['aff_r'];
	$aff_number = $_POST['aff_number'];
#	$aff_number = mb_convert_kana($aff_number,"n","EUC-JP");
	$aff_number = mb_convert_kana($aff_number,"n","UTF-8");
	$aff_number = trim($aff_number);
	$sells_number = $_POST['sells_number'];
#	$sells_number = mb_convert_kana($sells_number,"n","EUC-JP");
	$sells_number = mb_convert_kana($sells_number,"n","UTF-8");
	$sells_number = trim($sells_number);
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	//	詳細読み込み
	$sql  = "SELECT distinct ap.ap_num, ap.sells_num, a.name_s, a.name_n," .
			" ap.point, ap.p_ritu, ap.order_day, ap.send_day, ap.af_num" .
			" FROM $appoint_table ap, $sells_table c, $member_table a" .
			" WHERE ap.sells_num=c.sells_num AND a.kojin_num=c.kojin_num AND ap.ap_num='$ap_num'" .
			" LIMIT 1;";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$ap_num = $list['ap_num'];
		$sells_num = $list['sells_num'];
		$name_s = $list['name_s'];
		$name_n = $list['name_n'];
		$point = $list['point'];
		$p_ritu = $list['p_ritu'];
		$order_day = $list['order_day'];
		$send_day = $list['send_day'];
		$af_num = $list['af_num'];
	}

	$point = number_format($point);

	echo<<<WAKABA
$errors
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="sells_number" value="$sells_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="ap_num" value="$ap_num">
<INPUT type="hidden" name="p_ritu" value="$p_ritu">
<INPUT type="hidden" name="mode" value="change">
<table border="0" cellspacing="1" bgcolor="#666666">
  <tbody>
    <tr bgcolor="#cccccc" align="center">
      <td>処理番号</td>
      <td bgcolor="#ffffff">$ap_num</td>
      <td>アフェリエイター番号</td>
      <td bgcolor="#ffffff">$af_num</td>
   </tr>
    <tr bgcolor="#cccccc" align="center">
      <td>注文番号</td>
      <td bgcolor="#ffffff">$sells_num</td>
      <td>注文者名</td>
      <td bgcolor="#ffffff">$name_s $name_n</td>
    </tr>
    <tr bgcolor="#cccccc" align="center">
      <td>今現在のポイント</td>
      <td bgcolor="#ffffff">{$point}pt</td>
      <td>率</td>
      <td bgcolor="#ffffff">{$p_ritu}%</td>
    </tr>
    <tr bgcolor="#cccccc" align="center">
      <td>注文日</td>
      <td bgcolor="#ffffff">$order_day</td>
      <td>発送日</td>
      <td bgcolor="#ffffff">$send_day</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td colspan="4">変更後のポイントもしくは金額を入力して下さい。<br>
      ※金額を入力した場合は、金額が優先され自動的に計算されポイントに変換され登録されます。</td>
    </tr>
    <tr bgcolor="#cccccc" align="center">
      <td>ポイント</td>
      <td bgcolor="#ffffff"><input size="10" type="text" name="point" value="$point">pt</td>
      <td>金額</td>
      <td bgcolor="#ffffff"><input size="10" type="text" name="price" value="$price">円</td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td colspan="4"><input type="submit" value="変更"></td>
    </tr>
  </tbody>
</table>
</form>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="sells_number" value="$sells_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<input type="submit" value="一覧に戻る">
</form>

WAKABA;

}



//	変更処理
function change($ap_num) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$point = $_POST['point'];
#	$point = mb_convert_kana($point,"sn","EUC-JP");
	$point = mb_convert_kana($point,"sn","UTF-8");
	$point = trim($point);
	$price = $_POST['price'];
#	$price = mb_convert_kana($price,"sn","EUC-JP");
	$price = mb_convert_kana($price,"sn","UTF-8");
	$price = trim($price);
	$p_ritu = $_POST['p_ritu'];
#	$p_ritu = mb_convert_kana($p_ritu,"sn","EUC-JP");
	$p_ritu = mb_convert_kana($p_ritu,"sn","UTF-8");
	$p_ritu = trim($p_ritu);

	if ($point == "" && $price == "") { $ERROR[] = "変更後のポイント又は金額が入力されておりません。"; }

	if (!$ERROR) {
		if ($price) {
			$point = floor($price * $p_ritu / 100);
		}

		$point = (int)$point;

		$sql  = "UPDATE $appoint_table SET" .
				" point='$point'" .
				" WHERE ap_num='$ap_num';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "変更処理できませんでした。"; }
	}

	if ($ERROR) { $errors = ERROR($ERROR); }
	else { $msg = "<b>変更しました。</b><br>\n<br>\n"; }

	return array($errors,$msg);

}
?>
