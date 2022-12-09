<?PHP

function zaiko_2() {
// global $mode; //必要ではない
$mode=$_POST['mode'];
	if ($mode == "add") { add(); }

	frist_00();

}



function frist_00() {
global $PHP_SELF,$ERROR;
global $ok,$zaiko_list;// @source add()
global $main,$zaiko,$cate1,$cate2,$cate3,$cate1_r,$cate2_r;// _POST
include './array.inc';
include './list.inc';
$cate1=$_POST['cate1']; //ToDo check
$cate2=$_POST['cate2']; //ToDo check
$cate3=$_POST['cate3']; //ToDo check
$cate1_r=$_POST['cate1_r']; //ToDo check
$cate2_r=$_POST['cate2_r']; //ToDo check

	if ($ok > 0) { $ok_msg = "$ok 点の商品を登録しました。<BR>\n<BR>\n"; }

	echo <<<ALPHA
$ok_msg
登録したい商品のカテゴリーを選択して下さい。<BR>
<BR>
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
<TABLE border="0">
  <TBODY>
    <TR>
      <TD>メインカテゴリー<BR><SELECT name='cate1'>
ALPHA;

	if ($cate1 != $cate1_r) { $cate2 = ""; $cate3 = ""; }
	if ($cate2 != $cate2_r) { $cate3 = ""; }

	if (!$cate1 || $cate1 == 0) { $select = "selected"; } else { $select = "";}
	echo ("		<OPTION value='' $select>$KATE[0]</OPTION>\n");
	$max = count($KATE);
	for ($i=1; $i<$max; $i++) {
		if ($cate1 == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$KATE[$i]</OPTION>\n");
			}
	echo ("      </SELECT>\n");
	echo ("      <INPUT type=\"hidden\" name=\"cate1_r\" value=\"$cate1\">\n");
	echo ("      </TD>\n");

	if ($cate1 && $cate1 != 0) {

		$KATE_N2 = "KATE_$cate1";

		echo ("      <TD>カテゴリー2<BR><SELECT name='cate2'>\n");
		if (!$cate2 || ($cate2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${"$KATE_N2"}[0]}</OPTION>\n");
		$max = count(${$KATE_N2});
		for ($i=1; $i<$max; $i++) {
			if ($cate2 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${"$KATE_N2"}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT>\n");
		echo ("      <INPUT type=\"hidden\" name=\"cate2_r\" value=\"$cate2\">\n");
		echo ("      </TD>\n");

		}

	if ($cate2 && $cate2 != 0) {

		$KATE_N3 = "KATE_$cate1" . "_$cate2";

		echo ("      <TD>カテゴリー3<BR><SELECT name='cate3'>\n");
		if (!$cate2 || ($cate2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${$KATE_N3}[0]}</OPTION>\n");
		$max = count(${$KATE_N3});
		for ($i=1; $i<$max; $i++) {
			if ($cate3 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${$KATE_N3}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT></TD>\n");

		}

	echo <<<ALPHA
      <TD valign="bottom">
      <INPUT type="submit" value="変更">
      </TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

	if ($cate3 && $cate3 != 0) {

		if ($ERROR) {
			foreach ($ERROR AS $val) {
				$error .= "$val <BR>\n";
				}
			}

		if ($zaiko_list) {
			$count = count($zaiko_list);
			$error .= "$count 点の商品が登録できませんでした。<BR>\n";

			for($i=0; $i<$count; $i++) {
				$hinban_n	= "hinban_"	. "$i";
				$title_n	= "title_"	. "$i";
				$size_n		= "size_"	. "$i";
				$money_n	= "money_"	. "$i";
				$stock_n	= "stock_"	. "$i";
##				list(${$hinban_n},${$title_n},${$size_n},${$money_n},${$stock_n}) = split("<>",$zaiko_list[$i]);
				list(${$hinban_n},${$title_n},${$size_n},${$money_n},${$stock_n}) = explode("<>",$zaiko_list[$i]);
				}
			}

		echo <<<ALPHA
$error
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="mode" value="add">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="zaiko" value="$zaiko">
<INPUT type="hidden" name="main_r" value="$main">
<INPUT type="hidden" name="zaiko_r" value="$zaiko">
<INPUT type="hidden" name="cate1" value="$cate1">
<INPUT type="hidden" name="cate2" value="$cate2">
<INPUT type="hidden" name="cate3" value="$cate3">
<INPUT type="hidden" name="cate1_r" value="$cate1">
<INPUT type="hidden" name="cate2_r" value="$cate2">
$KATE[$cate1] &gt; {${$KATE_N2}[$cate2]} &gt; {${$KATE_N3}[$cate3]} <BR>
上記のカテゴリーにする商品を入力し登録を押して下さい。<BR>
<TABLE border="0" bgcolor="#666666" cellpadding="4">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD width="20" align="center"></TD>
      <TD>商品番号</TD>
      <TD>商品名</TD>
      <TD align="center">サイズ</TD>
      <TD align="center">金額</TD>
      <TD align="center">在庫数</TD>
    </TR>
ALPHA;

	for ($i=0; $i<=9; $i++) {

		if ($i%2 == 0) { $bgcolor = "#ffffff"; } else { $bgcolor = "#dfdfdf"; }

		$hinban_n	= "hinban_"	. "$i";
		$title_n	= "title_"	. "$i";
		$size_n		= "size_"	. "$i";
		$money_n	= "money_"	. "$i";
		$stock_n	= "stock_"	. "$i";

		echo <<<ALPHA
    <TR bgcolor="$bgcolor">
      <TD align="center">$i</TD>
      <TD><INPUT size="40" type="text" name="hinban_$i" value="${$hinban_n}"></TD>
      <TD><INPUT size="40" type="text" name="title_$i" value="${$title_n}"></TD>
      <TD><INPUT size="8" type="text" name="size_$i" value="${$size_n}"></TD>
      <TD><INPUT size="6" type="text" name="money_$i" value="${$money_n}"></TD>
      <TD><INPUT size="6" type="text" name="stock_$i" value="${$stock_n}"></TD>
    </TR>
ALPHA;

		}

	echo <<<ALPHA
    <TR bgcolor="#ffffff">
      <TD colspan="6" align="center"><INPUT type="submit" value="登録">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
ALPHA;

		}

}



function add() {
// global	$cate1,$cate2,$cate3,$zaiko_list,$ok,
// 		$hinban_0,$title_0,$size_0,$money_0,$stock_0,
// 		$hinban_1,$title_1,$size_1,$money_1,$stock_1,
// 		$hinban_2,$title_2,$size_2,$money_2,$stock_2,
// 		$hinban_3,$title_3,$size_3,$money_3,$stock_3,
// 		$hinban_4,$title_4,$size_4,$money_4,$stock_4,
// 		$hinban_5,$title_5,$size_5,$money_5,$stock_5,
// 		$hinban_6,$title_6,$size_6,$money_6,$stock_6,
// 		$hinban_7,$title_7,$size_7,$money_7,$stock_7,
// 		$hinban_8,$title_8,$size_8,$money_8,$stock_8,
// 		$hinban_9,$title_9,$size_9,$money_9,$stock_9;
global $ok,$zaiko_list,$conn_id;
include "../../cone.inc";
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];
$hinban_0=$_POST["hinban_0"];$title_0=$_POST["title_0"];$size_0=$_POST["size_0"];$money_0=$_POST["money_0"];$stock_0=$_POST["stock_0"];
$hinban_1=$_POST["hinban_1"];$title_1=$_POST["title_1"];$size_1=$_POST["size_1"];$money_1=$_POST["money_1"];$stock_1=$_POST["stock_1"];
$hinban_2=$_POST["hinban_2"];$title_2=$_POST["title_2"];$size_2=$_POST["size_2"];$money_2=$_POST["money_2"];$stock_2=$_POST["stock_2"];
$hinban_3=$_POST["hinban_3"];$title_3=$_POST["title_3"];$size_3=$_POST["size_3"];$money_3=$_POST["money_3"];$stock_3=$_POST["stock_3"];
$hinban_4=$_POST["hinban_4"];$title_4=$_POST["title_4"];$size_4=$_POST["size_4"];$money_4=$_POST["money_4"];$stock_4=$_POST["stock_4"];
$hinban_5=$_POST["hinban_5"];$title_5=$_POST["title_5"];$size_5=$_POST["size_5"];$money_5=$_POST["money_5"];$stock_5=$_POST["stock_5"];
$hinban_6=$_POST["hinban_6"];$title_6=$_POST["title_6"];$size_6=$_POST["size_6"];$money_6=$_POST["money_6"];$stock_6=$_POST["stock_6"];
$hinban_7=$_POST["hinban_7"];$title_7=$_POST["title_7"];$size_7=$_POST["size_7"];$money_7=$_POST["money_7"];$stock_7=$_POST["stock_7"];
$hinban_8=$_POST["hinban_8"];$title_8=$_POST["title_8"];$size_8=$_POST["size_8"];$money_8=$_POST["money_8"];$stock_8=$_POST["stock_8"];
$hinban_9=$_POST["hinban_9"];$title_9=$_POST["title_9"];$size_9=$_POST["size_9"];$money_9=$_POST["money_9"];$stock_9=$_POST["stock_9"];

$zaiko_list=$_POST["zaiko_list"];

	$ok = 0;
	for ($i=0; $i<=9; $i++) {
		$err = 0;
		$hinban_n	= "hinban_"	. "$i";
		$title_n	= "title_"	. "$i";
		$size_n		= "size_"	. "$i";
		$money_n	= "money_"	. "$i";
		$stock_n	= "stock_"	. "$i";

		if (${$hinban_n} == "" && ${$title_n} == "" && ${$size_n} == "" && ${$money_n} == "" && ${$stock_n} == "") { continue; }

#		${$money_n} = mb_convert_kana(${$money_n},a,"EUC-JP");
		${$money_n} = mb_convert_kana(${$money_n},a,"UTF-8");
#		${$stock_n} = mb_convert_kana(${$stock_n},a,"EUC-JP");
		${$stock_n} = mb_convert_kana(${$stock_n},a,"UTF-8");

##		if (!ereg("[0-9]",${$money_n})) { $ERROR[] = "入力された金額が不正な商品があります。"; $err = 1; }
		if (!preg_match("/[0-9]/",${$money_n})) { $ERROR[] = "入力された金額が不正な商品があります。"; $err = 1; }
##		if (!ereg("[0-9]",${$stock_n})) { $ERROR[] = "入力された在庫数が不正な商品があります。"; $err = 1; }
		if (!preg_match("/[0-9]/",${$stock_n})) { $ERROR[] = "入力された在庫数が不正な商品があります。"; $err = 1; }
		if ($err == 1 || ${$hinban_n} == "" || ${$title_n} == "") {
			$zaiko_list[] = "${$hinban_n}<>${$title_n}<>${$size_n}<>${$money_n}<>${$stock_n}<>";
			var_dump($zaiko_list);
			 continue;
			}
		$sql = "SELECT hinban FROM zaiko WHERE hinban='${$hinban_n}';";
		$sql1 = pg_exec($conn_id,$sql);
		$check = pg_numrows($sql1);
		if ($check <= 0) {

			$sql = "SELECT MAX(zaiko_num) FROM zaiko;";
			$sql1 = pg_exec($conn_id,$sql);
			list($zaiko_num) = pg_fetch_array($sql1,0);
			$zaiko_num = $zaiko_num + 1;

			$sql =  "insert into zaiko values (" .
					"'$zaiko_num'," .
					"'${$hinban_n}'," .
					"'${$title_n}'," .
					"'${$size_n}'," .
					"'$cate1'," .
					"'$cate2'," .
					"'$cate3'," .
					"'${$money_n}'," .
					"'${$stock_n}');";
			$sql1 = pg_exec($conn_id,$sql);
			$check = pg_numrows($sql1);
			if ($check <= 0) { $ERROR[] = "商品が記録できませんでした。"; }
			$ok = $ok + 1;

			}
		}
}

?>
