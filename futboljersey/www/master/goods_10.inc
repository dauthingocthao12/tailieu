<?PHP
//	カテゴリー挿入タグリスト
function goods_10() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$mc,$sc,$mode,$name,$num,$url;
global	$PHP_SELF,$CATE_DIR;
	$CATE_DIR = "../category";

	first_00();

}



function first_00() {
// global	$PHP_SELF,$CATE_DIR,$main,$goods,$mc,$mc_r,$sc;
global	$PHP_SELF,$CATE_DIR;
$main=$_POST['main'];
$goods=$_POST['goods'];
$mc=$_POST['mc'];
$sc=$_POST['sc'];
$mc_r=$_POST['mc_r'];

	$file = "$CATE_DIR/category.inc";
	$LIST = file($file);
	foreach ($LIST AS $val) {
		list($h_num_,$num_,$mc_name_,$mc_dir_) = explode("<>",$val);
		$CATEGORY[$h_num_] = "$num_<>$mc_name_<>$mc_dir_<>";
	}
	ksort($CATEGORY);

	if (!$mc) { $selected = "selected"; } else { $selected = ""; }
	$mc_l = "        <OPTION value=\"\" $selected>選択して下さい。</OPTION>\n";
	foreach ($CATEGORY AS $val) {
		$list = explode("<>",$val);
		if ($mc == $list[0]) { $selected = "selected"; $mc_name = $list[1]; $mc_dir = $list[2]; }
		else { $selected = ""; }
		$mc_l .= "        <OPTION value=\"$list[0]\" $selected>$list[1]</OPTION>\n";
	}

	if ($mc != $mc_r) { $sc = ""; }

	$file = "$CATE_DIR/$mc.dat";
	if ($mc && file_exists($file)) {
		if (!$sc) { $selected = "selected"; } else { $selected = ""; }
		$sc_l = "        <OPTION value=\"\" $selected>-------------</OPTION>\n";
		$SUB_LIST = file($file);
		foreach ($SUB_LIST AS $val) {
			list($h_num_,$num_,$sc_name_) = explode("<>",$val);
			if ($sc == $num_) { $selected = "selected"; $sc_name = $sc_name_; } else { $selected = ""; }
			$sc_l .= "        <OPTION value=\"$num_\" $selected>$sc_name_</OPTION>\n";
		}
	}

	$file = "$CATE_DIR/$mc" . "_$sc.dat";
	if ($mc && $sc && file_exists($file)) {
		$LAST_LIST = file($file);
	}

	echo <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mc_r" value="$mc">
カテゴリーを選択すると、挿入するタグの一覧が表示されます。<BR>
<TABLE border="0">
  <TBODY>
    <TR>
      <TD><SELECT name="mc">
$mc_l
      </SELECT></TD>

WAKABA;

	if ($sc_l) {
		echo <<<WAKABA
      <TD><SELECT name="sc">
$sc_l
      </SELECT></TD>

WAKABA;
	}

	echo <<<WAKABA
      <TD><INPUT type="submit" value="選択"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>

WAKABA;


	if ($mc_name && $sc_name) {
		$cate_name = "$mc_name - $sc_name";
	}
	elseif ($mc_name) {
		$cate_name = "$mc_name";
	}

	if ($mc && $sc) {
		if (!$LAST_LIST) {
			echo ("今現在、<B>$cate_name</B>には、何も登録されておりません。<BR>\n");
		}
		else {
			lists($mc,$sc,$LAST_LIST,$cate_name,$mc_dir);
		}
	}
//	elseif ($mc && !$sc) {
//		if (!$SUB_LIST) {
//			echo ("今現在、<B>$cate_name</B>には、何も登録されておりません。<BR>\n");
//		}
//		else {
//			lists($mc,$sc,$SUB_LIST,$cate_name,$mc_dir);
//		}
//	}

}



function lists($mc,$sc,$LIST,$cate_name,$mc_dir) {
// global $PHP_SELF,$main,$goods,$mc,$mc_r,$sc;
global $PHP_SELF;

	echo <<<WAKABA
<TABLE border="0" cellspacing="1" bgcolor="#666666" cellpadding="3">
  <TBODY>
    <TR bgcolor="#cccccc">
      <TD colspan="3">メインカテゴリー：$cate_name</TD>
    </TR>
    <TR bgcolor="#cccccc" align="center">
      <TD>カテゴリー名</TD>
      <TD>ファイル名</TD>
      <TD>タグ</TD>
    </TR>

WAKABA;

	$max = count($LIST);
	$last = $max - 1;
	for($i=0; $i<$max; $i++) {
		list($h_num_,$num_,$name_,$url_) = explode("<>",$LIST[$i]);
		$url_ = rtrim($url_);
		$tag = "&lt;!--#include virtual=\"../display.php?mc=$mc&sc=$sc&lc=$num_\"--&gt;";
		echo <<<WAKABA
    <TR bgcolor="#ffffff">
      <TD>$name_</TD>
      <TD>/www/$mc_dir/$url_</TD>
      <TD>$tag</TD>
    </TR>

WAKABA;

	}

	echo <<<WAKABA
  </TBODY>
</TABLE>

WAKABA;

}
?>
