<?PHP
//	分類コード登録
function goods_6() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n,$check,$mode;

if (!$mode) {
	if ($work == 1) { first_01(); }
	if ($work == 2) { first_02(); }
	if ($work == 3) { first_03(); }
	}

	if ($mode == "hensyu") { hensyu(); }
	if ($mode == "hen") { hen(); }
	if ($mode == "write") { write(); }
	if ($mode == "del") { del(); }

}



//	編集一覧
function first_01() {

ichiran();

}


//	編集画面
function hensyu() {
global $PHP_SELF,$main,$goods,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';
include "../../cone.inc";

	$sql = "select * from class where class_m='$class_m';";
	$sql1 = pg_exec($conn_id,$sql);
	$cla = pg_fetch_array($sql1,0);

?>
中分類コードと名称を入力し登録を押して下さい。<BR>
<FORM action='<? echo ("$PHP_SELF"); ?>' method='POST'>
<INPUT type='hidden' name='mode' value='hen'>
<INPUT type='hidden' name='main' value='<? echo ("$main"); ?>'>
<INPUT type='hidden' name='goods' value='<? echo ("$goods"); ?>'>
<INPUT type='hidden' name='work' value='<? echo ("$work"); ?>'>
<INPUT type='hidden' name='class_l' value='<? echo ("$cla[0]"); ?>'>
<INPUT type='hidden' name='class_m' value='<? echo ("$cla[1]"); ?>'>
		<TABLE border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
		  <TBODY>
<TR bgcolor="#ffffff"><TD>中分類コード</TD><TD><? echo ("$cla[1]"); ?></TD></TR>
<TR bgcolor="#ffffff"><TD>中分類名称</TD><TD><INPUT size='30' type='text' name='class_m_n' value='<? echo ("$cla[2]"); ?>'></TD></TR>
<TR bgcolor="#ffffff"><TD colspan='2'><INPUT type='submit' value='登録'> <INPUT type='reset'></TD></TR>
  </TBODY>
</TABLE>
</FORM>
<?

	pg_close($conn_id);

}


//	編集記録
function hen() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';
include "../../cone.inc";

	$sql = "update class set class_m_n='$class_m_n' where class_m=$class_m;";
	$sql1 = pg_exec($conn_id,$sql);

	if ($sql1) {
		echo "$class_l:$CLASS_N[$class_l]、$class_m:$class_m_n を変更完了しました。<BR><BR>\n"; $check = "";
		} else {
		echo "$class_l:$CLASS_N[$class_l]、$class_m:$class_m_n を変更失敗しました。<BR>\n";
		}

	pg_close($conn_id);
	first_01();

}


//	新規登録
function first_02() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';

?>
        中分類コードと名称を入力し登録を押して下さい。<BR>
<FORM action='<? echo ("$PHP_SELF"); ?>' method='POST'>
<INPUT type='hidden' name='mode' value='write'>
<INPUT type='hidden' name='main' value='<? echo ("$main"); ?>'>
<INPUT type='hidden' name='goods' value='<? echo ("$goods"); ?>'>
<INPUT type='hidden' name='work' value='<? echo ("$work"); ?>'>
<TABLE border="0" cellpadding="3" cellspacing="2" bgcolor="#999999">
  <TBODY>
   <TR bgcolor="#ffffff">
    <TD>中分類コード</TD>
    <TD><INPUT size='6' type='text' maxlength='3' name='class_m' value='<? echo ("$class_m"); ?>'>（半角数字３桁）</TD>
   </TR>
   <TR bgcolor="#ffffff">
    <TD>中分類名称</TD>
    <TD><INPUT size='30' type='text' name='class_m_n' value='<? echo ("$class_m_n"); ?>'></TD>
   </TR>
   <TR bgcolor="#ffffff">
    <TD colspan='2'><INPUT type='submit' value='登録'> <INPUT type='reset'></TD>
   </TR>
  </TBODY>
</TABLE>
</FORM>
<?

}


//	新規登録記録
function write() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';
include "../../cone.inc";

	if (!$class_m) { $ERROR[] = "中分類コードを入力して下さい。"; }
	## $class_m = mb_convert_kana($class_m,"n","EUC-JP");
	$class_m = mb_convert_kana($class_m,"n","UTF-8");
##	if ($class_m && ((!eregi("[0-9]",$class_m)) || (strlen($class_m) != 3))) { $ERROR[] = '中分類コードが不正です。'; }
	if ($class_m && ((!preg_match("/[0-9]/i",$class_m)) || (strlen($class_m) != 3))) { $ERROR[] = '中分類コードが不正です。'; }
	if (!$class_m_n) { $ERROR[] = "中分類コードの名称を入力して下さい。"; }
	if ($ERROR) { $check=""; ERROR(&$ERROR); first_02(); }

	if (!$ERROR) {
		$sql = "select * from class where class_m=$class_m;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);

		if ($count > 0) { $ERROR[] = "既にその中分類コードは登録されております。"; }
		if ($ERROR) { $check=""; ERROR(&$ERROR); }

		if (!$ERROR) {
			$class_l = substr($class_m,0,1);
				$sql =	"insert into class values (" .
						"'$class_l'," .
						"'$class_m'," .
						"'$class_m_n');";
				$sql1 = pg_exec($conn_id,$sql);

				if ($sql1) {
					echo "$class_l:$CLASS_N[$class_l]、$class_m:$class_m_n を登録完了しました。<BR><BR>\n";
					} else {
					echo "$class_l:$CLASS_N[$class_l]、$class_m:$class_m_n を登録失敗しました。<BR>\n";
					}
			}

		}

	pg_close($conn_id);

}


//	削除一覧
function first_03() {

ichiran();

}


//	削除
function del() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';
include "../../cone.inc";

	$sql = "select * from class where class_m='$class_m';";
	$sql1 = pg_exec($conn_id,$sql);
	$cla = pg_fetch_array($sql1,0);
	$class_l = $cla[0];
	$class_m_n = $cla[2];

	$sql = "delete from class where class_m='$class_m';";
	$sql1 = pg_exec($conn_id,$sql);
	if ($sql1) { echo ("$class_l:$CLASS_N[$class_l] $class_m:$class_m_n を削除しました。<BR><BR>\n"); }
	else { echo ("$class_l:$CLASS_N[$class_l] $class_m:$class_m_n を削除出来ませんでした。<BR>\n"); }

	pg_close($conn_id);

}


//	一覧
function ichiran() {
global $main,$goods,$PHP_SELF,$work,$class_l,$class_l_n,$class_m,$class_m_n;
include './array.inc';
include "../../cone.inc";

	$sql = "select * from class order by class_m;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);

	if ($count) {
		if ($work == 1) { echo ("編集をしたい中分類コードをクリックして下さい。<BR>\n"); $mode = "hensyu"; }
		if ($work == 3) { echo ("削除をしたい中分類コードをクリックして下さい。<BR>\n"); $mode = "del"; }

?>
<FORM action='<? echo ("$PHP_SELF"); ?>' method='POST'>
<INPUT type='hidden' name='mode' value='<? echo ("$mode"); ?>'>
<INPUT type='hidden' name='main' value='<? echo ("$main"); ?>'>
<INPUT type='hidden' name='goods' value='<? echo ("$goods"); ?>'>
<INPUT type='hidden' name='work' value='<? echo ("$work"); ?>'>
<INPUT type='hidden' name='check' value='1'>
<TABLE border="0" cellpadding="3" cellspacing="2" bgcolor='#999999'>
  <TBODY>
    <TR bgcolor='#cccccc'>
      <TD align="center">大分類コード</TD><TD align="center">名称</TD>
      <TD align="center">中分類コード</TD><TD align="center">名称</TD>
    </TR>
<?
		--$count;
		for ($i=0; $i<=$count; $i++) {
			$cla = pg_fetch_array($sql1,$i);
			$class_l = "$cla[0]";
			$class_l_n = "$CLASS_N[$class_l]";
			echo ("    <TR bgcolor='#ffffff'>\n");
			echo ("      <TD align='center'>$class_l</TD>\n");
			echo ("      <TD>$class_l_n</TD>\n");
			echo ("      <TD align='center'><INPUT type='submit' name='class_m' value='$cla[1]'></TD>\n");
			echo ("      <TD>$cla[2]</TD>\n");
			echo ("    </TR>\n");
			}
?>
  </TBODY>
</TABLE>
</FORM>
<?
		}
	else { echo ("現在登録されております分類はありません。<BR>\n"); }

	pg_close($conn_id);
	$mode = "";

}

?>


