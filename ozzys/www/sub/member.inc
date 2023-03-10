<?PHP
//	会員登録
function member() {

	$idpass = $_SESSION['idpass'];

	$check = 0;
	if ($_POST['mode']) {
		$mode = $_POST['mode'];
		$check = 1;
	}
	elseif ($_GET['mode']) {
		$mode = $_GET['mode'];
	}

	if ($idpass) {
		if ($check == 1) {
			if ($mode == "enter") { c_usercheck($idpass); }		//	ユーザー情報確認
			elseif ($mode == "check") { c_check($idpass); }		//	情報確認
			elseif ($mode == "add") { renew($idpass); }		//	情報更新
		}
		$title = "会員登録情報変更";
		if ($mode == "enter") { $html = first($idpass); }		//	変更画面
		elseif ($mode == "check") {								//	確認画面
			$html = check($idpass);
			$title .= "確認";
		}
		else { $html = usercheck($idpass); }					//	ID,PASS入力画面
	}
	else {
		if ($check == 1) {
			if ($mode == "check") { c_check($idpass); }			//	情報確認
			elseif ($mode == "add") { add(); }					//	新規登録
		}
		$title = "会員登録";
		if ($mode == "enter") { $html = first($idpass); }		//	情報入力画面
		elseif ($mode == "check") {								//	確認画面
			$html = check($idpass);
			$title .= "確認";
		}
		else { $html = enter(); }								//	登録入り口画面
	}

	return array($html,$title);

}


//	登録確認画面
function enter() {

	$html = <<<WAKABA
	<h2 class="sub-title-prod">会員登録</h2>
      <BR>
      <TABLE class="table-resp">
          <TR bgcolor="#ffcc00">
            <TD class="tdpadding"><B>●新規会員登録一時停止のお知らせ</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD class="tdpadding">
ポイントシステムの新システム移行にともない一時新規会員登録の停止をしております。<BR>
ご利用者の皆様にはご御迷惑をお掛けいたしますが何卒ご容赦下さい。<BR>
新ポイントシステムの開始は平成20年4月を予定しております。<BR>
<BR>
尚、現在会員登録されているお客様は従来通りにお買い物を楽しんでいただけます。<BR>
            </TD>
          </TR>
      </TABLE>
      <BR>
WAKABA;

	$html = <<<WAKABA
      <TABLE class="table-resp" id="goods_list">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD><B>●会員特典</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD><FONT color="#ffcc00">●</FONT>登録後のお買い物時、面倒な住所などの入力が不要になります。<BR>
                  <BR>
<!--
                  <FONT color="#ffcc00">●</FONT>ポイントが1,000円お買いあげ毎に、1ポイント付き、50ポイント貯まりますと2,000円分のお買い物ができます。<BR>
                  <BR>
-->
                  <FONT color="#ffcc00">●</FONT>注文の発送状況や、過去の注文内容がご確認できます。<BR>
                  <BR>
                  <FONT color="#ffcc00">●</FONT>希望される方にはメールマガジンを発行させていただきます。</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD>●<B>注意事項</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD><FONT color="#ffcc00">●</FONT>文字を入力する際、各項目で半角入力か全角入力か指定してありますので、それに従って入力してください。<BR>
                  <BR>
                  <FONT color="#ffcc00">●</FONT>赤い文字で「<FONT color="#ff0000">必記</FONT>」と書いてある項目は、必ず入力してください。<BR>
                  <BR>
                  <FONT color="#ffcc00">●</FONT>必要事項を全て記入の上、画面下にある「<B>確認画面へ</B>」というボタンをクリックしてください。<BR>
                  <BR>
                  <FONT color="#ffcc00">●</FONT>登録内容を変更する場合は、ログイン後、左フレームにある「<B>登録変更</B>」から変更を行ってください。</TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
	  <center>
		  <A href="/member.php?mode=enter"><IMG src="/images/touroku.gif" width="121" height="36" border="0" alt="登録画面へ"></A><BR>
	  </center>
      <BR>

WAKABA;

	unset($_SESSION['ERROR']);
	unset($_SESSION['checklist']);

	return $html;

}



//	ID,PASS入力画面
function usercheck($idpass) {
global $PHP_SELF;

	$ERROR = $_SESSION['ERROR'];
	if ($ERROR) {
		$errors = ERROR($ERROR);
		list($c_email,$c_pass) = explode("<>",$_SESSION['checklist']);
	}
	unset($_SESSION['ERROR']);
	unset($_SESSION['checklist']);

	$html = <<<WAKABA
		<h2 class="sub-title-prod">会員登録変更</h2>
$errors
		<TABLE class="table-resp table-resp-nogap" id="goods_list">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>注意事項</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<FONT color="#ffcc00">●</FONT>住所、メールアドレス等登録内容に変更があった場合は、必ず会員登録変更を行ってください。<BR>
				<BR>
				<FONT color="#ffcc00">●</FONT>文字を入力する際、各項目で半角入力か全角入力か指定してありますので、それに従って入力してください。<BR>
				<BR>
				<FONT color="#ffcc00">●</FONT>下の入力欄に確認のため登録時のメールアドレス、パスワードを入力後、「<B>登録変更画面へ</B>」というボタンをクリックして次に進んでください。<BR>
			</TD>
          </TR>
        </TBODY>
		</TABLE>

		<BR>

		<CENTER　class="login-form">
			<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="mode" value="enter">
				<TABLE class="table-resp">
				<TBODY>
					<TR>
						<th>メールアドレス</th>
						<TD><INPUT class="input-block" name="c_email" value="$c_email"></TD>
					</TR>
					<TR>
						<th>パスワード</th>
						<TD><INPUT class="input-block" type="password" name="c_pass" value=""></TD>
					</TR>
				</TBODY>
				</TABLE>
				<BR>
				<center>
					<INPUT type="image" alt="登録編集画面へ" src="/images/touroku_2.gif">
				</center>
			</FORM>
		</CENTER>

		<div class="secession-link">
			<a href="/secession.php">会員脱会の方はこちらから。</a>
		</div>
		
WAKABA;

	return $html;

}



//	ユーザー確認
function c_usercheck($idpass) {
global $PHP_SELF,$db;

	$c_email = $_POST['c_email'];
	## $c_email = mb_convert_kana($c_email,"as","EUC-JP");
	$c_email = mb_convert_kana($c_email,"as","UTF-8");
	$c_email = trim($c_email);
	$c_pass = $_POST['c_pass'];
	## $c_pass = mb_convert_kana($c_pass,"as","EUC-JP");
	$c_pass = mb_convert_kana($c_pass,"as","UTF-8");
	$c_pass = trim($c_pass);
	if (!$c_email) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$c_pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (!$ERROR) {
		$check = 0;
		$sql =	"SELECT kojin_num FROM kojin" .
				" WHERE saku!='1' AND email='$c_email' AND pass='$c_pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			if ($kojin_num) { $check = 1; }
		}
		if ($check == 0) { $ERROR[] = "入力された情報が間違っています。"; }
	}

	$url = "";
	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$_SESSION['checklist'] = "$c_email<>$c_pass<>";
	}
	else {
		$url = "?mode=enter";
	}

	header ("Location: $PHP_SELF$url\n\n");

	exit;

}


function first($idpass) {
global $PHP_SELF,$db,$PRF_L;

	$ERROR = $_SESSION['ERROR'];
	$checklist = $_SESSION['checklist'];
	if ($ERROR) {
		$errors = ERROR($ERROR);
		list($name_s,$name_n,$kana_s,$kana_n,$sex,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$pass1,$pass2) = explode("<>",$checklist);
	}
	elseif ($checklist) {
		list($name_s,$name_n,$kana_s,$kana_n,$sex,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$pass1,$pass2) = explode("<>",$checklist);
	}
	elseif ($idpass) {
		list($email,$pass,$memo) = explode("<>",$idpass);
		$sql =	"SELECT * FROM kojin" .
				" WHERE saku!='1' AND email='$email' AND pass='$pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$kana_s = $list['kana_s'];
			$kana_n = $list['kana_n'];
			$sex = $list['sex'];
			$b_date = $list['b_date'];
			if ($b_date == "1000-01-01") { $b_date = ""; }
			list($b_year,$b_mon,$b_day) = explode("-",$b_date);
			$zip1 = $list['zip1'];
			$zip2 = $list['zip2'];
			$prf = $list['prf'];
			$city = $list['city'];
			$add1 = $list['add1'];
			$add2 = $list['add2'];
			$tel1 = $list['tel1'];
			$tel2 = $list['tel2'];
			$tel3 = $list['tel3'];
			$fax1 = $list['fax1'];
			$fax2 = $list['fax2'];
			$fax3 = $list['fax3'];
			$email = $list['email'];
			$pass = $list['pass'];
			$meruma = $list['meruma'];
		}
	}

	//	性別
	$sex_checked1 = $sex_checked2 = "";
	if ($sex == 1) { $sex_checked1 = "checked"; }
	elseif ($sex == 2) { $sex_checked2 = "checked"; }
	//	誕生日
	if (!$b_mon) { $selected = "selected"; } else { $selected = ""; }
	$l_b_mon = "                    <OPTION value=\"\" $selected>--</OPTION>\n";
	for ($i=1; $i<=12; $i++) {
		if ($b_mon == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_b_mon .= "                    <OPTION value=\"$i\" $selected>$i</OPTION>\n";
	}
	if (!$b_day) { $selected = "selected"; } else { $selected = ""; }
	$l_b_day = "                    <OPTION value=\"\" $selected>--</OPTION>\n";
	for ($i=1; $i<=31; $i++) {
		if ($b_day == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_b_day .= "                    <OPTION value=\"$i\" $selected>$i</OPTION>\n";
	}

	//	都道府県
	if (!$prf) {  $selected = "selected"; } else { $selected = ""; }
	$l_prf = "                    <OPTION value=\"\" $selected>選択して下さい</OPTION>\n";
	for ($i=1; $i<=47; $i++) {
		if ($prf == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_prf .= "                    <OPTION value=\"$i\" $selected>$PRF_L[$i]</OPTION>\n";
	}

	//	メルマガ購読
	$meru_checked1 = $meru_checked2 = "";
	if ($meruma == 1) { $meru_checked1 = "checked"; }
	elseif ($meruma == 2) { $meru_checked2 = "checked"; }

	if ($idpass) {
		$title = "会員登録情報変更";
		$title2 = "お客様の情報変更フォーム";
		$pas_msg = "変更後のパスワード";
		$hiki = "";
		$n_pass_msg = <<<WAKABA
				<h3 class="form-member-title">
				  <B>現在のパスワード</B>
				</h3>
				<p class="form-group">
				  <INPUT class="input-inline" size="16" type="text" name="pass" value="$pass" readonly>
				</p>
				<p class="form-group">
				  <B>パスワードを変更する場合は下記も入力して下さい。</B>
				</p>

WAKABA;
	}
	else {
		$title = "会員登録";
		$title2 = "お客様の情報登録フォーム";
		$pas_msg = "パスワード";
		$hiki = "(必記)";
	}

	$html = <<<WAKABA
	  <h2 class="sub-title-prod">$title</h2>
$errors
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="mode" value="check">
      <TABLE class="table-resp table-resp-nogap table-enter" id="goods_list">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>$title2</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>
			<div class="end-order-container">
				<br>

				<h3 class="form-member-title">
				  <B>漢字氏名</B> (全角)：(例　山田　太郎) <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
					<span class="nowrap">
						姓：<INPUT class="input-inline" name="name_s" value="$name_s">
					</span>
					<span class="nowrap">
						名：<INPUT class="input-inline" name="name_n" value="$name_n">
					</span>
				</div>

				<h3 class="form-member-title">
				  <B>ふりがな</B> (全角)：(例　やまだ　たろう) <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
				  <span class="nowrap">
					  姓：<INPUT class="input-inline" name="kana_s" value="$kana_s">
				  </span>
				  <span class="nowrap">
					  名：<INPUT class="input-inline" name="kana_n" value="$kana_n">
				  </span>
				</div>

				<h3 class="form-member-title">
				  <B>性別</B> <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
				  <label class="nowrap">
					  <INPUT type="radio" name="sex" value="1" $sex_checked1>：男性 
				  </label>
				  <label class="nowrap">
					  <INPUT type="radio" name="sex" value="2" $sex_checked2>：女性
				  </label>
				</div>

				<h3 class="form-member-title">
				  <B>生年月日</B>（半角）：（例　1975年9月9日）
				</h3>
				<div class="form-group">
				  <span class="nowrap">
					  西暦<INPUT class="input-inline" size="8" name="b_year" value="$b_year">年&nbsp;
				  </span>
				  <span class="nowrap">
					  <SELECT class="input-inline" name="b_mon">
	$l_b_mon
					  </SELECT>月 
					  <SELECT class="input-inline" name="b_day">
	$l_b_day
					  </SELECT>日
				  </span>
				</div>

				<h3 class="form-member-title">
				  <B>住所</B> <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
				  <FONT color="#ffcc00">●</FONT><B>郵便番号</B> (半角)：(例　123-4567)<BR>
				  〒<INPUT class="input-inline" size="6" name="zip1" value="$zip1">-<INPUT class="input-inline" size="8" name="zip2" value="$zip2">
				</div>
				<div class="form-group">
				  <B><FONT color="#ffcc00">●</FONT>都道府県</B><BR>
				  <SELECT class="input-inline" name="prf">
$l_prf
				  </SELECT>
				</div>
				<div class="form-group">
				  <FONT color="#ffcc00">●</FONT><B>市区町村</B> (全角)：(例　太田市)<BR>
				  <INPUT class="input-block" name="city" value="$city">
				</div>
				<div class="form-group">
				  <FONT color="#ffcc00">●</FONT><B>所番地</B> (全角)：(例　浜町　６３-３１)<BR>
				  <INPUT class="input-block" name="add1" value="$add1">
				</div>
				<div class="form-group">
				  <FONT color="#ffcc00">●</FONT><B>マンション名など</B>（全角）：（例　OZZYSマンション３０２号）<BR>
				  <INPUT class="input-block" name="add2" value="$add2">
				</div>

				<h3 class="form-member-title">
				  <B>電話番号</B>（半角）：（例　0276-49-2021） <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
					<INPUT type="text" class="input-inline input-phone" size="6" name="tel1" value="$tel1">
					<span class="sep-phone">-</span>
					<INPUT type="text" class="input-inline input-phone" size="6" name="tel2" value="$tel2">
					<span class="sep-phone">-</span>
					<INPUT type="text" class="input-inline input-phone" size="6" name="tel3" value="$tel3">
				</div>

				<h3 class="form-member-title">
				  <B>FAX番号</B>（半角）：（例　0276-49-2021）
				</h3>
				<div class="form-group">
					<INPUT type="text" class="input-inline input-phone" size="6" name="fax1" value="$fax1">
					<span class="sep-phone">-</span>
					<INPUT type="text" class="input-inline input-phone" size="6" name="fax2" value="$fax2">
					<span class="sep-phone">-</span>
					<INPUT type="text" class="input-inline input-phone" size="6" name="fax3" value="$fax3">
				</div>

				<h3 class="form-member-title">
				  <B>E-mailアドレス</B>（半角）：（例　info@ozzys.jp） <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
				  <INPUT class="input-block" name="email" value="$email">
				</div>

$n_pass_msg

				<h3 class="form-member-title">
				  <B>$pas_msg</B>（半角英数字６文字以上８文字以内） <FONT color="#ff0000">$hiki</FONT>
				</h3>
				<div class="form-group">
				  <INPUT class="input-inline" type="text" size="16" name="pass1" value="$pass1">
				</div>

				<h3 class="form-member-title">
				  <B>$pas_msg</B>（確認の為もう一度入力して下さい。） <FONT color="#ff0000">$hiki</FONT>
				</h3>
				<div class="form-group">
				  <INPUT class="input-inline" type="text" size="16" name="pass2" value="$pass2">
				</div>

				<h3 class="form-member-title">
				  <B>メールマガジン購読</B> <FONT color="#ff0000">(必記)</FONT>
				</h3>
				<div class="form-group">
				  <label class="nowrap">
					  <INPUT type="radio" name="meruma" value="1" $meru_checked1>：購読する
				  </label>
				  <label class="nowrap">
					  <INPUT type="radio" name="meruma" value="2" $meru_checked2>：購読しない
				  </label>
				</div>

				<center>
					<button class="btn btn-submit" type="submit">確認画面へ</button>　<button class="btn btn-reset" type="reset">リセット</button>
				</center>
			</div>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      </FORM>

WAKABA;

	return $html;

}



//	登録内容チェック
function c_check($idpass) {
global $PHP_SELF,$db;

	$name_s = $_POST['name_s'];
	## $name_s = mb_convert_kana($name_s,"AsKV","EUC-JP");
	$name_s = mb_convert_kana($name_s,"AsKV","UTF-8");
	$name_s = trim($name_s);
	$name_n = $_POST['name_n'];
	## $name_n = mb_convert_kana($name_n,"AsKV","EUC-JP");
	$name_n = mb_convert_kana($name_n,"AsKV","UTF-8");
	$name_n = trim($name_n);
	$kana_s = $_POST['kana_s'];
	## $kana_s = mb_convert_kana($kana_s,"AsHV","EUC-JP");
	$kana_s = mb_convert_kana($kana_s,"AsHV","UTF-8");
	$kana_s = trim($kana_s);
	$kana_n = $_POST['kana_n'];
	## $kana_n = mb_convert_kana($kana_n,"AsHV","EUC-JP");
	$kana_n = mb_convert_kana($kana_n,"AsHV","UTF-8");
	$kana_n = trim($kana_n);
	$sex = $_POST['sex'];
	$b_year = $_POST['b_year'];
	## $b_year = mb_convert_kana($b_year,"na","EUC-JP");
	$b_year = mb_convert_kana($b_year,"na","UTF-8");
	$b_year = trim($b_year);
	$b_mon = $_POST['b_mon'];
	$b_day = $_POST['b_day'];
	$zip1 = $_POST['zip1'];
	## $zip1 = mb_convert_kana($zip1,"na","EUC-JP");
	$zip1 = mb_convert_kana($zip1,"na","UTF-8");
	$zip1 = trim($zip1);
	$zip2 = $_POST['zip2'];
	## $zip2 = mb_convert_kana($zip2,"na","EUC-JP");
	$zip2 = mb_convert_kana($zip2,"na","UTF-8");
	$zip2 = trim($zip2);
	$prf = $_POST['prf'];
	$city = $_POST['city'];
	## $city = mb_convert_kana($city,"AsKV","EUC-JP");
	$city = mb_convert_kana($city,"AsKV","UTF-8");
	$city = trim($city);
	$add1 = $_POST['add1'];
	## $add1 = mb_convert_kana($add1,"AsKV","EUC-JP");
	$add1 = mb_convert_kana($add1,"AsKV","UTF-8");
	$add1 = trim($add1);
	$add2 = $_POST['add2'];
	## $add2 = mb_convert_kana($add2,"AsKV","EUC-JP");
	$add2 = mb_convert_kana($add2,"AsKV","UTF-8");
	$add2 = trim($add2);
	$tel1 = $_POST['tel1'];
	## $tel1 = mb_convert_kana($tel1,"ns","EUC-JP");
	$tel1 = mb_convert_kana($tel1,"ns","UTF-8");
	$tel1 = trim($tel1);
	$tel2 = $_POST['tel2'];
	## $tel2 = mb_convert_kana($tel2,"ns","EUC-JP");
	$tel2 = mb_convert_kana($tel2,"ns","UTF-8");
	$tel2 = trim($tel2);
	$tel3 = $_POST['tel3'];
	## $tel3 = mb_convert_kana($tel3,"ns","EUC-JP");
	$tel3 = mb_convert_kana($tel3,"ns","UTF-8");
	$tel3 = trim($tel3);
	$fax1 = $_POST['fax1'];
	## $fax1 = mb_convert_kana($fax1,"ns","EUC-JP");
	$fax1 = mb_convert_kana($fax1,"ns","UTF-8");
	$fax1 = trim($fax1);
	$fax2 = $_POST['fax2'];
	## $fax2 = mb_convert_kana($fax2,"ns","EUC-JP");
	$fax2 = mb_convert_kana($fax2,"ns","UTF-8");
	$fax2 = trim($fax2);
	$fax3 = $_POST['fax3'];
	## $fax3 = mb_convert_kana($fax3,"ns","EUC-JP");
	$fax3 = mb_convert_kana($fax3,"ns","UTF-8");
	$fax3 = trim($fax3);
	$email = $_POST['email'];
	## $email = mb_convert_kana($email,"as","EUC-JP");
	$email = mb_convert_kana($email,"as","UTF-8");
	$email = trim($email);
	$email = strtolower($email);
	$pass = $_POST['pass'];
	## $pass = mb_convert_kana($pass,"as","EUC-JP");
	$pass = mb_convert_kana($pass,"as","UTF-8");
	$pass = trim($pass);
	$meruma = $_POST['meruma'];
	$pass1 = $_POST['pass1'];
	## $pass1 = mb_convert_kana($pass1,"as","EUC-JP");
	$pass1 = mb_convert_kana($pass1,"as","UTF-8");
	$pass1 = trim($pass1);
	$pass2 = $_POST['pass2'];
	## $pass2 = mb_convert_kana($pass2,"as","EUC-JP");
	$pass2 = mb_convert_kana($pass2,"as","UTF-8");
	$pass2 = trim($pass2);

	if (!$name_s) { $ERROR[] = "漢字氏名（姓）が入力されておりません。"; }
	if (!$name_n) { $ERROR[] = "漢字氏名（名）が入力されておりません。"; }
	if (!$kana_s) { $ERROR[] = "ひらがな氏名（姓）が入力されておりません。"; }
	if (!$kana_n) { $ERROR[] = "ひらがな氏名（名）が入力されておりません。"; }
	if (!$sex) { $ERROR[] = "性別が選択されておりません。"; }
	$b_year_n = strlen($b_year);
##	if ($b_year && (eregi("[^0-9]",$b_year) || ($b_year_n != 4))) { $ERROR[] = "生年月日の年が不正です。"; }
	if ($b_year && (preg_match("/[^0-9]/i",$b_year) || ($b_year_n != 4))) { $ERROR[] = "生年月日の年が不正です。"; }
	if (!$zip1) { $ERROR[] = "郵便番号３桁が入力されておりません。"; }
	$zip1_n = strlen($zip1);
##	if ($zip1 && (eregi("[^0-9]",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
	if ($zip1 && (preg_match("/[^0-9]/i",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
	if (!$zip2) { $ERROR[] = "郵便番号４桁が入力されておりません。"; }
	$zip2_n = strlen($zip2);
##	if ($zip2 && (eregi("[^0-9]",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
	if ($zip2 && (preg_match("/[^0-9]/i",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
	if (!$prf) { $ERROR[] = "都道府県名が選択されておりません。"; }
	if (!$city) { $ERROR[] = "市区町村名が入力されておりません。"; }
	if (!$add1) { $ERROR[] = "所番地が入力されておりません。"; }
	if (!$tel1 || !$tel2 || !$tel3) { $ERROR[] = "電話番号が入力されておりません。"; }
//	if (!$fax1 || !$fax2 || !$fax3) { $ERROR[] = "FAX番号が入力されておりません。"; }
	if (!$email) { $ERROR[] = "E-mailアドレスが入力されておりません。"; }
##	if ($email && !ereg("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	$mail_host = $regs[1];
//	if ($email &&!getmxrr($mail_host,$mxhostarr)) { $ERROR[] = "E-mailアドレスのホスト名が見つかりませんでした。"; }
	if (!$meruma) { $ERROR[] = "メールマガジン購読する、購読しないが選択されておりません。"; }
	if ($idpass) {
		$pass_n = strlen($pass1);
		if ($pass1 && ($pass_n < 6 || $pass_n > 8)) { $ERROR[] = "パスワードの文字数が不正です。"; }
		if (($pass1 && !$pass2) || (!$pass1 && $pass2)) { $ERROR[] = "変更後のパスワードは２カ所入力してください。"; }
		if ($pass1 != $pass2) { $ERROR[] = "入力された変更後のパスワードが一致しません。"; }
	}
	else {
		$pass_n = strlen($pass1);
		if ($pass1 && ($pass_n < 6 || $pass_n > 8)) { $ERROR[] = "パスワードの文字数が不正です。"; }
		if (($pass1 && !$pass2) || (!$pass1 && $pass2)) { $ERROR[] = "確認のためパスワードは２カ所入力してください。"; }
		if ($pass1 != $pass2) { $ERROR[] = "入力されたパスワードが一致しません。"; }
		else { $pass = $pass1; }
	}

	if (!$ERROR && !$idpass) {
		$sql = "SELECT * FROM kojin WHERE email='$email' AND saku='0' AND kojin_num BETWEEN 100001 AND 600000;";
		if ($result = pg_query($db,$sql)) {
			$count = pg_numrows($result);
			if ($count >= 1) { $ERROR[] = "既にそのE-mailアドレスは登録されております。"; }
		}
	}

	if ($ERROR) { $_SESSION['ERROR'] = $ERROR; }
	$_SESSION['checklist'] = "$name_s<>$name_n<>$kana_s<>$kana_n<>$sex<>$b_year<>$b_mon<>$b_day<>$zip1<>$zip2<>$prf<>$city<>$add1<>$add2<>$tel1<>$tel2<>$tel3<>$fax1<>$fax2<>$fax3<>$email<>$pass<>$meruma<>$pass1<>$pass2<>";

	if ($ERROR) { $url = "?mode=enter"; }
	else { $url = "?mode=check"; }

	header ("Location: $PHP_SELF$url\n\n");

	exit;

}



//	情報確認画面
function check($idpass) {
global $PHP_SELF,$SEX_L,$PRF_L,$MERUMA_L;

	if ($_SESSION['checklist']) {
		$checklist = $_SESSION['checklist'];
		list($name_s,$name_n,$kana_s,$kana_n,$sex,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$pass1,$pass2) = explode("<>",$checklist);
	}
	else {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($idpass) {
		$title = "会員登録情報変更確認";
		$title2 = "お客様の情報変更確認";
	}
	else {
		$title = "会員登録";
		$title2 = "お客様の情報登録確認";
	}

	$birthday = "西暦" . $b_year . "年 " . $b_mon . "月 " . $b_day . "日";
	$zip = "〒" . $zip1 . "-" . $zip2;
	$tel = $tel1 . "-" . $tel2 . "-" . $tel3;
	$fax = $fax1 . "-" . $fax2 . "-" . $fax3;
	if ($pass1) { $pass = $pass1; }

	$html = <<<WAKABA
	  <h2 class="sub-title-prod">$title</h2>

	  <TABLE class="table-resp table-resp-nogap table-enter">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>$title2</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<div class="end-order-container">

					<h3 class="form-member-title"> <B>漢字氏名</B> </h3>
					<div class="form-group">
					　$name_s $name_n
					</div>

					<h3 class="form-member-title"> <B>ふりがな</B>
					</h3>
					<div class="form-group">
					　$kana_s $kana_n
					</div>

					<h3 class="form-member-title"> <B>性別</B> </h3>
					<div class="form-group">
					　$SEX_L[$sex]
					</div>

					<h3 class="form-member-title"> <B>生年月日</B> </h3>
					<div class="form-group">
					　$birthday
					</div>

					<h3 class="form-member-title"> <B>住所</B> </h3>
					<div class="form-group">
					　$zip<BR>　$PRF_L[$prf]$city$add1$add2
					</div>

					<h3 class="form-member-title"> <B>電話番号</B> </h3>
					<div class="form-group">
					　$tel
					</div>

					<h3 class="form-member-title"> <B>FAX番号</B> </h3>
					<div class="form-group">
					　$fax
					</div>

					<h3 class="form-member-title"> <B>E-mailアドレス</B> </h3>
					<div class="form-group">
					　$email
					</div>

					<h3 class="form-member-title"> <B>パスワード</B> </h3>
					<div class="form-group">
					　$pass
					</div>

					<h3 class="form-member-title"> <B>メールマガジン購読</B> </h3>
					<div class="form-group">
					　$MERUMA_L[$meruma]
					</div>

					<center>
						<FORM action="$PHP_SELF" method="POST">
							<INPUT type="hidden" name="mode" value="add">
							<button class="btn btn-submit" type="submit">登録</button>　
							　<button class="btn btn-reset" onclick="history.back()" type="button">戻る</button>
						</FORM>
					</center>
				</div>
			</TD>
		</TR>
		</TBODY>
	</TABLE>


WAKABA;

	return $html;

}



//	新規登録
function add() {
global $PHP_SELF,$db,$SEX_L,$PRF_L,$MERUMA_L,$admin_mail,$admin_name,$m_footer;

	if ($_SESSION['checklist']) {
		$checklist = $_SESSION['checklist'];
		list($name_s,$name_n,$kana_s,$kana_n,$sex,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$pass1,$pass2) = explode("<>",$checklist);
	}
	else {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($b_year <= 1000) { $b_year = "1000"; }
	if ($b_mon < 1 || $b_mon > 12) { $b_mon = "01"; }
	if ($b_day < 1 || $b_day > 31) { $b_day = "01"; }
	$b_date = sprintf ("%04d-%02d-%02d",$b_year,$b_mon,$b_day);
	if ($b_date == '0000-00-00') { $b_date = "1000-01-01"; }

	$sql = "SELECT MAX(kojin_num) AS max from kojin WHERE kojin_num<='600000';";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$max = $list['max'];
	}
	if ($max < 100001) { $kojin_num = 100001; }
	else { $kojin_num = $max + 1; }

	$sql  = "insert into kojin values (" .
			"'$kojin_num','$name_s','$name_n','$kana_s','$kana_n','$sex','$b_date','$zip1','$zip2','$prf','$city'," .
			"'$add1','$add2','$tel1','$tel2','$tel3','$fax1','$fax2','$fax3','$email','$pass','now()'," .
			"'$meruma','0','0');";
	if (!$result = pg_query($db,$sql)) { $ERROR[] = "お客様情報を登録できませんでした。"; }

	if (!$ERROR) {
		//	メール作成
		$t_msg = <<<OZZYS
$name_s 様 会員登録有り難う御座います。
皆様に喜んでいただけるようなサイトを目指してがんばって
いきますのでこれからもよろしくお願いします。
登録内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがホームページの
会員登録変更で修正お願い致します。
OZZYS;

		//	会員メール送信
		$subject = "会員登録確認 - ozzys -";

		$msr = <<<OZZYS
$subject
$t_msg
------------------------------------------------------
漢字氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

性別
　$SEX_L[$sex]

生年月日
　$b_year 年 $b_mon 月 $b_day 日

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email

パスワード
　$pass

メールマガジン
　$MERUMA_L[$meruma]

$m_footer
OZZYS;

		$send_email = $admin_mail;
		$send_name = $admin_name;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		//	受け取り用
		$subject = "会員登録されました。(No.$kojin_num) - ozzys -";

		$addr = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($addr);
		if (!$host) { $host = $addr; }

		$msr = <<<OZZYS
$name_s $name_n 様が $subject
------------------------------------------------------
漢字氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

性別
　$SEX_L[$sex]

生年月日
　$b_year 年 $b_mon 月 $b_day 日

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email

パスワード
　$pass

メールマガジン
　$MERUMA_L[$meruma]

------------------------------------------------------
$host ($addr)
OZZYS;

		$send_email = $email;
		$send_name = "$name_s $name_n";
		$email = $admin_mail;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
	}

	if (!$ERROR) {
		unset($_SESSION['ERROR']);
		unset($_SESSION['checklist']);
	}

	if ($ERROR) {
		$url = "$PHP_SELF?mode=enter";
	}
	else {
		$url = "/endhtml/thank.htm";
	}

	header ("Location: $url\n\n");

	exit;

}



//	変更登録
function renew($idpass) {
global $PHP_SELF,$db,$SEX_L,$PRF_L,$MERUMA_L,$admin_mail,$admin_name,$m_footer;

	if ($_SESSION['checklist'] && $idpass) {
		$checklist = $_SESSION['checklist'];
		list($name_s,$name_n,$kana_s,$kana_n,$sex,$b_year,$b_mon,$b_day,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$pass,$meruma,$pass1,$pass2) = explode("<>",$checklist);
		list($email_,$pass_,$memo_) = explode("<>",$idpass);
	}
	else {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($b_year <= 1000) { $b_year = "1000"; }
	if ($b_mon < 1 || $b_mon > 12) { $b_mon = "01"; }
	if ($b_day < 1 || $b_day > 31) { $b_day = "01"; }
	$b_date = sprintf ("%04d-%02d-%02d",$b_year,$b_mon,$b_day);
	if ($b_date == '0000-00-00') { $b_date = "1000-01-01"; }
	if ($pass1) { $pass = $pass1; }

	$sql  = "SELECT kojin_num FROM kojin" .
			" WHERE saku!='1' AND email='$email_' AND pass='$pass_' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$kojin_num = $list['kojin_num'];
	}
	if (!$kojin_num) { $ERROR[] = "お客様登録情報が確認できません。"; }
	if (!$ERROR) {
		$sql  = "UPDATE kojin SET " .
				"name_s='$name_s'," .
				"name_n='$name_n'," .
				"kana_s='$kana_s'," .
				"kana_n='$kana_n'," .
				"sex='$sex'," .
				"b_date='$b_date'," .
				"zip1='$zip1'," .
				"zip2='$zip2'," .
				"prf='$prf'," .
				"city='$city'," .
				"add1='$add1'," .
				"add2='$add2'," .
				"tel1='$tel1'," .
				"tel2='$tel2'," .
				"tel3='$tel3'," .
				"fax1='$fax1'," .
				"fax2='$fax2'," .
				"fax3='$fax3'," .
				"email='$email'," .
				"pass='$pass'," .
				"meruma='$meruma'" .
				" WHERE kojin_num='$kojin_num';";
		if (!$result = pg_query($db,$sql)) { $ERROR[] = "お客様情報を更新できませんでした。"; }
	}

	if (!$ERROR) {
		//	メール作成
		$t_msg = <<<OZZYS
$name_s 様 登録変更内容は以下でよろしいでしょうか？
もし間違いがある場合は、お手数ですがホームページの
会員登録変更で修正お願い致します。
皆様に喜んでいただけるようなサイトを目指してがんばって
いきますのでこれからもよろしくお願いします。
OZZYS;

		//	会員メール送信
		$subject = "会員登録情報変更確認 - ozzys -";

		$msr = <<<OZZYS
$subject
$t_msg
------------------------------------------------------
漢字氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

性別
　$SEX_L[$sex]

生年月日
　$b_year 年 $b_mon 月 $b_day 日

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email

パスワード
　$pass

メールマガジン
　$MERUMA_L[$meruma]

$m_footer
OZZYS;

		$send_email = $admin_mail;
		$send_name = $admin_name;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);

		//	受け取り用
		$subject = "会員登録情報変更されました。(No.$kojin_num) - ozzys -";

		$addr = getenv("REMOTE_ADDR");
		$host = gethostbyaddr($addr);
		if (!$host) { $host = $addr; }

		$msr = <<<OZZYS
$name_s $name_n 様が $subject
------------------------------------------------------
漢字氏名
　$name_s $name_n

ふりがな
　$kana_s $kana_n

性別
　$SEX_L[$sex]

生年月日
　$b_year 年 $b_mon 月 $b_day 日

住所
　〒$zip1-$zip2
　$PRF_L[$prf] $city $add1 $add2

電話番号
　$tel1 - $tel2 - $tel3

FAX番号
　$fax1 - $fax2 - $fax3

E-mailアドレス
　$email

パスワード
　$pass

メールマガジン
　$MERUMA_L[$meruma]

------------------------------------------------------
$host ($addr)
OZZYS;

		$send_email = $email;
		$send_name = "$name_s $name_n";
		$email = $admin_mail;
		send_email($send_email,$send_name,$mail_get,$get_email,$email,$subject,$msr);
	}

	if (!$ERROR) {
		unset($_SESSION['ERROR']);
		unset($_SESSION['checklist']);
	}
	else {
		$idpass = "$send_email<>$pass<>$memo_<>";
		$_SESSION['idpass'] = $idpass;
	}

	if ($ERROR) {
		$url = "$PHP_SELF?mode=enter";
	}
	else {
		$url = "/endhtml/thank_hen.htm";
	}

	header ("Location: $url\n\n");

	exit;

}



//	エラー表示
function ERROR($ERROR) {

	foreach ($ERROR AS $VAL) {
		if ($VAL) { $err .= "・$VAL<BR>\n"; }
	}

	$errors = <<<WAKABA
      <TABLE border="0" width="95%" id="goods_list">
        <TBODY>
          <TR>
            <TD>
            <B><FONT color="#ff0000">エラー</FONT></B><BR>
$err
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	return $errors;

}
?>
