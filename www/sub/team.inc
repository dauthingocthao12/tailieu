<?PHP
function main() {

	$team = <<<ALPHA
      <BR>
      <BR>
      <BR>
      商品を選択してきて下さい。<BR>
      <BR>
ALPHA;

	return $team;

}


function order() {
global $PHP_SELF,$team,$modes,$customer,$seban_l,$sename_l,$muneban_l,$pant_l,$bach_l,$t_addr,$t_opt,$t_type,$p_set,$s_set,
		$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$msr;

	include "./sub/array.inc";

	unset($KAGO);
	$customer = $_SESSION['customer'];
##	$KAGO = split("<>",$customer);
	$KAGO = explode("<>",$customer);
	sort($KAGO);

	if ($t_opt) {
##		$m_goods = split("<->",$t_opt);
		$m_goods = explode("<->",$t_opt);
		foreach ($m_goods AS $val) {
			if (!$val) { continue; }
##			list($hinban,$gl) = split("<>",$val);
			list($hinban,$gl) = explode("<>",$val);
##			$hinban_h = ereg_replace("-","_",$hinban);
			$hinban_h = preg_replace("/-/","_",$hinban);
##			$ban = split("<=>",$gl);
			$ban = explode("<=>",$gl);


			foreach ($ban AS $val2) {
##				list($num,$u_ban,$u_name) = split("::",$val2);
				list($num,$u_ban,$u_name) = explode("::",$val2);
				$names = "$hinban_h" . "_ban_" . "$num";
				$GLOBALS["$names"] = $u_ban;
				$names = "$hinban_h" . "_name_" . "$num";
				$GLOBALS["$names"] = $u_name;
			}
		}
	}

	if ($t_addr) {
##		list($name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$msr) = split("<>",$t_addr);
		list($name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$msr) = explode("<>",$t_addr);
##		$msr = eregi_replace("<BR>","\n",$msr);
		$msr = preg_replace("/<BR>/i","\n",$msr);
	}

	if ($t_type) {
##		list($seban_l,$sename_l,$muneban_l,$pant_l,$bach_l,$p_set,$s_set) = split("<>",$t_type);
		list($seban_l,$sename_l,$muneban_l,$pant_l,$bach_l,$p_set,$s_set) = explode("<>",$t_type);
	}

	//	背番号
	if (!$seban_l) { $selected_seban = "selected"; } else { $selected_seban = ""; }
	$max = count($SEBAN_N);
	for($i=1; $i<$max; $i++) {
		if ($seban_l == $i) { $selected = "selected"; } else { $selected = ""; }
		$price_t = number_format($SEBAN_P_N[$i]);
		$seban_list .= "              <OPTION value=\"$i\" $selected>$SEBAN_N[$i]：$price_t 円/1文字</OPTION>\n";
	}

	//	背ネーム
	if (!$sename_l) { $selected_seban = "selected"; } else { $selected_seban = ""; }
	$max = count($SENAME_N);
	for($i=1; $i<$max; $i++) {
		if ($sename_l == $i) { $selected = "selected"; } else { $selected = ""; }
		$price_t = number_format($SENAME_P_N[$i]);
		$sename_list .= "              <OPTION value=\"$i\" $selected>$SENAME_N[$i]：$price_t 円/1文字</OPTION>\n";
	}

	//	胸番号
	if (!$muneban_l) { $selected_muneban = "selected"; } else { $selected_muneban = ""; }
	$max = count($MUNEBAN_N);
	for($i=1; $i<$max; $i++) {
		if ($muneban_l == $i) { $selected = "selected"; } else { $selected = ""; }
		$price_t = number_format($MUNEBAN_P_N[$i]);
		$muneban_list .= "              <OPTION value=\"$i\" $selected>$MUNEBAN_N[$i]：$price_t 円/1文字</OPTION>\n";
	}

	//	パンツ番号
	if (!$pant_l) { $selected_pant = "selected"; } else { $selected_pant = ""; }
	$max = count($PANT_N);
	for($i=1; $i<$max; $i++) {
		if ($pant_l == $i) { $selected = "selected"; } else { $selected = ""; }
		$price_t = number_format($PANT_P_N[$i]);
		$pant_list .= "              <OPTION value=\"$i\" $selected>$PANT_N[$i]：$price_t 円/1文字</OPTION>\n";
	}

	//	バッジ
	if (!$bach_l) { $selected_bach = "selected"; } else { $selected_bach = ""; }
	$max = count($BACH_N);
	for($i=1; $i<$max; $i++) {
		if ($bach_l == $i) { $selected = "selected"; } else { $selected = ""; }
		$price_t = number_format($BACH_P_N[$i]);
		$bach_list .= "              <OPTION value=\"$i\" $selected>$BACH_N[$i]：$price_t 円</OPTION>\n";
	}

	$team .= <<<ALPHA
      <FORM action="$PHP_SELF" method="POST">
      <INPUT type="hidden" name="modes" value="kaku">
      <table width="780px">
          <TR>
            <th colspan="2" class="cate2" height="15"><B>御見積フォーム</B></TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
          <TR>
            <th class="cate2">背番号タイプ</th>
            <TD width="450" bgcolor="#ffffff">
             <SELECT name="seban_l">
              <OPTION value="" $selected_seban>無し</OPTION>

$seban_list
            </SELECT></TD>
          </TR>
          <TR>
           <th class="cate2">背ネームタイプ</th>
            <TD bgcolor="#ffffff">
             <SELECT name="sename_l">
              <OPTION value="" $selected_sename>無し</OPTION>

$sename_list
            </SELECT></TD>
          </TR>
          <TR>
            <th class="cate2">胸番号タイプ</th>
            <TD bgcolor="#ffffff">
             <SELECT name="muneban_l">
              <OPTION value="" $selected_muneban>無し</OPTION>

$muneban_list
            </SELECT></TD>
          </TR>
          <TR>
            <th class="cate2">パンツ番号タイプ</th>
            <TD bgcolor="#ffffff">
             <SELECT name="pant_l">
              <OPTION value="" $selected_pant>無し</OPTION>

$pant_list
            </SELECT></TD>
          </TR>
          <TR>
            <th class="cate2">バッジタイプ</th>
            <TD bgcolor="#ffffff">
             <SELECT name="bach_l">
              <OPTION value="" $selected_bach>無し</OPTION>

$bach_list
            </SELECT></TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
ALPHA;

	foreach ($KAGO AS $val) {
		if (!$val) { continue; }
##		list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
		list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
##		list($typ) = split("-",$hinban_);
		list($typ) = explode("-",$hinban_);
//		if ($typ != "A") { continue; }

##		$hinban_h = ereg_replace("-","_",$hinban_);
		$hinban_h = preg_replace("/-/","_",$hinban_);

		$team .= <<<ALPHA
          <TR>
            <th colspan="2" height="15" class="cate2">
            商品番号：$hinban_ <BR>
            商 品 名 ：$title_ <BR>
            数　　　量：$num_ </TD>
          </TR>
ALPHA;

//		if ($typ == "A") {
			$team .= <<<ALPHA
          <TR>
            <th class="cate2">番号</th>
            <TD bgcolor="#ffffff">

ALPHA;

			for($i=1; $i<=$num_; $i++) {
				$names = "$hinban_h" . "_ban_" . "$i";
				$values = $GLOBALS["$names"];
				$team .= "            $i:<INPUT size=\"6\" type=\"text\" maxlength=\"3\" name=\"$names\" value=\"$values\"> \n";
				$amari = $i % 5;
				if ($amari == 0) { $team .= "            <BR>\n"; }
			}

			$team .=<<<ALPHA
            </TD>
          </TR>
          <TR>
            <th class="cate2">背ネーム</th>
            <TD bgcolor="#ffffff">

ALPHA;

			for($i=1; $i<=$num_; $i++) {
				$names = "$hinban_h" . "_name_" . "$i";
				$values = $GLOBALS["$names"];
				$team .= "            $i:<INPUT size=\"40\" type=\"text\" maxlength=\"20\" name=\"$names\" value=\"$values\"><BR> \n";
			}

			$team .=<<<ALPHA
            </TD>
          </TR>
ALPHA;

//		}

		$team .=<<<ALPHA
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
ALPHA;

	}

	if ($p_set == 1) { $checked_p = "checked"; } else { $checked_p = ""; }
	if ($s_set == 1) { $checked_s = "checked"; } else { $checked_s = ""; }

	$team .= <<<ALPHA
<!--
          <TR>
            <th class="cate2">パンツ</th>
            <TD bgcolor="#ffffff"><INPUT type="checkbox" name="p_set" value="1" $checked_p>：パンツをセットで申し込む場合はチェックを入れて下さい。</TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="2"></TD>
          </TR>
          <TR>
            <th class="cate2">ソックス</th>
            <TD bgcolor="#ffffff"><INPUT type="checkbox" name="s_set" value="1" $checked_s>：ソックスをセットで申し込む場合はチェックを入れて下さい。</TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="2"></TD>
          </TR>
-->
        </TBODY>
      </TABLE>
      <BR>
      <table width="780px">
        <TBODY>
          <TR>
            <th height="15" class="cate2"><B>御連絡先</B></TD>
          </TR>
          <TR>
            <TD bgcolor="#ffffff"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD align="center" valign="middle">
            <BR>
            <TABLE width="580" border="0" class="category2">
                <TR>
                  <th class="cate2" height="20"><B>漢字氏名</B>（全角）：（例　山田　太郎）<FONT color="#ff0000">（必記）</FONT></th>
                </TR>
                <TR>
                  <TD>　姓：<INPUT name="name_s" value="$name_s">　名：<INPUT name="name_n" value="$name_n"></TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>ふりがな</B>（全角）：（例　やまだ　たろう）<FONT color="#ff0000">（必記）</FONT></th>
                </TR>
                <TR>
                  <TD>　姓：<INPUT name="kana_s" value="$kana_s">　名：<INPUT name="kana_n" value="$kana_n"></TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>住所</B><FONT color="#ff0000">（必記）</FONT></TD>
                </TR>
                <TR>
                  <TD>
                  <TABLE border="0" class="category2">
                      <TR>
                        <TD>　●<B>郵便番号</B>（半角）：（例 999-9999）</TD>
                      </TR>
                      <TR>
                        <TD>　〒<INPUT maxlength="3" size="6" name="zip1" value="$zip1">－<INPUT maxlength="4" size="8" name="zip2" value="$zip2"></TD>
                      </TR>
                      <TR>
                        <TD>　●<B>都道府県</B></TD>
                      </TR>
                      <TR>
                        <TD>　<SELECT name="prf">
ALPHA;

	if (!$prf) { $selected = "selected"; } else { $selected = ""; }

	$team .= "                          <OPTION value=\"\" $selected>選択して下さい。</OPTION>\n";

	for($i=1; $i<=47; $i++) {
		if ($i == $prf) { $selected = "selected"; } else { $selected = ""; }
		$team .= "                          <OPTION value=\"$i\" $selected>$PRF_N[$i]</OPTION>\n";
	}

	$team .= <<<ALPHA
                        </SELECT></TD>
                      </TR>
                      <TR>
                        <TD>　●<B>市区町村名</B>（全角）：（例　千代田区）</TD>
                      </TR>
                      <TR>
                        <TD>　<INPUT size="40" name="city" value="$city"></TD>
                      </TR>
                      <TR>
                        <TD>　●<B>所番地</B>（全角）：（例　永田町９－９－９）</TD>
                      </TR>
                      <TR>
                        <TD>　<INPUT size="40" name="add1" value="$add1"></TD>
                      </TR>
                      <TR>
                        <TD>　●<B>マンション名など</B>（全角）：（ネイバーズマンション　１０１号室）</TD>
                      </TR>
                      <TR>
                        <TD>　<INPUT size="40" name="add2" value="$add2"></TD>
                      </TR>
                  </TABLE>
                  </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20">
                  <B>電話番号</B>（半角）：（例　012-345-6789）<FONT color="#ff0000">（必記）</FONT>
                  </th>
                </TR>
                <TR>
                  <TD>　<INPUT size="8" name="tel1" value="$tel1">－<INPUT size="8" name="tel2" value="$tel2">－<INPUT size="8" name="tel3" value="$tel3"></TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>FAX番号</B>（半角）：（例　012-345-6789）</th>
                </TR>
                <TR>
                  <TD>　<INPUT size="8" name="fax1" value="$fax1">－<INPUT size="8" name="fax2" value="$fax2">－<INPUT size="8" name="fax3" value="$fax3"></TD>
                </TR>
                <TR>
                  <th class="cate2" height="20">
                  <B>メールアドレス</B>（半角）：（例　query@futboljersey.com）<FONT color="#ff0000">（必記）</FONT>
                  </th>
                </TR>
                <TR>
                  <TD>　<INPUT size="40" name="email" value="$email"></TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>ご意見ご要望など</B></th>
                </TR>
                <TR>
                  <TD>　 <TEXTAREA name="msr" rows="10" cols="60">$msr</TEXTAREA></TD>
                </TR>
            </TABLE>
            </TD>
          </TR>
      </TABLE>
      <BR>
      <table width="780px">
          <TR>
            <TD align="center" bgcolor="#ffffff"><INPUT class="button" type="submit" value="確認画面へ" class="button">　<INPUT class="button" type="reset" value="取り消し" class="button"></TD>
          </TR>
      </TABLE>
      </FORM>
      <BR>
ALPHA;

	return $team;

}


function kaku() {
global $PHP_SELF,$team,$modes,$customer,$seban_l,$sename_l,$muneban_l,$pant_l,$bach_l,$t_addr,$t_opt,$t_type,$ERROR,$p_set,$s_set,
		$name_s,$name_n,$kana_s,$kana_n,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$email,$msr;

	include "./sub/array.inc";

#	$zip1 = mb_convert_kana($zip1,n,"EUC-JP");
#	$zip2 = mb_convert_kana($zip2,n,"EUC-JP");
#	$tel1 = mb_convert_kana($tel1,n,"EUC-JP");
#	$tel2 = mb_convert_kana($tel2,n,"EUC-JP");
#	$tel3 = mb_convert_kana($tel3,n,"EUC-JP");
#	$fax1 = mb_convert_kana($fax1,n,"EUC-JP");
#	$fax2 = mb_convert_kana($fax2,n,"EUC-JP");
#	$fax3 = mb_convert_kana($fax3,n,"EUC-JP");
#	$email = mb_convert_kana($email,n,"EUC-JP");
#	$email = strtolower($email);

	$zip1 = mb_convert_kana($zip1,n,"UTF-8");
	$zip2 = mb_convert_kana($zip2,n,"UTF-8");
	$tel1 = mb_convert_kana($tel1,n,"UTF-8");
	$tel2 = mb_convert_kana($tel2,n,"UTF-8");
	$tel3 = mb_convert_kana($tel3,n,"UTF-8");
	$fax1 = mb_convert_kana($fax1,n,"UTF-8");
	$fax2 = mb_convert_kana($fax2,n,"UTF-8");
	$fax3 = mb_convert_kana($fax3,n,"UTF-8");
	$email = mb_convert_kana($email,n,"UTF-8");
	$email = strtolower($email);

	$ERROR = "";
	if (!$name_s) { $ERROR[] = "漢字氏名（姓）が入力されておりません。"; }
	if (!$name_n) { $ERROR[] = "漢字氏名（名）が入力されておりません。"; }
	if (!$kana_s) { $ERROR[] = "ふりがな氏名（姓）が入力されておりません。"; }
	if (!$kana_n) { $ERROR[] = "ふりがな氏名（名）が入力されておりません。"; }
//	if (!$zip1) { $ERROR[] = "郵便番号３桁が入力されておりません。"; }
//	$zip1_n = strlen($zip1);
## //	if ($zip1 && (!eregi("[0-9]",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
   //   if ($zip1 && (!preg_match("/[0-9]/i",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
//	if (!$zip2) { $ERROR[] = "郵便番号４桁が入力されておりません。"; }
//	$zip2_n = strlen($zip2);
## //	if ($zip2 && (!eregi("[0-9]",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
		if ($zip2 && (!preg_match("/[0-9]/i",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
	if (!$prf) { $ERROR[] = "都道府県名が選択されておりません。"; }
	if (!$city) { $ERROR[] = "市区町村名が入力されておりません。"; }
	if (!$add1) { $ERROR[] = "所番地が入力されておりません。"; }
	if (!$tel1 || !$tel2 || !$tel3) { $ERROR[] = "電話番号が入力されておりません。"; }
//	if (!$fax1 || !$fax2 || !$fax3) { $ERROR[] = "FAX番号が入力されておりません。"; }
	if (!$email) { $ERROR[] = "E-mailアドレスが入力されておりません。"; }
##	if ($email && !ereg("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	$mail_host = $regs[1];
	if ($email && $mail_host != "ybb.ne.jp") {
		if (!getmxrr($mail_host,$mxhostarr)) { $ERROR[] = "E-mailアドレスのホスト名が見つかりませんでした。"; }
		}

#	$msr = mb_convert_kana($msr,"asKV","EUC-JP");
	$msr = mb_convert_kana($msr,"asKV","UTF-8");
##	$msr_ = eregi_replace("\r","",$msr);
	$msr_ = preg_replace("/\r/i","",$msr);
##	$msr_ = eregi_replace("\n","<BR>",$msr_);
	$msr_ = preg_replace("/\n/i","<BR>",$msr_);

	$t_addr = "$name_s<>$name_n<>$kana_s<>$kana_n<>$zip1<>$zip2<>$prf<>$city<>$add1<>$add2<>$tel1<>$tel2<>$tel3<>$fax1<>$fax2<>$fax3<>$email<>$msr_<>";
##	$msr_ = eregi_replace("<BR>","<BR>　",$msr_);
	$msr_ = preg_replace("/<BR>/i","<BR>　",$msr_);

	$KAGO = "";
##	$KAGO = split("<>",$customer);
	$KAGO = explode("<>",$customer);
	sort($KAGO);


	$flag_ban = 0;
	$flag_name = 0;
	$t_opt = "";
	foreach ($KAGO AS $val) {

		if (!$val) { continue; }
##		list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
		list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
##		list($typ) = split("-",$hinban_);
		list($typ) = explode("-",$hinban_);
//		if ($typ != "A") { continue; }

##		$hinban_h = ereg_replace("-","_",$hinban_);
		$hinban_h = preg_replace("/-/","_",$hinban_);


		$t_opt .= "$hinban_<>";

		for($i=1; $i<=$num_; $i++) {

			$t_opt .= "$i::";

			$names = "$hinban_h" . "_ban_" . "$i";
#			$GLOBALS["$names"] = mb_convert_kana($GLOBALS["$names"],n,"EUC-JP");
			$GLOBALS["$names"] = mb_convert_kana($GLOBALS["$names"],n,"UTF-8");
			$values = str_replace('　', '', $GLOBALS["$names"]);
			$values = str_replace(' ', '', $values);
##			if ($values && ereg("[^0-9]",$values)) { $ERROR[] = "商品番号：$hinban_ の $i 番の番号が不正です。"; }
			if ($values && preg_match("/[^0-9]/",$values)) { $ERROR[] = "商品番号：$hinban_ の $i 番の番号が不正です。"; }
			if ($values != "") { $flag_sb = 1; }
			$t_opt .= "$values::";

			$names = "$hinban_h" . "_name_" . "$i";
#			$GLOBALS["$names"] = mb_convert_kana($GLOBALS["$names"],n,"EUC-JP");
			$GLOBALS["$names"] = mb_convert_kana($GLOBALS["$names"],n,"UTF-8");
			$values = str_replace('　', ' ', $GLOBALS["$names"]);
##			if ($values && !eregi("[A-Za-z0-9,-_.']",$values)) { $ERROR[] = "商品番号：$hinban_ の $i 番の背ネームが不正です。"; }
			if ($values && !preg_match("/[A-Za-z0-9,-_.']/i",$values)) { $ERROR[] = "商品番号：$hinban_ の $i 番の背ネームが不正です。"; }
			if ($values != "") { $flag_sn = 1; }
			$t_opt .= "$values::<=>";
		}
		$t_opt .= "<->";
	}

	if ($flag_sb == 1 && $seban_l == "") { $ERROR[] = "背番号のタイプが選択されておりません。"; }
	if ($flag_sn == 1 && $sename_l == "") { $ERROR[] = "背ネームのタイプが選択されておりません。"; }
	if ($flag_mb == 1 && $muneban_l == "") { $ERROR[] = "胸番号のタイプが選択されておりません。"; }
	if ($flag_pb == 1 && $pant_l == "") { $ERROR[] = "パンツ番号のタイプが選択されておりません。"; }

	$t_type = "$seban_l<>$sename_l<>$muneban_l<>$pant_l<>$bach_l<>$p_set<>$s_set<>";

	if ($ERROR) { order(); }
	else {

		$team .= <<<ALPHA
      <BR>
      <TABLE border="0" bgcolor="#666600" width="600" cellpadding="5" cellspacing="1">
        <TBODY>
          <TR>
            <TD colspan="2" bgcolor="#cecf63" height="15"><B>御見積商品確認</B></TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
          <TR>
            <th width="150" class="cate2">背番号タイプ</th>
            <TD width="450" bgcolor="#ffffff">
             $SEBAN_N[$seban_l]
            </TD>
          </TR>
          <TR>
            <th class="cate2">背ネームタイプ</th>
            <TD bgcolor="#ffffff">
             $SENAME_N[$sename_l]
            </TD>
          </TR>
          <TR>
            <th class="cate2">胸番号タイプ</th>
            <TD bgcolor="#ffffff">
             $MUNEBAN_N[$muneban_l]
            </TD>
          </TR>
          <TR>
            <th class="cate2">パンツ番号タイプ</th>
            <TD bgcolor="#ffffff">
             $PANT_N[$pant_l]
            </TD>
          </TR>
          <TR>
            <th class="cate2">バッジタイプ</th>
            <TD bgcolor="#ffffff">
             $BACH_N[$bach_l]
            </TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
      <TABLE border="0" bgcolor="#666600" width="600" cellpadding="5" cellspacing="1">
ALPHA;

		foreach ($KAGO AS $val) {
			if (!$val) { continue; }
##			list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
			list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);

##			$hinban_h = ereg_replace("-","_",$hinban_);
			$hinban_h = preg_replace("/-/","_",$hinban_);

##			list($typ) = split("-",$hinban_);
			list($typ) = explode("-",$hinban_);

//			if ($typ != "A") { continue; }

			$team .= <<<ALPHA
          <TR>
            <TD colspan="2" height="15" background="../image/back_1.gif">
            商品番号：$hinban_ <BR>
            商 品 名 ：$title_ <BR>
            数　　　量：$num_ </TD>
          </TR>
ALPHA;

			for($i=1; $i<=$num_; $i++) {
				$sb = "";
				$sn = "";
				$op_l = "";

				$names = "$hinban_h" . "_ban_" . "$i";
				$sb = $GLOBALS["$names"];

				$names = "$hinban_h" . "_name_" . "$i";
				$sn = $GLOBALS["$names"];

				if (!isset($sb) && !isset($sn)) { $op_l = "マーキング無し"; }
				if (isset($sb)) {
					if ($seban_l != "") { $op_l .= "背番号 "; }
					if ($muneban_l != "") { $op_l .= "胸番号 "; }
					$op_l .= "[ $sb ] ";
				}
				if (isset($sb) && isset($sn)) { $op_l .= ", "; }
				if (isset($sn)) { $op_l .= "背ネーム [ $sn ] "; }

				$team .= <<<ALPHA
          <TR>
            <th width="60" height="15" class="cate2">
            $i
            </TD>
            <TD width="540" height="15" bgcolor="#ffffff">
            $op_l
            </TD>
          </TR>
ALPHA;
			}

			$team .= <<<ALPHA
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
ALPHA;
		}

		if ($p_set == 1) {
					$team .= <<<ALPHA
          <TR>
            <th width="60" height="15" class="cate2">
            パンツ
            </TD>
            <TD width="540" height="15" bgcolor="#ffffff">
            パンツセットでお見積もり
            </TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
ALPHA;
		}

		if ($s_set == 1) {
			$team .= <<<ALPHA
          <TR>
            <th width="60" height="15" class="cate2">
            ソックス
            </TD>
            <TD width="540" height="15" bgcolor="#ffffff">
            ソックスセットでお見積もり
            </TD>
          </TR>
          <TR>
            <TD colspan="2" bgcolor="#ffffff"></TD>
          </TR>
ALPHA;
		}

		$team .= <<<ALPHA
        </TBODY>
      </TABLE>
      <BR>
ALPHA;

		$team .= <<<ALPHA
      <table width="780px">
          <TR>
            <th height="15" class="cate2"><B>御連絡先</B></TD>
          </TR>
          <TR>
            <TD bgcolor="#ffffff"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD align="center" valign="middle">
            <BR>
            <TABLE width="580" style="border:0px;">
              <TBODY>
                <TR>
                  <th class="cate2" height="20"><B>漢字氏名</B></th>
                </TR>
                <TR>
                  <TD>　$name_s $name_n </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>ふりがな</B></th>
                </TR>
                <TR>
                  <TD>　$kana_s $kana_n </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>住所</B></th>
                </TR>
                <TR>
                  <TD>
                  　〒$zip1 - $zip2<BR>
                  　$PRF_N[$prf] $city<BR>
                  　$add1 $add2
                  </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20">
                  <B>電話番号</B></FONT>
                  </th>
                </TR>
                <TR>
                  <TD>　$tel1 - $tel2 - $tel3 </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>FAX番号</B></th>
                </TR>
                <TR>
                  <TD>　$fax1 - $fax2 - $fax3 </TD>
                </TR>
                <TR>
                  <th class="cate2" height="20">
                  <B>メールアドレス</B>
                  </TD>
                </TR>
                <TR>
                  <TD>　$email</TD>
                </TR>
                <TR>
                  <th class="cate2" height="20"><B>ご意見ご要望など</B></th>
                </TR>
                <TR>
                  <TD>　 $msr_</TD>
                </TR>
              </TBODY>
            </TABLE>
            </TD>
          </TR>
      </TABLE>
      <BR>
      <table width="780px">
          <TR>
            <TD bgcolor="#cecf63" colspan="2">
            上記でよろしい場合は下記の御見積ボタンを押して下さい。<BR>
            もし修正する場合は、修正ボタンを押して下さい。
            </TD>
          </TR>
          <TR>
            <TD bgcolor="#ffffff" colspan="2">
            </TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD width="300" align="center">
            <FORM action="./1-10/send.php" method="POST">
            <INPUT type="submit" value="御見積" class="button">
            </TD></FORM>
            <TD width="300" align="center">
            <FORM action="$PHP_SELF" method="POST">
            <INPUT type="hidden" name="mode" value="kaku">
            <INPUT type="submit" value="修正" class="button">
            </TD></FORM>
          </TR>
      </TABLE>
      <BR>
ALPHA;

	}

	return $team;

}
?>
