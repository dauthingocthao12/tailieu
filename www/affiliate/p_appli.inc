<?PHP
//	アフィリエイトポイント変換申請

//	デフォルト
function pap_defaults($mode,$action,$af_num) {
global $PHP_SELF,$set_point_mon,$change_point,$CHENGE_TYPE_L,$DEPOSIT_L;

	//	獲得ポイント
	$point = point_checks($af_num);
	$point_ = number_format($point);

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	if ($_SESSION['ERROR']) {
		$errors = ERROR($_SESSION['ERROR']);
		unset($_SESSION['ERROR']);
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

	$change_point = number_format($change_point);

	$html = <<<WAKABA
$errors
<form action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="p_appli">
<INPUT type="hidden" name="action" value="check">
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
<table border="0" width="600">
  <tbody>
    <tr>
      <td>変換するポイントと、お振り込みする口座を入力し確認ボタンを押して下さい。</td>
    </tr>
  </tbody>
</table>
<table border="0" cellspacing="1" cellpadding="0" width="600" bgcolor="#666666">
  <tbody>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc" width="150">変換ポイント<br />（可能ポイント：{$point_}pt）</td>
      <td width="450"><input size="10" type="text" name="point" value="$point"> pt 
      ※{$change_point}pt以上から変換可能です。
      </td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換種類</td>
      <td><input type="radio" name="change_type" value="1" $checked1 onclick="hyouji('on');">：{$CHENGE_TYPE_L[1]}<br>
      <input type="radio" name="change_type" value="2" $checked2 onclick="hyouji('off');">：{$CHENGE_TYPE_L[2]}</td>
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
      <font color="#ff0000">※お振り込みさせて頂く金額は、お振り込み手数料を引いた金額とさせて頂きます。</font></td>
    </tr>
    <tr bgcolor="#ffffff" align="center">
      <td colspan="2"><input type="submit" value="確認">　<input type="reset" onclick="hyouji('{$hyouji}');"></td>
    </tr>
  </tbody>
</table>
<br>
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

	return $html;

}



//	入力確認処理
function p_pap_check($mode,$action,$af_num) {
global $PHP_SELF,$set_point_mon,$change_point;

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

	$change_point_ = number_format($change_point);
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

	if (!$ERROR) {
		//	獲得ポイント
		$point_ = point_checks($af_num);
		if ($point_ < $point) { $ERROR[] = "入力されたポイントが獲得ポイントを越えております。"; }
	}

	//	セッション埋め込み
	unset($_SESSION['BANK']);
	$_SESSION['BANK']['point'] = $point;
	$_SESSION['BANK']['change_type'] = $change_type;
	if ($change_type == 2) {
		$_SESSION['BANK']['bank_name'] = $bank_name;
		$_SESSION['BANK']['store_name'] = $store_name;
		$_SESSION['BANK']['deposit'] = $deposit;
		$_SESSION['BANK']['account_num'] = $account_num;
		$_SESSION['BANK']['account_name'] = $account_name;
	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$action = "";
	}
	else {
		$action = "&action=check";
	}

	header ("Location: $PHP_SELF?mode=$mode$action\n\n");

	exit;

}



//	登録処理
function p_pap_end($mode,$action,$af_num) {
global $PHP_SELF,$conn_id,$bank_table,$afuser_table,$appoint_table,$application_table,$member_table,
		$set_point_mon,$change_point,$CHENGE_TYPE_L,$DEPOSIT_L,$DIR_CPT,
		$CHENGE_TYPE2_L,
		$m_footer,$admin_mail_a,$admin_name;

	if (!$_SESSION['BANK'] || !$af_num) {
		$ERROR[] = "入力された情報が確認できません。";
		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF?mode=$mode\n\n");
		exit;
	}

	//	ロック
	if (!$ERROR) {
		$lockdir = "$DIR_CPT/$af_num";
		lock($lockdir);
	}

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	//	ポイント確認
	$point_ = point_checks($af_num);
	if ($point > $point_) { $ERROR[] = "変換するポイントをご確認下さい。"; }

	//	振込先登録
	if (!$ERROR && $change_type == 2) {
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
	if (!$bank_num) { $bank_num = 0; }

	//	支払いテーブル記録
	if (!$ERROR) {
		$sql  = "INSERT INTO $application_table" .
				" (af_num,change_type,bank_num,af_point,appli_day)" .
				" VALUES('$af_num','$change_type','$bank_num','$point',now());";
		if (!$result = pg_exec($conn_id,$sql)) { $ERROR[] = "変換情報を登録出来ませんでした。"; }
		else {
			$sql  = "SELECT MAX(appli_num) AS max FROM $application_table" .
					" WHERE af_num='$af_num';";
			if ($result = pg_query($conn_id,$sql)) {
				$list = pg_fetch_array($result);
				$appli_num = $list['max'];
			}
		}
	}

	//	メール配信
	if (!$ERROR) {	//	ユーザー情報抜き出し
		$sql  = "SELECT a.kojin_num, a.name_s, a.name_n, a.email" .
				" FROM $member_table a, $afuser_table b" .
				" WHERE a.kojin_num=b.kojin_num AND b.state!='1' AND b.af_num='$af_num' LIMIT 1;";
		if ($result = pg_query($conn_id,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$name = "$name_s $name_n";
			$email = $list['email'];
		}
		if (!$kojin_num || !$name_s || !$name_n || !$email) { $ERROR[] = "会員情報が確認出来ません。"; }
	}

	if (!$ERROR) {
		unset($bank_msg);
		if ($change_type == 2) {
			$bank_msg = <<<WAKABA

お振り込み口座
金融機関名：{$bank_name}
支店名：{$store_name}
振込先科目：{$DEPOSIT_L[$deposit]}
振込先口座番号：{$account_num}
受取人名：{$account_name}

WAKABA;
		}
		$point = number_format($point);
		$appli_num_ = sprintf("%05d",$appli_num);

		//	受け取り
		//	件名
		$subject = "アフィリエイトポイント変換申請 (申請番号：{$appli_num_})";

		$ip = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($ip);
		if (!$host) { $host = $ip; }

		$msr = <<<WAKABA
{$name}様がアフィリエイトポイント変換申請しました。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num_}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
$bank_msg
------------------------------------------------------
$host ($ip)

WAKABA;

		//	送信処理
		$send_email = $email;
		$send_name = "$name";
		$get_email = $admin_mail_a;
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

		//	会員
		//	件名
		$subject = "アフィリエイトポイント変換申請確認";

		//	メッセージ
		$msr = <<<WAKABA
{$name}様、ネイバーズスポーツアフィリエイトポイント変換申請受付完了致しました。
受付内容は以下でよろしいでしょうか？
情報がおかしな場合、申請した記憶がない場合はお手数ですがご連絡お願い致します。

名前：{$name}
会員番号：{$kojin_num}
ID：{$af_num}
E-mail：{$email}

申請番号：{$appli_num_}
変換ポイント：{$point}pt
変換種類：{$CHENGE_TYPE2_L[$change_type]}
$bank_msg

それではこれからもよろしくお願い致します。

$m_footer

WAKABA;

		//	送信処理
		$send_email = $admin_mail_a;
		$send_name = $admin_name;
		$get_email = $email;
		send_email($send_email,$send_name,$mail_bcc,$bcc_email,$get_email,$subject,$msr);

	}

	//	セッション削除
	if (!$ERROR) {
		unset($_SESSION['BANK']);
	}
	else {
		if ($bank_num) {
			$sql  = "DELETE FROM $bank_table WHERE bank_num='$bank_num';";
			$result = pg_query($conn_id,$sql);
		}
		if ($appli_num) {
			$sql  = "DELETE FROM $application_table WHERE appli_num='$appli_num';";
			$result = pg_query($conn_id,$sql);
		}
	}

	//	LOCK解除
	if (file_exists($lockdir)) { rmdir($lockdir); }

	if (!$ERROR) {
		$action = "end";
	}
	else {
		$_SESSION['ERROR'] = $ERROR;
	}

	header ("Location: $PHP_SELF?mode=$mode&action=$action\n\n");

	exit;

}



//	確認ページ
function pap_check($mode,$action,$af_num) {
global $PHP_SELF,$CHENGE_TYPE_L,$DEPOSIT_L;

	if (!$_SESSION['BANK'] || !$af_num) {
		$ERROR[] = "入力された情報が確認できません。";
		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF?mode=$mode\n\n");
		exit;
	}

	if ($_SESSION['BANK']) {
		$point = $_SESSION['BANK']['point'];
		$change_type = $_SESSION['BANK']['change_type'];
		$bank_name = $_SESSION['BANK']['bank_name'];
		$store_name = $_SESSION['BANK']['store_name'];
		$deposit = $_SESSION['BANK']['deposit'];
		$account_num = $_SESSION['BANK']['account_num'];
		$account_name = $_SESSION['BANK']['account_name'];
	}

	$point = number_format($point);

	//	獲得ポイント
	$point_ = point_checks($af_num);
	$point_ = number_format($point_);

	$html = <<<WAKABA
<table border="0" width="600">
  <tbody>
  <form action="$PHP_SELF" method="POST">
  <INPUT type="hidden" name="mode" value="p_appli">
    <tr>
      <td>以下の内容でよろしければ申請ボタンを押して下さい。</td>
      <td align="right"><input type="submit" value="修正する"></td>
    </tr>
  </form>
  </tbody>
</table>
<table border="0" cellspacing="1" cellpadding="0" width="600" bgcolor="#666666">
  <tbody>
  <form action="$PHP_SELF" method="POST">
  <INPUT type="hidden" name="mode" value="p_appli">
  <INPUT type="hidden" name="action" value="regist">
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc" width="200">変換ポイント（可能ポイント：{$point_}pt）</td>
      <td width="400">　{$point}pt</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">変換種類</td>
      <td>　{$CHENGE_TYPE_L[$change_type]}</td>
    </tr>

WAKABA;

	if ($change_type == 2) {
		$html .= <<<WAKABA
    <tr bgcolor="#ffffff">
      <td bgcolor="#cccccc">お振り込み口座</td>
      <td>
      <table border="0">
        <tbody>
          <tr>
            <td>金融機関名</td>
            <td>：</td>
            <td>　{$bank_name}</td>
          </tr>
          <tr>
            <td>支店名</td>
            <td>：</td>
            <td>　{$store_name}</td>
          </tr>
          <tr>
            <td>振込先科目</td>
            <td>：</td>
            <td>　{$DEPOSIT_L[$deposit]}</td>
          </tr>
          <tr>
            <td>振込先口座番号</td>
            <td>：</td>
            <td>　{$account_num}</td>
          </tr>
          <tr>
            <td>受取人名</td>
            <td>：</td>
            <td>　{$account_name}</td>
          </tr>
        </tbody>
      </table>
      <font color="#ff0000">※お振り込み金額は、お振り込み手数料を引いた金額とさせて頂きます。</font></td>
    </tr>

WAKABA;
	}

	$html .= <<<WAKABA
    <tr bgcolor="#ffffff" align="center">
      <td colspan="2"><input type="submit" value="申請"></td>
    </tr>
  </form>
  </tbody>
</table>
<br>

WAKABA;

	return $html;

}



//	登録処理
function pap_end($mode,$action,$af_num) {
global $PHP_SELF;

	$html = <<<WAKABA
申請完了致しました。<br>
確認のメールをお送り致しましたのでご確認下さい。<br>
それではこれからもよろしくお願い致します。<br>
<br>

WAKABA;

	return $html;

}
?>
