<?PHP
//	アフェリエイト管理
//　変換処理
function aff_3() {
global $PHP_SELF,$conn_id;
include "../../cone.inc";

	$appli_num = $_POST['appli_num'];

	if ($appli_num) {
		detaile($appli_num);
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
	$affs = $_POST['affs'];
	$afft = $_POST['afft'];
	$k_number = $_POST['k_number'];
#	$k_number = mb_convert_kana($k_number,"n","EUC-JP");
	$k_number = mb_convert_kana($k_number,"n","UTF-8");
	$k_number = trim($k_number);
	$aff_number = $_POST['aff_number'];
#	$aff_number = mb_convert_kana($aff_number,"n","EUC-JP");
	$aff_number = mb_convert_kana($aff_number,"n","UTF-8");
	$aff_number = trim($aff_number);
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];
	if (!$view) { $view = 1; }
	if (!$page) { $page = 1; }

	unset($where);
	if ($affs != 3 && $affs != "") { $where .= " AND app.state='$affs'"; }
	if ($afft) { $where .= " AND app.change_type='$afft'"; }
	if ($k_number) { $where .= " AND a.kojin_num='$k_number'"; }
	if ($aff_number) { $where .= " AND app.af_num='$aff_number'"; }
	if ($name_s_) { $where .= " AND a.name_s LIKE '$name_s_'"; }
	if ($hlist == "" || $hlist == 1) { $DESC = "DESC"; }

	$sql  = "SELECT * FROM $application_table app, $afuser_table af, $member_table a" .
			" WHERE app.af_num=af.af_num AND af.kojin_num=a.kojin_num $where;";
	if ($result = pg_query($conn_id,$sql)) {
		$max = pg_numrows($result);
	}

	if ($max > 0) {
		$search_view = 20 * $view;
		$max_page = ceil($max/$search_view);
		$s = $search_view * ($page - 1);
		$e = ($search_view * $page) - 1;
		if ($e >= $max) { $e = $max - 1; }
		$view_s = $s + 1;
		$view_e = $e+1;

		$page_msg = $max . "件条件に当てはまる申請がありました。 ";
		if ($max_page > 1) { $page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "Page"; }

		echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affs" value="$affs">
<INPUT type="hidden" name="afft" value="$afft">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
情報の詳細閲覧・変更したい変換申請番号をクリックして下さい。<br />
該当数:{$max}件　{$page}/{$max_page} PAGE
<table border="0" bgcolor="#666666" cellspacing="1">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>変換申請番号</td>
      <td>アフリエイター番号</td>
      <td>アフェリエイター名</td>
      <td>変換タイプ</td>
      <td>変換ポイント</td>
      <td>申請日</td>
      <td>ポイント・金額</td>
      <td>変換日</td>
      <td>変換状態</td>
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

		$sql  = "SELECT app.appli_num, app.af_num, a.name_s, a.name_n, app.change_type," .
				" app.af_point, app.appli_day, app.pay_price, app.pay_day, app.state" .
				" FROM $application_table app, $afuser_table af, $member_table a" .
				" WHERE app.af_num=af.af_num AND af.kojin_num=a.kojin_num $where" .
				" ORDER BY app.appli_day $DESC $limit_num;";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$appli_num = $list['appli_num'];
				$af_num = $list['af_num'];
				$name_s = $list['name_s'];
				$name_n = $list['name_n'];
				$change_type = $list['change_type'];
				$af_point = $list['af_point'];
				$appli_day = $list['appli_day'];
				$pay_price = $list['pay_price'];
				$pay_day = $list['pay_day'];
				$state = $list['state'];

				$af_point = number_format($af_point);
				list($appli_day) = explode(" ",$appli_day);
				$pay_price = number_format($pay_price);
				if ($change_type == 1) { $pay_price .= "pt"; }
				else { $pay_price .= "円"; }
				if (!$pay_day) { $pay_day = "----------"; }
				else { list($pay_day) = explode(" ",$pay_day); }

				if ($state != 2) {
					$submit = "<input type=\"submit\" name=\"appli_num\" value=\"$appli_num\">";
				}
				else {
					$submit = "$appli_num";
				}

				echo <<<WAKABA
    <tr bgcolor="#ffffff" align="center">
      <td>$submit</td>
      <td>$af_num</td>
      <td>$name_s $name_n</td>
      <td>{$CHENGE_TYPE2_L[$change_type]}</td>
      <td align="right">{$af_point}pt</td>
      <td>$appli_day</td>
      <td align="right">{$pay_price}</td>
      <td>$pay_day</td>
      <td>{$AFRS_N[$state]}</td>
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
            <input type="hidden" name="affs" value="$affs">
            <input type="hidden" name="afft" value="$afft">
            <input type="hidden" name="k_number" value="$k_number">
            <input type="hidden" name="aff_number" value="$aff_number">
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
            <input type="hidden" name="affs" value="$affs">
            <input type="hidden" name="afft" value="$afft">
            <input type="hidden" name="k_number" value="$k_number">
            <input type="hidden" name="aff_number" value="$aff_number">
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
今現在情報はありません。<br>

WAKABA;

	}

}



//	承認ページ
function detaile($appli_num) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	$mode = $_POST['mode'];
	if ($mode == "change") { list($ERROR,$msg) = change($appli_num); }

	$main = $_POST['main'];
	$main_r = $_POST['main_r'];
	$aff = $_POST['aff'];
	$aff_r = $_POST['aff_r'];
	$affs = $_POST['affs'];
	$afft = $_POST['afft'];
	$k_number = $_POST['k_number'];
#	$k_number = mb_convert_kana($k_number,"n","EUC-JP");
	$k_number = mb_convert_kana($k_number,"n","UTF-8");
	$k_number = trim($k_number);
	$aff_number = $_POST['aff_number'];
#	$aff_number = mb_convert_kana($aff_number,"n","EUC-JP");
	$aff_number = mb_convert_kana($aff_number,"n","UTF-8");
	$aff_number = trim($aff_number);
	$hlist = $_POST['hlist'];
	$view = $_POST['view'];
	$page = $_POST['page'];

	if (!$ERROR) {
		//	情報読み込み
		$sql  = "SELECT app.af_num, a.name_s, a.name_n, app.change_type, app.bank_num," .
				" app.af_point, app.appli_day, app.pay_price, app.pay_day, app.state" .
				" FROM $application_table app, $afuser_table af, $member_table a" .
				" WHERE app.af_num=af.af_num AND af.kojin_num=a.kojin_num AND app.appli_num='$appli_num' LIMIT 1;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$af_num = $list['af_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$change_type = $list['change_type'];
			$bank_num = $list['bank_num'];
			$point = $list['af_point'];
			$appli_day = $list['appli_day'];
			$b_pay_price = $pay_price = $list['pay_price'];
			$pay_day = $list['pay_day'];
			$b_state = $state = (int)$list['state'];
		}

		//	口座チェック
		if ($bank_num > 0) {
			$sql  = "SELECT * FROM $bank_table" .
					" WHERE bank_num='$bank_num' AND state='0' LIMIT 1;";
			if ($result = pg_query($conn_id,$sql)) {
				$list = pg_fetch_array($result);
				$bank_num = $list['bank_num'];
				$bank_name = $list['bank_name'];
				$store_name = $list['store_name'];
				$deposit = $list['deposit'];
				$account_num = $list['account_num'];
				$account_name = $list['account_name'];
			}
			if (!$bank_num) { $ERROR[] = "お振り込み口座情報が確認出来ませんでした。"; }
		}
		$sendsmail = 1;
	}
	else {
		$af_num = $_POST['af_num'];
#		$af_num = mb_convert_kana($af_num,"sn","EUC-JP");
		$af_num = mb_convert_kana($af_num,"sn","UTF-8");
		$af_num = trim($af_num);
		$point = $_POST['point'];
#		$point = mb_convert_kana($point,"sn","EUC-JP");
		$point = mb_convert_kana($point,"sn","UTF-8");
		$point = trim($point);
		$change_type = $_POST['change_type'];
		$bank_name = $_POST['bank_name'];
#		$bank_name = mb_convert_kana($bank_name,"sCKV","EUC-JP");
		$bank_name = mb_convert_kana($bank_name,"sCKV","UTF-8");
		$bank_name = trim($bank_name);
		$store_name = $_POST['store_name'];
#		$store_name = mb_convert_kana($store_name,"sCKV","EUC-JP");
		$store_name = mb_convert_kana($store_name,"sCKV","UTF-8");
		$store_name = trim($store_name);
		$deposit = $_POST['deposit'];
		$account_num = $_POST['account_num'];
#		$account_num = mb_convert_kana($account_num,"sn","EUC-JP");
		$account_num = mb_convert_kana($account_num,"sn","UTF-8");
		$account_num = trim($account_num);
		$account_name = $_POST['account_name'];
#		$account_name = mb_convert_kana($account_name,"sCKV","EUC-JP");
		$account_name = mb_convert_kana($account_name,"sCKV","UTF-8");
		$account_name = trim($account_name);
		$pay_price = $_POST['pay_price'];
#		$pay_price = mb_convert_kana($pay_price,"sn","EUC-JP");
		$pay_price = mb_convert_kana($pay_price,"sn","UTF-8");
		$pay_price = trim($pay_price);
		$b_pay_price = $_POST['b_pay_price'];
#		$b_pay_price = mb_convert_kana($b_pay_price,"sn","EUC-JP");
		$b_pay_price = mb_convert_kana($b_pay_price,"sn","UTF-8");
		$b_pay_price = trim($b_pay_price);
		$pay_day = $_POST['pay_day'];
#		$pay_day = mb_convert_kana($pay_day,"sn","EUC-JP");
		$pay_day = mb_convert_kana($pay_day,"sn","UTF-8");
		$pay_day = trim($pay_day);
		$state = $_POST['state'];
		$b_state = $_POST['b_state'];
		$sendsmail = $_POST['sendsmail'];
	}

	if ($change_type == 1) {
		$checked1 = "checked";
		$display = "none";
		$hyouji = "on";
	}
	else {
		$checked2 = "checked";
		$display = "block";
		$hyouji = "off";
	}

	$selected21 = $selected22 = "";
	if ($deposit == 2) { $selected22 = "selected"; } else { $selected21 = "selected"; }

	if ($sendsmail == 1) { $checked = "checked"; }

	//	申請日
	list($appli_day) = explode(" ",$appli_day);

	//	変換日
	if (!$pay_day) { $pay_day = date("Y-m-d"); }


	//	変換状態
	$max = count($AFRS_N) - 1;
	for($i=0; $i<$max; $i++) {
		if ($i == $state) { $selected = "selected"; } else { $selected = ""; }
		$l_state .= "        <option value=\"$i\" $selected>$AFRS_N[$i]</option>";
	}

	if ($ERROR) { ERROR($ERROR); }

	echo <<<WAKABA
$msg
<script language="JavaScript">
<!--
function hyouji(lId) {
	if (lId == 'off') {
		msg1.style.display = "block";
	}
	else {
		msg1.style.display = "none";
	}
}
//-->
</script>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affs" value="$affs">
<INPUT type="hidden" name="afft" value="$afft">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<INPUT type="hidden" name="af_num" value="$af_num">
<INPUT type="hidden" name="mode" value="change">
<INPUT type="hidden" name="appli_num" value="$appli_num">
<INPUT type="hidden" name="bank_num" value="$bank_num">
<INPUT type="hidden" name="b_pay_price" value="$b_pay_price">
<INPUT type="hidden" name="b_state" value="$b_state">
<table border="0" width="600">
  <tbody>
    <tr>
      <td>
      手続きが完了した場合は、変換日を入力し、変換状態を支払い済みにして更新ボタンを押して下さい。
      </td>
    </tr>
  </tbody>
</table>
<table border="0" cellspacing="1" cellpadding="0" width="600" bgcolor="#666666">
  <tbody>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換申請番号</td>
      <td>$appli_num</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">アフリエイター番号</td>
      <td>$af_num</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">アフェリエイター名</td>
      <td>$name_s $name_n</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換タイプ</td>
      <td><input type="radio" name="change_type" value="1" $checked1 onclick="hyouji('on');">：{$CHENGE_TYPE_L[1]}<br>
      <input type="radio" name="change_type" value="2" $checked2 onclick="hyouji('off');">：{$CHENGE_TYPE_L[2]}</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換ポイント</td>
      <td><input size="10" type="text" name="point" value="$point"> pt</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">申請日</td>
      <td>$appli_day</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">ポイント・金額</td>
      <td><input size="10" type="text" name="pay_price" value="$pay_price"> pt</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換日or処理日</td>
      <td><input size="20" type="text" name="pay_day" value="$pay_day"> 例：2000-01-01</td>
    </tr>
    <tr bgcolor="#ffffff" style="display:{$display};" id="msg1">
      <td bgcolor="#cccccc">お振り込み口座</td>
      <td>
      <table border="0">
        <tbody>
          <tr>
            <td>金融機関名</td>
            <td>：</td>
            <td><input size="30" type="text" name="bank_name" value="$bank_name">　<font color="#ff0000">※カタカナ</font></td>
          </tr>
          <tr>
            <td>支店名</td>
            <td>：</td>
            <td><input size="30" type="text" name="store_name" value="$store_name">　<font color="#ff0000">※カタカナ</font></td>
          </tr>
          <tr>
            <td>振込先科目</td>
            <td>：</td>
            <td><select name="deposit">
              <option value="1" $selected21>{$DEPOSIT_L[1]}</option>
              <option value="2" $selected22>{$DEPOSIT_L[2]}</option>
            </select></td>
          </tr>
          <tr>
            <td>振込先口座番号</td>
            <td>：</td>
            <td><input size="30" type="text" name="account_num" value="$account_num"></td>
          </tr>
          <tr>
            <td>受取人名</td>
            <td>：</td>
            <td><input size="30" type="text" name="account_name" value="$account_name">　<font color="#ff0000">※カタカナ</font></td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換状態</td>
      <td><select name="state">
$l_state
      </select></td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">メール送信</td>
      <td><input type="checkbox" name="sendsmail" value="1" $checked>：送信する</td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td colspan="2"><input type="submit" value="更新">　<input type="reset"></td>
    </tr>
  </tbody>
</table>
  </form>
<br>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="aff" value="$aff">
<INPUT type="hidden" name="aff_r" value="$aff">
<INPUT type="hidden" name="affs" value="$affs">
<INPUT type="hidden" name="afft" value="$afft">
<INPUT type="hidden" name="k_number" value="$k_number">
<INPUT type="hidden" name="aff_number" value="$aff_number">
<INPUT type="hidden" name="hlist" value="$hlist">
<INPUT type="hidden" name="view" value="$view">
<INPUT type="hidden" name="page" value="$page">
<table border="0" width="600">
  <tbody>
    <tr>
      <td align="right"><input type="submit" value="一覧に戻る"></td>
    </tr>
  </tbody>
</table>
</form>
<script language="JavaScript">
<!--
function _HpbShowObj(lId)
{
  var ob;ob=new Array;
  var appVer=parseInt(navigator.appVersion);
  var isNC=false,isN6=false,isIE=false;
  if (document.all && appVer >= 4) isIE=true; else
    if (document.getElementById && appVer > 4) isN6=true; else
      if (document.layers && appVer >= 4) isNC=true;
  if (isNC)
  {
    w_str = "document." + lId;ob[lId] = eval(w_str);
    if (!ob[lId]) ob[lId] = _HpbFindHiddenObj(document, lId);
    if (ob[lId]) ob[lId].visibility = "show";
  }
  if (isN6)
  {
    ob[lId] = document.getElementById(lId);
    ob[lId].style.visibility = "visible";
  }
  if (isIE)
  {
    w_str = "document.all.item(\"" + lId + "\").style";ob[lId] = eval(w_str);
    ob[lId].visibility = "visible";
  }
}

function _HpbFindHiddenObj(doc, lId)
{
  for (var i=0; i < doc.layers.length; i++)
  {
    var w_str = "doc.layers[i].document." + lId;
    var obj;obj=new Array;
    obj[lId] = eval(w_str);
    if (!obj[lId]) obj[lId] = _HpbFindHiddenObj(doc.layers[i], lId);
    if (obj[lId]) return obj[lId];
  }
  return null;
}
function _HpbHideObj(lId)
{
  var ob;ob=new Array;
  var appVer=parseInt(navigator.appVersion);
  var isNC=false,isN6=false,isIE=false;
  if (document.all && appVer >= 4) isIE=true; else
    if (document.getElementById && appVer > 4) isN6=true; else
      if (document.layers && appVer >= 4) isNC=true;
  if (isNC)
  {
    w_str = "document." + lId;ob[lId] = eval(w_str);
    if (!ob[lId]) ob[lId] = _HpbFindShownObj(document, lId);
    if (ob[lId]) ob[lId].visibility = "hide";
  }
  if (isN6)
  {
    ob[lId] = document.getElementById(lId);
    ob[lId].style.visibility = "hidden";
  }
  if (isIE)
  {
    w_str = "document.all.item(\"" + lId + "\").style";ob[lId] = eval(w_str);
    ob[lId].visibility = "hidden";
  }
}

function _HpbFindShownObj(doc, lId)
{
  for (var i=0; i < doc.layers.length; i++)
  {
    var w_str = "doc.layers[i].document." + lId;
    var obj;obj=new Array;
    obj[lId] = eval(w_str);
    if (!obj[lId]) obj[lId] = _HpbFindShownObj(doc.layers[i], lId);
    if (obj[lId]) return obj[lId];
  }
  return null;
}
//-->
</script>

WAKABA;

}



//	登録処理
function change($appli_num) {
global $PHP_SELF,$conn_id;
include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';
include '../sub/mail.inc';

	$af_num = $_POST['af_num'];
#	$af_num = mb_convert_kana($af_num,"sn","EUC-JP");
	$af_num = mb_convert_kana($af_num,"sn","UTF-8");
	$af_num = trim($af_num);
	$bank_num = $_POST['bank_num'];
#	$bank_num = mb_convert_kana($bank_num,"sn","EUC-JP");
	$bank_num = mb_convert_kana($bank_num,"sn","UTF-8");
	$bank_num = trim($bank_num);
	$point = $_POST['point'];
#	$point = mb_convert_kana($point,"sn","EUC-JP");
	$point = mb_convert_kana($point,"sn","UTF-8");
	$point = trim($point);
	$change_type = $_POST['change_type'];
	$bank_name = $_POST['bank_name'];
#	$bank_name = mb_convert_kana($bank_name,"sCKV","EUC-JP");
	$bank_name = mb_convert_kana($bank_name,"sCKV","UTF-8");
	$bank_name = trim($bank_name);
	$store_name = $_POST['store_name'];
#	$store_name = mb_convert_kana($store_name,"sCKV","EUC-JP");
	$store_name = mb_convert_kana($store_name,"sCKV","UTF-8");
	$store_name = trim($store_name);
	$deposit = $_POST['deposit'];
	$account_num = $_POST['account_num'];
#	$account_num = mb_convert_kana($account_num,"sn","EUC-JP");
	$account_num = mb_convert_kana($account_num,"sn","UTF-8");
	$account_num = trim($account_num);
	$account_name = $_POST['account_name'];
#	$account_name = mb_convert_kana($account_name,"sCKV","EUC-JP");
	$account_name = mb_convert_kana($account_name,"sCKV","UTF-8");
	$account_name = trim($account_name);
	$pay_price = $_POST['pay_price'];
#	$pay_price = mb_convert_kana($pay_price,"sn","EUC-JP");
	$pay_price = mb_convert_kana($pay_price,"sn","UTF-8");
	$pay_price = trim($pay_price);
	$b_pay_price = $_POST['b_pay_price'];
#	$b_pay_price = mb_convert_kana($b_pay_price,"sn","EUC-JP");
	$b_pay_price = mb_convert_kana($b_pay_price,"sn","UTF-8");
	$b_pay_price = trim($b_pay_price);
	$pay_day = $_POST['pay_day'];
#	$pay_day = mb_convert_kana($pay_day,"sn","EUC-JP");
	$pay_day = mb_convert_kana($pay_day,"sn","UTF-8");
	$pay_day = trim($pay_day);
	$state = $_POST['state'];
	$b_state = $_POST['b_state'];
	$sendsmail = $_POST['sendsmail'];

	if (!$point) { $ERROR[] = "変換するポイントが入力されておりません。"; }
	elseif (!isNum($point)) { $ERROR[] = "数字以外の文字が含まれております。"; }
	elseif ($point < $change_point) { $ERROR[] = "変換出来るポイントは、{$change_point_}pt以上です。"; }
	if ($change_type == 2) {
		if (!$bank_name) { $ERROR[] = "金融機関名が入力されておりません。"; }
		elseif (!isZenKkana($bank_name)) { $ERROR[] = "金融機関名に、カタカナ以外の文字が含まれております。"; }
		if (!$store_name) { $ERROR[] = "支店名が入力されておりません。"; }
		elseif (!isZenKkana($store_name)) { $ERROR[] = "支店名に、カタカナ以外の文字が含まれております。"; }
		if (!$deposit) { $ERROR[] = "振込先科目が選択されておりません。"; }
		if (!$account_num) { $ERROR[] = "振込先口座番号が入力されておりません。"; }
		elseif (!isNum($account_num)) { $ERROR[] = "振込先口座番号に、数字以外の文字が含まれております。"; }
		if (!$account_name) { $ERROR[] = "受取人名が入力されておりません。"; }
		elseif (!isZenKkana($account_name)) { $ERROR[] = "受取人名に、カタカナ以外の文字が含まれております。"; }
	}
	elseif ($change_type != 1) { $ERROR[] = "変換種類が確認出来ません。"; }
	if (!isNum($pay_price)) { $ERROR[] = "ポイント・金額に、数字以外の物が含まれております。"; }
	if ($state == 1) {
		if (!$pay_day) { $ERROR[] = "変換日が入力されておりません。"; }
		else {
			list($year,$mon,$day) = explode("-",$pay_day);
			if (!checkdate($mon,$day,$year)) { $ERROR[] = "変換日の日付が不正です。"; }
		}
	}
	elseif ($state < 1) { $pay_day = "NULL"; }
	if ($pay_day && $pay_day!="NULL") { $pay_day = "'$pay_day'"; }

	if (!$ERROR) {
		//	獲得ポイント
		$point_ = point_checks($af_num);
		if ($point_ < $point) { $ERROR[] = "入力されたポイントが獲得ポイントを越えております。"; }
	}

	//	登録処理
	//	銀行情報
	if (!$ERROR) {
		if ($change_type == 2) {
			if ($bank_num > 0) {
				$sql  = "UPDATE $bank_table SET" .
						" bank_name='$bank_name'," .
						" store_name='$store_name'," .
						" deposit='$deposit'," .
						" account_num='$account_num'," .
						" account_name='$account_name'" .
						" WHERE bank_num='$bank_num';";
				if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "振込先情報を更新出来ませんでした。"; }
			}
			else {
				$sql  = "INSERT INTO $bank_table" .
						" (af_num,bank_name,store_name,deposit,account_num,account_name)" .
						" VALUES('$af_num','$bank_name','$store_name','$deposit','$account_num','$account_name');";
				if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "振込先情報を登録出来ませんでした。"; }
				else {
					$sql  = "SELECT MAX(bank_num) AS max FROM $bank_table" .
							" WHERE af_num='$af_num' AND state='0';";
					if ($result = pg_query($conn_id,$sql)) {
						$list = pg_fetch_array($result);
						$bank_num = $list['max'];
					}
				}
			}
		}
		elseif ($bank_num > 0) {
			$sql  = "UPDATE $bank_table SET" .
					" state='1'" .
					" WHERE bank_num='$bank_num';";
			if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "振込先情報を削除出来ませんでした。"; }
			unset($bank_num);
		}
	}
	if (!$bank_num) { $bank_num = 0; }
	if ($state != 1) { $pay_price = 0; }

	if (!$pay_day || $state != 1) {
		$pay_day_msg = " pay_day=now(),";
	}
	else {
		$pay_day_msg = " pay_day=$pay_day,";
	}

	//	基本情報
	if (!$ERROR) {
		$sql  = "UPDATE $application_table SET" .
				" change_type='$change_type'," .
				" bank_num='$bank_num'," .
				" af_point='$point'," .
				" pay_price='$pay_price'," .
				" pay_day=$pay_day," .
				" state='$state'" .
				" WHERE appli_num='$appli_num';";
		if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "変換情報を更新出来ませんでした。"; }
	}

	//	ポイント追加
	//	$check = 0 変化なし
	//	$check = 1 ポイント追加
	//	$check = 2 ポイント減す
	if (!$ERROR) {
		$check = 0;
		$add_point = 0;
		$del_point = 0;
		$check = 0;
		if ($change_type == 1) {
			//	未処理
			if ($state < 1 && $b_state != 1) {	//	状況に変化がない、又はキャンセルから未処理になった場合（未処理）
				//	変化なし
			}
			elseif ($state < 1 && $b_state > 0) {	//	支払い済みから未処理に戻した場合
				if ($b_pay_price > 0) {	//	ポイントを与えていた場合差し引く
					$check = 2;
					$del_point = $b_pay_price;
				}
				else {
					//	変化なし
				}
			}
			//	払い済み
			elseif ($state == 1 && $b_state != 1) {	//	支払い済み以外から支払い済みになった場合
				$check = 1;
				$add_point = $pay_price;
			}
			elseif ($state == 1 && $b_state == 1) {	//	支払い済みで変化がない場合
				if ($b_pay_price > $pay_price) {	//	支払いポイントが減った場合
					$check = 2;
					$del_point = $b_pay_price - $pay_price;
				}
				elseif ($b_pay_price < $pay_price) {	//	支払いポイントが増えた場合
					$check = 1;
					$add_point = $pay_price - $b_pay_price;
				}
			}
			//	キャンセル
			elseif ($state == 2 && $b_state != 1) {	//	未処理でキャンセルになった、又は変化がない場合
				//	変化なし
			}
			elseif ($state == 2 && $b_state == 1) {	//	支払い済みでキャンセルになった場合
				$check = 2;
				$del_point = $b_pay_price;
			}

			//	会員番号抽出
			if ($check > 0) {
				$sql  = "SELECT af.kojin_num FROM $application_table app, $afuser_table af" .
						" WHERE app.af_num=af.af_num AND app.appli_num='$appli_num' LIMIT 1;";
				if ($result = pg_query($conn_id,$sql)) {
					$list = pg_fetch_array($result);
					$kojin_num = $list['kojin_num'];
				}
			}

			//	既存ポイント抽出
			if ($check > 0 && $kojin_num) {
				$sql  = "SELECT point, name_s, name_n, email FROM $member_table" .
						" WHERE kojin_num='$kojin_num' LIMIT 1;";
				if ($result = pg_query($conn_id,$sql)) {
					$list = pg_fetch_array($result);
					$now_point = $list['point'];
					$name_s = $list['name_s'];
					$name_n = $list['name_n'];
					$name = "$name_s $name_n";
					$email = $list['email'];
				}
			}

			if ($check == 1) {
				$point = $now_point + $add_point;
			}
			elseif ($check == 2) {
				$point = $now_point - $del_point;
				if ($point < 0) { $point = 0; }
			}

			//	ポイント更新
			if ($check > 0) {
				$sql  = "UPDATE $member_table SET" .
						" point='$point'" .
						" WHERE kojin_num='$kojin_num';";
				if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "ポイント情報を更新出来ませんでした。"; }
			}
		}
	}

	//	メール送信処理
	if (!$ERROR && $sendsmail == 1 && $state!='0') {
		$point = number_format($point);
		$appli_num_ = sprintf("%05d",$appli_num);

		//	会員
		if ($state == 1) {	//	処理済み時
			//	件名
			$subject = "アフィリエイトポイント変換完了のご連絡 (申請番号：{$appli_num_})";

			$msr = <<<WAKABA
{$name}様、申請して頂いておりましたネイバーズスポーツアフィリエイトポイント変換作業が完了致しました。
ご指定の変換種類にて変換させて頂いておりますのでご確認下さい。
※お振り込みの場合、手数料を差し引いた金額をお振り込みさせて頂いております。

申請番号：{$appli_num_}

それではこれからもよろしくお願い致します。

$m_footer

WAKABA;
		}
		else {	//	キャンセル時
			//	件名
			$subject = "アフィリエイトポイント変換申請キャンセルのご連絡 (申請番号：{$appli_num_})";

			$msr = <<<WAKABA
{$name}様、ネイバーズスポーツアフィリエイトポイント変換申請を管理者によりキャンセル致しました。

申請番号：{$appli_num_}

$m_footer

WAKABA;
		}

		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	if (!$ERROR) { $msg = "<b>更新しました。</b><br>\n<br>\n"; }

	return array($ERROR,$msg);

}



//	獲得ポイント
function point_checks($af_num) {
global $conn_id;

include './array.inc';
include '../sub/array.inc';
include '../sub/setup.inc';

	//	確定ポイント(全て)
	$order_time = mktime(0,0,0,date("m")-$set_point_mon,1,date("Y"));
	$order_day = date("Y-m-d",$order_time);
	$sql =  "SELECT SUM(point) AS dec_point FROM $appoint_table" .
			" WHERE af_num='$af_num' AND state='1'" .
			" AND send_day<'$order_day';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		(int)$dec_point = $list['dec_point'];
	}

	//	支払い情報(全て)
	$sql =  "SELECT SUM(af_point) AS pay_point FROM $application_table" .
			" WHERE af_num='$af_num' AND state!='2';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		(int)$pay_point = $list['pay_point'];
	}

	//	獲得確定ポイント
	$point = $dec_point - $pay_point;
	if ($point < 1) { $point = 0; }

	return $point;

}



//	カタカナチェック
function isZenKkana($data) {

    //magic_quotes_gpcがONの時は、エスケープを解除する
    if (get_magic_quotes_gpc()) {
        $data = stripslashes($data);
    }

    $data= trim($data);
    $pat = "^[ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲン]+$";   
    if (mb_ereg_match($pat, $data)) {
        return true;
    } else {
        return false;
    }

}



//	数字チェック
function isNum($data) {

    $pat = "^[0-9]+$";
##    if (ereg($pat, trim($data))) {
	  if (preg_match($pat, trim($data))) {
        return true;
    } else {
        return false;
    }

}



//	ロック
function lock($lockdir) {

	if (file_exists($lockdir)) {
		$now = time();
		list($device,$inode,$remode,$num_of_link,$user_id,$group_id,$rdev,$size,
		$atime,$mtime,$ctime,$blocksize,$num_of_blocks) = stat($lockdir);
		$sa = $now - $mtime;

		if ($sa >= 30) {
			if (file_exists($lockdir)) { rmdir($lockdir); }
		}
	}

	$flag = 0;
	for($i=1; $i<=5; $i++) {
		if (!file_exists($lockdir)) {
			mkdir ($lockdir,0777);
			@chmod ($lockdir,0777);
			$flag = 1;
			break;
		}
		else {
			sleep(1);
		}
	}

}
?>
