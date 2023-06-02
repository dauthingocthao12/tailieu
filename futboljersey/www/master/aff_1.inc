<?PHP
//	アフェリエイト管理
//　ユーザー管理
function aff_1() {
global $PHP_SELF,$conn_id;
include "../../cone.inc";

	$af_num = $_POST['af_num'];

	//	詳細処理
	if ($af_num > 0) {
		detaile($af_num);
	}
	else {	//	一覧
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
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
#	$k_number = mb_convert_kana($k_number,"n","EUC-JP");
	$k_number = mb_convert_kana($k_number,"n","UTF-8");
	$k_number = trim($k_number);
	$aff_number = $_POST['aff_number'];
#	$aff_number = mb_convert_kana($aff_number,"n","EUC-JP");
	$aff_number = mb_convert_kana($aff_number,"n","UTF-8");
	$aff_number = trim($aff_number);
	$name_s_ = $_POST['name_s_'];
#	$name_s_ = mb_convert_kana($name_s_,"sKV","EUC-JP");
	$name_s_ = mb_convert_kana($name_s_,"sKV","UTF-8");
	$name_s_ = trim($name_s_);
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];
	if (!$view) { $view = 1; }
	if (!$page) { $page = 1; }
	unset($where);
	if ($affm != 2 && $affm != "") { $where .= " AND af.state='$affm'"; }
	if ($k_number) { $where .= " AND af.kojin_num='$k_number'"; }
	if ($aff_number) { $where .= " AND af.af_num='$aff_number'"; }
	if ($name_s_) { $where .= " AND a.name_s LIKE '$name_s_'"; }
	if ($hlist == "" || $hlist == 1) { $DESC = "DESC"; }

	//	件数
	$sql  = "SELECT * FROM $member_table a, $afuser_table af" .
			" WHERE af.kojin_num=a.kojin_num $where;";
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

		$page_msg = $max . "件条件に当てはまるユーザーがいました。 ";
		if ($max_page > 1) { $page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "Page"; }
		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
詳細を見たい場合、各ユーザーのアフェリエイター番号をクリックして下さい。<br />
該当者数:{$max}人　{$page}/{$max_page} PAGE
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>アフェリエイター番号</td>
      <td>会員番号</td>
      <td>名前</td>
      <td>登録状態</td>
      <td>確定予定ポイント</td>
      <td>確定ポイント</td>
      <td>最終GETポイント日</td>
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

		$sql  = "SELECT af.af_num, af.kojin_num, a.name_s, a.name_n, af.state FROM $member_table a, $afuser_table af" .
				" WHERE af.kojin_num=a.kojin_num $where" .
				" ORDER BY af.af_num $DESC $limit_num;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$af_num = $list['af_num'];
				$kojin_num = $list['kojin_num'];
				$name_s = $list['name_s'];
				$name_n = $list['name_n'];
				$state = $list['state'];

				list($indent_point_,$point_) = point_checks($af_num);
				$indent_point_ = number_format($indent_point_);
				$point_ = number_format($point_);

				$days = last_get_days($af_num);

				echo <<<WAKABA
    <tr bgcolor="#ffffff" align="center">
      <td><input type="submit" name="af_num" value="$af_num"></td>
      <td>$kojin_num</td>
      <td>$name_s $name_n</td>
      <td>{$AFRM_N[$state]}</td>
      <td align="right">{$indent_point_}pt</td>
      <td align="right">{$point_}pt</td>
      <td>{$days}</td>
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
            <input type="hidden" name="affm" value="$affm">
            <input type="hidden" name="k_number" value="$k_number">
            <input type="hidden" name="aff_number" value="$aff_number">
            <input type="hidden" name="name_s_" value="$name_s_">
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
            <input type="hidden" name="affm" value="$affm">
            <input type="hidden" name="k_number" value="$k_number">
            <input type="hidden" name="aff_number" value="$aff_number">
            <input type="hidden" name="name_s_" value="$name_s_">
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
今現在登録されてるユーザーはおりません。<br>

WAKABA;

	}

}



//	ポイント
function point_checks($af_num) {
global $conn_id,$afuser_table,$appoint_table,$application_table;
include '../sub/array.inc';
include '../sub/setup.inc';

	//	総ポイント
	$sql =  "SELECT SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state<='1';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$all_point = $list['all_point'];
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$dec_point = $list['dec_point'];
	}

	//	予定ポイント
	$indent_point = $all_point - $dec_point;
	if ($indent_point < 1) { $indent_point = 0; }

	//	支払い情報(全て)
	$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		(int)$pay_point = $list['pay_point'];
	}

	//	獲得確定ポイント
	$point = $dec_point - $pay_point;
	if ($point < 1) { $point = 0; }

	return array($indent_point,$point);

}



//	最終GETポイント日
function last_get_days($af_num) {
global $conn_id,$appoint_table;

	$sql  = "SELECT order_day FROM $appoint_table" .
			" WHERE af_num='$af_num' ORDER BY order_day DESC LIMIT 1;";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$days = $list['order_day'];
	}

	if (!$days) { $days = "----------"; }

	return $days;

}



//	詳細表示
//	ポイント履歴・アフェリエイトユーザー削除
function detaile($af_num) {
global $PHP_SELF;
	$mode = $_POST['mode'];
	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	if ($mode == "del_check") {	//	削除確認ページ
		del_check($af_num);
	}
	elseif ($mode == "del") {	//	削除処理
		del($af_num);
	}
	else {

		//	ユーザー情報
		user_date($af_num);

		if ($mode == "mons") {	//	月毎表示
			mons($af_num);
		}
		elseif ($mode == "days") {	//	日毎表示
			days($af_num);
		}
		else {	//	年毎表示
			years($af_num);
		}

	}

	echo <<<WAKABA
<br>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="submit" value="一覧に戻る">
</FORM>

WAKABA;

}



//	削除メッセージ
function user_date($af_num) {
global $conn_id,$member_table,$afuser_table;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	//	詳細情報読み込み
	$sql  = "SELECT af.af_num, af.kojin_num, a.name_s, a.name_n, af.state FROM $member_table a, $afuser_table af" .
			" WHERE af.kojin_num=a.kojin_num AND af.af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$af_num = $list['af_num'];
		$kojin_num = $list['kojin_num'];
		$name_s = $list['name_s'];
		$name_n = $list['name_n'];
		$state = $list['state'];

		list($indent_point_,$point_) = point_checks($af_num);
		$indent_point_ = number_format($indent_point_);
		$point_ = number_format($point_);

		$days = last_get_days($af_num);

		if ($state == 1) {
			$state_msg = "---";
		}
		else {
			$state_msg = "<input type=\"submit\" value=\"削除\">";
		}

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="del_check">
このユーザーを削除する場合は、削除ボタンを押して下さい。
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>アフェリエイター番号</td>
      <td>会員番号</td>
      <td>名前</td>
      <td>登録状態</td>
      <td>確定予定ポイント</td>
      <td>確定ポイント</td>
      <td>最終GETポイント日</td>
      <td>ユーザー削除</td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td>$af_num</td>
      <td>$kojin_num</td>
      <td>$name_s $name_n</td>
      <td>{$AFRM_N[$state]}</td>
      <td align="right">{$indent_point_}pt</td>
      <td align="right">{$point_}pt</td>
      <td>{$days}</td>
      <td>$state_msg</td>
    </tr>
  </tbody>
</table>
</form>

WAKABA;

	}
}



//	削除確認処理
function del_check($af_num) {
global $conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	//	詳細情報読み込み
	$sql  = "SELECT af.af_num, af.kojin_num, a.name_s, a.name_n, af.state FROM $member_table a, $afuser_table af" .
			" WHERE af.kojin_num=a.kojin_num AND af.af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$af_num = $list['af_num'];
		$kojin_num = $list['kojin_num'];
		$name_s = $list['name_s'];
		$name_n = $list['name_n'];
		$state = $list['state'];

		list($indent_point_,$point_) = point_checks($af_num);
		$indent_point_ = number_format($indent_point_);
		$point_ = number_format($point_);

		$days = last_get_days($af_num);

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="del">
本当にこのユーザーを削除してよろしいでしょうか？よければ、削除ボタンを押して下さい。
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>アフェリエイター番号</td>
      <td>会員番号</td>
      <td>名前</td>
      <td>登録状態</td>
      <td>確定予定ポイント</td>
      <td>確定ポイント</td>
      <td>最終GETポイント日</td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td>$af_num</td>
      <td>$kojin_num</td>
      <td>$name_s $name_n</td>
      <td>{$AFRM_N[$state]}</td>
      <td align="right">{$indent_point_}pt</td>
      <td align="right">{$point_}pt</td>
      <td>{$days}</td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td colspan="7"><input type="submit" value="削除"></td>
    </tr>
  </tbody>
</table>
</form>

WAKABA;

	}

	echo<<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<input type="submit" value="戻る">
</form>

WAKABA;

}



//	削除処理
function del($af_num) {
global $conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	//	削除処理
	$sql  = "UPDATE $afuser_table SET" .
			" state='1'" .
			" WHERE af_num='$af_num';";
	if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "削除処理できませんでした。"; }

	if (!$ERROR) {
		//	退会メール処理
		$sql  = "SELECT af.af_num, af.kojin_num, a.name_s, a.name_n, af.state, a.email, a.email2" .
				" FROM $member_table a, $afuser_table af" .
				" WHERE af.kojin_num=a.kojin_num AND af.af_num='$af_num';";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$af_num = $list['af_num'];
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$state = $list['state'];
			$email = $list['email'];
			$email2 = $list['email2'];
		}

//	メール配信処理







		echo <<<WAKABA
削除処理完了しました。<br>
<br>

WAKABA;
	}
	else {
		echo <<<WAKABA
<font color="#ff0000"><b>エラー</b></font><br>
・{$ERROR[0]}<br>
<br>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<input type="submit" value="戻る">
</form>

WAKABA;
	}

}



//	年毎表示
function years($af_num) {
global $conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	$first_year = $last_year = date("Y");

	//	クリック数
	$sql  = "SELECT YEAR(click_time) AS year, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num'" .
			" GROUP BY YEAR(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$click = $list['click'];
			$CLICK[$year] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT YEAR(order_day) AS year, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num'" .
			" GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$count = $list['count'];
			$COUNT[$year] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT YEAR(order_day) AS year, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$all_point = $list['all_point'];

			$ALL[$year] = $all_point;
			if ($first_year > $year) { $first_year = $year; }
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT YEAR(order_day) AS year, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day'" .
			" GROUP BY YEAR(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$dec_point = $list['dec_point'];

			$DEC[$year] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT YEAR(appli_day) AS year, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' GROUP BY YEAR(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$year = $list['year'];
			$pay_point = $list['pay_point'];

			$PAY[$year] = $pay_point;
		}
	}

	$html = "ポイント詳細（年毎）";

	if ($ALL) {
		$html .= <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="mons">
ポイント獲得履歴
<table border="0" cellspacing="1" bgcolor="#666666" width="600">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>年</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

		for($year=$last_year; $year>=$first_year; $year--) {
			//	クリック数
			$click = $CLICK[$year];
			if ($click < 1) { $click = 0; }

			//	売上件数
			$count = $COUNT[$year];
			if ($count < 1) { $count = 0; }

			//	獲得確定ポイント
			$point = $DEC[$year];
			if ($point < 1) { $point = 0; }

			//	獲得予定ポイント
			$indent_point = $ALL[$year] - $DEC[$year];

			//	変換ポイント
			$pay_point = $PAY[$year];

			//	有効ポイント
			$now_point = $point - $pay_point;
			if ($now_point < 1) { $now_point = 0; }

			$click_ = number_format($click);
			$count_ = number_format($count);
			$point_ = number_format($point);
			$indent_point_ = number_format($indent_point);
			$pay_point_ = number_format($pay_point);
			$now_point_ = number_format($now_point);

			$a_click += $click;
			$a_count += $count;
			$a_indent_point += $indent_point;
			$a_point += $point;
			$a_pay_point += $pay_point;
			$a_now_point += $now_point;

			$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center"><input type="submit" name="year" value="{$year}">年</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
		}

		$a_click = number_format($a_click);
		$a_count = number_format($a_count);
		$a_indent_point = number_format($a_indent_point);
		$a_point = number_format($a_point);
		$a_pay_point = number_format($a_pay_point);
		$a_now_point = number_format($a_now_point);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</table>
</form>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td>
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
  </tbody>
</table>

WAKABA;

	}

	echo($html);

}



//	月毎表示
function mons($af_num) {
global $conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];
	$year = $_POST['year'];

	$year = trim($_POST['year']);

	//	クリック数
	$sql  = "SELECT MONTH(click_time) AS mon, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num' AND YEAR(click_time)='$year'" .
			" GROUP BY MONTH(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$click = $list['click'];
			$CLICK[$mon] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT MONTH(order_day) AS mon, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$count = $list['count'];
			$COUNT[$mon] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT MONTH(order_day) AS mon, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$all_point = $list['all_point'];

			$ALL[$mon] = $all_point;
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT MONTH(order_day) AS mon, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1' AND YEAR(order_day)='$year'" .
			" AND send_day<'$order_day'" .
			" GROUP BY MONTH(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$dec_point = $list['dec_point'];

			$DEC[$mon] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT MONTH(appli_day) AS mon, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2' AND YEAR(appli_day)='$year'" .
			" GROUP BY MONTH(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$mon = $list['mon'];
			$pay_point = $list['pay_point'];

			$PAY[$mon] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(YEAR(order_day)) AS first_year FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$first_year = $list['first_year'];
	}
//$first_year = 2000;
	if ($first_year < $year) {
        $back_year = $year - 1;
		$first = <<<WAKABA
      <td align="left">
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
       <input type="hidden" name="mode" value="mons">
       <input type="hidden" name="year" value="$back_year">
       <input type="submit" value="&lt;&lt;">
      </td>
      </form>

WAKABA;
	}

	$now_year = date("Y");
	if ($now_year > $year) {
		$next_year = $year + 1;
		$next = <<<WAKABA
      <td align="left">
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
       <input type="hidden" name="mode" value="mons">
       <input type="hidden" name="year" value="$next_year">
       <input type="submit" value="&gt;&gt;">
      </td>
      </form>

WAKABA;
	}

	$html = "ポイント詳細（月毎）<br />";

	$html .= <<<WAKABA
<table border="0">
  <tbody>
    <tr align="center">
{$first}
      <td>ポイント獲得履歴　{$year}年</td>
{$next}
    </tr>
  </tbody>
</table>

<table border="0" cellspacing="1" bgcolor="#666666" width="600">
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="days">
<input type="hidden" name="year" value="$year">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>月</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

	for($mon=1; $mon<=12; $mon++) {
		//	クリック数
		$click = $CLICK[$mon];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$mon];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$mon];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$mon] - $DEC[$mon];

		//	変換ポイント
		$pay_point = $PAY[$mon];

		//	有効ポイント
		$now_point = $point - $pay_point;
		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
		$a_now_point += $now_point;

		$mon = sprintf('%02d',$mon);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center"><input type="submit" name="mon"  class="btn-standard" value="{$mon}">月</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
	$a_now_point = number_format($a_now_point);

	$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</form>
</table>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td>
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
    <tr align="right">
      <td>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
      <input type="submit" value="年一覧に戻る">
      </td>
      </form>
    </tr>
  </tbody>
</table>

WAKABA;

	echo($html);

}



//	日毎表示
function days($af_num) {
global $conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	$affm = $_POST['affm'];
	$k_number = $_POST['k_number'];
	$aff_number = $_POST['aff_number'];
	$name_s_ = $_POST['name_s_'];
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];
	$year = $_POST['year'];
	$mon = $_POST['mon'];

	$dis_time = mktime(0,0,0,$mon,1,$year);

	//	クリック数
	$sql  = "SELECT DAY(click_time) AS day, COUNT(*) AS click FROM $afrefere_table" .
			" WHERE af_num='$af_num' AND YEAR(click_time)='$year' AND MONTH(click_time)='$mon'" .
			" GROUP BY DAY(click_time);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$click = $list['click'];
			$CLICK[$day] = $click;
		}
	}

	//	売上件数
	$sql =  "SELECT DAY(order_day) AS day, COUNT(*) AS count FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$count = $list['count'];
			$COUNT[$day] = $count;
		}
	}

	//	総ポイント
	$sql =  "SELECT DAY(order_day) AS day, SUM(point) AS all_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$all_point = $list['all_point'];

			$ALL[$day] = $all_point;
		}
	}

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT DAY(order_day) AS day, SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND YEAR(order_day)='$year' AND MONTH(order_day)='$mon'" .
			" AND send_day<'$order_day'" .
			" GROUP BY DAY(order_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$dec_point = $list['dec_point'];

			$DEC[$day] = $dec_point;
		}
	}

	//	支払い情報(全て)
	$sql =  "SELECT DAY(appli_day) AS day, SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2'" .
			" AND YEAR(appli_day)='$year' AND MONTH(appli_day)='$mon'" .
			" GROUP BY DAY(appli_day);";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$day = $list['day'];
			$pay_point = $list['pay_point'];

			$PAY[$day] = $pay_point;
		}
	}

	//	初期年度
	$sql =  "SELECT MIN(order_day) AS first_day FROM $appoint_table" .
			" WHERE af_num='$af_num';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$first_day = $list['first_day'];
		list($f_year,$f_mon,$f_day) = explode("-",$first_day);
		$first_time = mktime(0,0,0,$f_mon,1,$f_year);
	}
//$first_time = mktime(0,0,0,1,1,2000);
	if ($first_time < $dis_time) {
		$back_mon = $mon - 1;
		if ($back_mon < 1) {
			$back_mon = 12;
			$back_year = $year - 1;
		}
		else {
			$back_year = $year;
		}
		$first = <<<WAKABA
      <td align="left">
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="days">
<input type="hidden" name="year" value="$back_year">
       <input type="hidden" name="mon" value="$back_mon">
       <input type="submit" value="&lt;&lt;">
      </td>
      </form>

WAKABA;
	}

	$now_time = mktime(0,0,0,date("m"),1,date("Y"));
	if ($now_time > $dis_time) {
		$next_mon = $mon + 1;
		if ($next_mon > 12) {
			$next_mon = 1;
			$next_year = $year + 1;
		}
		else {
			$next_year = $year;
		}
		$next = <<<WAKABA
      <td align="left">
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="days">
<input type="hidden" name="year" value="$next_year">
       <input type="hidden" name="mon" value="$next_mon">
       <input type="submit" value="&gt;&gt;">
      </td>
      </form>

WAKABA;
	}

	$html = "ポイント詳細（日毎）<br />";

	$html .= <<<WAKABA
<table border="0">
  <tbody>
    <tr align="center">
{$first}
      <td>ポイント獲得履歴　{$year}年{$mon}月</td>
{$next}
    </tr>
  </tbody>
</table>

<table border="0" cellspacing="1" bgcolor="#666666" width="600">
  <tbody>
    <tr bgcolor="#cccccc">
      <th>日</th>
      <th>クリック数</th>
      <th>売上件数</th>
      <th>予定ポイント</th>
      <th>確定ポイント</th>
      <th>変換ポイント</th>
    </tr>

WAKABA;

	$last_day = date("t",$dis_time);
	for($day=1; $day<=$last_day; $day++) {
		//	クリック数
		$click = $CLICK[$day];
		if ($click < 1) { $click = 0; }

		//	売上件数
		$count = $COUNT[$day];
		if ($count < 1) { $count = 0; }

		//	獲得確定ポイント
		$point = $DEC[$day];
		if ($point < 1) { $point = 0; }

		//	獲得予定ポイント
		$indent_point = $ALL[$day] - $DEC[$day];

		//	変換ポイント
		$pay_point = $PAY[$day];

		//	有効ポイント
		$now_point = $point - $pay_point;
		if ($now_point < 1) { $now_point = 0; }

		$click_ = number_format($click);
		$count_ = number_format($count);
		$point_ = number_format($point);
		$indent_point_ = number_format($indent_point);
		$pay_point_ = number_format($pay_point);
		$now_point_ = number_format($now_point);

		$a_click += $click;
		$a_count += $count;
		$a_indent_point += $indent_point;
		$a_point += $point;
		$a_pay_point += $pay_point;
		$a_now_point += $now_point;

		$day = sprintf('%02d',$day);

		$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="right">
      <td align="center">{$day}日</td>
      <td>{$click_}件</td>
      <td>{$count_}件</td>
      <td>{$indent_point_}pt</td>
      <td>{$point_}pt</td>
      <td>{$pay_point_}pt</td>
    </tr>

WAKABA;
	}

	$a_click = number_format($a_click);
	$a_count = number_format($a_count);
	$a_indent_point = number_format($a_indent_point);
	$a_point = number_format($a_point);
	$a_pay_point = number_format($a_pay_point);
	$a_now_point = number_format($a_now_point);

	$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td colspan="6"></td>
    </tr>
    <tr bgcolor="#ffffff" align="right">
      <th align="center">合計</th>
      <th>{$a_click}件</th>
      <th>{$a_count}件</th>
      <th>{$a_indent_point}pt</th>
      <th>{$a_point}pt</th>
      <th>{$a_pay_point}pt</th>
    </tr>
  </tbody>
</table>
<table border="0" width="600">
  <tbody>
    <tr align="right">
      <td colspan="2">
      ※表示されている確定ポイントは、今現在残っている確定ポイントではありません。
      </td>
    </tr>
    <tr align="right">
      <td>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="mons">
<input type="hidden" name="year" value="$year">
      <input type="submit" value="{$year}年の月一覧に戻る">
      </td>
      </form>
      <td>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affm" value="$affm">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="name_s_" value="$name_s_">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
      <input type="submit" value="年一覧に戻る">
      </td>
      </form>
    </tr>
  </tbody>
</table>

WAKABA;

	echo($html);

}
?>
